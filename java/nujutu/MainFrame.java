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


abstract class ObjectDroppableView implements ComponentAcceptingDrop {

    // Actually should have a
    // boolean editable;
    // to determine whether typing / dropping is allowed

	JobContext jobContext;
	Class type;
    Variable variable;
    Component component;

	ObjectDroppableView(Class _type, Variable _variable) {
		type = _type;
		variable = _variable;
	}
	ObjectDroppableView(Class _type, Object _object) {
		this(_type, new Variable(null,_object));
	}

	void setJobContext(JobContext _context) {
		jobContext = _context;
	}

	void init() {
        updateComponent();
        // Set default properties for Component:
        // Background color:
        component.setBackground(new java.awt.Color(0.6f,0.9f,0.6f));
        // Draggable from:
        component.addInputMethodListener(new InputMethodListener() {
            public void caretPositionChanged(InputMethodEvent e) {
                System.out.println("Got caret moved event "+e);
            }
            public void inputMethodTextChanged(InputMethodEvent e) {
                System.out.println("Got text changed event "+e);
            }
        } );

		DnDManager.setSource(component,variable);
		final ObjectDroppableView x = this;
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
            System.out.println("ArgumentView received Variable");
            dropVar((Variable)received);
		} else {
            System.out.println("ArgumentView received Object "+received);
			dropVar(new Variable(null,received));
		}
	}
	void dropVar(Variable var) {
		if (type.isInstance(var.value)) {
			variable = var;
			// init();
			// component.repaint();
            // jobContext.mainFrame.repaint();
            updateComponent();
            jobContext.repaintAll();
			jobContext.log("Dropped "+var+" with value "+var.value+" into "+type);
		} else if (type.isPrimitive()) {
            jobContext.log(type+" is a primitive");
            // Object o = type.newInstance();
            // todo ...
        } else {
			jobContext.log("Cannot drop object type "+var.getNiceType()+" onto "+type);
		}
	}
    String getVariableAsString() {
        return (
            variable.value == null
            ? "null " + NuJuTu.niceName(type)
            : "" + variable.value
        );
    }
    abstract void updateComponent();
}


abstract class ArgumentView extends ObjectDroppableView {
    ArgumentView(Class _type, Variable _variable) {
        super(_type, _variable);
    }

    ArgumentView(Class _type, Object _object) {
        super(_type, _object);
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
		component = new JTextField();
        ((JTextField)component).addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent event) {
                        drop( ((JTextField)component).getText() );
                    }
                }
        );
		super.init();
	}

    void updateComponent() {
        ((JTextField)component).setText(
                // variable.value == null
                // ? "(" + NuJuTu.niceName(variable.getType()) +")"
                // :
                getVariableAsString()
        );
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
		component = new JLabel();
		super.init();
	}

    void updateComponent() {
        ((JLabel)component).setText(getVariableAsString());
    }
}

interface HasContext {
    public JobContext getJobContext();
    public void setJobContext(JobContext _jobContext);
}

class HasContextImpl implements HasContext {
    JobContext jobContext;
    public JobContext getJobContext() {
        return jobContext;
    }
    public void setJobContext(JobContext _jobContext) {
        jobContext = _jobContext;
    }
}


class MemberView extends HasContextImpl implements ActionListener {
    JobContext jobContext;
    Member member;
    Object contextObject; // parent object of this member, null => static member or disabled
    JButton executeButton;
    MemberView(JobContext _jobContext, Member _member, Object _contextObject) {
        jobContext = _jobContext;
        member = _member;
        contextObject = _contextObject;
        executeButton = new JButton(member.getName());
        executeButton.addActionListener(this);
    }
    public void actionPerformed(ActionEvent event) {
    }
}


class FieldView extends MemberView {
    // The value of a public field view should be droppable, similarly to an argument!
    FieldView(JobContext _jobContext, Member member, Object contextObj) {
        super(_jobContext,member,contextObj);
    }
}


