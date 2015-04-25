#pragma once

#include <memory>
#include <GL/glu.h>

namespace kazakami
{

class imageLoader;
  
class Texture2D
{
  std::shared_ptr<imageLoader> img;
  GLuint texName;
  bool texNameGenerated;
public:
  Texture2D()
    :img(nullptr), texName(GL_INVALID_VALUE), texNameGenerated(false){}
  Texture2D(const Texture2D &) = delete;
  ~Texture2D();
  int Load(const std::string & filename);
  GLuint GetTexName() const;
  uint Width() const;
  uint Height() const;
};

}
