#pragma once

#include <memory>
#include <GL/gl.h>

#include "Vector.hpp"
#include "pmxLoader.hpp"

namespace kazakami
{

#ifdef __linux__
const char directorySeparator = '/';
#elif defined __APPLE__
const char directorySeparator = '/'; 
#else
assert("Set directory Sparator");
#endif

const static double pi = 3.141592653589793238;

struct Material
{
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess;
};

const Material red =
{
  { 1, 0, 0, 1},
  { 1, 0, 0, 1},
  { 1, 0, 0, 1},
  20
};

Material PMXMat_to_Mat(const PMXMaterial & mat);

void setMaterial(const Material & mat);

void setPMXMaterial(const PMXMaterial & mat);

void glVertex3d(const Vector3d & vec);
  
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

  

}
