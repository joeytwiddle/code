import java.lang.String;
import java.awt.*;
import java.awt.image.*;
import java.awt.event.*;

public class BufferedFrame extends Frame {

	public BufferedImage img;
	public Graphics2D gfx;
	private Graphics framegfx;

	BufferedFrame(String t,int w,int h) {
		super(t);
		setSize(w,h);
		setVisible(true);
		getbuffer();
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				System.exit(0);
			}
		} );
	}

	public void getbuffer() {
		img=(BufferedImage)createImage(getSize().width,getSize().height);
		gfx=(Graphics2D)img.getGraphics();
		framegfx=getGraphics();
	}

	public void update() {
		update(framegfx);
	}

	public void update(Graphics g) {
		//    System.out.println("U: "+img);
		if (g!=null && img!=null)
			g.drawImage(img,0,0,this);
	}

	/*  public void paint(Graphics g) {
		 System.out.println("P: "+img);
		 if (g!=null && img!=null)
		 g.drawImage(img,0,0,this);
		 }*/

}
