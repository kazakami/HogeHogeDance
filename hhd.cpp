#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <memory>
#include <vector>
#include <tuple>

#include "utility.hpp"
#include "mesh.hpp"
#include "pmxLoader.hpp"
#include "Shader.hpp"

kazakami::Mesh mesh;
std::vector<std::tuple<std::shared_ptr<kazakami::Shader>, std::string>> shaders;
int usingShaderIndex = -1;

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
  
  //透視変換行列の設定
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);

  //モデルビュー変換行列の設定
  glMatrixMode(GL_MODELVIEW);
}

void idle()
{
  glutPostRedisplay();
}

void loadShaders()
{
  const std::string filename = "shadernames";
  std::ifstream ifs(filename);
  if (ifs.fail())
    return;

  std::string str;
  while (getline(ifs, str))
  {
    auto shader = std::make_shared<kazakami::Shader>();
    shader->ReadVertex(str + ".vert");
    shader->ReadFragment(str + ".frag");
    shader->MakeProgram();
    shaders.push_back(std::make_tuple(shader, str));
  }
}

void initGLSL()
{
  glewInit();

  loadShaders();
  glUseProgram(0);
}

void key(unsigned char key , int, int)
{
  if (key == 13)
    usingShaderIndex++;
  else
    return;
  if (usingShaderIndex >= (int)shaders.size())
  {
    usingShaderIndex = -1;
    glUseProgram(0);
    std::cerr << "set shader : Fixed pipeline" << std::endl;
  }
  else
  {
    glUseProgram(std::get<0>(shaders[usingShaderIndex])->GetProgram());
    std::cerr << "set shader : " << std::get<1>(shaders[usingShaderIndex]) << std::endl;
  }
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
  glutKeyboardFunc(key);
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
