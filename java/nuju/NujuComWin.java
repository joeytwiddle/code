package nuju;

import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;
import java.util.*;
import java.lang.*;
import java.lang.reflect.*;

import jlib.*;

import nuju.NujuFrame;

class ArgPair {
  public Class type;
  public Object obj=null;
  ArgPair(Class t) {
    type=t;
    if (type.toString().equals("int"))
      type=JReflect.classcalled("java.lang.Integer");
  }
}

class NujuTextField extends TextField implements ActionListener {
  Class desttype=null;
  NujuTextField() {
    super("...");
  }
  NujuTextField(String starttext) {
    super(starttext);
  }
  NujuTextField(Class dt) {
    super("...");
    desttype=dt;
//    setActionCommand("Click");
  }
  public static boolean canconvert(Class c) {
    return stringConstructor(c)!=null;
  }
  public static Constructor stringConstructor(Class c) {
    try {
      Class[] cs=new Class[1];
      cs[0]="".getClass();
      Constructor co=c.getConstructor(cs);
      return co;
    } catch (Exception e) {
      return null;
    }
  }
  public void actionPerformed(ActionEvent e){
    Constructor c=stringConstructor(desttype);
    try {
      Object[] as=new Object[1];
      as[0]=getText();
      Object o=c.newInstance(as);
      if (o!=null)
        NujuComWin.provideObject(o);
    } catch (Exception ex) {
      System.out.println("processActionEvent(): "+ex);
    }
  }
/*  protected void processEvent(AWTEvent e) {
    System.out.println("processActionEvent("+e+")");
    if (JReflect.canbe(e,"java.awt.event.KeyEvent")) {
      KeyEvent ke=(KeyEvent)e;
      if (ke.getKeyCode()==Event.ENTER) {
      }
    }
  }*/
}

class NujuComWin extends Frame implements ActionListener {
  static Vector components=new Vector();
  static Object o=null;
  static Method m=null;
  static Vector args=new Vector();
  static Vector spareargs=new Vector();
  static NujuTextField classstarter;
  static Panel leftpanel,middlepanel,rightpanel;
  static Label messagetext;
  static NujuComWin me;
  static Vector tmp=new Vector();

  NujuComWin() {
  	
  	// Constructor, sets title, size etc.
	  super("Command Window");
	  me=this;
	  try {
	  provideMethod(this.getClass().getMethod("execute",new Class[0]));
	  } catch (Exception e) {
	  }
  	System.out.println("Constructing new NujuComWin");
	  setSize(400,120);
  	setResizable(true);
  	GridBagLayout lay=new GridBagLayout();
	  setLayout(lay);
	  addWindowListener(new WindowAdapter() {
      public void windowClosing(WindowEvent e) {
	    	dispose();
	    	System.exit(0);
      }
    } );

    classstarter=new NujuTextField("path.of.Class");
    classstarter.addActionListener(this);
    add(classstarter);
    lay.setConstraints(classstarter,simpleconstraints(0,1,1,1));
    leftpanel=new Panel();
    middlepanel=new Panel();
    rightpanel=new Panel();
//    Label l7=new Label("Hello");
//    tmp.add(l7);
//    add(l7);
    lay.setConstraints(leftpanel,simpleconstraints(0,0,2,1));
    leftpanel.setLayout(lay);
    add(leftpanel);
    lay.setConstraints(middlepanel,simpleconstraints(2,0,2,1));
    middlepanel.setLayout(lay);
    add(middlepanel);
    lay.setConstraints(rightpanel,simpleconstraints(4,0,6,1));
    rightpanel.setLayout(lay);
    add(rightpanel);
    messagetext=new Label("Welcome                                               ");
    lay.setConstraints(messagetext,simpleconstraints(1,1,9,1));
    add(messagetext);
    recompile();

    setVisible(true);
  }
  public void execute() {
    message("Executing "+o+"."+m+"("+argarray()+")");
      try {
        Nuju.present(m.invoke(o,argarray()));
      } catch (Exception x) {
        System.out.println("Couldn't invoke: "+x);
      }
    o=null;
    m=null;
    for (int i=0;i<args.size();i++)
      ((ArgPair)args.get(i)).obj=null;;
    recompile();
  }
  public void actionPerformed(ActionEvent e){
    String command=e.getActionCommand();
    if (command.equals("Execute")) {
      execute();
    } else if (e.getSource()==classstarter) {
      String type=classstarter.getText();
      try {
      Class c=JReflect.classcalled(type);
      if (c==null) {
        message("Class "+type+" does not exist.");
      } else {
        Constructor co=c.getConstructor(new Class[0]);
        Nuju.present(co.newInstance(new Object[0]));
      }
      } catch (Exception ex) {
        message(""+ex);
      }
    }
  }
  public static void message(String m) {
    System.out.println(m);
    messagetext.setText(m);
  }
  public static void present(Object j) {
    NujuFrame f=new NujuFrame(j);
    f.show();
  }
  private static GridBagConstraints simpleconstraints(int x,int y,int w,int h) {
    GridBagConstraints gbc=new GridBagConstraints();
    gbc.gridx=x;
    gbc.gridy=y;
    gbc.gridwidth=w;
    gbc.gridheight=h;
    gbc.weightx=(double)0.0;
    gbc.weighty=(double)0.0;
    gbc.insets=new Insets(0,0,0,0);
    gbc.ipadx=0;
    gbc.ipady=0;
    return gbc;
  }

