package visualjava;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JSeparator;


import org.neuralyte.Logger;
import org.neuralyte.common.JavaStackUtils;
import org.neuralyte.common.swing.draganddrop.DragAndDropManager;
import org.neuralyte.common.swing.draganddrop.HasDragDroppableObject;
import org.neuralyte.common.swing.jmenus.DetachableJMenu;
import org.neuralyte.common.swing.jmenus.SplittingJMenu;

/** joey Nov 1, 2004 2:27:04 AM */
public class VariableView extends JLabel implements HasDragDroppableObject {

    Desktop desktop;

    VariableModel variable;
    
    // Class type;
    // String name;
    // Object value;

    public VariableView(Desktop _desktop, String name, Object _obj) {
        this(_desktop,_obj.getClass(),name,_obj);
    }
    public VariableView(Desktop _desktop, Class _type, String _name, Object _obj) {
        // super(_obj.getClass().getName() + " x = " + _obj);
        super(
        		VisualJavaStatics.getSimpleClassName(_obj.getClass())
        		+ " " + _name + " = " + niceValue(_obj),
        		// new ImageIcon("/usr/share/pixmaps/gnome-gmush.png", "" + _obj),
        		new ImageIcon("src/visualjava/"+VisualJavaGUIStatics.getColorForClass(_type)+"dot-32x32.png", "" + _obj),
        		JLabel.RIGHT
        );
        desktop = _desktop;
        variable = new VariableModel(_type,_name,_obj);
        // JMenu statics = new JMenu("Statics");
        // this.add(statics);
        // JMenuItem statics2 = new JMenuItem("Statics 2");
        // this.add(statics2);
        // VisualJava.addMenuBar(this);
        // Ne marche pas: setBackground(Color.GRAY);
        final VariableView thisVariable = this;
        new Thread() { public void run() { addPopupMenuTo(thisVariable); // Note argument is also used as the Component/Variable to remove from the Desktop.
        } }.start();
        // desktop.displayMethod(_obj.getClass().getDeclaredMethods()[0],value);
        // Moveability.allowUserToMove(this); // Implied by:
        DragAndDropManager.hasObjectCanBeDropped(this);
    }
    
    private static String niceValue(Object _obj) {
    	// DONE: We could do this using the actual result of _obj.toString().
    	// CONSIDER: We could check extra strictly with _obj.hashCode().
    	// CONSIDER: We could check even check the hierarchy of the object's class's toString() method.
    	if ((""+_obj).startsWith(_obj.getClass().getName()+"@")) {
    		// The object's toString() is the default, so let's make our own string...
    		// String niceValue = _obj.getClass().getName() + " {";
    		String niceValue = VisualJavaStatics.getShortClassName(_obj) + " {";
    		// Field[] fields = _obj.getClass().getDeclaredFields(); // Lots of private methods cannot be accessed!
    		Field[] fields = _obj.getClass().getFields();
    		for (int i=0;i<fields.length;i++) {
    			try {
    				niceValue += fields[i].getName() + "=" +VisualJavaStatics.niceString(fields[i].get(_obj));
    				if (i < fields.length-1)
    					niceValue += ", ";
    			} catch (Throwable e) {
    				Logger.info(e);
    			}
    		}
    		niceValue += "}";
    		return VisualJavaStatics.niceString(niceValue);
    	}
    	return VisualJavaStatics.niceString(_obj);
	}

	void addPopupMenuTo(Component thing) {

		final JPopupMenu popup = new JPopupMenu();
		
		// popup.add(DetachableJMenu.getDetacherFor(popup));
		// popup.add(new JSeparator());

        MouseAdapter listener = new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                maybeShowPopup(e);
            }

            public void mouseReleased(MouseEvent e) {
                maybeShowPopup(e);
            }

