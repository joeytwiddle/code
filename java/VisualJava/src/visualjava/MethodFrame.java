package visualjava;

import javax.swing.*;
import java.lang.reflect.Method;
import java.lang.reflect.Constructor;
import java.lang.reflect.Member;
import java.lang.reflect.InvocationTargetException;
import java.awt.*;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.beans.PropertyVetoException;

/** joey Nov 1, 2004 7:51:00 PM */
public class MethodFrame extends JInternalFrame {

    Member member; // Will be either the method or the constructor, the other will be null.
    Method method;
    Constructor constructor;
    Object obj; // Null if constructor, or if method is static
    Class[] parameterTypes;

    ParameterTarget[] parameterTargets;

    public MethodFrame(Method _m, Object _obj) {
        super("" + _m,false,true,false,false);
        method = _m;
        obj = _obj;
        constructor = null;
        member = method;
        parameterTypes = method.getParameterTypes();
        init();
    }

    public MethodFrame(Constructor _con) {
        super("" + _con,false,true,false,false);
        method = null;
        obj = null;
        constructor = _con;
        member = constructor;
        parameterTypes = constructor.getParameterTypes();
        init();
    }

    void init() {
        // getContentPane().add(new JLabel(VisualJava.getSimpleClassName(method.getReturnType()) + " " + method.getName() + "(" + VisualJava.listParams(method.getParameterTypes()) + ")"));
        getContentPane().setLayout(new FlowLayout());
        if (member instanceof Method) {
            getContentPane().add(new JLabel(VisualJavaStatics.getSimpleClassName(method.getReturnType())));
        } else {
            // getContentPane().add(new JLabel("new " + constructor.getDeclaringClass().getName()));
            getContentPane().add(new JLabel("new"));
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
        Object[] arguments = new Object[parameterTypes.length];
        for (int i=0;i<parameterTargets.length;i++) {
            ParameterTarget parameterTarget = parameterTargets[i];
            arguments[i] = parameterTarget.getObject();
        }
        try {
            Object result;
            if (member instanceof Method) {
                result = method.invoke(obj,arguments);
            } else {
                result = constructor.newInstance(arguments);
            }
            if (result == null) {
                System.out.println("Result was null.");
            } else {
                VisualJava.desktop.showObject(result);
            }
        } catch (Exception e) {
            // e.printStackTrace();
            System.err.println(e);
            VisualJava.desktop.showObject(e);
        }
        setSize(this.getPreferredSize());
    }

}