class MethodView extends MemberView {
    ArgumentView[] arguments;
    MethodView(JobContext _jobContext, Member member, Object contextObj) {
        super(_jobContext,member,contextObj);
        Class[] params = ((Method)member).getParameterTypes();
        arguments = new ArgumentView[params.length];
        for (int j=0;j<params.length;j++) {
            ArgumentView arg = argumentViewFor(params[j],null);
            arg.setJobContext(jobContext);
            arguments[j] = arg;
        }
    }
    ArgumentView argumentViewFor(Class cls,Object o) {

        System.err.println("Dealing with object "+o);

        // First, check if it is possible to instantiate an
        // object of type cls with a single string constructor.
        Constructor con=null;
        try {
            Class[] conPars={ "".getClass() };
            con=cls.getConstructor(conPars);
        } catch (Exception e) { }

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

        return view;
    }
    public void actionPerformed(ActionEvent event) {
        try {
            Object[] params = new Object[arguments.length];
            for (int i=0;i<arguments.length;i++) {
                Object obj = arguments[i].variable.value;
                params[i] = obj;
            }
            Object result = ((Method)member).invoke(contextObject,params);
            if (result != null) {
                jobContext.workspace.newVariable(result);
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
    }
}


class MemberBrowser extends JPanel {

	JobContext jobContext;

	GridBagConstraints c = new GridBagConstraints();
	GridBagLayout gridbag = new GridBagLayout();

    int countMembers;

	MemberBrowser(JobContext _context, Class cls, Object contextObj,
		List members,
		boolean stat,boolean notstat,
		boolean priv,boolean notpriv,
		boolean field,
		boolean method,
		boolean constructor
	) {
		super();

        jobContext = _context;

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
                        FieldView fieldView = new FieldView(jobContext,member,contextObj);
						rightBox.add(fieldView.executeButton);
					} else { // Method or Constructor
                        MethodView methodView;
						if (member instanceof Method) {
                            methodView = new MethodView(jobContext,member,contextObj);
							leftBox.add(new JLabel(NuJuTu.niceName(((Method)member).getReturnType())+" "));
							leftBox.add(methodView.executeButton);
						} else { // assert (member instanceof Constructor); /** @todo Doesn't work on Constructors! */
                            methodView = new MethodView(jobContext,member,contextObj);
							leftBox.add(new JButton("new"));
							leftBox.add(new JLabel(" "+NuJuTu.trimPackagePath(member.getName())));
						}
						rightBox.add(new JLabel(" ( "));
                        for (int j=0;j<methodView.arguments.length;j++) {
                            rightBox.add((methodView.arguments[j].component));
                            if (j<methodView.arguments.length-1)
                                rightBox.add(new JLabel(", "));
                        }
						// if (params.length==0) {
							// rightBox.add(new JLabel(" ) "));
							// rightBox.add(new JButton("Execute"));
						// } else {
							// c.gridwidth = GridBagConstraints.REMAINDER;
							rightBox.add(new JLabel(" )"));
						// }
                        methodView.setJobContext(jobContext);
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
                    countMembers++;
				}
			} catch (Exception e) {
				e.printStackTrace(System.err);
			}
		}
	}

    void addComp(Component comp) {
        gridbag.setConstraints(comp,c);
        add(comp);
        c.gridx++;
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
    Box box;
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

		box=Box.createVerticalBox();
		add(box);

	}

    void populateTab(int i,
        JobContext _context, Class cls, Object contextObj,
		List members,
		boolean stat,boolean notstat,
		boolean priv,boolean notpriv,
		boolean field,
		boolean method,
		boolean constructor
    ) {
        MemberBrowser mb = new MemberBrowser(jobContext,cls,contextObj,members,stat,notstat,priv,notpriv,field,method,constructor);
                    tabs[i]=new JPanel();
                    tabbedPane.addTab(tabNames[i],new JScrollPane(tabs[i]));
        // tabs[i].removeAll();
        tabs[i].setName(tabNames[i] + "("+mb.countMembers+")");
        tabs[i].add(new JScrollPane(mb));
    }

	void browse(Class c,Object o) {

        tabbedPane=new JTabbedPane();
                tabs=new JPanel[tabNames.length];
                for (int i=0;i<tabs.length;i++) {

                }

        box.removeAll();
        box.add(header);
        box.add(tabbedPane);

		header.removeAll();
		header.add(new JLabel("Browsing "+NuJuTu.niceName(o.getClass())+" "));
		header.add(new JButton(o.toString()));
		header.add(new JLabel(" as "));
		header.add(new JButton(NuJuTu.niceName(c,false)));
		// header.add(new ClassChooser(this,o,c));

		List members=new Vector();
		jlib.JList.addArray(members,c.getFields());
		jlib.JList.addArray(members,c.getConstructors());
		jlib.JList.addArray(members,c.getMethods());
		// tabs[0].add(new JButton("wicked"));
        populateTab(0,jobContext,c,o,members,true,false,false,true,true,false,false);
		populateTab(1,jobContext,c,o,members,true,false,false,true,false,true,false);
		populateTab(2,jobContext,c,o,members,false,false,false,true,false,false,true);
		populateTab(3,jobContext,c,o,members,false,false,false,true,true,false,false);
		populateTab(4,jobContext,c,o,members,false,false,false,true,false,true,false);
		// tabs[5].removeAll();
		// tabs[5].add(new JScrollPane(new MemberBrowser(jobContext,c,o,members,false,false,true,false,true,false,false)));
		// tabs[6].removeAll();
		// tabs[6].add(new JScrollPane(new MemberBrowser(jobContext,c,o,members,false,false,true,false,false,true,false)));
		// System.out.println(""+members.size());
		// jobContext.repaintAll();
        box.repaint();
        header.repaint();
        tabbedPane.repaint();

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
	String label; // may be null
	Object value; // may be null
	Variable(String _label, Object _value) {
		label = _label;
		value = _value;
		// jobContext = JobContext.getContextFromThread();
	}
	Class getType() {
		return value.getClass();
	}
    String getNiceType() {
        return NuJuTu.niceName(value.getClass());
    }
    String getValueAsString() {
        if (value == null)
            return "null " + getNiceType();
        return "" + value;
    }
    String getDisplayText() {
        return getNiceType()+" "+label+" = "+getValueAsString();
    }
}


