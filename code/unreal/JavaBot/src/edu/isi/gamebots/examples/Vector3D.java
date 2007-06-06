package edu.isi.gamebots.examples;

import java.lang.*;

import edu.isi.gamebots.client.Bot;
import edu.isi.gamebots.examples.*;


/**
 *  Vector3D is a 3 by 1 matrix (or is that 1 by 3?). The 'D' in Vector3D stands
 *  for Double, not dimension.  This class is based off of a class in the
 *  javax.vecmath library.  Since I didn't think everyone wanted to install an
 *  entire library just for these example implementations, I recreate it here.
 */
public class Vector3D implements Cloneable {
  //  Public Data
  public double x;
  public double y;
  public double z;

  //  Public Methods
  ///////////////////////////////////////////////////////////////////////////
  public Vector3D() {
  }

  public Vector3D( double x, double y, double z ) {
    this.x = x;
    this.y = y;
    this.z = z;
  }

  public Vector3D( Vector3D vec ) {
    x = vec.x;
    y = vec.y;
    z = vec.z;
  }

  public boolean near( Vector3D loc, double delta ) {
    return near( loc.x, loc.y, loc.z, delta );
  }

  public boolean near( double x, double y, double z, double delta ) {
    double d, dist;

    d = x-this.x;
    dist = d*d;
    d = y-this.y;
    dist += d*d;
    d = z-this.z;
    dist += d*d;

    return dist < delta*delta;
/*    // Old code: 2delta Cube
    delta = Math.abs( delta );
    return (Math.abs(x-this.x)<delta) &&
           (Math.abs(y-this.y)<delta) &&
           (Math.abs(z-this.z)<delta);
*/
  }

  public String toString() {
    return x+","+y+","+z;
  }
}
