import java.lang.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.color.*;

import java.awt.Shape;
import java.awt.geom.PathIterator;
import java.awt.geom.Rectangle2D;
import java.awt.font.GlyphVector;
import java.awt.font.FontRenderContext;

import jlib.*;

	class StarsWindowAdapter extends WindowAdapter {
			Stars tt;
			public StarsWindowAdapter(Stars t) {
			  tt=t;
			}
      public void windowClosing(WindowEvent e) {
        // System.exit(0);
				tt.stillgoing=false;
      }
	}

public class Stars extends Thread implements ComponentListener {

  public static final int size=400;
	volatile boolean change=false;
  java.util.Vector points=new java.util.Vector();
  java.util.Vector randomPoints=new java.util.Vector();
  java.util.Vector sparePoints=new java.util.Vector();

  boolean hideText=false; // radiates the points outwards after coherence to hide them better
  int whichText;

  Echoer echoer=null;

  Frame frame;
  ComponentBuffer buffer;
  PPImage pp;

  V3d p=V3d.o;
  int numps=0;

  int nrs=1;
  int[] rs=new int[nrs];
  int rem=0;

  double rotspd=Math.PI/600.0;
  double rotang=-40.0*rotspd*JMaths.rnd(1.0,2.0);
  V3d rotaxis=V3d.random();
  Matrix m;

	boolean stillgoing=true;

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

  public Color hue(float a) {
    return new Color(huefor(a,0),huefor(a,(float)1/3),huefor(a,(float)2/3));
  }

  public void componentHidden(ComponentEvent e) {
  }

  public void componentMoved(ComponentEvent e) {
  }

  public void componentResized(ComponentEvent e) {
    echo("Resized! "+e);
//    buffer.getbuffer();
//    pp=new PPImage(buffer.img,3.0);
  }

  public void componentShown(ComponentEvent e) {
  }

  public final static void main(String[] args) {
    Frame frame=new Frame("Stars");
    frame.setSize(size,size);
    frame.setBackground(Color.black);

    ComponentBuffer buffer=new ComponentBuffer(frame);
    // BufferedComponent buffer=new BufferedComponent("",size,size);
    // frame.add(buffer);
    frame.show();

    Stars t=new Stars(buffer);
    
    frame.addComponentListener(t);
		frame.addWindowListener(new StarsWindowAdapter(t));

    t.start();
  }

  public Stars(ComponentBuffer bc) {
    buffer=bc;
  }

  public void justone() {

      // Graphics2D gfx=(Graphics2D)g;
      // GlyphVector gv=font.createGlyphVector(gfx.getFontRenderContext(),"NEURALYTE!");
      Graphics gfx=buffer.gfx;
      // Graphics2D gfx2d=new Graphics2D();
      // Font font = new Font("Courier",Font.PLAIN,1);
      // Font font = new Font("Times Roman",Font.PLAIN,14);
      Font font = buffer.com.getFont();
			// font=font.deriveFont(40);
      echo("Got font "+font);
      FontRenderContext frc=new FontRenderContext(null,true,true);
      // FontRenderContext frc=gfx2d.getFontRenderContext();
      echo("Got FRC "+frc);
      GlyphVector gv=font.createGlyphVector(frc,"HAPPY 2002");
      Shape s=gv.getOutline();
      echo("The shape is a "+s);

      Rectangle2D bounds=s.getBounds2D();

			for (int loop=0;loop<2;loop++) {
				java.util.List addPoints=( loop==0 ? points : sparePoints );
				int spacing=4;
				int height=12;
				int width=(int)(height*bounds.getWidth()/bounds.getHeight());
        for (int x=0;x<width;x++)
        for (int y=0;y<height;y++)
        if (JMaths.rnd(1,1)==1) {
          double xx=bounds.getX()+x*bounds.getWidth()/width;
          double yy=bounds.getY()+y*bounds.getHeight()/height;
          if (s.contains(xx,yy)) {
            // V3d v=pp.projectOut(pp.hscrwid+x-width/2,pp.hscrhei+y-height/2,JMaths.rnd(-1.0f,1.0f));
            V3d v=pp.projectOut(pp.hscrwid+spacing*(x-width/2),pp.hscrhei+spacing*(y-height/2),JMaths.rnd(-1.0f,1.0f));
            // echo(""+v);
            addPoints.add(new V3d(-v.x,-v.y,-v.z));
          }
        }
        whichText=points.size();
			}

      // Add some random stars
      for (int i=0;i<1000;i++)
        randomPoints.add(V3d.randomcube());
			points.addAll(randomPoints);
			
      echo("Setting up done");

			new Thread() {
				public void run() {
					String dummy=CommandLine.getUserInput();
					change=true;
			// frame.setActionListener(new ActionListener() {
				// public void actionPerformed(ActionEvent e) {
				}
			// });
		  }.start();

  }