  public static void providesourceObject(Object oo) {
    o=oo;
    System.out.println("Adding source object "+oo);
    if (m!=null) {
      boolean good=true;
      try {
        Method m2=o.getClass().getMethod(m.getName(),m.getParameterTypes());
      } catch (Exception e) {
        good=false;
        System.out.println("ComWin.providesourceObject(): Couldn't use method "+m+" with object "+o);
      }
      if (!good)
        m=null;
    }
    recompile();
  }

  public static void provideObject(Object oo) {
    // Find a suitable empty space for it
    System.out.println("You want me to use Object "+oo);
    boolean done=false;
    for (int i=0;i<args.size() && !done;i++) {
      ArgPair ap=(ArgPair)args.get(i);
      System.out.println("Seeing if "+ap.type+" is a type for "+oo+" (with type "+oo.getClass()+")");
//      if (ap.type.isInstance(oo) && ap.obj==null) {
      if (JReflect.canbe(oo,ap.type)) {
        ap.obj=oo;
        done=true;
      }
    }
    // Overwrite any suitable space
    for (int i=0;i<args.size() && !done;i++) {
      ArgPair ap=(ArgPair)args.get(i);
      if (ap.type.isInstance(oo)) {
        ap.obj=oo;
        done=true;
      }
    }
    if (!done) {
      System.out.println("I didn't find a place for the Object");
      spareargs.add(oo);
    }
    recompile();
  }

  public static void provideMethod(Method mm) {
    m=mm;
    if (o!=null) {
      boolean good=true;
      try {
        Method m2=o.getClass().getMethod(m.getName(),m.getParameterTypes());
        System.out.println("Accepted method "+m);
      } catch (Exception e) {
        good=false;
        System.out.println("ComWin.provideMethod(): Couldn't use method "+m+" on object "+o);
      }
      if (!good)
        m=null;
    }
    args.clear();
    Class[] ts=m.getParameterTypes();
    for (int i=0;i<ts.length;i++)
      args.add(new ArgPair(ts[i]));
    recompile();
  }

  public static void provide(Object oo,Method mm) {
    o=oo; // Could do providesourceObject
    provideMethod(mm);
    recompile();
  }

  static void recompile() {

    // Redraw window
//    me.removeAll();
    if (o!=null) {
//      me.remove(leftpanel);
      leftpanel.removeAll();
      leftpanel.add(new Label(NujuFrame.neaten(""+o.getClass())+" "+o));
//      me.add(new Label("Hello"));
//      leftpanel.add(new Label("Left"));
      ((GridBagLayout)me.getLayout()).setConstraints(leftpanel,simpleconstraints(0,0,2,1));
      leftpanel.setLayout(me.getLayout());
//      me.add(leftpanel);
    }
    if (m!=null) {
//      me.remove(middlepanel);
      middlepanel.removeAll();
      Button b=new Button("."+m.getName());
      b.setActionCommand("Execute");
      b.addActionListener(me);
      middlepanel.add(b);
      ((GridBagLayout)me.getLayout()).setConstraints(middlepanel,simpleconstraints(2,0,2,1));
      middlepanel.setLayout(me.getLayout());
//      me.add(middlepanel);
    }
//    me.remove(rightpanel);
    rightpanel.removeAll();
    for (int i=0;i<args.size();i++) {
      ArgPair a=(ArgPair)args.get(i);
      rightpanel.add(new Label(""+NujuFrame.neaten(""+a.type)+" "+a.obj));
      if (NujuTextField.canconvert(a.type)) {
        NujuTextField ntf=new NujuTextField(a.type);
        ntf.addActionListener(ntf);
        rightpanel.add(ntf);
      }
      ((GridBagLayout)me.getLayout()).setConstraints(rightpanel,simpleconstraints(4,0,6,1));
      rightpanel.setLayout(me.getLayout());
    }
 //   me.add(rightpanel);

//    System.out.println("Trying to update graphics.");
    me.setSize(400+(int)(Math.random()*20),120+(int)(Math.random()*10));
//    leftpanel.repaint();
//    middlepanel.repaint();
//    rightpanel.repaint();
//    me.pack();
    me.repaint();
//    me.update(me.getGraphics());
//    me.paint(me.getGraphics());

    // Check if ready to run
    // put at end so user can see intermediary stage
    if (o!=null)
      if (m!=null) {
        boolean done=true;
        for (int i=0;i<args.size();i++)
          if (((ArgPair)args.get(i)).obj==null)
            done=false;
        if (done)
          me.execute();
      }

  }

/*  public void repaint() {
    super.repaint();
    Component[] cs=getComponents();
    for (int i=0;i<cs.length;i++)
      cs[i].repaint();
  }

  public void update(Graphics g) {
    super.update(g);
    Component[] cs=getComponents();
    for (int i=0;i<cs.length;i++)
      cs[i].update(g);
  }*/

  Object[] argarray() {
    Object[] as=new Object[args.size()];
    for (int i=0;i<args.size();i++)
      as[i]=((ArgPair)args.get(i)).obj;
    return as;
  }

}
