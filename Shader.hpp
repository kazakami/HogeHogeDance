#pragma once

#include <string>
#include <GL/glew.h>

namespace kazakami
{

class Shader
{
  GLuint vertShader;
  GLuint fragShader;
  GLuint program;
  bool readShaderSource(GLuint shader, const std::string & filename);
  void printShaderInfoLog(GLuint shader);
  void printProgramInfoLog(GLuint program);
public:
  Shader();
  ~Shader();
  Shader & operator=(const Shader&) = delete;
  Shader(const Shader&) = delete;
  GLuint GetProgram() const;
  bool ReadVertex(const std::string & filename);
  bool ReadFragment(const std::string & filename);
  bool MakeProgram();
};

}
