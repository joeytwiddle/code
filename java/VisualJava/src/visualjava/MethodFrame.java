package visualjava;

import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.beans.PropertyVetoException;
import java.lang.reflect.Constructor;
import java.lang.reflect.Member;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

import javax.swing.JButton;
import javax.swing.JInternalFrame;
import javax.swing.JLabel;

/** joey Nov 1, 2004 7:51:00 PM */
public class MethodFrame extends JInternalFrame {

    //// Defined:
    // Object value; // Null if constructor, or if method is static
    VariableHolder objTarget; // (well actually generated!) Null if constructor, or if method is static
    Member member; // Will be either the method or the constructor, the other will be null.

    //// Obtained:
    Method method;
    Constructor constructor;
    Class[] parameterTypes;

    //// Generated:
    VariableHolder[] parameterTargets;

    private static String getTitle(Method m, Object o) {
        return m.getDeclaringClass().getName() + "." + m.getName() + "()";
    }
    
    public MethodFrame(Method _m, Object _obj) {
        super(getTitle(_m,_obj),true,true,false,true);
        method = _m;
        constructor = null;
        member = method;
        parameterTypes = method.getParameterTypes();
        // @todo Now here I really want to find the top declaration of this method (including if it comes from an interface!).
        // Because sometimes we cannot access the methods of inner classes
        // even though the method is overriding a public interface.
        try {
            // if (!method.isAccessible())
            method.setAccessible(true);
            // Well =) this appears to do the trick for Arrays.ArrayList.get(int)
            // But still a proper search for a public ancestor (of a non-inner and hence accessible) class,
            // might still be needed, if this setAccessible(true) doesn't work in Java Security restricted JVMs.
        } catch (SecurityException e) {
        	org.neuralyte.Logger.log("Couldn't make method accessible: " + method);
        }
        // value = _obj;
        // if (_obj == null)
        init(_obj);
    }

    public MethodFrame(Constructor _con) {
        super("" + _con,false,true,false,true);
        method = null;
        // objTarget = null;
        constructor = _con;
        member = constructor;
        parameterTypes = constructor.getParameterTypes();
        init(null);
    }

    void init(Object _obj) {
        // getContentPane().add(new JLabel(VisualJava.getSimpleClassName(method.getReturnType()) + " " + method.getName() + "(" + VisualJava.listParams(method.getParameterTypes()) + ")"));
        getContentPane().setLayout(new FlowLayout());
        if (member instanceof Method) {
            getContentPane().add(new JLabel(VisualJavaStatics.getSimpleClassName(method.getReturnType())));
            if (Modifier.isStatic(method.getModifiers())) {
                objTarget = null;
                getContentPane().add(new JLabel(VisualJavaStatics.getSimpleClassName(member.getDeclaringClass())));
            } else {
                objTarget = new VariableHolder(member.getDeclaringClass(),_obj);
                getContentPane().add(objTarget);
            }
            getContentPane().add(new JLabel("."));
        } else {
            // getContentPane().add(new JLabel("new " + constructor.getDeclaringClass().getName()));
            getContentPane().add(new JLabel("new"));
            objTarget = null;
        }
        JButton button = new JButton(member.getName());
        button.addActionListener(
            new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    tryToInvoke();
                }
            }
        );
        getContentPane().add(button);
        getContentPane().add(new JLabel("("));
        parameterTargets = new VariableHolder[parameterTypes.length];
        for (int i=0;i<parameterTypes.length;i++) {
            Class type = parameterTypes[i];
            VariableHolder parameterTarget = new VariableHolder(type);
            parameterTargets[i] = parameterTarget;
            getContentPane().add(parameterTarget);
            if (i < parameterTypes.length - 1) {
                getContentPane().add(new JLabel(","));
            }
        }
        getContentPane().add(new JLabel(")"));
        setSize(this.getPreferredSize());

        // This hack attempts to fix the problem that the DropTarget
        // components inside InternalFrames weren't receiving mouseEnter/Exit
        // events when the InternalFrame was not the one currently selected:
        final JInternalFrame frame = this;
        addMouseListener(
            new MouseAdapter() {
                public void mouseEntered(MouseEvent e) {
                    try {
                        frame.setSelected(true);
                    } catch (PropertyVetoException e1) {
                    	org.neuralyte.Logger.error(e1);
                    }
                }
            }
        );
    }

    void tryToInvoke() {
        VisualJava.executeMethodOrConstructor(objTarget, member, parameterTargets);
		// TODO: this used to be called at the end of the invoke thread
        // But really it should be called whenever a parameterTarget is modified.
        setSize(getPreferredSize());
    }

	/*
	private Object getVariableNameFromPT(ParameterTarget parameterTarget) {
		Object toShow = (
		    parameterTarget.variable != null
		    ? parameterTarget.variable
		    : parameterTarget.getObject()
		);
		return toShow;
	}
	*/

}
