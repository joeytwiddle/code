class V3d;
class Ori;

class Matrix {
public:
  float** v;
  V3d operator*(V3d u);
  Matrix();
  static Matrix rotation(V3d axis,float ang);
  void makerotation(V3d axis,float ang);
  void makeorientation(Ori o);
};
