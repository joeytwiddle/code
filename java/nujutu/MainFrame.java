package nujutu;

// import java.applet.*;
import java.awt.Component;
import java.awt.Container;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
// import java.net.*;
import java.util.*;
import java.util.Map;
import java.util.List;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import nuju.*;
import jlib.*;
import jlib.dnd.hack.*;


class NuJuTu {

	static String trimPackagePath(String n) { // Strip package path
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
			n=trimPackagePath(n);
		return n;
	}

}


class ArgumentView implements ComponentAcceptingDrop {

	JobContext context;
	Class type;
	Variable variable;
	Component component;

	ArgumentView(Class _type, Variable _variable) {
		type = _type;
		variable = _variable;
	}
	ArgumentView(Class _type, Object _object) {
		this(_type, new Variable(null,_object));
	}

	void setJobContext(JobContext _context) {
		context = _context;
	}

	void init() {
		DnDManager.setSource(component,variable);
		final ArgumentView x = this;
		// DnDManager.setTarget(component);
		DnDManager.setTarget(component,
			new jlib.simple.Code() {
				public void execute() {
					// System.out.println("Something received "+input);
					x.drop(input);
				}
			}
		);
	}

	public void drop(Object received) {
		if (received instanceof Variable) {
			dropVar(variable);
		} else {
			dropVar(new Variable(null,received));
		}
	}
	void dropVar(Variable var) {
		if (type.isInstance(var.value)) {
			variable = var;
			init();
			component.repaint();
			JobContext.log("Dropped "+variable+" with value "+variable.value+" into "+type);
		} else {
			JobContext.log("Cannot drop object type "+var+" onto "+type);
		}
	}

}


class TextArgumentView extends ArgumentView {
	TextArgumentView(Class _type, Variable _variable) {
		super(_type,_variable);
		init();
	}
	TextArgumentView(Class _type, Object _object) {
		super(_type,_object);
		init();
	}
	void init() {
		component = new JTextField("" + variable.value);
		super.init();
	}
}


class NonTextArgumentView extends ArgumentView {
	NonTextArgumentView(Class _type, Variable _variable) {
		super(_type,_variable);
		init();
	}
	NonTextArgumentView(Class _type, Object _object) {
		super(_type,_object);
		init();
	}
	void init() {
		component = new JLabel("" + variable.value);
		super.init();
	}
}



class MemberBrowser extends JPanel {

	JobContext context;

	GridBagConstraints c = new GridBagConstraints();
	GridBagLayout gridbag = new GridBagLayout();

	void addComp(Component comp) {
		gridbag.setConstraints(comp,c);
		add(comp);
		c.gridx++;
	}

	Component componentFor(Class c) {
		return componentFor(c,NuJuTu.niceName(c));
	}

	Component componentFor(Object o) {
		return componentFor(o.getClass(),o);
	}

	Component componentFor(Class cls,Object o) {

		System.err.println("Dealing with object "+o);

		// First, check if it is possible to instantiate an
		// object of type cls with a single string constructor.
		Constructor con=null;
		try {
			Class[] conPars={ "".getClass() };
			con=cls.getConstructor(conPars);
		} catch (Exception e) { }

		// Component c=null;
		ArgumentView view;
		// Decide what type of Component to use.
		if (
				cls.isPrimitive()
				|| cls.getName().equals("java.lang.String")
				|| con != null
				// || hasStringConstructor, eg. StringBuffer, Date, ...
				// || isPrimClass (all fit above?)
				// also some array types, eg. byte[] and char[]
		) {
			view = new TextArgumentView(cls,o);
			// JTextField field = new JTextField(""+o,4);
			// if (field.getColumns()>=10) {
				// field.setColumns(10);
			// }
			// c = field;
		} else {
			view = new NonTextArgumentView(cls,o);
			// c = new JButton(""+o);
		}

		view.setJobContext(context);
		Component c = view.component;
		// Set default properties for Component:
		// Background color:
		c.setBackground(new java.awt.Color(0.6f,0.9f,0.6f));
		// Draggable from:
		c.addInputMethodListener(new InputMethodListener() {
			public void caretPositionChanged(InputMethodEvent e) {
				System.out.println("Got caret moved event "+e);
			}
			public void inputMethodTextChanged(InputMethodEvent e) {
				System.out.println("Got text changed event "+e);
			}
		} );

		return c;
	}

