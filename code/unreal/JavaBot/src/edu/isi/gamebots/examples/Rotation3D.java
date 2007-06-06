package edu.isi.gamebots.examples;

import java.lang.*;

import edu.isi.gamebots.client.Bot;
import edu.isi.gamebots.examples.*;


/**
 *  Rotation3D is a 3 by 1 matrix (or is that 1 by 3?). The 'D' in Rotation3D
 *  stands for Double, not dimension.  This class is based off of a class in the
 *  javax.vecmath library.  Since I didn't think everyone wanted to install an
 *  entire library just for these example implementations, I recreate it here.
 */
class Rotation3D implements Cloneable {
  //  Public Data
  ///////////////////////////////////////////////////////////////////////////

  // All angles in radians
  public double pitch;
  public double yaw;
  public double roll;

  //  Public Methods
  ///////////////////////////////////////////////////////////////////////////
  public Rotation3D() {
  }

  public Rotation3D( double pitch, double yaw, double roll ) {
    this.pitch = pitch;
    this.yaw   = yaw;
    this.roll  = roll;
  }

  public Rotation3D( Rotation3D vec ) {
    pitch = vec.pitch;
    yaw   = vec.yaw;
    roll  = vec.roll;
  }

  public boolean near( Rotation3D rot, double delta ) {
    return near( rot.pitch, rot.yaw, rot.roll, delta );
  }

  public boolean near( double pitch, double yaw, double roll, double delta ) {
    delta = Math.abs( delta );
    return (Math.abs(pitch-this.pitch)<delta) &&
           (Math.abs(yaw-this.yaw)<delta) &&
           (Math.abs(roll-this.roll)<delta);
  }

  public String toString() {
    return pitch+","+yaw+","+roll;
  }
}
