package jlib;

import java.lang.Math;

import jlib.Log;

public class JMaths {
  public static int square(int a) {
    return a*a;
  }
  public static int min(int a,int b) {
    return ( a<b ? a : b);
  }
  public static int max(int a,int b) {
    return ( a>b ? a : b);
  }
  public static double min(double a,double b) {
    return ( a<b ? a : b);
  }
  public static double max(double a,double b) {
    return ( a>b ? a : b);
  }
  public static int minnominus(int a,int b) { // will return b if both < 0
    if (a<0 && b<0)
      return -1;
    if (a<0)
      return b;
    if (b<0)
      return a;
    return min(a,b);
  }
  /** Inclusive **/
  public static int rnd(int a,int b) {
    int x=a+(int)(Math.random()*(b-a+1));
    if (x<a || x>b)
      System.out.println("JMaths.rnd("+a+","+b+"): "+x);
    return x;
  }
  public static double rnd() {
    return Math.random();
  }
  public static double rnd(double a,double b) {
    return a+(float)rnd()*(b-a);
  }
  public static float rnd(float a,float b) {
    return a+(float)rnd()*(b-a);
  }
  public static float crop(float x,float a,float b) {
    if (x<a)
      return a;
    if (x>b)
      return b;
    return x;
  }

  public static float mod(float a) {
    return mod(a,1.0f);
  }
  public static float mod(float a,float b) {
  while (a<0 || a>=b) {
    if (a<0)
      a=a+b;
    if (a>=b)
      a=a-b;
  }
  return a;
  }

}
