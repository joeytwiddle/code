package nujutu;

// import java.applet.*;
import java.awt.Container;
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

class MemberBrowser extends SimpleTable {

	MemberBrowser(Class cls,Object o,
		List members,
		boolean stat,boolean notstat,
		boolean priv,boolean notpriv,
		boolean field,
		boolean method,
		boolean constructor
	) {
		super(2,2);
		// set(new Integer(1),0,2);
		// set(new JButton("hello"),2,0);
		int row=0;
		for (int i=0;i<members.size();i++) {
			try {
				Member member=(Member)members.get(i);
				if ( ! (
					( stat && ! Modifier.isStatic(member.getModifiers()) )
						||
					( notstat && Modifier.isStatic(member.getModifiers()) )
						||
					( priv && ! Modifier.isPrivate(member.getModifiers()) )
						||
					( notpriv && Modifier.isPrivate(member.getModifiers()) )
						||
					( field && ! (member instanceof Field) )
						||
					( method && ! (member instanceof Method) )
						||
					( constructor && ! (member instanceof Constructor) )
				) ) {
					int col=0;
					if (member instanceof Field) {
						set(((Field)member).getType().toString(),col++,row);
					}
					if (member instanceof Method) {
						set(((Method)member).getReturnType().toString(),col++,row);
					}
					set(member.getName()+(member instanceof Field?"":"("),col++,row);
					if (member instanceof Field) {
						set("=",col++,row);
						set(((Field)member).get(o).toString(),col++,row);
					} else {
						Class[] params=( member instanceof Method
							? ((Method)member).getParameterTypes()
							: ((Constructor)member).getParameterTypes()
						);
						for (int j=0;j<params.length;j++) {
							set(params[j].getName(),col++,row);
						}
						set(")",col++,row);
					}
					members.remove(member);
					i--;
					row++;
				}
			} catch (Exception e) {
				System.err.println(""+e);
			}
		}
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
		List members=new Vector();
		jlib.JList.addArray(members,c.getFields());
		jlib.JList.addArray(members,c.getConstructors());
		jlib.JList.addArray(members,c.getMethods());
		// tabs[0].add(new JButton("wicked"));
		tabs[0].removeAll();
		tabs[0].add(new JScrollPane(new MemberBrowser(c,o,members,true,false,false,true,true,false,false)));
		tabs[1].add(new JScrollPane(new MemberBrowser(c,o,members,true,false,false,true,false,true,false)));
		tabs[2].add(new JScrollPane(new MemberBrowser(c,o,members,false,false,false,true,false,false,true)));
		tabs[3].add(new JScrollPane(new MemberBrowser(c,o,members,false,false,false,true,true,false,false)));
		tabs[4].add(new JScrollPane(new MemberBrowser(c,o,members,false,false,false,true,false,true,false)));
		tabs[5].add(new JScrollPane(new MemberBrowser(c,o,members,false,false,true,false,true,false,false)));
		tabs[6].add(new JScrollPane(new MemberBrowser(c,o,members,false,false,true,false,false,true,false)));
		System.out.println(""+members.size());
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