  public void run() {

    echo("Stars run");

    echo("A");
    buffer.getbuffer();
    echo("B >"+buffer.img+"<");
    pp=new PPImage(buffer.img,size,size,3.0);
    echo("C");

    echo("here");

    justone();

    float red=(float)JMaths.rnd();
    float green=(float)JMaths.rnd();
    float blue=(float)JMaths.rnd();
    float colspd=0.005f;
    Color cola=new Color(0.5f,0.5f,1.0f);
    Color colb=new Color(0.0f,0.6f,0.0f);

		while (stillgoing) {

			if (change) {
				points=new java.util.Vector();
				points.addAll(sparePoints);
				points.addAll(randomPoints);
				change=false;
			}

      // echo("loop "+buffer.gfx);

      rotang+=rotspd;

      // echo("size: "+buffer.com.getSize());

      buffer.gfx.clearRect(0,0,buffer.com.getSize().width,buffer.com.getSize().height);
      // V3d rotax=new Matrix(rotaxis,rotang).mult(V3d.i);
      m=new Matrix(rotaxis,rotang);

      buffer.gfx.setColor(Color.white);
      for (int i=0;i<points.size();i++) {
  
        V3d v=(V3d)points.get(i);
  
        if (hideText && i<whichText) {
          int j=(i+points.size()/2) % points.size();
          V3d vb=(V3d)points.get(j);
          v=v.add(vb.subtract(v).mult(rotang));
        }
  
        Point pnt=pp.project(m.mult(v));
        if (pnt!=null)
          buffer.gfx.drawLine(pnt.x,pnt.y,pnt.x+1,pnt.y);
      }

      drawbox();

      buffer.update();

      try {
        this.sleep(100);
      } catch (Exception e) {
        echo("Test.run() sleeping: "+e);
      }
    
		}

		// frame.hide();
		// frame.dispose();
		this.stop();
		
  }

  public void drawbox() {
    buffer.gfx.setColor(Color.gray);
    Point aaa=pp.project(m.mult(new V3d(-1.0,-1.0,-1.0)));
    Point baa=pp.project(m.mult(new V3d(+1.0,-1.0,-1.0)));
    Point aba=pp.project(m.mult(new V3d(-1.0,+1.0,-1.0)));
    Point bba=pp.project(m.mult(new V3d(+1.0,+1.0,-1.0)));
    Point aab=pp.project(m.mult(new V3d(-1.0,-1.0,+1.0)));
    Point bab=pp.project(m.mult(new V3d(+1.0,-1.0,+1.0)));
    Point abb=pp.project(m.mult(new V3d(-1.0,+1.0,+1.0)));
    Point bbb=pp.project(m.mult(new V3d(+1.0,+1.0,+1.0)));
    try {
      buffer.gfx.drawLine(aaa.x,aaa.y,baa.x,baa.y);
      buffer.gfx.drawLine(aaa.x,aaa.y,aba.x,aba.y);
      buffer.gfx.drawLine(aaa.x,aaa.y,aab.x,aab.y);
      buffer.gfx.drawLine(baa.x,baa.y,bba.x,bba.y);
      buffer.gfx.drawLine(baa.x,baa.y,bab.x,bab.y);
      buffer.gfx.drawLine(aba.x,aba.y,bba.x,bba.y);
      buffer.gfx.drawLine(aba.x,aba.y,abb.x,abb.y);
      buffer.gfx.drawLine(bab.x,bab.y,aab.x,aab.y);
      buffer.gfx.drawLine(bbb.x,bbb.y,abb.x,abb.y);
      buffer.gfx.drawLine(bbb.x,bbb.y,bab.x,bab.y);
      buffer.gfx.drawLine(bbb.x,bbb.y,bba.x,bba.y);
      buffer.gfx.drawLine(aab.x,aab.y,abb.x,abb.y);
    } catch (Exception e) {
      echo("Problem drawing cube");
    }
  }

  public void echo(String s) {
    if (echoer==null)
      System.out.println(s);
    else
      echoer.echo(s);
  }

}
