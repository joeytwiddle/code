class Vnd {
public:
  int dim;
  float *ds;
  Vnd() {
  }
  Vnd(int ss) {
    dim=ss;
    ds=new float[dim];
    for (int i=0;i<dim;i++)
      ds[i]=0;
  }
/*  Vnd(int ss, ... ) {
    dim=ss;
    ds=new float[dim];
    va_list ap;
    va_start(ap, count);
    for (int i=0;i<ss;i++)
      ds[i]=va_arg(ap,float);
  }*/
  Vnd operator+(Vnd o) {
    Vnd n=Vnd(dim);
    for (int i=0;i<dim;i++)
      n.ds[i]=ds[i]+o.ds[i];
    return n;
  }
  Vnd operator*(float f) {
    Vnd n=Vnd(dim);
    for (int i=0;i<dim;i++)
      n.ds[i]=ds[i]*f;
    return n;
  }
};

float tofloat(Vnd v) { // Euclidean distance
  float f=0;
  for (int i=0;i<v.dim;i++) {
    f=f+mysquare(v.ds[i]);
  }
//  printf("%f -> ",sqrt(f));
  return sqrt(f);
}
