package visualjava;

import javax.swing.*;
import java.lang.reflect.Method;
import java.awt.*;

/** joey Nov 1, 2004 7:51:00 PM */
public class MethodFrame extends JInternalFrame {
    Method method;
    Object obj;
    public MethodFrame(Method _m, Object _obj) {
        super("" + _m,false,true,false,false);
        method = _m;
        obj = _obj;
        // getContentPane().add(new JLabel(VisualJava.getSimpleClassName(method.getReturnType()) + " " + method.getName() + "(" + VisualJava.listParams(method.getParameterTypes()) + ")"));
        getContentPane().setLayout(new FlowLayout());
        getContentPane().add(new JLabel(VisualJava.getSimpleClassName(method.getReturnType())));
        getContentPane().add(new JButton(method.getName()));
        getContentPane().add(new JLabel("("));
        for (int i=0;i<method.getParameterTypes().length;i++) {
            Class type = method.getParameterTypes()[i];
            getContentPane().add(new ParameterTarget(type));
            if (i < method.getParameterTypes().length - 1) {
                getContentPane().add(new JLabel(","));
            }
        }
        getContentPane().add(new JLabel(")"));
        setSize(this.getPreferredSize());
    }
}
