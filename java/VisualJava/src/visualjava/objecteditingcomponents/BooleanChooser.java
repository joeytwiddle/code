package visualjava.objecteditingcomponents;

import visualjava.HasObject;

import javax.swing.*;

/** joey Nov 2, 2004 3:53:53 PM */
public class BooleanChooser extends JCheckBox implements HasObject {
    public Object getObject() {
        // return new Boolean(this.isSelected());
        return Boolean.valueOf(this.isSelected()); // more efficient
    }
}
