import java.lang.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.color.*;

import jlib.*;

class Particle {

  public V3d cen=V3d.random();
  public Ori ori=new Ori(V3d.random(),V3d.random());
  public V3d vel=V3d.o;
  public V3d oriv=V3d.o;
  public V3d acc=V3d.o;
  public V3d oria=V3d.o;

  public double rad=0.0;

  public V3d hang(V3d v,V3d c,double s,double p) {
    return c.add(v.subtract(c).mult(s)).add(V3d.randomvolume().mult(p));
  }

  public void movement() {
    oria=hang(oria,V3d.o,0.99,0.0001);
    acc=hang(acc,V3d.o,0.99,0.0001);
    oriv=oria.add(hang(oriv,V3d.o,0.995,0.0));
    vel=acc.add(hang(vel,V3d.o,0.9995,0.0));
    cen=cen.add(vel);
    ori.roll(oriv.x);
    ori.pitch(oriv.y);
    ori.turn(oriv.z);
  }

}

class IfsTransform extends Particle {

  public V3d scale=new V3d(0.2,0.2,0.2);
  public V3d scalev=V3d.o;
  public V3d scalea=V3d.o;

  IfsTransform() {
    super();
    rad=scale.mod();
  }

  public V3d apply(V3d v) {
    v=v.mult(scale);
    v=v.orientate(ori);
    v=v.add(cen);
    return v;
  }

  public void move() {
    movement();
    scalea=hang(scalea,V3d.o,0.99,0.00001);
    scalev=scalea.add(hang(scalev,V3d.o,0.99,0.0));
    scale=hang(scale,new V3d(0.5,0.5,0.5),0.9,0.0).add(scalev);
    if (scale.x<0)
      scale.x=-scale.x;
    scale.y=scale.x;
    scale.z=scale.x;
    rad=Math.max(scale.x,Math.max(scale.y,scale.z));
//    rad=0.5;
//    scale=scale.norm().mult(0.5);
//    scale=new V3d(Math.sqrt(rad),Math.sqrt(rad),Math.sqrt(rad)).div(3.0);
//    cen=hang(cen,V3d.o,0.99,0.0).add(vel);
/*    for (int d=0;d<3;d++)
      for (double s=-1.0;s<0;s=+1.0)
        if (Math.abs(cen.var(d))>1.0-Math.abs(scale.var(d)))
          vel.setvar(d,-s*Math.abs(vel.var(d))); */
    // Ensure not out of bounds
    double tol=1.2;
    for (int d=0;d<3;d++)
      for (int sd=-1;sd<=1;sd+=2) {
        double s=(double)sd;
        if (s*(cen.var(d)+s*rad)>tol) {
//          System.out.println(d+":"+s+": "+cen+" < "+rad);
        final double dampbounce=0.5;
          cen.setvar(d,tol*s-s*rad);
          vel.setvar(d,-dampbounce*s*Math.abs(vel.var(d)));
          acc.setvar(d,-dampbounce*s*Math.abs(acc.var(d)));
          oriv.setvar(d,dampbounce*oriv.var(d));
          oria.setvar(d,dampbounce*oria.var(d));
          scalev.setvar(d,dampbounce*scalev.var(d));
          scalea.setvar(d,dampbounce*scalea.var(d));
//          System.out.println("  set "+d+"="+(s-s*rad));
//          System.out.println("  "+cen);
          for (int e=0;e<3;e++) {
//            scalev.setvar(e,-Math.abs(scalev.var(e))/2.0);
//            scalea.setvar(e,-Math.abs(scalea.var(e))/2.0);
          }
        }
      }
  }

  public void draw(PPImage p,Graphics g,Matrix m) {
    g.setColor(Color.darkGray);
//    p.sphere(cen,rad);
    Point aaa=p.project(m.mult(apply(new V3d(-scale.x*2.0,-scale.y*2.0,-scale.z*2.0))));
    Point baa=p.project(m.mult(apply(new V3d(+scale.x*2.0,-scale.y*2.0,-scale.z*2.0))));
    Point aba=p.project(m.mult(apply(new V3d(-scale.x*2.0,+scale.y*2.0,-scale.z*2.0))));
    Point bba=p.project(m.mult(apply(new V3d(+scale.x*2.0,+scale.y*2.0,-scale.z*2.0))));
    Point aab=p.project(m.mult(apply(new V3d(-scale.x*2.0,-scale.y*2.0,+scale.z*2.0))));
    Point bab=p.project(m.mult(apply(new V3d(+scale.x*2.0,-scale.y*2.0,+scale.z*2.0))));
    Point abb=p.project(m.mult(apply(new V3d(-scale.x*2.0,+scale.y*2.0,+scale.z*2.0))));
    Point bbb=p.project(m.mult(apply(new V3d(+scale.x*2.0,+scale.y*2.0,+scale.z*2.0))));
    try {
      g.drawLine(aaa.x,aaa.y,baa.x,baa.y);
      g.drawLine(aaa.x,aaa.y,aba.x,aba.y);
      g.drawLine(aaa.x,aaa.y,aab.x,aab.y);
      g.drawLine(baa.x,baa.y,bba.x,bba.y);
      g.drawLine(baa.x,baa.y,bab.x,bab.y);
      g.drawLine(aba.x,aba.y,bba.x,bba.y);
      g.drawLine(aba.x,aba.y,abb.x,abb.y);
      g.drawLine(bab.x,bab.y,aab.x,aab.y);
      g.drawLine(bbb.x,bbb.y,abb.x,abb.y);
      g.drawLine(bbb.x,bbb.y,bab.x,bab.y);
      g.drawLine(bbb.x,bbb.y,bba.x,bba.y);
      g.drawLine(aab.x,aab.y,abb.x,abb.y);
    } catch (Exception e) {
    }
//    System.out.println((float)cen.mod()+" "+(float)scale.mod());
  }

