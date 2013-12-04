#include "texture.h"
#include "bitmap_image.h"
void Texture::load(const char * filename){
    if(bimg!=0){
        delete bimg;
    }
    bimg=new bitmap_image(filename);
    height = bimg->height();
    width = bimg->width();
}

int Texture::getWidth() {
    return width;
}

int Texture::getHeight() {
    return height;
}


GLubyte* Texture::getGLTexture() {
    int w = getWidth();
    int h = getHeight();
    GLubyte* image =  new GLubyte[h * w * 3];
    unsigned char red, green, blue;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            bimg->get_pixel(j, i,red, green, blue );
            // If meshes are upside down, flip this y
            int ii = h - i - 1;
            image[ii * w * 3+ j * 3 + 0] = (GLubyte) red;
            image[ii * w * 3+ j * 3 + 1] = (GLubyte) green;
            image[ii * w * 3+ j * 3 + 2] = (GLubyte) blue;
        }
    }
    return image;
}


bitmap_image* Texture::getImg() {
    return bimg;
}

const unsigned char*  Texture::getData() {
    return bimg->data();
}

void
    Texture::operator()(int x, int y,  unsigned char * color)
{
    x = clamp(x,0,width-1);
    y = clamp(y,0,height-1);
    bimg->get_pixel(x,y,color[0],color[1],color[2]);
}
bool Texture::valid()
{
    return bimg!=0;
}
///@param x assumed to be between 0 and 1
Vector3f
    Texture::operator()(float x, float y)
{
    Vector3f color;
    int ix,iy;
    x=x*width;
    y=(1-y)*height;
    ix = (int)x;
    iy = (int)y;
    unsigned char pixels[4][3];
    float alpha = x-ix;
    float beta = y-iy;
    operator()(ix  ,iy  ,pixels[0]);
    operator()(ix+1,iy  ,pixels[1]);
    operator()(ix  ,iy+1,pixels[2]);
    operator()(ix+1,iy+1,pixels[3]);
    for(int ii=0;ii<3;ii++){
        color[ii] = (1-alpha)*(1-beta)*pixels[0][ii]
        +    alpha *(1-beta)*pixels[1][ii]
        + (1-alpha)*   beta *pixels[2][ii]
        +    alpha *   beta *pixels[3][ii];
    }
    return color/255;
}

Texture::~Texture()
{
    if(bimg!=0){
        delete bimg;
    }
}

Texture::Texture():bimg(0),width(0),height(0)
{
}

