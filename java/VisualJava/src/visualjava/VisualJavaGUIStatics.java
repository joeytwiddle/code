package visualjava;

import javax.swing.*;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.io.BufferedReader;
import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.Modifier;
import java.lang.reflect.Method;
import java.lang.reflect.Field;

/** joey Nov 2, 2004 3:15:13 PM */
public class VisualJavaGUIStatics {

    public static void addMenuBar(JFrame frame) {
        // public static void addMenuBar(JMenu myMenu) {

		JMenuBar menuBar;
		JMenu menu, submenu;
		JMenuItem menuItem;
		JRadioButtonMenuItem rbMenuItem;
		JCheckBoxMenuItem cbMenuItem;

		//Create the menu bar.
		menuBar = new JMenuBar();

        menu = new JMenu("File");
        menuItem = new JMenuItem("Exit");
        menuItem.addActionListener(
            new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    System.exit(0);
                }
            }
        );
        menu.add(menuItem);
        menuBar.add(menu);

        menu = buildClassMenu();
        menuBar.add(menu);

        frame.setJMenuBar(menuBar);

        /*

		//Build the first menu.
		menu = new JMenu("Class");
        menu = myMenu;
		menu.setMnemonic(KeyEvent.VK_C);
		menu.getAccessibleContext().setAccessibleDescription("Create a new Object or find a Class");
		menuBar.add(menu);

		//a group of JMenuItems
		menuItem = new JMenuItem("A text-only menu item",KeyEvent.VK_T);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
					KeyEvent.VK_1, ActionEvent.ALT_MASK));
		menuItem.getAccessibleContext().setAccessibleDescription(
				"This doesn't really do anything");
		menu.add(menuItem);

		menuItem = new JMenuItem("Both text and icon",
				new ImageIcon("images/middle.gif"));
		menuItem.setMnemonic(KeyEvent.VK_B);
		menu.add(menuItem);

		menuItem = new JMenuItem(new ImageIcon("images/middle.gif"));
		menuItem.setMnemonic(KeyEvent.VK_D);
		menu.add(menuItem);

		//a group of radio button menu items
		menu.addSeparator();
		ButtonGroup group = new ButtonGroup();
		rbMenuItem = new JRadioButtonMenuItem("A radio button menu item");
		rbMenuItem.setSelected(true);
		rbMenuItem.setMnemonic(KeyEvent.VK_R);
		group.add(rbMenuItem);
		menu.add(rbMenuItem);

		rbMenuItem = new JRadioButtonMenuItem("Another one");
		rbMenuItem.setMnemonic(KeyEvent.VK_O);
		group.add(rbMenuItem);
		menu.add(rbMenuItem);

		//a group of check box menu items
		menu.addSeparator();
		cbMenuItem = new JCheckBoxMenuItem("A check box menu item");
		cbMenuItem.setMnemonic(KeyEvent.VK_C);
		menu.add(cbMenuItem);

		cbMenuItem = new JCheckBoxMenuItem("Another one");
		cbMenuItem.setMnemonic(KeyEvent.VK_H);
		menu.add(cbMenuItem);

		//a submenu
		menu.addSeparator();
		submenu = new JMenu("A submenu");
		submenu.setMnemonic(KeyEvent.VK_S);

		menuItem = new JMenuItem("An item in the submenu");
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
					KeyEvent.VK_2, ActionEvent.ALT_MASK));
		submenu.add(menuItem);

		menuItem = new JMenuItem("Another item");
		submenu.add(menuItem);
		menu.add(submenu);

		//Build second menu in the menu bar.
		menu = new JMenu("Another Menu");
		menu.setMnemonic(KeyEvent.VK_N);
		menu.getAccessibleContext().setAccessibleDescription(
				"This menu does nothing");
		menuBar.add(menu);

//		frame.setJMenuBar(menuBar);

        */

	}

    static JMenu buildClassMenu() {
        final ClassMenu menu = new ClassMenu("");
        BufferedReader in = VisualJavaStatics.getListOfAllClasses();
        try {
            while (true) {
                String line = in.readLine();
                if (line == null)
                    break;
                if (line.indexOf("WebResponse")>=0)
                    System.out.println("< "+line);
                menu.addClass(line);
            }
        } catch (IOException e) {
            // if (!e.getMessage().equals("Write end dead")) {
                e.printStackTrace(System.err);
            // }
        }
        // A nice touch: starts to split/populate the menu in the background (because it is a slow operation!)
        // Strangely if I wrap the above population reading in this Thread, the menu still does not appear.
        new Thread() {
            public synchronized void start() {
                menu.ensurePopulated();
            }
        }.start();
        return menu;
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
                if (Modifier.isStatic(m.getModifiers()) && Modifier.isPublic(m.getModifiers())) {
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
        JMenuItem menuItem = new JMenuItem("new " + VisualJavaStatics.getSimpleClassName(c) + "(" + VisualJavaStatics.listParams(con.getParameterTypes()) + ")");
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
        // JMenuItem menuItem = new JMenuItem(VisualJavaStatics.getSimpleClassName(m.getReturnType()) + " " + m.getName() + "(" + VisualJavaStatics.listParams(m.getParameterTypes()) + ")");
        JMenuItem menuItem = new JMenuItem(m.getName() + "(" + VisualJavaStatics.listParams(m.getParameterTypes()) + ") : " + VisualJavaStatics.getSimpleClassName(m.getReturnType()));
        menuItem.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        VisualJava.desktop.displayMethod(m,obj);
                    }
                }
        );
        menu.add(menuItem);
    }
    
    public static void addFieldToMenu(final Field f, JMenu menu, final Object obj) throws IllegalAccessException {
        JMenuItem menuItem = new JMenuItem(VisualJavaStatics.getSimpleClassName(f.getType()) + " " + f.getName() + " = \"" + f.get(obj) + "\"");
        menuItem.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        try {
                            Object value = f.get(obj);
                            VisualJava.desktop.showObject(value);
                        } catch (Exception ex) {
                            VisualJava.desktop.showObject(ex);
                        }
                    }
                }
        );
        menu.add(menuItem);
    }
    
}
