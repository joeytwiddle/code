package jlib;

import java.lang.Math;
import java.awt.Point;

import jlib.V3d;

public class PP {

  public int hscrwid,hscrhei;
  double dist=3.0; // how many 1/2 screen height are there between screen and user's eye?

  public PP(int x,int y) {
    hscrwid=x/2;
    hscrhei=y/2;
  }

  public PP(int x,int y,double d) {
    hscrwid=x/2;
    hscrhei=y/2;
    dist=d;
  }

  public Point project(V3d v) {
    if (v.z+dist<0.1)
      return null;
    double t=(double)hscrhei*dist/(v.z+dist);
    return new Point(hscrwid+(int)(v.x*t),hscrhei+(int)(v.y*t));
  }

  // x=v.x*dist/(v.z-dist)
  // (v.z/dist - 1) * x = v.x
  public V3d projectOut(int x,int y,float distance) { // Actually z at the moment
    double t=(distance/dist-1.0)/(double)hscrhei; // *dist/(distance+dist);
    V3d v=new V3d( (double)(x-hscrwid+0.5)*t, (double)(y-hscrhei+0.5)*t, distance );
    return v; // .mult(distance).subtract(new V3d(0.0,0.0,distance));
  }

}
