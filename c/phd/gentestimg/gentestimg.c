#include <joeylib.c>

int main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  float focallength=a.floatafter("-focal","focal length (prop to img width)",1.0);
  a.comment("Orientation of plane in degrees:");
  float roll=deg2rad(a.floatafter("-roll","roll",3.0));
  float yaw=deg2rad(a.floatafter("-yaw","yaw",20.0));
  float pitch=deg2rad(a.floatafter("-pitch","pitch",-40.0));
  float depth=a.floatafter("-depth","depth",100.0);
  float scale=a.floatafter("-scale","scale",0.8);
  int imgwidth=a.intafter("-width",640);
  int imgheight=imgwidth*3/4;
  String inname=a.argor("document image file","in.bmp");
  String outname=a.argor("output filename","out.bmp");
  a.done();

  // RGBmp *inputimg=RGBmp::toRGBmp(Map2d<float>::readfile(inname));
  RGBmp *inputimg=RGBmp::readfile(inname);
  RGBmp outputimg=RGBmp(imgwidth,imgheight,myRGB::white);

  inputimg->writefile("test.bmp");
  
  V3d cen=V3d(imgwidth/2,imgheight/2,depth);
  V3d right=V3d(1,0,0);
  V3d down=V3d(0,-1,0);
  right=right.rotated(V3d::i,pitch);
  right=right.rotated(V3d::j,yaw);
  right=right.rotated(V3d::k,roll);
  down=down.rotated(V3d::i,pitch);
  down=down.rotated(V3d::j,yaw);
  down=down.rotated(V3d::k,roll);
  printf("%s %s\n",right.toString(),down.toString());
  TexturedRectangle3d rec=TexturedRectangle3d(cen,right,down,inputimg,scale*imgwidth);

  V3d eye=V3d(imgwidth/2,imgheight/2,-focallength*imgwidth);
  for (int i=0;i<imgwidth;i++) {
    if ( (i % 30) == 0 )
      printf("%i/%i\n",i,imgwidth);
    for (int j=0;j<imgheight;j++) {
      V3d pixel=V3d((float)i,imgheight-1-(float)j,0);
      Line3d l=Line3d(eye,pixel);
      V3d intersect=rec.intersect(l);
      if (!rec.onplane(intersect))
        printf("Error here %f\n",rec.distto(intersect));
      // printf("%s %s %s %s\n",eye.toString(),pixel.toString(),l.toString(),intersect.toString());
      if (rec.inimage(intersect)) {
        // printf("*\n");
        outputimg.setpos(i,j,rec.colAt(intersect));   
      }
    }
  }

  printf("Hello\n");

  outputimg.writefile(outname);
  // Output data to accompany
//   List<String> data;
//   data.add(Sformat("Eye:%s",eye.toString());
//   data.add(Sformat("Eye:%s",eye.toString());
//   data.add(Sformat("Eye:%s",eye.toString());
//   data.add(Sformat("Eye:%s",eye.toString());
//   data.add(Sformat("Eye:%s",eye.toString());
//   data.add(Sformat("Eye:%s",eye.toString());
//   data.add(Sformat("Eye:%s",eye.toString());
  
//   Map2d<bool> *in=Map2d<float>::readfile(ifname)->threshold(0.5);
// //  in->display();
//   Map2d<bool> *n=new Map2d<bool>(in->width,in->height);
// 
//   for (int i=0;i<n->width;i++)
//     for (int j=0;j<=n->height;j++) {
//       if (npixelwithinrad(in,i,j,r))
//         n->pos[i][j]=true;
//       else
//         n->pos[i][j]=false;
//     }
// 
//   n->writefile(ofname);
// //  n->display();
//   printf("expand: written file %s radius %i\n",ofname,r);

}
