import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;

public class zoom extends Frame implements ActionListener {

  static int scrwid=320,scrhei=200;
  static int fs=8;
  static int large=255;

  int[][] img=new int[scrhei][scrwid],img2=new int[scrhei][scrwid],imgtmp=new int[scrhei][scrwid];
  double[][][][][] amount=new double[scrwid][scrhei][2][2][2];
  int[][][] pix=new int[scrwid][scrhei][2],piy=new int[scrwid][scrhei][2];
  boolean[] fon=new boolean[fs];
  double[] var=new double[fs],op=new double[fs],damp=new double[fs],force=new double[fs];
  int usingmap,makingmap,mmx,mmy,tmpmap;
  Random r=new Random();
  double xaspect,yaspect;

  int mysquare(int x) {
    return x*x;
  }

  double mysgn(double x) {
    if (x>0)
      return 1;
    if (x<0)
      return -1;
    return 0;
  }

  double myabs(double x) {
    return x*mysgn(x);
  }

  boolean rnd(int p) {
    int q=r.nextInt() % p;
    if (q<0)
        q=-q;
    if (q==0)
        return true;
    else
        return false;
  }

 double myrnd() {
    return r.nextDouble();
  }

  public void actionPerformed(ActionEvent e){
  }

  void perturb (int f) {
    var[f] = (var[f] - op[f]) * damp[f] + force[f] * (myrnd() - .5) + op[f];
  }
  
  void mycircle(int cx,int cy,int r,int c) {
    for (int x=-r;x<=r;x++) {
      int h=(int)Math.sqrt(mysquare(r)-mysquare(x));
      for (int y=-h;y<=h;y++) {
        int px=cx+x,py=cy+y;
        if (px>=0 && px<scrwid && py>=0 && py<scrhei)
          img[py][px]=(int)(myrnd()*255);
      }
    }
  }

  // Frame needs main function
  public static void main(String argv[]) {
    zoom z=new zoom("Zoom");
    z.show();
  }

    zoom(String title) {
	// Constructor, sets title, size etc.
  super(title);
  setSize(scrwid,scrhei);
	setResizable(false);
	setLayout(new FlowLayout());
	addWindowListener(new WindowAdapter() {
	    public void windowClosing(WindowEvent e) {
		System.exit(0);
	    }
	} );
	init();
    }

  public void init() {

    usingmap=0; makingmap=1; mmx=0; mmy=0;

    xaspect=getSize().width/scrwid;
    yaspect=getSize().height/scrhei;

    for (int y=0;y<scrhei;y++) {
      for (int x=0;x<scrwid;x++) {
        img[y][x]=(int)(255*myrnd());
//        img2[y][x]=(int)(myrnd()*255);
        pix[x][y][usingmap]=x;
        piy[x][y][usingmap]=y;
        if (x==scrwid-1)
          pix[x][y][usingmap]=x-2;
        if (y==scrhei-1)
          piy[x][y][usingmap]=y-2;
        for (int i=0;i<=1;i++) {
          for (int j=0;j<=1;j++) {
            amount[x][y][i][j][usingmap]=myrnd()/4;
          }
        }
      }

    }
    
    // 0 Accelerate
    op[0] = 1; damp[0] = .999; force[0] = .005;
    // 1 Velocity
    op[1] = 1.02; damp[1] = .999; force[1] = .01;
    // 2 Rotation
    op[2] = 0; damp[2] = .999; force[2] = .05;
    // 3 y splurge
    op[3] = 0; damp[3] = .999; force[3] = .01;
    // 4 Dribble
    op[4] = 1; damp[4] = 0; force[4] = .01;
    // 5 x splurge
    op[5] = 0; damp[5] = .999; force[5] = .01;
    op[6]=2;damp[6]=.9999;force[6]=.01;
    op[7]=1;damp[7]=.999;force[7]=.01;
    
    for (int f=0;f<fs;f++) {
      var[f] = op[f];
      fon[f]=true;
    }

    for (int i=0;i<scrwid*scrhei;i++) {
      moremap();
    }
    
    	// Set colours of frame
    setBackground(Color.black);
    setForeground(new Color((float)12/255,(float)104/255,(float)100/255));

    Thread t=new animthread(this,1);
    t.start();

  }

