class Vnd {
public:
  int dim; // Exists
  float *ds; // Exists
   Vnd(); // Method

   Vnd(int ss); // Method

/*  Vnd(int ss, ... ) {
    dim=ss;
    ds=new float[dim];
    va_list ap;
    va_start(ap, count);
    for (int i=0;i<ss;i++)
      ds[i]=va_arg(ap,float);
  }*/
  Vnd operator+(Vnd o); // Method

  Vnd operator*(float f); // Method

};

float tofloat(Vnd v); // Method

