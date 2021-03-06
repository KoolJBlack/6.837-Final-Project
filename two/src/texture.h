#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include "bitmap_image.h"
#include "Vector3f.h"
#include "extra.h"

///@brief helper class that stores a texture and faciliates lookup
///assume 4byte RGBA image data
class Texture{
public:
  Texture();
  bool valid();
  void load(const char * filename);
  int getWidth();
  int getHeight();
  GLubyte*  getGLTexture();
  bitmap_image * getImg();
  const unsigned char*  getData();
  void operator()(int x, int y,  unsigned char * color);
  ///@param x assumed to be between 0 and 1
  Vector3f operator()(float x, float y);
  ~Texture();
private:
  bitmap_image * bimg;
  int width , height;

};




#endif