  public void paint(Graphics g) {

    for (int i=1;i<=5;i++) {
      mycircle((int)myrnd()*scrwid,(int)myrnd()*scrhei,(int)(2+myrnd()*8),(int)myrnd()*255);
    }
    for (int y=0; y<scrhei; y++) {
      for (int x=0; x<scrwid; x++) {
//        System.out.println("color "+img[y][x]);
        if (img[y][x]<128)
          g.setColor(new Color(2*img[y][x],0,0));
        else
          g.setColor(new Color(255,2*(img[y][x]-128),2*(img[y][x]-128)));
        g.drawRect(x,y,0,0);
      }
    }

    for (int f=0;f<fs;f++) {
      perturb(f);
    }
    // Generate some more of the map
    for (int maploop=1;maploop<scrwid*scrhei/20;maploop++) {
      moremap();
    }
    
    // Animate
    for (int x=0; x<scrwid; x++) {
      for (int y=0; y<scrhei; y++) {
        int c=0;
        for (int i=0;i<=1;i++) {
          for (int j=0;j<=1;j++) {
            c=(int)((float)(c+amount[x][y][i][j][usingmap]*img[piy[x][y][usingmap]+j][pix[x][y][usingmap]+i]));
          }
        }
        c--;
//        if (c==0)
//          System.out.println(""+x+" "+y+" 0 . ");
        if (c<0)
          c=255;
        if (c>255)
          c=0;
        img2[y][x]=c;
      }
    }


    imgtmp=img;
    img=img2;
    img2=imgtmp;

  }
  
  void moremap() {
      double rx=(double)mmx/scrwid*2-1;
      double ry=((double)mmy-scrhei/2)/scrwid*2;
      if (fon[0]) {
        rx = mysgn(rx)/var[7]*Math.pow(myabs(rx),1/var[0]);
        ry = mysgn(ry)/var[7]*Math.pow(myabs(ry),1/var[0]);
      }
      if (fon[1]) {
        rx = rx / var[1]; ry = ry / var[1];
      }
      if (fon[2]) {
        double nrx = rx * Math.cos(var[2]) + ry * Math.sin(var[2]);
        double nry = -rx * Math.sin(var[2]) + ry * Math.cos(var[2]);
        rx = nrx; ry=nry;
      }
      if (fon[3]) {
        ry = ry - mysgn(ry) * Math.sin(var[6]*Math.PI*myabs(ry)) * var[3];
      }
      if (fon[4]) {
        ry = ((myabs(ry) - 1) / var[4] + 1) * mysgn(ry);
      }
      if (fon[5]) {
        rx = rx - mysgn(rx) * Math.sin(var[6]*Math.PI*myabs(rx)) * var[5];
      }
      double px=(rx+1)/2*scrwid;
      double py=scrhei/2+(ry)/2*scrwid;
      int ix=(int)px;
      int iy=(int)py;
      if (ix<0 || ix>=scrwid-1 || iy<0 || iy>=scrhei-1) {
        ix=(int)px; iy=(int)py;
      }
      amount[mmx][mmy][0][0][makingmap]=((double)ix+1-(double)px)*((double)(iy+1)-(double)py);
      amount[mmx][mmy][1][0][makingmap]=((double)px-(double)ix)*((double)(iy+1)-(double)py);
      amount[mmx][mmy][0][1][makingmap]=((double)ix+1-(double)px)*((double)py-(double)iy);
      amount[mmx][mmy][1][1][makingmap]=((double)px-(double)ix)*((double)py-(double)iy);
      pix[mmx][mmy][makingmap]=ix;
      piy[mmx][mmy][makingmap]=iy;
      if (ix<0 || ix>=scrwid-1 || iy<0 || iy>=scrhei-1) {
        pix[mmx][mmy][makingmap]=scrwid/2;
        piy[mmx][mmy][makingmap]=scrhei/2;
        for (int i=0;i<=1;i++) {
          for (int j=0;j<=1;j++) {
            amount[mmx][mmy][i][j][makingmap]=0;
          }
        }
      }
      mmx++;
      if (mmx>=scrwid) {
        mmx=0;
        mmy++;
        if (mmy>=scrhei) {
          mmy=0;
          tmpmap=usingmap;
          usingmap=makingmap;
          makingmap=tmpmap;
          for (int f=0;f<fs;f++) {
            perturb(f);
          }
        }
      }
    }
}

class mythread extends Thread implements Serializable {
    zoom z;
    int sleeptime;
    // Constructor stores info
    mythread(zoom temp1,int temp3) {
  z=temp1;
	sleeptime=temp3;
    }
    public void run() {
	// Keep looping
	while (true) {
	    // Call the moment method at each loop
	    this.moment();
	    // Then sleep for sleeptime before going around again
	    try {
		this.sleep(sleeptime);
	    }
	    catch (Exception e) {
		System.out.println("Oh dear: "+(e));
		System.exit(0);
	    }
	}
    }
    // Overwritten when extended
    void moment() {
    }
}

class animthread extends mythread {
    animthread(zoom temp1,int temp3) {
  super(temp1,temp3);
    }
    void moment() {
  z.repaint();
    }
}


