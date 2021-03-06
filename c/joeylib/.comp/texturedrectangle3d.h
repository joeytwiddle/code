#ifndef texturedrectangle3d_H
  #define texturedrectangle3d_H

  #include <rectangle3d.h>
  #include <rgbmp.h>

class TexturedRectangle3d : public Rectangle3d {
public:
  RGBmp *textureimg; // Exists
  float scale; // Exists

   TexturedRectangle3d(V3d cen,V3d r,V3d d,RGBmp *img); // Method

  
  // scale is the width of the image in world space
   TexturedRectangle3d(V3d cen,V3d r,V3d d,RGBmp *img,float s); // Method


  bool inimage(V3d v); // Method


  myRGB colAt(V3d v); // Method


};

#endif
