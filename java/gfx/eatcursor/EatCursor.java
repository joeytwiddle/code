import java.applet.*;
import java.awt.*;
import java.awt.image.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;

import jlib.Files;

public class EatCursor extends Applet implements ActionListener {

  public String message="=)";

  public void init() {
  	setBackground(Color.white);
	  setForeground(new Color((float)12/255,(float)104/255,(float)100/255));
	
	  setLayout(new FlowLayout());
	
	  setCursor(new Cursor(Cursor.CROSSHAIR_CURSOR));
	
	  Cursor c=null;
	
	  try {
/*  	  Cursor c=(Cursor)java.security.AccessController.doPrivileged(
  	    new java.security.PrivilegedExceptionAction() {
  	      public Object run() throws Exception {
        	  Toolkit toolkit=Toolkit.getDefaultToolkit();
        	  Image image=new BufferedImage(32,32,BufferedImage.TYPE_BYTE_BINARY);
  	        return toolkit.createCustomCursor(image,new Point(20,20),"tmp");
  	        return null;
  	      }
  	    }
  	  );*/
  	  Toolkit toolkit=Toolkit.getDefaultToolkit();
//   	  Image image=toolkit.createImage("/home/joey/cursor.gif");
   	  Image image=new BufferedImage(32,32,BufferedImage.TYPE_BYTE_BINARY);
//      c=toolkit.createCustomCursor(image,new Point(16,16),"tmp");
    	if (c!=null)
        setCursor(c);
	  } catch (Exception e) {
	    showStatus("Error: "+e);
	    message="Error: "+e;
	  }
	
	  Button b=new Button("Hello");
	  add(b);
	  b.addActionListener(this);
  	
  }

  public void start() {
  }

  public void stop() {

  }

  public void actionPerformed(ActionEvent e){
    String command=e.getActionCommand();
    Cursor c=getCursor();
    int t=c.getType();
    message=""+t;
    c=new Cursor(t+1);
    setCursor(c);
    repaint();
  }

  public void paint(Graphics g) {
	  Color fg=getForeground();
    Color bg=getBackground();
    g.drawString(message,5,50);
  }

}
