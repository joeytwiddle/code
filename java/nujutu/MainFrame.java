package nujutu;

// import java.applet.*;
import java.awt.Component;
import java.awt.Container;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
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

class NuJuTu {

	static String niceNameString(String n) { // Strip package path
		int i=n.lastIndexOf(".");
		if (i>=0)
			n=n.substring(i+1);
		return n;
	}

	static String niceName(Class c) {
		return niceName(c,true);
	}

	static String niceName(Class c,boolean trimPackagePath) {
		String n=c.getName();
		if (c.isArray()) {
			String[] encoding={"B","byte","C","char","D","double","F","float","I","int","J","long","S","short","Z","boolean"};
			java.util.Map tmp=new HashMap();
			for (int i=0;i<encoding.length;i+=2)
				tmp.put(encoding[i],encoding[i+1]);
			String post="";
			int order=0;
			while (n.charAt(0)=='[') {
				n=n.substring(1);
				order++;
				post+="[]";
			}
			Object prim=tmp.get(n.substring(0,1));
			if (prim==null) {
				String obj=n.substring(1);
				n=obj;
			} else {
				n=(String)prim;
			}
			n=n+post;
			// System.out.println("Array: "+c+"\n  "+c.getName());
		}
		if (trimPackagePath)
			n=niceNameString(n);
		return n;
	}

}



class MemberBrowser extends JPanel {

	GridBagConstraints c = new GridBagConstraints();
	GridBagLayout gridbag = new GridBagLayout();

	void addComp(Component comp) {
		gridbag.setConstraints(comp,c);
		add(comp);
		c.gridx++;
	}

	MemberBrowser(Class cls,Object o,
		List members,
		boolean stat,boolean notstat,
		boolean priv,boolean notpriv,
		boolean field,
		boolean method,
		boolean constructor
	) {
		super();

		setLayout(gridbag);
		c.fill = GridBagConstraints.BOTH;
		c.weightx = 0d;
		c.weighty = 0d;
		c.gridx = 0;
		c.gridy = 0;

		// set(new Integer(1),0,2);
		// set(new JButton("hello"),2,0);
		for (int i=0;i<members.size();i++) {
			try {
				Member member=(Member)members.get(i);
				if ( ! ( /* The list of things which mean the member should not go in this browser follow: */
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
					Box boxRow=Box.createHorizontalBox();
					int col=0;
					c.anchor = GridBagConstraints.EAST;
					String s="";
					if (member instanceof Field) {
						s+=NuJuTu.niceName(((Field)member).getType())+" ";
						s+=member.getName()+" = ";
						boxRow.add(new JLabel(((Field)member).get(o).toString()));
					} else { // Method or Constructor
						Class[] params;
						if (member instanceof Method) {
							s+=NuJuTu.niceName(((Method)member).getReturnType())+" ";
							params=((Method)member).getParameterTypes();
							s+=member.getName();
						} else { // assert (member instanceof Constructor);
							params=((Constructor)member).getParameterTypes();
							s+="new "+NuJuTu.niceNameString(member.getName());
						}
						s+=" ( ";
						if (params.length==0) {
							boxRow.add(new JLabel(" ) "));
							boxRow.add(new JButton("Execute"));
						} else {
							for (int j=0;j<params.length;j++) {
								if (
										params[j].isPrimitive()
										|| params[j].getName().equals("java.lang.String")
										// || hasStringConstructor, eg. StringBuffer, Date, ...
										// also some array types, eg. byte[] and char[]
									) {
									boxRow.add(new JTextField(NuJuTu.niceName(params[j]),4));
								} else {
									boxRow.add(new JButton(NuJuTu.niceName(params[j])));
								}
								if (j<params.length-1)
									boxRow.add(new JLabel(", "));
							}
							// c.gridwidth = GridBagConstraints.REMAINDER;
							boxRow.add(new JLabel(" )"));
						}
					}
					c.anchor = GridBagConstraints.EAST;
					c.fill = GridBagConstraints.NONE;
					addComp(new JLabel(s));
					c.anchor = GridBagConstraints.WEST;
					c.fill = GridBagConstraints.BOTH;
					addComp(boxRow);
					// c.gridwidth = GridBagConstraints.RELATIVE;
					members.remove(member);
					i--;
					// boxRow.add(leftBit);
					// boxRow.add(rightBit);
					// add(boxRow);
					c.gridy++;
					c.gridx = 0;
				}
			} catch (Exception e) {
				System.err.println(""+e);
			}
		}
	}

}



class Desktop extends JPanel {

	Desktop() {
	}

}

class InfoPane extends JPanel {

	Box header=Box.createHorizontalBox();
	JTabbedPane tabbedPane;
	String[] tabNames={
		"Static Fields",
		"Static Methods",
		"Constructors",
		"Fields",
		"Methods"
		// "Private Fields",
		// "Private Methods"
	};
	JPanel[] tabs;

	InfoPane() {

		tabbedPane=new JTabbedPane();
		tabs=new JPanel[tabNames.length];
		for (int i=0;i<tabs.length;i++) {
			tabs[i]=new JPanel();
			tabbedPane.addTab(tabNames[i],new JScrollPane(tabs[i]));
		}

		Box box=Box.createVerticalBox();
		add(box);

		box.add(header);
		box.add(tabbedPane);

	}

	void introspect(Class c,Object o) {

		header.removeAll();
		header.add(new JLabel("Browsing "+NuJuTu.niceName(o.getClass())+" "));
		header.add(new JButton(o.toString()));
		header.add(new JLabel(" as "));
		header.add(new JButton(NuJuTu.niceName(c,false)));

		List members=new Vector();
		jlib.JList.addArray(members,c.getFields());
		jlib.JList.addArray(members,c.getConstructors());
		jlib.JList.addArray(members,c.getMethods());
		// tabs[0].add(new JButton("wicked"));
		tabs[0].removeAll();
		tabs[0].add(new JScrollPane(new MemberBrowser(c,o,members,true,false,false,true,true,false,false)));
		tabs[1].removeAll();
		tabs[1].add(new JScrollPane(new MemberBrowser(c,o,members,true,false,false,true,false,true,false)));
		tabs[2].removeAll();
		tabs[2].add(new JScrollPane(new MemberBrowser(c,o,members,false,false,false,true,false,false,true)));
		tabs[3].removeAll();
		tabs[3].add(new JScrollPane(new MemberBrowser(c,o,members,false,false,false,true,true,false,false)));
		tabs[4].removeAll();
		tabs[4].add(new JScrollPane(new MemberBrowser(c,o,members,false,false,false,true,false,true,false)));
		// tabs[5].removeAll();
		// tabs[5].add(new JScrollPane(new MemberBrowser(c,o,members,false,false,true,false,true,false,false)));
		// tabs[6].removeAll();
		// tabs[6].add(new JScrollPane(new MemberBrowser(c,o,members,false,false,true,false,false,true,false)));
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
		InfoPane infoPane2=new InfoPane();
		Container cp=getContentPane();
		infoPane.introspect("hello");
		infoPane2.introspect(new Integer(7));
		// JSplitPane sp=new JSplitPane(JSplitPane.VERTICAL_SPLIT,desktop,new JScrollPane(infoPane));
		JSplitPane sp=new JSplitPane(JSplitPane.VERTICAL_SPLIT,new JScrollPane(infoPane),new JScrollPane(infoPane2));
		cp.add(sp);
	}

	public static void main(String[] args) {
		ArgParser a=new ArgParser(args);
		a.done();
		MainFrame mf=new MainFrame();
		mf.setSize(800,600);
		mf.setVisible(true);
	}

}
