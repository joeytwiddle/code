import java.lang.String;
import java.awt.*;
import java.awt.image.*;
import java.awt.event.*;

public class BufferedPanel extends Panel {

  public BufferedImage img;
  public Graphics2D gfx;
  private Graphics framegfx;
  private Container src;

  BufferedPanel(Container s) {
    src=s;
  }

  BufferedPanel(String t,int w,int h) {
    // super(t);
    setSize(w,h);
    setVisible(true);
//    getbuffer();
  	/* addWindowListener(new WindowAdapter() {
	    public void windowClosing(WindowEvent e) {
	  	  System.exit(0);
	    }
	  } ); */
	  src=this;
  }

  /** This must not be called until the BufferedPanel has been
   *  added to its parent.  Otherwise, the call to createImage will
   *  return null!
  **/
  public void getbuffer() {
    System.out.println("A");
    img=(BufferedImage)src.createImage(getSize().width,getSize().height);
    System.out.println("B: "+img);
    gfx=(Graphics2D)img.getGraphics();
    System.out.println("C");
    framegfx=src.getGraphics();
    System.out.println("D");
  }

  public void update() {
    update(framegfx);
  }

  public void update(Graphics g) {
//    System.out.println("U: "+img);
    if (g!=null && img!=null)
      g.drawImage(img,0,0,src);
  }

/*  public void paint(Graphics g) {
    System.out.println("P: "+img);
    if (g!=null && img!=null)
    g.drawImage(img,0,0,this);
  }*/

}