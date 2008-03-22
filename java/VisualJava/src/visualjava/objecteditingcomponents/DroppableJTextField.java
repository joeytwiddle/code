package visualjava.objecteditingcomponents;

import java.awt.Dimension;

import javax.swing.JTextField;

import org.neuralyte.common.swing.draganddrop.CanAcceptDroppedObject;

/** joey Nov 5, 2004 4:55:17 PM */
public class DroppableJTextField extends JTextField implements CanAcceptDroppedObject {
    Class type;
    public DroppableJTextField(String text, Class _type) {
        super(text);
        type = _type;
    }
    public boolean isAcceptable(Object o) {
        if (!type.isAssignableFrom(o.getClass()));
        org.neuralyte.Logger.log("Reason for rejection: ! "+type+".isAssignableFrom("+o.getClass()+")");
        return (type.isAssignableFrom(o.getClass()));
    }
    public boolean acceptObject(Object o) {
        setMaximumSize(new Dimension(200,getHeight()));
        setText("" + o);
        return true;
    }
}
