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
		// c.fill = GridBagConstraints.BOTH;
		c.gridx = 0;
		c.gridy = 0;

		// set(new Integer(1),0,2);
		// set(new JButton("hello"),2,0);
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
					Box boxRow=Box.createHorizontalBox();
					Box leftBit=Box.createHorizontalBox();
					Box rightBit=Box.createHorizontalBox();
					int col=0;
					c.anchor = GridBagConstraints.EAST;
					if (member instanceof Field) {
						addComp(new JLabel(((Field)member).getType().toString()+" "));
					}
					if (member instanceof Method) {
						addComp(new JLabel(((Method)member).getReturnType().toString()+" "));
					}
					addComp(new JLabel(member.getName()+(member instanceof Field?"":"(")));
					c.anchor = GridBagConstraints.WEST;
					if (member instanceof Field) {
						addComp(new JLabel(" = "));
						// c.gridx++;
						c.gridwidth = GridBagConstraints.REMAINDER;
						addComp(new JLabel(((Field)member).get(o).toString()));
					} else {
						// c.gridx++;
						Class[] params=( member instanceof Method
								? ((Method)member).getParameterTypes()
								: ((Constructor)member).getParameterTypes()
								);
						if (params.length==0) {
							addComp(new JLabel(") "));
							// c.gridwidth = GridBagConstraints.REMAINDER;
							addComp(new JButton("Execute"));
						} else {
							addComp(new JLabel(" "));
							for (int j=0;j<params.length;j++) {
								if (params[j].isPrimitive()) {
									addComp(new JTextField(params[j].getName(),6));
								} else {
									addComp(new JButton(params[j].getName()));
								}
								if (j<params.length-1)
									addComp(new JLabel(", "));
							}
							// c.gridwidth = GridBagConstraints.REMAINDER;
							addComp(new JLabel(" )"));
						}
					}
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
			addTab(tabNames[i],new JScrollPane(tabs[i]));
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
