package visualjava;

import javax.swing.*;
import java.lang.reflect.Method;

/** joey Nov 1, 2004 7:51:00 PM */
public class MethodFrame extends JInternalFrame {
    Method method;
    Object obj;
    public MethodFrame(Method _m, Object _obj) {
        super("" + _m,false,true,false,false);
        method = _m;
        obj = _obj;
        getContentPane().add(new JLabel(VisualJava.getSimpleClassName(method.getReturnType()) + " " + method.getName() + "(" + VisualJava.listParams(method.getParameterTypes()) + ")"));
    }
}
