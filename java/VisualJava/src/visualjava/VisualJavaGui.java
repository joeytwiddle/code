package visualjava;

import visualjava.Viewer;

import javax.swing.*;
import java.lang.reflect.Constructor;
import java.lang.reflect.Modifier;
import java.lang.reflect.Method;
import java.lang.reflect.Field;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

/** joey Jul 11, 2004 3:27:54 AM */
public class VisualJavaGui {

	public static void oldShowDeprecated(Object toShow) {
		java.lang.Class viewerType = VisualJava.viewerForObject(toShow);
		Viewer viewer = VisualJava.invokeViewer(viewerType,toShow);
		javax.swing.JFrame frame = new javax.swing.JFrame();
		javax.swing.JPanel panel = new javax.swing.JPanel();
		viewer.refreshGui(panel);
		frame.getContentPane().add(panel);
		frame.setSize(400,400);
		frame.setTitle(viewer.getTitle());
		frame.setVisible(true);
	}

    public static void addStaticsToMenu(JMenu statics, Class c) {
        try {
            for (int i=0;i<c.getConstructors().length;i++) {
                Constructor con = c.getConstructors()[i];
                if (Modifier.isPublic(con.getModifiers())) {
                    // menuItem = new JMenuItem("new " + con.getName() + "(" + listParams(con.getParameterTypes()) + ")");
                    addConstructorToMenu(con, statics);
                }
            }
            // statics.add(new JMenuItem("--------"));
            statics.add(new JSeparator());
            for (int i=0;i<c.getDeclaredMethods().length;i++) {
                Method m = c.getDeclaredMethods()[i];
                if (Modifier.isStatic(m.getModifiers())) {
                    addMethodToMenu(m, statics, null);
                }
            }
            statics.add(new JSeparator());
            for (int i=0;i<c.getDeclaredFields().length;i++) {
                Field f = c.getDeclaredFields()[i];
                if (Modifier.isStatic(f.getModifiers()) && Modifier.isPublic(f.getModifiers())) {
                    addFieldToMenu(f, statics, null);
                }
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
    }

    public static void addConstructorToMenu(final Constructor con, JMenu menu) {
        Class c = con.getDeclaringClass();
        JMenuItem menuItem = new JMenuItem("new " + VisualJava.getSimpleClassName(c) + "(" + VisualJava.listParams(con.getParameterTypes()) + ")");
        menuItem.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        VisualJava.desktop.displayConstructor(con);
                    }
                }
        );
        menu.add(menuItem);
    }

    public static void addMethodToMenu(final Method m, JMenu menu, final Object obj) {
        JMenuItem menuItem = new JMenuItem(VisualJava.getSimpleClassName(m.getReturnType()) + " " + m.getName() + "(" + VisualJava.listParams(m.getParameterTypes()) + ")");
        menuItem.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        VisualJava.desktop.displayMethod(m,obj);
                    }
                }
        );
        menu.add(menuItem);
    }

    public static void addFieldToMenu(Field f, JMenu menu, final Object obj) throws IllegalAccessException {
        JMenuItem menuItem = new JMenuItem(VisualJava.getSimpleClassName(f.getType()) + " " + f.getName() + " = \"" + f.get(obj) + "\"");
        menu.add(menuItem);
    }

}

