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

// import com.sun.javadoc.*;

class NujuFrame extends Frame implements ActionListener {

	public static final int maxlabelwidth=50;

	Vector components=new Vector();
	Object o;

	void addComponent(Component c,java.util.List components,int col,int row,GridBagLayout lay) {
		lay.setConstraints(c,simpleconstraints(col,row,1,1));
		components.add(c);
	}

	NujuFrame(Object obj) {

		// Constructor, sets title, size etc.
		super(neaten(""+obj.getClass())+" = "+obj.toString());
		o=obj;
		setResizable(true);
		GridBagLayout lay=new GridBagLayout();
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				dispose();
				// System.exit(0);
			}
		} );

		// Need to add an action listener to each button

		Class c=o.getClass();

		int row=0;

		//    Label l0=new Label(""+c+": "+JString.clip(o.toString(),30));
		Label l0=new Label(this.getTitle()); //neaten(c.toString())+" = "+o.toString());
		Dimension d=l0.getSize();
		//    if (d.getWidth()>maxlabelwidth)
		l0.setSize(maxlabelwidth,(int)d.getHeight());
		lay.setConstraints(l0,simpleconstraints(1,row,1,1));
		components.add(l0);

		Button b0=new Button("Use me");
		b0.setActionCommand("Use me");
		b0.addActionListener(this);
		lay.setConstraints(b0,simpleconstraints(2,row,1,1));
		components.add(b0);

		row++;
		addComponent(new Label("Class "+c.getName()),components,1,row,lay);
		row++;
		// Field[] fs=c.getDeclaredFields();
		Field[] fs=c.getFields();
		Label l1=new Label(fs.length+" Fields:");
		components.add(l1);
		lay.setConstraints(l1,simpleconstraints(2,row,1,1));
		row++;

		for (int i=0;i<fs.length;i++) {
			Panel p=new Panel();
			Field f=fs[i];
			//      if (!Modifier.isPrivate(f.getModifiers())) {
			addComponent(new Label("Field "+f+" has parent "+f.getDeclaringClass()),components,1,row,lay);
			row++;
			String pre=(Modifier.isPrivate(f.getModifiers()) ? "private " : "" );
			p.add(new Label(pre+neaten(f.getType().toString())));
			Button b=new Button(f.getName());
			b.setActionCommand("Field "+i+" ("+f.getName()+")");
			b.addActionListener(this);
			if (Modifier.isPrivate(f.getModifiers()))
				b.setEnabled(false);
			p.add(b);
			try {
				TextField tf=new TextField(""+f.get(o));
				//Dimension d2=tf.getSize();
				//        if (d2.getWidth()>maxlabelwidth)
				//          tf.setSize(maxlabelwidth,(int)d2.getHeight());
				Rectangle r=tf.getBounds();
				tf.setBounds((int)r.getX(),(int)r.getY(),maxlabelwidth,(int)r.getHeight());
				p.add(tf);
			} catch (Exception e) { }
			components.add(p);
			lay.setConstraints(p,simpleconstraints(0,row,3,1));
			row++;
			//      }
		}

		row++;
		//    Method[] ms=c.getMethods();/*Declared*/

		// Method[] ms=c.getDeclaredMethods();/*Declared*/
		Method[] ms=c.getMethods();/*Declared*/
		Label l2=new Label(ms.length+" Methods:");
		components.add(l2);
		lay.setConstraints(l2,simpleconstraints(2,row,1,1));
		row++;
		for (int i=0;i<ms.length;i++) {
			Panel p=new Panel();
			Method m=ms[i];
			//      if (!Modifier.isPrivate(m.getModifiers())) {
			String pre=(Modifier.isPrivate(m.getModifiers()) ? "private " : "" );
			p.add(new Label(pre+neaten(m.getReturnType().toString())));
			Button b=new Button(m.getName());
			b.setActionCommand("Method "+i+" ("+m.getName()+"("+listarray(m.getParameterTypes())+"))");
			b.addActionListener(this);
			if (Modifier.isPrivate(m.getModifiers()))
				b.setEnabled(false);
			p.add(b);
			p.add(new Label("("+listarray(m.getParameterTypes())+")"));
			lay.setConstraints(p,simpleconstraints(0,row,3,1));
			components.add(p);
			row++;
			//      }
		}

		//	  components.add(new TextField("hello"));
		Container con=new Container();
		con.setLayout(lay);
		for (int i=0;i<components.size();i++)
			con.add((Component)components.get(i));
		ScrollPane p=new ScrollPane(ScrollPane.SCROLLBARS_AS_NEEDED);
		p.add(con);
		add((Component)p);

		pack();
		setSize(320,200);
		setVisible(true);
	}

	public void actionPerformed(ActionEvent e){
		String command=e.getActionCommand();
		System.out.println("NujuFrame.actionPerformed("+e+");");
		if (command.equals("Use me")) {
			NujuComWin.provideObject(o);
		} else if (command.startsWith("Field ")) {
			command=command.substring(6,command.indexOf(" ",6));
			int i=new Integer(command).intValue();
			try {
				// NujuFrame.present(o.getClass().getDeclaredFields()[i].get(o));
				NujuFrame.present(o.getClass().getFields()[i].get(o));
			} catch (Exception ex) { System.out.println(ex); }
		} else if (command.startsWith("Method ")) {
			System.out.println("It's a method allright!");
			command=command.substring(7,command.indexOf(" ",7));
			int i=new Integer(command).intValue();
			try {
				System.out.print("In fact it's ");
				// System.out.println(""+o.getClass().getDeclaredMethods()[i]);
				// NujuComWin.provide(o,o.getClass().getDeclaredMethods()[i]);
				System.out.println(""+o.getClass().getMethods()[i]);
				NujuComWin.provide(o,o.getClass().getMethods()[i]);
				// Declared
				//        NujuComWin.provide(o,o.getClass().getMethods()[i]); // Declared
				// without o,  NujuComWin.provideMethod(o.getClass().getDeclaredMethods()[i]);
			} catch (Exception ex) { System.out.println(ex); }
		} else {
			System.out.println("Action "+command+" performed.\n");
		}
	}

	public static void present(Object j) {
		NujuFrame f=new NujuFrame(j);
		f.show();
	}

	public String listarray(Object os[]) {
		String s="";
		for (int i=0;i<os.length;i++) {
			Class c=os[i].getClass();
			Class co=c.getComponentType();
			//      s=s+"(Co: "+co+")";
			if (co==null)
				s=s+neaten(os[i].toString());
			else
				s+="**********["+co+"]***why doesn't this appear?!*******";
			if (i<os.length-1)
				s+=", ";
		}
		return s;
	}
	private GridBagConstraints simpleconstraints(int x,int y,int w,int h) {
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
	public static String neaten(String t) {
		int i=t.lastIndexOf(".");
		if (i>0)
			return t.substring(i+1);
		return t;
	}
}