class VariableView extends JButton {
	JobContext jobContext;
	Variable variable;
	VariableView(JobContext _jobContext,Variable _variable) {
		super(_variable.getDisplayText());
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

    int varNum = 0;

	Workspace(JobContext _jobContext) {
		jobContext = _jobContext;
        // setMaximumSize(new Dimension(200,100000));
        // setLayout(new FlowLayout());
        setLayout(new GridLayout(100000,1));
		for (int i=0;i<5;i++) {
			Object obj = new String( "" + Math.random() );
            newVariable(obj);
		}
	}

    public void newVariable(Object obj) {
        String label = "" + (char)(((int)'a') + varNum);
        varNum++;
        System.out.println("New variable on the workspace.");
        System.out.println("    "+label+" = "+obj+";");
        VariableView variable = new VariableView(jobContext,new Variable(label,obj));
        add(variable);
        try { jobContext.repaintAll(); } catch (Exception e) { jobContext.log("Whilst repainting, got: "+e); };
        repaint();
        variable.repaint();
        // getParent().repaint();
        // getParent().getParent().repaint();
    }

}


class JobConsole extends JPanel {
	StringBuffer contents = new StringBuffer();
	JTextArea contentsView = new JTextArea("Log console");
    JobConsole() {
        super();
        setLayout(new FlowLayout());
        add(contentsView);
    }
	public void log(String text) {
		System.err.println(text);
		if (contents.length() > 0)
            contents.append('\n');
        contents.append(text);
		contentsView.setText(contents.toString());
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
	// JobBrowser infoPane2=new JobBrowser(this);
	JobConsole console;

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

    public void repaintAll() {
        hackRepaint(mainFrame);
        hackRepaint(workspace);
        mainFrame.horSplit.repaint();
        mainFrame.vertSplit.repaint();

        // repaint();
        // try { Thread.currentThread().sleep(1000); } catch (Exception e) { }
    }

    /** Doesn't work! */
    private void hackRepaint(Component component) {
         if (Math.random() < 0.5) {
            component.resize(component.getWidth() + 10, component.getHeight() + 10);
        } else {
            component.resize(component.getWidth() - 10, component.getHeight() - 10);
        }
    }

    public void log(Object o) {
        if (console == null)
            console = new JobConsole();
        console.log(o);
        System.out.println("" + o);
    }

    public static void globalLog(Object o) {
        System.out.println("" + o);
	}

	/*

	static Map threadMap = new HashMap();

	static void setContextForThread(JobContext jobContext) {
		threadMap.put(Thread.currentThread(),jobContext);
	}

	static JobContext getContextFromThread() {
		try {
			Thread thread = Thread.currentThread();
			Object result = threadMap.get(thread);
			if (result == null) {
				System.err.println("No jobContext found for "+thread);
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

    JSplitPane horSplit, vertSplit;

	MainFrame() {
		super("NuJuTu");
		addWindowListener( new WindowAdapter() {
				public void windowClosing(WindowEvent e) { System.exit(0); }
		} );
		JobContext jobContext = new JobContext();
		jobContext.mainFrame = this;
		Container cp=getContentPane();
		jobContext.infoPane.browse("hello");
		// jobContext.infoPane2.browse(new Integer(7));
		// JSplitPane sp=new JSplitPane(JSplitPane.VERTICAL_SPLIT,workspace,new JScrollPane(jobContext.infoPane));
        // JSplitPane browserPanes = new JSplitPane(JSplitPane.VERTICAL_SPLIT,new JScrollPane(jobContext.infoPane),new JScrollPane(jobContext.infoPane2));
        JComponent browserPanel = new JScrollPane(jobContext.infoPane);
		horSplit = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,new JScrollPane(jobContext.workspace),browserPanel);
        vertSplit = new JSplitPane(JSplitPane.VERTICAL_SPLIT,horSplit,new JScrollPane(jobContext.console));
		cp.add(vertSplit);
		setSize(800,600);
		setVisible(true);
		// browserPanes.setDividerLocation(0.5d);
        horSplit.setDividerLocation(0.5d);
        vertSplit.setDividerLocation(0.9d);
	}

	public static void main(String[] args) {
		ArgParser a=new ArgParser(args);
		a.done();
		new MainFrame();
	}

}
