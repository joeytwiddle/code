package visualjava;

import visualjava.objecteditingcomponents.BooleanChooser;

import javax.swing.*;
import java.awt.dnd.DropTarget;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

import org.neuralyte.common.swing.Moveability;

/**
 * @todo Consider how user might be able to pass null instead of a non-null Object.
 */

/** joey Nov 2, 2004 12:36:40 AM */
public class ParameterTarget extends JPanel {

    Class type;

    JComponent component;

    public ParameterTarget(Class _type) {
        type = _type;
        if (type.equals(Boolean.class) || type.equals(Boolean.TYPE)) {
            component = new BooleanChooser();
        /* } else if (type.isPrimitive() || type.equals(String.class) || type.equals(Double.class) ... */
        } else if (ReflectionHelper.isPrimitiveType(type) || type.equals(String.class)) {
            // System.out.println("type = " + type);
            // component = new JTextField("" + VisualJavaStatics.getSimpleClassName(type));
            component = new DroppableJTextField("" + VisualJavaStatics.getSimpleClassName(type),type);
            Moveability.canAcceptDroppedObject(component);
        } else {
            // component = new JButton("" + VisualJava.getSimpleClassName(type));
            // component = new JLabel("" + VisualJavaStatics.getSimpleClassName(type));
            component = new DroppableJLabel(VisualJavaStatics.getSimpleClassName(type),type);
            Moveability.canAcceptDroppedObject(component);
        }
        add(component);
        // this.setDropTarget();
    }

    public Object getObject() {
        if (component instanceof HasObject) {
            return ((HasObject)component).getObject();
        }
        if (component instanceof JTextField) {
            String text = ((JTextField)component).getText();
            Object object = ReflectionHelper.createPrimitiveObjectFromText(text,type);
            if (object != null) {
                return object;
            }
            System.err.println("Could not instantiate a " + type.getName() + " from \"" + text + "\".");
        // } else if (component instanceof ObjectTarget) {
            // return ((ObjectTarget)component).getObject();
        }
        /* || @todo ... */
        return null;
    }

    class DroppableJLabel extends JLabel implements HasObject, CanAcceptDroppedObject {
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
            setText(VisualJavaStatics.getSimpleClassName(type) + " \"" + o + "\"");
            return true;
        }
        public Object getObject() {
            return object;
        }
    }

    class DroppableJTextField extends JTextField implements CanAcceptDroppedObject {
        Class type;
        public DroppableJTextField(String text, Class _type) {
            super(text);
            type = _type;
        }
        public boolean isAcceptable(Object o) {
            return (type.isAssignableFrom(o.getClass()));
        }
        public boolean acceptObject(Object o) {
            setText("" + o);
            return true;
        }
    }

}
