package nujutu;

// import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
// import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import nuju.*;
import jlib.*;

class PropertyBrowser extends SimpleTable {

	PropertyBrowser(Class c,Object o) {
		super(4,4);
		Field[] fs=c.getFields();
		set(new Integer(1),0,2);
		set("a string",1,1);
		set(new JButton("hello"),2,0);
	}

}



class Desktop extends JPanel {

}

class InfoPane extends JTabbedPane {

	String[] tabNames={
		"Static Properties",
		"Static Methods",
		"Constructors",
		"Properties",
		"Methods",
		"Private Properties",
		"Private Methods"
	};
	JPanel[] tabs;

	InfoPane() {
		tabs=new JPanel[tabNames.length];
		for (int i=0;i<tabs.length;i++) {
			tabs[i]=new JPanel();
			addTab(tabNames[i],tabs[i]);
		}
	}

	void introspect(Class c,Object o) {
		tabs[0].removeAll();
		tabs[0].add(new JButton("wicked"));
		tabs[0].add(new PropertyBrowser(c,o));
	}

	void introspect(Class c) {
		introspect(c,null);
	}

	void introspect(Object o) {
		introspect(o.getClass(),o);
	}

}

public class MainFrame extends JFrame {

	Desktop desktop=new Desktop();
	InfoPane infoPane=new InfoPane();

	MainFrame() {
		super("NuJuTu");
		desktop=new Desktop();
		infoPane=new InfoPane();
		Container cp=getContentPane();
		cp.add(desktop);
		cp.add(infoPane);
		infoPane.introspect("hello");
	}

  public static void main(String[] args) {
		ArgParser a=new ArgParser(args);
		a.done();
		MainFrame mf=new MainFrame();
		mf.setSize(800,600);
		mf.setVisible(true);
	}

}
