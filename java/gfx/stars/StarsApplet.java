import java.applet.Applet;
import java.awt.Label;
import java.awt.*;
import java.awt.Graphics;

import jlib.Echoer;
import Stars;

public class StarsApplet extends Applet implements Echoer {

  int size=Stars.size;
  TextArea textarea;
  Stars s=null;

  public void init() {
//    setSize(size,size);
/*    textarea=new TextArea("Initial label..............................................");
    add(textarea); */
  }

  public void start() {

    try {

    echo("Creating buffer");

    /*Panel p=new Panel();
    p.setSize(size,size);
    add(p);*/
    ComponentBuffer buffer=new ComponentBuffer(this);
    buffer.e=this;
    // buffer.setSize(size,size);
    setBackground(Color.black);
    // buffer.setVisible(true);
    // add(buffer);

    echo("Creating stars");

    s=new Stars(buffer);
    s.echoer=this;
    echo("Starting stars");
    s.start();

    } catch (Exception e) {
      echo(""+e);
    }

  }

  public void update(Graphics g) {
    if (s!=null)
      s.buffer.update(g);
  }

  public void echo(String s) {
//    textarea.appendText(s+"\n");
  }

}