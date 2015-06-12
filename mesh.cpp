#include "Texture2D.hpp"
#include "mesh.hpp"
#include "pmxLoader.hpp"
#include "utility.hpp"


namespace kazakami
{

Mesh::Mesh()
{
  pmx = make_unique<PMXLoader>();
}
  
int Mesh::LoadPMX(const std::string & directoryName, const std::string & filename)
{
  const int ret = pmx->readPMX(filename);
  if (ret != 0)
    return ret;

  const int texNum = pmx->textureNames.size();
  for (int i = 0; i < texNum; i++)
  {
    std::shared_ptr<Texture2D> tex(new Texture2D());
    const int returnValue = tex->Load(directoryName + kazakami::directorySeparator + pmx->textureNames[i]);
    if (returnValue == -1)
      std::cerr << "eee" << std::endl;
    textures.push_back(tex);
  }
  return 0;
}

void Mesh::Draw()
{
  /*
  for (int i = 0; i < pmx->mtrNum; i++)
  {
    const auto & mat = pmx->materials[i];
    const int textureNumber = mat.normalTexture;
    //setPMXMaterial(mat);
    //setMaterial(red);
    if (textureNumber == -1) //テクスチャを使用するか
    {
      glBindTexture(GL_TEXTURE_2D, 0);
      glDisable(GL_TEXTURE_2D);
    }
    else
    {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textures[textureNumber]->GetTexName());
    }
    glBegin(GL_TRIANGLES);
    {
      glTexCoord2f(0, 1); glNormal3f(0, 0, -1); glVertex3f(0, 10+i, i);
      glTexCoord2f(0, 0); glNormal3f(0, 0, -1); glVertex3f(0, 0, i);
      glTexCoord2f(1, 0); glNormal3f(0, 0, -1); glVertex3f(10+i, 0, i);

      glTexCoord2f(1, 0); glNormal3f(0, 0, -1); glVertex3f(10+i, 0, i);
      glTexCoord2f(1, 1); glNormal3f(0, 0, -1); glVertex3f(10+i, 10+i, i);
      glTexCoord2f(0, 1); glNormal3f(0, 0, -1); glVertex3f(0, 10+i, i);
    }
    glEnd();
  }
  //*/
  /*
  int i = 0;
  for (const auto & tex : textures)
  {
    glBindTexture(GL_TEXTURE_2D, tex->GetTexName());
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);
    {
      glTexCoord2f(0, 1); glNormal3f(0, 0, -1); glVertex3f(0, 10+i, i);
      glTexCoord2f(0, 0); glNormal3f(0, 0, -1); glVertex3f(0, 0, i);
      glTexCoord2f(1, 0); glNormal3f(0, 0, -1); glVertex3f(10+i, 0, i);

      glTexCoord2f(1, 0); glNormal3f(0, 0, -1); glVertex3f(10+i, 0, i);
      glTexCoord2f(1, 1); glNormal3f(0, 0, -1); glVertex3f(10+i, 10+i, i);
      glTexCoord2f(0, 1); glNormal3f(0, 0, -1); glVertex3f(0, 10+i, i);
    }
    glEnd();
    i++;
  }
  //*/
  /*
  glColor3d(0.0, 0.0, 0.0);
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < pmx->faceNum/3; i++)
  {
    glVertex3fv(pmx->vertexes[pmx->faceIndex[i*3]].pos);
    glVertex3fv(pmx->vertexes[pmx->faceIndex[i*3+1]].pos);
    glVertex3fv(pmx->vertexes[pmx->faceIndex[i*3+2]].pos);
  }
  glEnd();
  //*/
  //*
  static bool compiled = false;
  static GLuint dispList;
  if (compiled)
  {
    glCallList(dispList);
    return;
  }
  compiled = true;
  dispList = glGenLists(1);
  glNewList(dispList , GL_COMPILE);
  int drawnVertexNum = 0;
  for (int mtr_i = 0; mtr_i < pmx->mtrNum; mtr_i++)
  {
    const auto & mat = pmx->materials[mtr_i];
    const int textureNumber = mat.normalTexture;
    setPMXMaterial(mat);
    //setMaterial(red);
    if (textureNumber == -1) //テクスチャを使用するか
    {
      glBindTexture(GL_TEXTURE_2D, 0);
      glDisable(GL_TEXTURE_2D);
    }
    else
    {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textures[textureNumber]->GetTexName());
    }
    if (mat.drawFlag & 0x01) //両面描画か
    {
      glDisable(GL_CULL_FACE);
    }
    else
    {
      glEnable(GL_CULL_FACE);
    }
    const int loopCount = mat.vertexNum/3;
    glBegin(GL_TRIANGLES);
    for (int face_i = 0; face_i < loopCount; face_i++)
    {
      //float * f = pmx->vertexes[pmx->faceIndex[drawnVertexNum+face_i*3]].UV;
      //std::cerr << "(" << f[0] << ", " << f[1] << ")" << std::endl;
      glTexCoord2fv(pmx->vertexes[pmx->faceIndex[drawnVertexNum+face_i*3]].UV);
      glNormal3fv(pmx->vertexes[pmx->faceIndex[drawnVertexNum+face_i*3]].normal);
      glVertex3fv(pmx->vertexes[pmx->faceIndex[drawnVertexNum+face_i*3]].pos);

      glTexCoord2fv(pmx->vertexes[pmx->faceIndex[drawnVertexNum+face_i*3+1]].UV);
      glNormal3fv(pmx->vertexes[pmx->faceIndex[drawnVertexNum+face_i*3+1]].normal);
      glVertex3fv(pmx->vertexes[pmx->faceIndex[drawnVertexNum+face_i*3+1]].pos);

      glTexCoord2fv(pmx->vertexes[pmx->faceIndex[drawnVertexNum+face_i*3+2]].UV);
      glNormal3fv(pmx->vertexes[pmx->faceIndex[drawnVertexNum+face_i*3+2]].normal);
      glVertex3fv(pmx->vertexes[pmx->faceIndex[drawnVertexNum+face_i*3+2]].pos);
    }
    glEnd();
    drawnVertexNum += mat.vertexNum;
  }
  glEndList();
  //*/
}
  
}