	MemberBrowser(JobContext _context, Class cls, Object o,
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
					Box leftBox=Box.createHorizontalBox();
					Box rightBox=Box.createHorizontalBox();
					int col=0;
					c.anchor = GridBagConstraints.EAST;
					String s="";
					if (member instanceof Field) {
						leftBox.add(new JLabel(NuJuTu.niceName(((Field)member).getType())+" "));
						leftBox.add(new JButton(member.getName()));
						leftBox.add(new JLabel(" = "));
						rightBox.add(componentFor(((Field)member).get(o)));
					} else { // Method or Constructor
						Class[] params;
						if (member instanceof Method) {
							leftBox.add(new JLabel(NuJuTu.niceName(((Method)member).getReturnType())+" "));
							leftBox.add(new JButton(member.getName()));
							params=((Method)member).getParameterTypes();
						} else { // assert (member instanceof Constructor);
							leftBox.add(new JButton("new"));
							leftBox.add(new JLabel(" "+NuJuTu.trimPackagePath(member.getName())));
							params=((Constructor)member).getParameterTypes();
						}
						rightBox.add(new JLabel(" ( "));
						// if (params.length==0) {
							// rightBox.add(new JLabel(" ) "));
							// rightBox.add(new JButton("Execute"));
						// } else {
							for (int j=0;j<params.length;j++) {
								rightBox.add(componentFor(params[j]));
								if (j<params.length-1)
									rightBox.add(new JLabel(", "));
							}
							// c.gridwidth = GridBagConstraints.REMAINDER;
							rightBox.add(new JLabel(" )"));
						// }
					}
					c.anchor = GridBagConstraints.EAST;
					c.fill = GridBagConstraints.NONE;
					addComp(leftBox);
					c.anchor = GridBagConstraints.WEST;
					c.fill = GridBagConstraints.BOTH;
					addComp(rightBox);
					// c.gridwidth = GridBagConstraints.RELATIVE;
					members.remove(member);
					i--;
					// leftBox.add(leftBit);
					// leftBox.add(rightBit);
					// add(leftBox);
					c.gridy++;
					c.gridx = 0;
				}
			} catch (Exception e) {
				System.err.println(""+e);
			}
		}
	}

}


class ClassChooser extends JComboBox {
	JobBrowser parentBrowser;
	ClassChooser(JobBrowser _parentBrowser, Object obj, Class classIn) {
		parentBrowser = _parentBrowser;
		Class c = ( obj == null ? classIn : obj.getClass() );
		while (true) {
			Object item = NuJuTu.niceName(c,false);
			addItem(item);
			if (obj != null && c.equals(classIn)) {
				setSelectedItem(item);
			}
			if (c.equals(new Object().getClass())) {
				break;
			}
			c = c.getSuperclass();
		}
	}
	public void selectedItemChanged() {
		Object item = getSelectedItem();
		// if (item instanceof String) {
			try {
				parentBrowser.browse(Class.forName((String)item));
			} catch (Exception e) {
				parentBrowser.jobContext.log(e);
			}
		// }
	}
	/*
	static Object[] classesFor(Object obj, Class c) {
		if (obj != null) {
			c = obj.getClass();
		}
		List classes = new java.util.Vector();
		while (true) {
			classes.add(NuJuTu.niceName(c,false));
			if (c.equals(new Object().getClass())) {
				break;
			}
			c = c.getSuperclass();
		}
		return classes.toArray();
	}
	*/
}


class JobBrowser extends JPanel {

	JobContext jobContext;

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

