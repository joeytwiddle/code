#include <joeylib.c>

float keepoffness;

Map2d<float> *mag,*angs;

float offness() {
  V2d v=V2d(0,0);
  float cnt=0;
  for (int x=0;x<=mag->width;x++)
  for (int y=0;y<=mag->height;y++) {
    if (angs->inmap(x,y)) {
      v=v+mag->pos[x][y]*V2d::rotate(V2d(1,0),angs->pos[x][y]);
      cnt+=mag->pos[x][y];
    }
  }
  float offness;
  // Using square offness=10.0*v.mod()*v.mod()/(float)cnt;
  // offness=v.mod();
  if (cnt>0) {
    offness=v.mod();
    offness=offness/(float)cnt;
//    offness=offness/(float)10000.0;
  } else
    offness=10000000;
  keepoffness=offness;
  return offness;
}

float scorefor() {
  return 1.0/offness()*100.0;
  return -offness()*100.0;
}

void main(int argc,String *argv) {

  randomise();
  
  ArgParser a=ArgParser(argc,argv);  
  String imname=a.getarg(1);
  int quant=a.intafter("-q","Quantisation",16);
//  if (a.argsleft())
  String oname=a.argor("edgehist.dat");
  a.done();

//int quant=16;

    RGBmp *oi=RGBmp::readfile(imname);
    Map2d<float> *gs=oi->getgreyscale();

    // Edge detection
    printf("Doing edge detection\n");
    gs->edgedetection(Map2d<float>::sobel(),&mag,&angs);
  
  mag->writefile("tmp.bmp");
  angs->writefile("tmp2.bmp");
  
//  Map1d<float> hist=Map1d<float>(quant);
  float *hist=new float[quant];
  for (int i=0;i<quant;i++)
    hist[i]=0;
  
  for (int x=0;x<gs->width;x++)
  for (int y=0;y<gs->height;y++) {
    float i=angs->pos[x][y]*(float)quant/2.0/pi;
    i=i+(float)quant/2.0;
    int j=i;
//    printf("%i",j);
    hist[j]+=mag->pos[x][y];
  }
  
  List<String> data;
  for (int i=0;i<quant;i++) {
    data.add(Sformat("%.2f",hist[i]));
  }
  
  data.add(Sformat("Score: %.2f",scorefor()));
  data.add(Sformat("V/area: %.2f",keepoffness));

  printf("Score: %.2f\n",scorefor());
  printf("V/area: %.2f\n",keepoffness);
  
  writelinestofile(data,oname);

}