            private void maybeShowPopup(MouseEvent e) {
                if (e.isPopupTrigger()) {
                    popup.show(e.getComponent(), e.getX(), e.getY());
                }
            }
        };
        //...where the GUI is constructed:
		//Create the popup menu.

        // JMenu statics = new JMenu("Statics");
        // VisualJavaGui.addStaticsToMenu(statics,value.getClass());
        // popup.add(statics);
        JMenu statics = new StaticsMenu("Statics",variable.type.getName());
        if (statics.getItemCount() > 1) // We do >1 rather than >0 because SplittingJMenu's are/were DetachableJMenu's, and hence always had the detach item.
            popup.add(statics);

        Class c = variable.type;

        JMenu properties = new SplittingJMenu("Properties"); // all non-static properties from all superclasses
        for (int i=0;i<c.getFields().length;i++) {
        	Field f = c.getFields()[i];                 // now including private properties!
        	if (!Modifier.isStatic(f.getModifiers())) { // && Modifier.isPublic(f.getModifiers())) {
        		MenuBuilder.addFieldToMenu(f,properties,variable);
        	}
        }
        // For some reason, we need to search DeclaredFields, to get e.g. Desktop's default properties, which we can access.
        // But it also means we get some fields listed twice.
        for (int i=0;i<c.getDeclaredFields().length;i++) {
        	Field f = c.getDeclaredFields()[i];
        	if (!Modifier.isStatic(f.getModifiers())) { // && Modifier.isPublic(f.getModifiers())) {
        		MenuBuilder.addFieldToMenu(f,properties,variable);
        	}
        }
        if (properties.getItemCount() > 1)
            popup.add(properties);

        JMenu getters = new SplittingJMenu("get"); // all non-static accessors from all superclasses
        for (int i=0;i<c.getMethods().length;i++) {
        	Method m = c.getMethods()[i];
        	if (m.getName().startsWith("get") && m.getParameterTypes().length == 0
        			&& !Modifier.isStatic(m.getModifiers())) { // && Modifier.isPublic(f.getModifiers())) {
        		MenuBuilder.addMethodToMenu(m,getters,this);
        		// For some reason this breaks the entire "get" menu!
//        		String decClass = VisualJavaStatics.getSimpleClassName(m.getDeclaringClass());
//        		JMenu subMenu = MenuBuilder.getOrCreateSubMenu(getters,decClass);
//        		MenuBuilder.addMethodToMenu(m,subMenu,this);
        	}
        }
        if (getters.getItemCount() > 1)
        	popup.add(getters);

        JMenu setters = new SplittingJMenu("set"); // all non-static mutators from all superclasses
        for (int i=0;i<c.getMethods().length;i++) {
        	Method m = c.getMethods()[i];
        	if (m.getName().startsWith("set") && m.getParameterTypes().length == 1
        			&& !Modifier.isStatic(m.getModifiers())) { // && Modifier.isPublic(f.getModifiers())) {
        		MenuBuilder.addMethodToMenu(m,setters,this);
        		// For some reason this breaks the entire "get" menu!
//        		String decClass = VisualJavaStatics.getSimpleClassName(m.getDeclaringClass());
//        		JMenu subMenu = MenuBuilder.getOrCreateSubMenu(setters,decClass);
//        		MenuBuilder.addMethodToMenu(m,subMenu,this);
        	}
        }
        if (setters.getItemCount() > 1)
            popup.add(setters);

        SplittingJMenu allMethods = new SplittingJMenu("All methods");
        addAllNonStaticMethodsToMenu(variable.type,allMethods);
        if (allMethods.getItemCount() > 1)
            popup.add(allMethods);

        forEachSuperclassAddAllNonStaticMethodsToMenu(variable.type,popup);

        final Component toRemove = thing;
        JMenuItem item = new JMenuItem("Dispose");
        item.addActionListener(
            new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    desktop.remove(toRemove);
                    // desktop.validate();
                    desktop.repaint();
                }
            }
        );
        popup.add(item);

		//Add listener to components that can bring up popup menus.
		// MouseListener popupListener = new PopupListener();
		thing.addMouseListener(listener);
		// menuBar.addMouseListener(popupListener);

	}

    private void addAllNonStaticMethodsToMenu(Class c, JMenu menu) {
        for (int i=0;i<c.getMethods().length;i++) {
            Method m = c.getMethods()[i];
            if (!Modifier.isStatic(m.getModifiers())) {
            	MenuBuilder.addMethodToMenu(m,menu,this);
            }
        }
    }
    private void forEachSuperclassAddAllNonStaticMethodsToMenu(Class c, JPopupMenu mainMenu) {
        while (c != null) {
            SplittingJMenu menu = new SplittingJMenu(c.getName());
            addNonStaticMethodsToSplittingJMenu(c,menu,false);
            mainMenu.add(menu);
            c = c.getSuperclass();
        }
    }
    private void addNonStaticMethodsToSplittingJMenu(Class c, SplittingJMenu methods, boolean andSuperClasses) {
        try {
            if (andSuperClasses) {
                Class superClass = c.getSuperclass();
                if (superClass != null) {
                    SplittingJMenu superClassMethodsMenu = new SplittingJMenu(VisualJavaStatics.getSimpleClassName(superClass));
                    addNonStaticMethodsToSplittingJMenu(superClass,superClassMethodsMenu,andSuperClasses);
                    methods.addDontSplit(superClassMethodsMenu);
                }
            }
            for (int i=0;i<c.getDeclaredMethods().length;i++) {
                Method m = c.getDeclaredMethods()[i];
                if (!Modifier.isStatic(m.getModifiers()) && !Modifier.isPrivate(m.getModifiers())) {
                	MenuBuilder.addMethodToMenu(m,methods,this);
                }
            }
        } catch (Exception e) {
            org.neuralyte.Logger.error(e);
        }
    }

    public Object getDraggedObject() {
        // return value;
        return this;
    }

}
