package jlib;

import java.lang.Math;
import java.awt.*;
import java.awt.image.*;

public class PPImage extends PP {

  Image img;
  Graphics gfx;

  public PPImage(Image ii,int width,int height) {
    super(width,height);
    img=ii;
    gfx=img.getGraphics();
  }

  public PPImage(Image ii,int width,int height,double d) {
    super(width,height,d);
    img=ii;
    gfx=img.getGraphics();
  }

  public PPImage(BufferedImage ii) {
    super(ii.getWidth(),ii.getHeight());
    img=ii;
    gfx=img.getGraphics();
  }

  public PPImage(BufferedImage ii,double d) {
    super(ii.getWidth(),ii.getHeight(),d);
    img=ii;
    gfx=img.getGraphics();
  }

  public void line(V3d a,V3d b) {
    Point pa=project(a);
    Point pb=project(b);
    gfx.drawLine(pa.x,pa.y,pb.x,pb.y);
  }

  public void sphere(V3d a,double r) {
    Point pa=project(a);
    Point pb=project(a.add(V3d.i.mult(r)));
    int dist=pb.x-pa.x;
    gfx.drawArc(pa.x-dist,pa.y-dist,dist*2-1,dist*2-1,0,360);
  }

}