	JobBrowser(JobContext _jobContext) {
		jobContext = _jobContext;

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

	void browse(Class c,Object o) {

		header.removeAll();
		header.add(new JLabel("Browsing "+NuJuTu.niceName(o.getClass())+" "));
		header.add(new JButton(o.toString()));
		header.add(new JLabel(" as "));
		// header.add(new JButton(NuJuTu.niceName(c,false)));
		header.add(new ClassChooser(this,o,c));

		List members=new Vector();
		jlib.JList.addArray(members,c.getFields());
		jlib.JList.addArray(members,c.getConstructors());
		jlib.JList.addArray(members,c.getMethods());
		// tabs[0].add(new JButton("wicked"));
		tabs[0].removeAll();
		tabs[0].add(new JScrollPane(new MemberBrowser(jobContext,c,o,members,true,false,false,true,true,false,false)));
		tabs[1].removeAll();
		tabs[1].add(new JScrollPane(new MemberBrowser(jobContext,c,o,members,true,false,false,true,false,true,false)));
		tabs[2].removeAll();
		tabs[2].add(new JScrollPane(new MemberBrowser(jobContext,c,o,members,false,false,false,true,false,false,true)));
		tabs[3].removeAll();
		tabs[3].add(new JScrollPane(new MemberBrowser(jobContext,c,o,members,false,false,false,true,true,false,false)));
		tabs[4].removeAll();
		tabs[4].add(new JScrollPane(new MemberBrowser(jobContext,c,o,members,false,false,false,true,false,true,false)));
		// tabs[5].removeAll();
		// tabs[5].add(new JScrollPane(new MemberBrowser(jobContext,c,o,members,false,false,true,false,true,false,false)));
		// tabs[6].removeAll();
		// tabs[6].add(new JScrollPane(new MemberBrowser(jobContext,c,o,members,false,false,true,false,false,true,false)));
		// System.out.println(""+members.size());
		jobContext.mainFrame.repaint();

	}

	void browse(Class c) {
		browse(c,null);
	}

	void browse(Object o) {
		browse(o.getClass(),o);
	}

}


class Variable {
	JobContext jobContext;
	String label;
	Object value;
	Variable(String _label, Object _value) {
		label = _label;
		value = _value;
		// jobContext = JobContext.getContextFromThread();
	}
	Class getType() {
		return value.getClass();
	}
}


class VariableView extends JButton {
	JobContext jobContext;
	Variable variable;
	VariableView(JobContext _jobContext,Variable _variable) {
		super(_variable.value.getClass()+" "+_variable.label);
		variable = _variable;
		jobContext = _jobContext;
		enableEvents(MouseEvent.MOUSE_CLICKED);
		DnDManager.setSource(this,variable);
		DnDManager.setTarget(this,new jlib.simple.Code() {
			public void execute() {
				System.out.println("VariableView "+variable+" received: "+input);
			}
		} );
}
	protected void processMouseEvent(MouseEvent e) {
		super.processMouseEvent(e);
		if (e.getID() == MouseEvent.MOUSE_CLICKED) {
			jobContext.browseVariable(variable);
		}
	}
}


class Workspace extends JPanel {

	JobContext jobContext;

	Workspace(JobContext _jobContext) {
		jobContext = _jobContext;
		setLayout(new FlowLayout());
		for (int i=0;i<5;i++) {
			Object obj = new String( "" + Math.random() );
			String label = "" + (char)(((int)'a') + i);
			VariableView variable = new VariableView(jobContext,new Variable(label,obj));
			add(variable);
		}
	}

}


class JobConsole extends Panel {
	StringBuffer contents = new StringBuffer();
	JLabel contentsView = new JLabel();
	public void log(String text) {
		System.err.println(text);
		contents.append(text + '\n');
		contentsView.setText(text);
	}
	public void log(Exception e) {
		log(""+e);
	}
	public void log(Object o) {
		log(""+o);
	}
}


class JobContext {

	MainFrame mainFrame;
	Workspace workspace=new Workspace(this);
	JobBrowser infoPane=new JobBrowser(this);
	JobBrowser infoPane2=new JobBrowser(this);
	JobConsole console = new JobConsole();

	JobContext() {
		// setContextForThread(this);
	}

	void browseVariable(Variable variable) {
		infoPane.browse(variable.value);
	}

	/*
	void browseVariableAs(Class c) {
		infoPane.browse(c);
	}
	*/

	// void log(Object o) {
		// console.log(o);
	// }

	public static void log(Object o) {
		// console.log(o);
		System.err.println("" + o);
	}

	/*

	static Map threadMap = new HashMap();

	static void setContextForThread(JobContext context) {
		threadMap.put(Thread.currentThread(),context);
	}

	static JobContext getContextFromThread() {
		try {
			Thread thread = Thread.currentThread();
			Object result = threadMap.get(thread);
			if (result == null) {
				System.err.println("No context found for "+thread);
			}
			return (JobContext)result;
		} catch (Exception e) {
			e.printStackTrace(System.err);
			return null;
		}
	}

	*/

}



public class MainFrame extends JFrame {

	MainFrame() {
		super("NuJuTu");
		addWindowListener( new WindowAdapter() {
				public void windowClosing(WindowEvent e) { System.exit(0); }
		} );
		JobContext jobContext = new JobContext();
		jobContext.mainFrame = this;
		Container cp=getContentPane();
		jobContext.infoPane.browse("hello");
		jobContext.infoPane2.browse(new Integer(7));
		// JSplitPane sp=new JSplitPane(JSplitPane.VERTICAL_SPLIT,workspace,new JScrollPane(jobContext.infoPane));
		JSplitPane browserPanes = new JSplitPane(JSplitPane.VERTICAL_SPLIT,new JScrollPane(jobContext.infoPane),new JScrollPane(jobContext.infoPane2));
		JSplitPane mainSplit = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,new JScrollPane(jobContext.workspace),browserPanes);
		cp.add(mainSplit);
		setSize(800,600);
		setVisible(true);
		browserPanes.setDividerLocation(0.5d);
		mainSplit.setDividerLocation(0.5d);
	}

	public static void main(String[] args) {
		ArgParser a=new ArgParser(args);
		a.done();
		new MainFrame();
	}

}
