#ifndef rgbhist_H
  #define rgbhist_H

class RGBHistogram {
public:
  int quant,numsamps; // Exists
  uchar ***rgb; // Exists
  
   RGBHistogram(); // Method

  
   RGBHistogram(int q); // Method

  
  float correlation(); // Method


    /*

    // Find averages
    long c=0;
    float ax=0,ay=0,az=0;
    for (int i=0;i<quant;i++)
    for (int j=0;j<quant;j++)
    for (int k=0;k<quant;k++) {
      int r=rgb[i][j][k];
      c=c+r;
      ax=ax+r*i;
      ay=ay+r*j;
      az=az+r*k;
    }
    ax=ax/(float)c;
    ay=ay/(float)c;
    az=az/(float)c;
    float sxyz=0,sxxx=0,syyy=0,szzz=0;
    for (i=0;i<quant;i++)
    for (int j=0;j<quant;j++)
    for (int k=0;k<quant;k++) {
      sxyz=sxyz+(float)rgb[i][j][k]*((float)i-ax)*((float)j-ay)*((float)k-az)/(float)quant/(float)quant/(float)quant;
      sxxx=sxxx+(float)rgb[i][j][k]*((float)i-ax)*((float)i-ax)*((float)i-ax)/(float)quant/(float)quant/(float)quant;
      syyy=syyy+(float)rgb[i][j][k]*((float)j-ay)*((float)j-ay)*((float)j-ay)/(float)quant/(float)quant/(float)quant;
      szzz=szzz+(float)rgb[i][j][k]*((float)k-az)*((float)k-az)*((float)k-az)/(float)quant/(float)quant/(float)quant;
    }
    // printf("<%f %f,%f,%f> ",sxyz,sxxx,syyy,szzz);
    return (sxyz*sxyz*sxyz)/(sxxx*syyy*szzz);
    // return (sxxx*syyy*szzz)/(sxyz*sxyz*sxyz);
  }   */
  
  float variance(); // Method

  
  void addocc(myRGB *c); // Method

  /* RGBHistogram *normsmoothed(int radius) {
       RGBHistogram *n=new RGBHistogram(quant);
       int count=0;
       for (int dx=-radius;dx<=radius;dx++) {
         int height=(int)sqrt(radius*radius-dx*dx);
         for (int dy=-height;dy<=height;dy++) {
           int depth=(int)sqrt(radius*radius-dx*dx-dy*dy);
           for (int dz=-height;dz<=height;dz++) {
             count++;
           }
         }
       }
       for (int i=radius;i<quant-radius;i++)
       for (int j=radius;j<quant-radius;j++)
       for (int k=radius;k<quant-radius;k++) {
         float sum=0;
         for (int dx=-radius;dx<=radius;dx++) {
           int height=(int)sqrt(radius*radius-dx*dx);
           for (int dy=-height;dy<=height;dy++) {
             int depth=(int)sqrt(radius*radius-dx*dx-dy*dy);
             for (int dz=-height;dz<=height;dz++) {
               sum=sum+rgb[i+dx][j+dy][k+dx];
             }
           }
         }
         n->rgb[i][j][k]=(int)sum/count;
       }
       // n.done();
       return n;
     }*/
  
  RGBHistogram *normsmoothed(int radius); // Method

  
  // Gaussian
  RGBHistogram *smoothed(int radius); // Method

  static float difference(RGBHistogram a,RGBHistogram b); // Method

  void display(); // Method

  int largest(); // Method

  int maximumat(int i,int j,int k); // Method

  int eightmaximumat(int i,int j,int k); // Method

  int fourmaximumat(int i,int j,int k); // Method

};

float tofloat(RGBHistogram r); // Method


#endif
