package visualjava.objecteditingcomponents;

import org.neuralyte.common.swing.HasDragDroppableObject;
import org.neuralyte.common.swing.CanAcceptDroppedObject;

import javax.swing.*;

import visualjava.VisualJavaStatics;

/** joey Nov 5, 2004 4:54:30 PM */
public class DroppableJLabel extends JLabel implements HasDragDroppableObject, CanAcceptDroppedObject {
    Object object;
    Class type;
    public DroppableJLabel(String text, Class _type) {
        super(text);
        type = _type;
        object = null;
    }
    public boolean isAcceptable(Object o) {
        return (type.isAssignableFrom(o.getClass()));
    }
    public boolean acceptObject(Object o) {
        object = o;
        // setText(VisualJavaStatics.getSimpleClassName(type) + " \"" + o + "\"");
        return true;
    }
    public Object getDraggedObject() {
        return object;
    }
}
