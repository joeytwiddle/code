import java.lang.String;
import java.awt.*;
import java.awt.image.*;
import java.awt.event.*;

import jlib.Echoer;

public class ComponentBuffer {

  Echoer e=null;

  public Component com;
  public Image img;
  public Graphics gfx;
  private Graphics framegfx;

  ComponentBuffer(Component c) {
    com=c;
    // super(t);
    // setSize(w,h);
    // setVisible(true);
    // getbuffer();
  	/* addWindowListener(new WindowAdapter() {
	    public void windowClosing(WindowEvent e) {
	  	  System.exit(0);
	    }
	  } ); */
  }

  public void getbuffer() {
    try {
    echo("getbuffer - start");
    Object imgo=com.createImage(com.getSize().width,com.getSize().height);
    echo("Got img="+imgo);
    img=(Image)imgo;
    Object gfxo=img.getGraphics();
    echo("Got gfx="+gfxo);
    gfx=(Graphics)gfxo;
    framegfx=com.getGraphics();
    echo("getbuffer - end");
    } catch (Exception e) {
      echo(""+e);
    }
  }

  public void update() {
    update(framegfx);
    // com.update(framegfx);
  }

  public void update(Graphics g) {
//    System.out.println("U: "+img);
    if (g!=null && img!=null)
      g.drawImage(img,0,0,com);
  }

/*  public void paint(Graphics g) {
    System.out.println("P: "+img);
    if (g!=null && img!=null)
    g.drawImage(img,0,0,this);
  }*/

  public void echo(String s) {
    if (e!=null)
      e.echo(s);
    else
      System.out.println(s);
  }

}
