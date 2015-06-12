#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>

#include "utility.hpp"
#include "mesh.hpp"
#include "pmxLoader.hpp"

kazakami::Mesh mesh;
static GLuint vertShader;
static GLuint fragShader;
static GLuint gl2Program;


struct Options
{
  int windowWidth;
  int windowHeight;
  std::string filename;
  std::string directoryName;
  Options()
    :windowWidth(1920),
     windowHeight(1080),
     filename()
  {
  }
};

Options readCommandLineOption(int, char**);

int readShaderSource(GLuint shader, const char *file);

void printShaderInfoLog(GLuint shader);

void printProgramInfoLog(GLuint program);

void sayonara(const std::string & str, int n = 1)
{
  std::cerr << str << std::endl;
  exit(n);
}



void display()
{
  const static GLfloat lightPos[] = { 0.0, 300.0, 500.0, 1.0 };
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  gluLookAt(3.0, 4.0, 5.0,
	    0.0, 0.0, 0.0,
	    0.0, 1.0, 0.0);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glTranslated(0, -2, 0);
  glScaled(0.2, 0.2, 0.2);
  static double i = 0;
  i += 1;
  glRotated(i, 0, 1, 0);
  mesh.Draw();
  glutSwapBuffers();
}

void initGLSL()
{
  glewInit();

  vertShader = glCreateShader(GL_VERTEX_SHADER);
  fragShader = glCreateShader(GL_FRAGMENT_SHADER);

  if (readShaderSource(vertShader, "texture.vert")) sayonara("read err in vert");
  if (readShaderSource(fragShader, "texture.frag")) sayonara("read err in frag");

  GLint compiled, linked;

  /* バーテックスシェーダのソースプログラムのコンパイル */
  glCompileShader(vertShader);
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
  printShaderInfoLog(vertShader);
  if (compiled == GL_FALSE) {
    fprintf(stderr, "Compile error in vertex shader.\n");
    exit(1);
  }

  /* フラグメントシェーダのソースプログラムのコンパイル */
  glCompileShader(fragShader);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
  printShaderInfoLog(fragShader);
  if (compiled == GL_FALSE) {
    fprintf(stderr, "Compile error in fragment shader.\n");
    exit(1);
  }

  /* プログラムオブジェクトの作成 */
  gl2Program = glCreateProgram();

  /* シェーダオブジェクトのシェーダプログラムへの登録 */
  glAttachShader(gl2Program, vertShader);
  glAttachShader(gl2Program, fragShader);

  /* シェーダオブジェクトの削除 */
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  /* シェーダプログラムのリンク */
  glLinkProgram(gl2Program);
  glGetProgramiv(gl2Program, GL_LINK_STATUS, &linked);
  printProgramInfoLog(gl2Program);
  if (linked == GL_FALSE) {
    fprintf(stderr, "Link error.\n");
    exit(1);
  }

  glUseProgram(gl2Program);
  return;
}

void init(const Options & opt)
{
  glClearColor(0.0, 0.0, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_ALPHA_TEST);
  
  mesh.LoadPMX(opt.directoryName, opt.filename);
}


void resize(int w, int h)
{
  glViewport(0, 0, w, h);
  
  /* 透視変換行列の設定 */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);

  /* モデルビュー変換行列の設定 */
  glMatrixMode(GL_MODELVIEW);
}

void idle(void)
{
  glutPostRedisplay();
}

int main(int argc, char * argv[])
{
  auto opt = readCommandLineOption(argc, argv);
  glutInitWindowSize(opt.windowWidth, opt.windowHeight);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("Hoge Hoge Dance");
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutIdleFunc(idle);
  init(opt);
  initGLSL();
  glutMainLoop();
  return 0;
}



Options readCommandLineOption(int argc, char * argv[])
{
  Options opt;
  int i = 0;
  while (i < argc)
  {
    std::string str(argv[i]);
    opt.filename = str;
    const size_t lastDirectorySeparatorIndex = str.find_last_of(kazakami::directorySeparator);
    if (lastDirectorySeparatorIndex == std::string::npos)
    {
      opt.directoryName = "";
    }
    else
    {
      opt.directoryName = str.substr(0, lastDirectorySeparatorIndex);
    }
    i++;
  }
  return opt;
}

/*
** シェーダーのソースプログラムをメモリに読み込む
*/
int readShaderSource(GLuint shader, const char *file)
{
  FILE * fp;
  GLchar * source;
  GLsizei length;
  int ret;

  /* ファイルを開く */
  fp = fopen(file, "rb");
  if (fp == NULL) {
    perror(file);
    return -1;
  }

  /* ファイルの末尾に移動し現在位置（つまりファイルサイズ）を得る */
  fseek(fp, 0L, SEEK_END);
  length = ftell(fp);

  /* ファイルサイズのメモリを確保 */
  source = (GLchar *)malloc(length);
  if (source == NULL) {
    fprintf(stderr, "Could not allocate read buffer.\n");
    return -1;
  }

  /* ファイルを先頭から読み込む */
  fseek(fp, 0L, SEEK_SET);
  ret = fread(source, 1, length, fp) != (size_t)length;
  fclose(fp);

  /* シェーダのソースプログラムのシェーダオブジェクトへの読み込み */
  if (ret)
    fprintf(stderr, "Could not read file: %s.\n", file);
  else
    glShaderSource(shader, 1, &source, &length);

  /* 確保したメモリの開放 */
  free(source);

  return ret;
}


void printShaderInfoLog(GLuint shader)
{
  GLsizei bufSize;

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);

  if (bufSize > 1) {
    GLchar *infoLog;

    infoLog = (GLchar *)malloc(bufSize);
    if (infoLog != NULL) {
      GLsizei length;

      glGetShaderInfoLog(shader, bufSize, &length, infoLog);
      fprintf(stderr, "InfoLog:\n%s\n\n", infoLog);
      free(infoLog);
    }
    else
      fprintf(stderr, "Could not allocate InfoLog buffer.\n");
  }
}


void printProgramInfoLog(GLuint program)
{
  GLsizei bufSize;

  glGetProgramiv(program, GL_INFO_LOG_LENGTH , &bufSize);

  if (bufSize > 1) {
    GLchar *infoLog;

    infoLog = (GLchar *)malloc(bufSize);
    if (infoLog != NULL) {
      GLsizei length;

      glGetProgramInfoLog(program, bufSize, &length, infoLog);
      fprintf(stderr, "InfoLog:\n%s\n\n", infoLog);
      free(infoLog);
    }
    else
      fprintf(stderr, "Could not allocate InfoLog buffer.\n");
  }
}
