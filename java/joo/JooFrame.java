import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;
import java.util.*;

import Joo.*;

class JooFrame extends Frame implements ActionListener {
  Vector components=new Vector();
  Joo o;
  JooFrame(Joo obj) {
  	// Constructor, sets title, size etc.
	  super(obj.type());
  	o=obj;
	  setSize(300,250);
  	setResizable(false);
  	  setLayout(new FlowLayout());
	  addWindowListener(new WindowAdapter() {
      public void windowClosing(WindowEvent e) {
	    	System.exit(0);
      }
    } );
    Vector fs=o.fields();
    for (int i=0;i<fs.size();i++) {
      Field f=(Field)fs.get(i);
      components.add(new Label(f.type));
    	Button b=new Button(f.name);
	    b.setActionCommand(f.name);
    	b.addActionListener(this);
      components.add(b);
      Object tmp=obj.getfield(f.name);
      System.out.println("Got field "+tmp);
      if (tmp!=null)
        components.add(new TextField(tmp.toString()));
    }
//	  components.add(new TextField("hello"));
  	for (int i=0;i<components.size();i++)
	    add((Component)components.get(i));
  }
  public void actionPerformed(ActionEvent e){
    String command=e.getActionCommand();
    System.out.println("Action "+command+" performed.\n");
    Vector fs=o.fields();
    for (int i=0;i<fs.size();i++) {
      Field f=(Field)fs.get(i);
      if (f.name.equals(command)) {
        Object obj=o.getfield(command);
      	JooFrame jf=new JooFrame((Joo)obj);
    	  jf.show();
      }
    }
  }
  public static void present(Joo j) {
    JooFrame f=new JooFrame(j);
    f.show();
  }
}

