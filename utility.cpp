#include "utility.hpp"

namespace kazakami
{

Material PMXMat_to_Mat(const PMXMaterial & mat)
{
  Material m =
  {
    { mat.ambient[0],  mat.ambient[1],  mat.ambient[2],  1.0f },
    { mat.diffuse[0],  mat.diffuse[1],  mat.diffuse[2],  mat.diffuse[3] },
    { mat.specular[0], mat.specular[1], mat.specular[2], 1.0f},
    mat.shininess
  };
  return  m;
}

void setMaterial(const Material & mat)
{
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat.ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat.specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, &mat.shininess);
}

void setPMXMaterial(const PMXMaterial & mat)
{
  setMaterial(PMXMat_to_Mat(mat));
}
  
void glVertex3d(const Vector3d & vec)
{
  glVertex3dv(vec.data);
}

}
