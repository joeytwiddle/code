package visualjava;

import javax.swing.*;
import java.awt.dnd.DropTarget;

/** joey Nov 2, 2004 12:36:40 AM */
public class ParameterTarget extends JPanel {
    Class type;
    JComponent component;
    public ParameterTarget(Class _type) {
        type = _type;
        if (type.isPrimitive()
            || type.equals(String.class)
            || type.equals(Double.class)
            /* || @todo ... */
        ) {
            component = new JTextField("" + VisualJava.getSimpleClassName(type));
        } else {
            // component = new JButton("" + VisualJava.getSimpleClassName(type));
            component = new JLabel("" + VisualJava.getSimpleClassName(type));
        }
        add(component);
        // this.setDropTarget();
    }
}
