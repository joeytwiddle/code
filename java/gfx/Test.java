import java.lang.*;
import java.awt.*;

public class Test extends Thread {

  BufferedFrame frame;

  public final static void main(String[] args) {
    Test t=new Test();
    t.start();
  }

  public Test() {
    init();
  }

  public void init() {
    frame=new BufferedFrame(" g f x test ",320,200);
  }

  public void run() {
    Dimension d=new Dimension(320,200);
    while (true) {
      for (int i=0;i<20;i++)
        frame.gfx.drawLine((int)(d.width*Math.random()),(int)(d.height*Math.random()),(int)(d.width*Math.random()),(int)(d.height*Math.random()));
//      frame.repaint();
//      frame.update(frame.getGraphics());
      frame.update();
/*      try {
        this.sleep(100);
      } catch (Exception e) {
        System.out.println("Test.run(): "+e);
      }
  */  }
  }

}