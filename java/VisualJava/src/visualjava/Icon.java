package visualjava;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.Field;

/** joey Nov 1, 2004 2:27:04 AM */
public class Icon extends JLabel {

    Object obj;

    public Icon(Object _obj) {
        // super(_obj.getClass().getName() + " x = " + _obj);
        super(new ImageIcon("/usr/share/pixmaps/gnome-gmush.png", "" + _obj));
        obj = _obj;
        // JMenu statics = new JMenu("Statics");
        // this.add(statics);
        // JMenuItem statics2 = new JMenuItem("Statics 2");
        // this.add(statics2);
        // VisualJava.addMenuBar(this);
        addPopupMenuTo(this);
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
        JMenuItem menuItem;
		menuItem = new JMenuItem("A popup menu item");
		// menuItem.addActionListener(listener);
		popup.add(menuItem);
		menuItem = new JMenuItem("Another popup menu item");
		// menuItem.addActionListener(listener);
		popup.add(menuItem);

        JMenu statics = new JMenu("Statics");
        try {
            Class c = obj.getClass();
            for (int i=0;i<c.getDeclaredMethods().length;i++) {
                Method m = c.getDeclaredMethods()[i];
                if (Modifier.isStatic(m.getModifiers())) {
                    menuItem = new JMenuItem(m.getName() + "(" + m.getParameterTypes() + ")");
                    statics.add(menuItem);
                }
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        popup.add(statics);

        JMenu properties = new JMenu("Methods");
        try {
            Class c = obj.getClass();
            for (int i=0;i<c.getDeclaredFields().length;i++) {
                Field f = c.getDeclaredFields()[i];
                if (!Modifier.isStatic(f.getModifiers())) {
                    menuItem = new JMenuItem(f.getName() + " = " + f.get(obj));
                    properties.add(menuItem);
                }
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        popup.add(properties);

        JMenu methods = new JMenu("Methods");
        try {
            Class c = obj.getClass();
            for (int i=0;i<c.getDeclaredMethods().length;i++) {
                Method m = c.getDeclaredMethods()[i];
                if (!Modifier.isStatic(m.getModifiers())) {
                    menuItem = new JMenuItem(m.getName() + "(" + m.getParameterTypes() + ")");
                    methods.add(menuItem);
                }
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        popup.add(methods);

		//Add listener to components that can bring up popup menus.
		// MouseListener popupListener = new PopupListener();
		thing.addMouseListener(listener);
		// menuBar.addMouseListener(popupListener);

	}
}
