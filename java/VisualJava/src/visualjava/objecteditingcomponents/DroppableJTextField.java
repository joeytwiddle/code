package visualjava.objecteditingcomponents;

import org.neuralyte.common.swing.CanAcceptDroppedObject;

import javax.swing.*;
import java.awt.*;

/** joey Nov 5, 2004 4:55:17 PM */
public class DroppableJTextField extends JTextField implements CanAcceptDroppedObject {
    Class type;
    public DroppableJTextField(String text, Class _type) {
        super(text);
        type = _type;
    }
    public boolean isAcceptable(Object o) {
        if (!type.isAssignableFrom(o.getClass()));
            System.out.println("Reason for rejection: ! "+type+".isAssignableFrom("+o.getClass()+")");
        return (type.isAssignableFrom(o.getClass()));
    }
    public boolean acceptObject(Object o) {
        setMaximumSize(new Dimension(200,getHeight()));
        setText("" + o);
        return true;
    }
}
