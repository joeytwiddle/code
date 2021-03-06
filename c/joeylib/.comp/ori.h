#ifndef ori_H
  #define ori_H

  #include <v3d.h>

class Ori { // Orientate pushes into real frame
            // Disorientate brings into local frame
public:
  V3d x,y,qz; // Exists
   Ori(); // Method

   Ori(V3d t1,V3d t2); // Method

  String toString(); // Method

  V3d z(); // Method

  void roll(float angle); // Method

  void pitch(float angle); // Method

  void turn(float angle); // Method

  void yaw(float angle); // Method

  void ensureaxesnormalisedbecauseimgoingtoreadthem(); // Method

  static Ori orientate(Ori a,Ori b); // Method

  void quickorisetup(); // Method

  void forcez(V3d nz); // Method

  static Ori indir(V3d v); // Method

  static Ori xz(V3d x,V3d z); // Method

};

#endif