  public double mass() {
    return 4.0*Math.PI*Math.pow(rad,3.0);
  }

}

public class Ifs3d extends Thread implements ComponentListener {

  int nts;
  IfsTransform[] ts;

  BufferedFrame frame;
  PPImage pp;

  V3d p=V3d.o;
  int numps=4000;

  int nrs=1;
  int[] rs=new int[nrs];
  int rem=0;

  double rotang=0.0;
  Matrix m;

  int[] dots=new int[numps];

  public float huefor(float t,float o) {
    float ctmp;
    ctmp=Math.abs(JMaths.mod(t-o));
    if (ctmp>0.5)
      ctmp=1.0f-ctmp;
    // return 255*((float)0.5+0.5*cos(2*pi*ctmp));
    ctmp=ctmp*6.0f;
    if (ctmp<1.0f)
      return 1.0f;
    if (ctmp>2f)
      return 0.0f;
    return 2.0f-ctmp;
  }

  public Color hue(float a,float depth) {
    return new Color(depth*huefor(a,0),depth*huefor(a,(float)1/3),depth*huefor(a,(float)2/3));
  }

  public Color hue(float a) {
    return new Color(huefor(a,0),huefor(a,(float)1/3),huefor(a,(float)2/3));
  }

  public void componentHidden(ComponentEvent e) {
  }

  public void componentMoved(ComponentEvent e) {
  }

  public void componentResized(ComponentEvent e) {
    System.out.println("Resized! "+e);
    frame.getbuffer();
    pp=new PPImage(frame.img,3.0);
  }

  public void componentShown(ComponentEvent e) {
  }

  public final static void main(String[] args) {
    Ifs3d t=new Ifs3d();
    t.start();
  }

  public Ifs3d() {
  }

  public void sierpinski() {
    nts=4;
    ts=new IfsTransform[nts];
    ts[0]=new IfsTransform();
    ts[0].cen=new V3d(0,-0.5,0);
    ts[0].ori=new Ori();
    ts[0].scale=new V3d(0.5,0.5,0.5);
    ts[1]=new IfsTransform();
    ts[1].cen=new V3d(0,0.5,0.5);
    ts[1].ori=new Ori();
    ts[1].scale=new V3d(0.5,0.5,0.5);
    ts[2]=new IfsTransform();
    ts[2].cen=new V3d(0.5,0.5,-0.5);
    ts[2].ori=new Ori();
    ts[2].scale=new V3d(0.5,0.5,0.5);
    ts[3]=new IfsTransform();
    ts[3].cen=new V3d(-0.5,0.5,-0.5);
    ts[3].ori=new Ori();
    ts[3].scale=new V3d(0.5,0.5,0.5);
  }

