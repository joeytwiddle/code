class TexturedRectangle3d : public Rectangle3d {
public:
  RGBmp *textureimg;
  float scale;

  TexturedRectangle3d(V3d cen,V3d r,V3d d,RGBmp *img) { 
    textureimg=img;
    scale=1.0; //
    setupRectangle3d(cen,r,d);
  }
  
  // scale is the width of the image in world space
  TexturedRectangle3d(V3d cen,V3d r,V3d d,RGBmp *img,float s) {
    scale=s;
    textureimg=img;
    r=r.normalised()*scale;
    d=d.normalised()*scale*(float)img->height/(float)img->width;
    setupRectangle3d(cen-r/2.0-d/2.0,r,d);
  }

  bool inimage(V3d v) {
    return inrectangle(v);
  }

  myRGB colAt(V3d v) {
    if (!inimage(v))
      return myRGB::random();
    V2d p=projectDown(v);
    // return textureimg->getpos(p.x*textureimg->width,p.y*textureimg->height);
    return textureimg->getposinterpolate(p.x*textureimg->width,p.y*textureimg->height);
  }

};
