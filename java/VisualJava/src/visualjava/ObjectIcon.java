package visualjava;

import org.neuralyte.common.swing.LargeCapacityJMenu;
import org.neuralyte.common.swing.Moveability;
import org.neuralyte.common.swing.SplittingJMenu;
import org.neuralyte.common.swing.DragAndDropManager;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.Field;
import java.lang.reflect.Constructor;

/** joey Nov 1, 2004 2:27:04 AM */
public class ObjectIcon extends JLabel implements HasObject {

    Desktop desktop;

    Object obj;

    public ObjectIcon(Desktop _desktop, Object _obj) {
        // super(_obj.getClass().getName() + " x = " + _obj);
        super(VisualJavaStatics.getSimpleClassName(_obj.getClass()) + " x = \"" + _obj + "\"", new ImageIcon("/usr/share/pixmaps/gnome-gmush.png", "" + _obj), JLabel.RIGHT);
        desktop = _desktop;
        obj = _obj;
        // JMenu statics = new JMenu("Statics");
        // this.add(statics);
        // JMenuItem statics2 = new JMenuItem("Statics 2");
        // this.add(statics2);
        // VisualJava.addMenuBar(this);
        setBackground(Color.GRAY);
        addPopupMenuTo(this);
        // desktop.displayMethod(_obj.getClass().getDeclaredMethods()[0],obj);
        // Moveability.allowUserToMove(this); // Implied by:
        DragAndDropManager.hasObjectCanBeDropped(this);
    }

	void addPopupMenuTo(Component thing) {

		final JPopupMenu popup = new JPopupMenu();

        MouseAdapter listener = new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                maybeShowPopup(e);
            }

            public void mouseReleased(MouseEvent e) {
                maybeShowPopup(e);
            }

            private void maybeShowPopup(MouseEvent e) {
                if (e.isPopupTrigger()) {
                    popup.show(e.getComponent(),
                            e.getX(), e.getY());
                }
            }
        };
        //...where the GUI is constructed:
		//Create the popup menu.

        // JMenu statics = new JMenu("Statics");
        // VisualJavaGui.addStaticsToMenu(statics,obj.getClass());
        // popup.add(statics);
        JMenu statics = new StaticsMenu("Statics",obj.getClass().getName());
        if (statics.getItemCount() > 1) // We do >1 rather than >0 because SplittingJMenu's are/were DetachableJMenu's, and hence always had the detach item.
            popup.add(statics);

        JMenu properties = new SplittingJMenu("Properties"); // all non-static properties from all superclasses
        try {
            Class c = obj.getClass();
            for (int i=0;i<c.getFields().length;i++) {
                Field f = c.getFields()[i];                 // now including private properties!
                if (!Modifier.isStatic(f.getModifiers())) { // && Modifier.isPublic(f.getModifiers())) {
                    VisualJavaGUIStatics.addFieldToMenu(f,properties,obj);
                }
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        if (properties.getItemCount() > 1)
            popup.add(properties);

        JMenu getters = new SplittingJMenu("get"); // all non-static accessors from all superclasses
        try {
            Class c = obj.getClass();
            for (int i=0;i<c.getMethods().length;i++) {
                Method m = c.getMethods()[i];
                if (m.getName().startsWith("get") && m.getParameterTypes().length == 0
                    && !Modifier.isStatic(m.getModifiers())) { // && Modifier.isPublic(f.getModifiers())) {
                    VisualJavaGUIStatics.addMethodToMenu(m,getters,obj);
                }
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        if (getters.getItemCount() > 1)
            popup.add(getters);

        JMenu setters = new SplittingJMenu("set"); // all non-static mutators from all superclasses
        try {
            Class c = obj.getClass();
            for (int i=0;i<c.getMethods().length;i++) {
                Method m = c.getMethods()[i];
                if (m.getName().startsWith("set") && m.getParameterTypes().length == 1
                    && !Modifier.isStatic(m.getModifiers())) { // && Modifier.isPublic(f.getModifiers())) {
                    VisualJavaGUIStatics.addMethodToMenu(m,setters,obj);
                }
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        if (setters.getItemCount() > 1)
            popup.add(setters);

        forEachSuperclassAddAllNonStaticMethodsToMenu(obj.getClass(),popup);

        SplittingJMenu allMethods = new SplittingJMenu("All methods");
        addAllNonStaticMethodsToMenu(obj.getClass(),allMethods);
        if (allMethods.getItemCount() > 1)
            popup.add(allMethods);

		//Add listener to components that can bring up popup menus.
		// MouseListener popupListener = new PopupListener();
		thing.addMouseListener(listener);
		// menuBar.addMouseListener(popupListener);

	}

    private void forEachSuperclassAddAllNonStaticMethodsToMenu(Class c, JPopupMenu mainMenu) {
        while (c != null) {
            SplittingJMenu menu = new SplittingJMenu(c.getName());
            addNonStaticMethodsToSplittingJMenu(c,menu,false);
            mainMenu.add(menu);
            c = c.getSuperclass();
        }
    }
    private void addAllNonStaticMethodsToMenu(Class c, JMenu menu) {
        for (int i=0;i<c.getMethods().length;i++) {
            Method m = c.getMethods()[i];
            if (!Modifier.isStatic(m.getModifiers())) {
                VisualJavaGUIStatics.addMethodToMenu(m,menu,obj);
            }
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
                if (!Modifier.isStatic(m.getModifiers())) {
                    VisualJavaGUIStatics.addMethodToMenu(m,methods,obj);
                }
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
    }

    public Object getObject() {
        return obj;
    }
}
