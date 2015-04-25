#include "Texture2D.hpp"
#include "imageLoader.hpp"

#include <iostream>

namespace kazakami
{


int Texture2D::Load(const std::string & filename)
{
  img = std::shared_ptr<imageLoader>(new imageLoader());
  const int imageLoad = img->load(filename);
  if (imageLoad != 0)
    return imageLoad;

  //既に割り当てられているのなら解放する。
  //再割当てが出来るかが分からないので、取り敢えずこうする。
  if (texNameGenerated)
    glDeleteTextures(1, &texName);

  glGenTextures(1, &texName);
  //テクスチャ名の取得に失敗したら
  if (texName == GL_INVALID_VALUE)
    return -1;

  glBindTexture(GL_TEXTURE_2D , texName);
  //拡大・縮小についての設定
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

  gluBuild2DMipmaps(GL_TEXTURE_2D, 3,img->Width(),img->Height(),GL_BGRA,GL_UNSIGNED_BYTE,img->GetData().data());
  return 0;
}

GLuint Texture2D::GetTexName() const
{
  return texName;
}

uint Texture2D::Width() const
{
  return img->Width();
}

uint Texture2D::Height() const
{
  return img->Height();
}

Texture2D::~Texture2D()
{
  if (texNameGenerated)
    glDeleteTextures(1, &texName);
}



}
