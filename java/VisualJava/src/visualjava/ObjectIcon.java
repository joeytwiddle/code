package visualjava;

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
public class ObjectIcon extends JLabel {

    Desktop desktop;

    Object obj;

    public ObjectIcon(Desktop _desktop, Object _obj) {
        // super(_obj.getClass().getName() + " x = " + _obj);
        super(VisualJava.getSimpleClassName(_obj.getClass()) + " x = \"" + _obj + "\"", new ImageIcon("/usr/share/pixmaps/gnome-gmush.png", "" + _obj), JLabel.RIGHT);
        desktop = _desktop;
        obj = _obj;
        // JMenu statics = new JMenu("Statics");
        // this.add(statics);
        // JMenuItem statics2 = new JMenuItem("Statics 2");
        // this.add(statics2);
        // VisualJava.addMenuBar(this);
        addPopupMenuTo(this);
        // desktop.displayMethod(_obj.getClass().getDeclaredMethods()[0],obj);
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
		// menuItem.addActionListener(listener);

        // JMenu statics = new JMenu("Statics");
        // VisualJavaGui.addStaticsToMenu(statics,obj.getClass());
        // popup.add(statics);
        popup.add(new StaticsMenu("Statics",obj.getClass().getName()));

        JMenu properties = new JMenu("Properties");
        try {
            Class c = obj.getClass();
            for (int i=0;i<c.getDeclaredFields().length;i++) {
                Field f = c.getDeclaredFields()[i];
                if (!Modifier.isStatic(f.getModifiers()) && Modifier.isPublic(f.getModifiers())) {
                    VisualJavaGui.addFieldToMenu(f,properties,obj);
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
                    VisualJavaGui.addMethodToMenu(m,methods,obj);
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
