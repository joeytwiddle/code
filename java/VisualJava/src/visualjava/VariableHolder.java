package visualjava;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JTextField;

import org.neuralyte.common.swing.draganddrop.CanAcceptDroppedObject;
import org.neuralyte.common.swing.draganddrop.DragAndDropManager;
import org.neuralyte.common.swing.draganddrop.HasDragDroppableObject;

import visualjava.objecteditingcomponents.BooleanChooser;
import visualjava.objecteditingcomponents.DroppableJLabel;
import visualjava.objecteditingcomponents.DroppableJTextField;

/**
 * VariableHolder is a Component which can have a Variable (or just a value) dropped into it.
 * It may start holding an existing variable or value.
 * It is also a Factory, generating Swing components which allow the user to edit the object, if the object is editable (currently implemented for java primitives only).
 * Otherwise it just shows a JLabel, and implements CanAcceptDroppedObject.
 * 
 * @todo Consider how user might be able to pass null instead of a non-null Object.
 */

/** joey Nov 2, 2004 12:36:40 AM */
public class VariableHolder extends JPanel implements CanAcceptDroppedObject {

	Class type;

	VariableView variable;

	JComponent component;

	public VariableHolder(Class _type) {
		this(_type, null);
	}

	// public ParameterTarget(Object object) {
	// this(object.getClass(),object);
	// }
	// public ParameterTarget(Variable variable) {
	// this(variable.value.getClass(),variable);
	// }
	public VariableHolder(Class _type, Object object) {
		// if (object instanceof Variable) {
		// _type = ((Variable)object).type;
		// org.neuralyte.Logger.log("Creating variable parametertarget for class
		// " + _type + " object " + object);
		// }

		// TODO: why do we bother to create DroppableJLabel and
		// DroppableJTextField, we we
		// already implement CanAcceptDraggedObject here, and in fact this is
		// the only place from
		// which we call canAcceptDroppedObject(this); ?

		// Oh I see, we pass the isAcceptable call down to them. O_o

		// But there is no need to do that check, since we have the class type
		// here!

		// Hmm well for some reason DroppableJTextField uses isAssignableFrom()
		// - maybe this is a special case needed for primitives.

		org.neuralyte.Logger.log("Creating parametertarget for class " + _type
				+ " object " + object);
		type = _type;
		if (type.equals(Boolean.class) || type.equals(Boolean.TYPE)) {
			component = new BooleanChooser();
			/*
			 * } else if (type.isPrimitive() || type.equals(String.class) ||
			 * type.equals(Double.class) ...
			 */
		} else if (ReflectionHelper.isPrimitiveType(type)
				|| type.equals(String.class)) {
			// org.neuralyte.Logger.log("type = " + type);
			// component = new JTextField("" +
			// VisualJavaStatics.getSimpleClassName(type));
			component = new DroppableJTextField(""
					+ VisualJavaStatics.getSimpleClassName(type), type);
		} else {
			// component = new JButton("" +
			// VisualJava.getSimpleClassName(type));
			// component = new JLabel("" +
			// VisualJavaStatics.getSimpleClassName(type));
			component = new DroppableJLabel(VisualJavaStatics
					.getSimpleClassName(type), type);
		}
		if (object != null && component != null) {
			// ((CanAcceptDroppedObject)component).acceptObject(object);
			acceptObject(object);
		}
		DragAndDropManager.canAcceptDroppedObject(this);
		// component.addMouseListener(
		this.addMouseListener(new MouseAdapter() {
			public void mouseClicked(MouseEvent e) {
				if (e.getButton() == MouseEvent.BUTTON3) {
					JMenu statics = new StaticsMenu(type.getName(), type
							.getName());
					JPopupMenu popup = new JPopupMenu(
							"Wish JMenu's and JPopupMenu's had a common ancestor!");
					popup.add(statics);
					popup.show(e.getComponent(), e.getX(), e.getY());
				}
			}
		});
		add(component);
		// this.setDropTarget();
	}

	public Object getObject() {
		if (component instanceof HasDragDroppableObject) {
			return ((HasDragDroppableObject) component).getDraggedObject();
		}
		if (component instanceof JTextField) {
			String text = ((JTextField) component).getText();
			Object object = ReflectionHelper.createPrimitiveObjectFromText(
					text, type);
			if (object != null) {
				return object;
			}
			org.neuralyte.Logger.error("Could not instantiate a "
					+ type.getName() + " from \"" + text + "\".");
			// } else if (component instanceof ObjectTarget) {
			// return ((ObjectTarget)component).getDraggedObject();
		}
		/* || @todo ... */
		return null;
	}

	public boolean isAcceptable(Object o) {
		if (o instanceof VariableView) {
			Object contained = ((VariableView) o).variable.value;
			return isAcceptable(contained);
		} else {
			return ((CanAcceptDroppedObject) component).isAcceptable(o);
		}
	}

	public boolean acceptObject(Object o) {
		if (o instanceof VariableView) {
			Object contained = ((VariableView) o).variable.value;
			boolean accepted = acceptObject(contained);
			if (accepted) {
				variable = (VariableView) o;
				if (component instanceof JLabel) {
					((JLabel) component).setText(variable.variable.name);
				}
			}
			return accepted;
		} else {
			boolean accepted = ((CanAcceptDroppedObject) component)
					.acceptObject(o);
			if (accepted) {
				variable = null;
			}
			if (component instanceof JLabel) {
				((JLabel) component)
						.setText("I hope you dropped a variable, cos that's what I expect!");
			}
			return accepted;
		}
	}

}
