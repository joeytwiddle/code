package visualjava.objecteditingcomponents;

import org.neuralyte.common.swing.HasDragDroppableObject;
import org.neuralyte.common.swing.CanAcceptDroppedObject;

import javax.swing.*;

/** joey Nov 2, 2004 3:53:53 PM */
public class BooleanChooser extends JCheckBox implements HasDragDroppableObject, CanAcceptDroppedObject {
    public Object getDraggedObject() {
        // return new Boolean(this.isSelected());
        return Boolean.valueOf(this.isSelected()); // more efficient
    }

    public boolean acceptObject(Object o) {
        return Boolean.class.equals(o.getClass());
    }

    public boolean isAcceptable(Object o) {
        this.setSelected(((Boolean)o).booleanValue());
        return true;
    }
}