  public void run() {

    frame=new BufferedFrame("IFS 3d",600,600);
    frame.addComponentListener(this);
    frame.setBackground(Color.black);
    frame.getbuffer();
    pp=new PPImage(frame.img,3.0);

    nts=3+(int)(4*Math.random());
    ts=new IfsTransform[nts];
    for (int i=0;i<nts;i++)
      ts[i]=new IfsTransform();

    sierpinski();

    for (int i=0;i<numps;i++)
      dots[i]=JMaths.rnd(0,nts-1);

    float red=(float)JMaths.rnd();
    float green=(float)JMaths.rnd();
    float blue=(float)JMaths.rnd();
    float colspd=0.005f;
    Color cola=new Color(0.5f,0.5f,1.0f);
    Color colb=new Color(0.0f,0.6f,0.0f);
    while (true) {

      rotang+=Math.PI/1000.0;

      frame.gfx.clearRect(0,0,frame.getSize().width,frame.getSize().height);
      m=new Matrix(V3d.j,rotang);

      drawbox();

      for (int i=0;i<ts.length;i++) {
        ts[i].move();
        ts[i].ori.recalcMatrix();
        ts[i].draw(pp,frame.gfx,m);
/*        for (int j=0;j<ts.length;j++)
          if (i!=j)
            checkcollision(i,j);
  */      }

      for (int i=0;i<numps;i++) {
//        int j=JMaths.rnd(0,ts.length-1);
        int j=dots[i];
        p=ts[j].apply(p);

				V3d rotatedp=m.mult(p);
				float depth=0.5f+0.5f*(float)((1.0-rotatedp.z)/2.0);
				if (depth<0) depth=0.0f;
				if (depth>1) depth=1.0f;
				// System.out.println(""+depth);
        Point q=pp.project(rotatedp);
        // Point q=pp.project(m.mult(p));
/*        red=JMaths.crop(red+JMaths.rnd(-colspd,+colspd),0.0f,1.0f);
        green=JMaths.crop(green+JMaths.rnd(-colspd,+colspd),0.0f,1.0f);
        blue=JMaths.crop(blue+JMaths.rnd(-colspd,+colspd),0.0f,1.0f);*/
        if (q!=null) {
//          frame.gfx.setColor(new Color(red,green,blue));
          float t=(float)i/(float)(numps-1);
//          frame.gfx.setColor(new Color((int)(t*cola.getRed()+(1.0-t)*colb.getRed()),(int)(t*cola.getGreen()+(1.0f-t)*colb.getGreen()),(int)(t*cola.getBlue()+(1.0f-t)*colb.getBlue())));
//          frame.gfx.setColor(hue(t));
          frame.gfx.setColor(hue((float)rs[rem]/(float)nts,depth));
          rs[rem]=j;
          rem=(rem+1)%nrs;
          frame.gfx.drawLine(q.x,q.y,q.x,q.y);
        }
      }

      frame.update();

      try {
        this.sleep(1);
      } catch (Exception e) {
        System.out.println("Test.run(): "+e);
      }
    }
  }

  public void checkcollision(int i,int j) {
    IfsTransform a=ts[i];
    IfsTransform b=ts[j];
    double dist=V3d.dist(a.cen,b.cen);
    if (dist<a.rad+b.rad) {
      // m . dv = M . dV
      // Assume they hit on line through centres
      V3d atob=(b.cen.minus(a.cen)).norm();
      // Decompose momentum along this line
      double ain=a.vel.dot(atob);
      double bin=-b.vel.dot(atob);
      double aout=b.mass()*bin/a.mass();
      double bout=a.mass()*ain/b.mass();
      a.vel=a.vel.sub(atob.mult(ain+aout));
      b.vel=b.vel.add(atob.mult(bin+bout));
      V3d colpnt=a.cen.add(atob.mult(dist*a.rad/(a.rad+b.rad)));
      a.cen=colpnt.sub(atob.mult(a.rad));
      b.cen=colpnt.add(atob.mult(b.rad));
    }
  }

  public void drawbox() {
    frame.gfx.setColor(Color.gray);
    Point aaa=pp.project(m.mult(new V3d(-1.0,-1.0,-1.0)));
    Point baa=pp.project(m.mult(new V3d(+1.0,-1.0,-1.0)));
    Point aba=pp.project(m.mult(new V3d(-1.0,+1.0,-1.0)));
    Point bba=pp.project(m.mult(new V3d(+1.0,+1.0,-1.0)));
    Point aab=pp.project(m.mult(new V3d(-1.0,-1.0,+1.0)));
    Point bab=pp.project(m.mult(new V3d(+1.0,-1.0,+1.0)));
    Point abb=pp.project(m.mult(new V3d(-1.0,+1.0,+1.0)));
    Point bbb=pp.project(m.mult(new V3d(+1.0,+1.0,+1.0)));
    try {
      frame.gfx.drawLine(aaa.x,aaa.y,baa.x,baa.y);
      frame.gfx.drawLine(aaa.x,aaa.y,aba.x,aba.y);
      frame.gfx.drawLine(aaa.x,aaa.y,aab.x,aab.y);
      frame.gfx.drawLine(baa.x,baa.y,bba.x,bba.y);
      frame.gfx.drawLine(baa.x,baa.y,bab.x,bab.y);
      frame.gfx.drawLine(aba.x,aba.y,bba.x,bba.y);
      frame.gfx.drawLine(aba.x,aba.y,abb.x,abb.y);
      frame.gfx.drawLine(bab.x,bab.y,aab.x,aab.y);
      frame.gfx.drawLine(bbb.x,bbb.y,abb.x,abb.y);
      frame.gfx.drawLine(bbb.x,bbb.y,bab.x,bab.y);
      frame.gfx.drawLine(bbb.x,bbb.y,bba.x,bba.y);
      frame.gfx.drawLine(aab.x,aab.y,abb.x,abb.y);
    } catch (Exception e) {
      System.out.println("Problem drawing cube");
    }
  }

}
