package visualjava;

import javax.swing.*;
import java.lang.reflect.*;
import java.awt.*;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.beans.PropertyVetoException;

/** joey Nov 1, 2004 7:51:00 PM */
public class MethodFrame extends JInternalFrame {

    //// Defined:
    // Object value; // Null if constructor, or if method is static
    ParameterTarget objTarget; // (well actually generated!) Null if constructor, or if method is static
    Member member; // Will be either the method or the constructor, the other will be null.

    //// Obtained:
    Method method;
    Constructor constructor;
    Class[] parameterTypes;

    //// Generated:
    ParameterTarget[] parameterTargets;

    public MethodFrame(Method _m, Object _obj) {
        super("" + _m,true,true,false,true);
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
        } catch (SecurityException e) {
            System.out.println("Couldn't make method accessible: " + method);
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
                objTarget = new ParameterTarget(member.getDeclaringClass(),_obj);
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
        parameterTargets = new ParameterTarget[parameterTypes.length];
        for (int i=0;i<parameterTypes.length;i++) {
            Class type = parameterTypes[i];
            ParameterTarget parameterTarget = new ParameterTarget(type);
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
                        e1.printStackTrace(System.err);
                    }
                }
            }
        );
    }

    void tryToInvoke() {
        StringBuffer parameterCode = new StringBuffer();
        Object[] arguments = new Object[parameterTypes.length];
        for (int i=0;i<parameterTargets.length;i++) {
            ParameterTarget parameterTarget = parameterTargets[i];
            arguments[i] = parameterTarget.getObject();
            Object toShow = (
                parameterTarget.variable != null
                ? parameterTarget.variable
                : parameterTarget.getObject()
            );
            parameterCode.append(getTextForObject(toShow));
            if (i < parameterTargets.length - 1) {
                parameterCode.append(", ");
            }
        }
        try {
            Object result;
            StringBuffer code = new StringBuffer();
            if (member instanceof Method) {
                Object on = (
                    objTarget == null
                    ? null
                    : objTarget.variable == null
                        ? objTarget.getObject()
                        : objTarget.variable
                );
                code.append(
                    on == null
                    ? method.getDeclaringClass().getName()
                    : getTextForObject(on)
                );
                code.append("." + method.getName()+ "(");
                Object reallyOn = ( objTarget == null ? null : objTarget.getObject() );
                result = method.invoke(reallyOn,arguments);
            } else {
                code.append("new " + constructor.getDeclaringClass().getName() /*+ "." + constructor.getName()*/ + "(");
                result = constructor.newInstance(arguments);
            }
            code.append(parameterCode);
            code.append(");");
            if (result == null) {
                System.out.println("Result was null.");
            } else {
                String nameOfNewDesktopVariable = VisualJava.desktop.showObject(result);
                Class returnType = ( member instanceof Method ? method.getReturnType() : constructor.getDeclaringClass() );
                code.insert(0,returnType.getName() + " " + nameOfNewDesktopVariable + " = ");
            }
            System.out.println("[CODE] " + code);
        } catch (InvocationTargetException e) {
            VisualJava.desktop.showObject(e.getCause());
        } catch (Exception e) {
            e.printStackTrace();
            // System.err.println(e);
            VisualJava.desktop.showObject(e);
        }
        setSize(this.getPreferredSize());
    }

    String getTextForObject(Object o) {
        if (o instanceof Variable) {
            return ((Variable)o).name;
        } else {
            return "<unknown " + o.getClass().getName()+">";
        }
    }

}
