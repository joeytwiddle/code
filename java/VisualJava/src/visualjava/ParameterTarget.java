package visualjava;

import visualjava.objecteditingcomponents.BooleanChooser;
import visualjava.objecteditingcomponents.DroppableJLabel;
import visualjava.objecteditingcomponents.DroppableJTextField;

import javax.swing.*;
import java.awt.dnd.DropTarget;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

import org.neuralyte.common.swing.Moveability;
import org.neuralyte.common.swing.DragAndDropManager;
import org.neuralyte.common.swing.HasDragDroppableObject;
import org.neuralyte.common.swing.CanAcceptDroppedObject;

/**
 * @todo Consider how user might be able to pass null instead of a non-null Object.
 */

/** joey Nov 2, 2004 12:36:40 AM */
public class ParameterTarget extends JPanel implements CanAcceptDroppedObject {

    Class type;

    Variable variable;

    JComponent component;

    public ParameterTarget(Class _type) {
        this(_type,null);
    }
    // public ParameterTarget(Object object) {
        // this(object.getClass(),object);
    // }
    // public ParameterTarget(Variable variable) {
        // this(variable.value.getClass(),variable);
    // }
    public ParameterTarget(Class _type, Object object) {
        // if (object instanceof Variable) {
            // _type = ((Variable)object).type;
            // System.out.println("Creating variable parametertarget for class " + _type + " object " + object);
        // }
        System.out.println("Creating parametertarget for class " + _type + " object " + object);
        type = _type;
        if (type.equals(Boolean.class) || type.equals(Boolean.TYPE)) {
            component = new BooleanChooser();
        /* } else if (type.isPrimitive() || type.equals(String.class) || type.equals(Double.class) ... */
        } else if (ReflectionHelper.isPrimitiveType(type) || type.equals(String.class)) {
            // System.out.println("type = " + type);
            // component = new JTextField("" + VisualJavaStatics.getSimpleClassName(type));
            component = new DroppableJTextField("" + VisualJavaStatics.getSimpleClassName(type),type);
        } else {
            // component = new JButton("" + VisualJava.getSimpleClassName(type));
            // component = new JLabel("" + VisualJavaStatics.getSimpleClassName(type));
            component = new DroppableJLabel(VisualJavaStatics.getSimpleClassName(type),type);
        }
        if (object != null && component != null) {
            // ((CanAcceptDroppedObject)component).acceptObject(object);
            acceptObject(object);
        }
        DragAndDropManager.canAcceptDroppedObject(this);
        // component.addMouseListener(
        this.addMouseListener(
            new MouseAdapter() {
                public void mouseClicked(MouseEvent e) {
                    if (e.getButton() == MouseEvent.BUTTON3) {
                        JMenu statics = new StaticsMenu(type.getName(),type.getName());
                        JPopupMenu popup = new JPopupMenu("Wish JMenu's and JPopupMenu's had a common ancestor!");
                        popup.add(statics);
                        popup.show(e.getComponent(), e.getX(), e.getY());
                    }
                }
            }
        );
        add(component);
        // this.setDropTarget();
    }

    public Object getObject() {
        if (component instanceof HasDragDroppableObject) {
            return ((HasDragDroppableObject)component).getDraggedObject();
        }
        if (component instanceof JTextField) {
            String text = ((JTextField)component).getText();
            Object object = ReflectionHelper.createPrimitiveObjectFromText(text,type);
            if (object != null) {
                return object;
            }
            System.err.println("Could not instantiate a " + type.getName() + " from \"" + text + "\".");
        // } else if (component instanceof ObjectTarget) {
            // return ((ObjectTarget)component).getDraggedObject();
        }
        /* || @todo ... */
        return null;
    }

    public boolean isAcceptable(Object o) {
        if (o instanceof Variable) {
            Object contained = ((Variable)o).value;
            return isAcceptable(contained);
        } else {
            return ((CanAcceptDroppedObject)component).isAcceptable(o);
        }
    }

    public boolean acceptObject(Object o) {
        if (o instanceof Variable) {
            Object contained = ((Variable)o).value;
            boolean accepted = acceptObject(contained);
            if (accepted) {
                variable = (Variable)o;
                if (component instanceof JLabel) {
                    ((JLabel)component).setText(variable.name);
                }
            }
            return accepted;
        } else {
            boolean accepted = ((CanAcceptDroppedObject)component).acceptObject(o);
            if (accepted) {
                variable = null;
            }
            if (component instanceof JLabel) {
                ((JLabel)component).setText("I hope you dropped a variable, cos that's what I expect!");
            }
            return accepted;
        }
    }

}
