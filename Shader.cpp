#include <iostream>
#include <fstream>

#include "Shader.hpp"

namespace kazakami
{

Shader::Shader()
  :vertShader(0),
   fragShader(0),
   program(0)
{
}

Shader::~Shader()
{
  if (vertShader != 0)
    glDeleteShader(vertShader);

  if (fragShader != 0)
    glDeleteShader(fragShader);

  if (program != 0)
    glDeleteProgram(program);
}
  
GLuint Shader::GetProgram() const
{
  return program;
}

bool Shader::readShaderSource(GLuint shader, const std::string & filename)
{
  std::ifstream ifs(filename);
  if (ifs.fail())
    return false;

  std::istreambuf_iterator<char> it(ifs);
  std::istreambuf_iterator<char> last;
  std::string str(it, last);

  const char * s = str.c_str();
  int l = str.length();
  glShaderSource(shader, 1, &s, &l);
  return true;
}

void Shader::printShaderInfoLog(GLuint shader)
{
  GLsizei bufSize;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);

  if (bufSize > 1)
  {
    GLchar * infoLog = new GLchar[bufSize];
    GLsizei length;
    glGetShaderInfoLog(shader, bufSize, &length, infoLog);
    std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
    delete infoLog;
  }
}


void Shader::printProgramInfoLog(GLuint program)
{
  GLsizei bufSize;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH , &bufSize);

  if (bufSize > 1)
  {
    GLchar * infoLog = new GLchar[bufSize];
    GLsizei length;
    glGetProgramInfoLog(program, bufSize, &length, infoLog);
    std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
    delete infoLog;
  }
}



bool Shader::ReadVertex(const std::string & filename)
{
  if (vertShader != 0)
  {
    glDeleteShader(vertShader);
  }
  vertShader = glCreateShader(GL_VERTEX_SHADER);
  if (!readShaderSource(vertShader, filename))
  {
    std::cerr << "read err in vertex" << std::endl;
    return false;
  }
  return true;
}

bool Shader::ReadFragment(const std::string & filename)
{
  if (fragShader != 0)
  {
    glDeleteShader(fragShader);
  }
  fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  if (!readShaderSource(fragShader, filename))
  {
    std::cerr << "read err in fragment Shader" << std::endl;
    return false;
  }
  return true;
}

bool Shader::MakeProgram()
{
  //compileを行う
  GLint compiled;
  if (vertShader == 0 || fragShader == 0)
  {
    std::cerr << "Shader object was not created." << std::endl;
    return false;
  }

  glCompileShader(vertShader);
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
  printShaderInfoLog(vertShader);
  if (compiled == GL_FALSE)
  {
    std::cerr << "Compile error in vertex shader." << std::endl;
    return false;
  }
  
  glCompileShader(fragShader);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
  printShaderInfoLog(fragShader);
  if (compiled == GL_FALSE)
  {
    std::cerr << "Compile error in fragment shader." << std::endl;
    return false;
  }

  //プログラムオブジェクトが生成済みなら既存のものを削除
  if (program != 0)
    glDeleteProgram(program);
  
  //プログラムオブジェクトの生成
  program = glCreateProgram();

  //登録
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);

  //削除
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
  vertShader = 0;
  fragShader = 0;

  GLint linked;
  
  //リンク
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  printProgramInfoLog(program);
  if (linked == GL_FALSE) {
    std::cerr << "Link error." << std::endl;
    return false;
  }
  return true;

}
  
  
  
};
