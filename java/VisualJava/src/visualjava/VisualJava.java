package visualjava;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.lang.reflect.Method;
import java.io.*;

/** joey Jul 11, 2004 3:26:31 AM */
public class VisualJava {

    public static Desktop desktop;

	public static void main(String[] args) {
		// visualjava.VisualJavaGui.oldShowDeprecated(new String("hello test"));
        JFrame f = new JFrame();
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        f.setSize(600,400);
        desktop = new Desktop(f);
        f.setContentPane(desktop);
        f.setVisible(true);
        addMenuBar(f);
        f.validate();
	}

	public static Class viewerForObject(Object toShow) {
		return viewerForType(toShow.getClass());
	}

	public static Class viewerForType(Class type) {
		Class testType = type;
		Class viewerType = null;
		while (viewerType == null && testType != null) {
			String test = "visualjava.viewers." + testType.getName();
			try {
				viewerType = Class.forName(test);
			} catch (Exception e) {
				// e.printStackTrace(System.err);
			}
			if (viewerType == null) {
				Class lastTestType = testType;
				testType = testType.getSuperclass();
				if (lastTestType == testType)
					break;
			}
		}
		return viewerType;
	}

	public static Viewer invokeViewer(Class viewerType,Object argument) {
		Class[] argTypes = { Object.class };
		java.lang.reflect.Constructor constructor = null;
		try {
			constructor = viewerType.getConstructor(argTypes);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		Object[] args = { argument };
		Viewer viewer = null;
		try {
			viewer = (Viewer)constructor.newInstance(args);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return viewer;
	}

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

    public static String listParams(Class[] parameterTypes) {
        String s = "";
        for (int i=0;i<parameterTypes.length;i++) {
            s += getSimpleClassName(parameterTypes[i]);
            if (i < parameterTypes.length - 1) {
                s += ", ";
            }
        }
        return s;
    }

    static String getSimpleClassName(Class c) {
        if (c.isArray()) {
            return getSimpleClassName(c.getComponentType()) + "[]";
        } else {
            String full = c.getName();
            int i = full.lastIndexOf('.');
            if (i>=0) {
                full = full.substring(i+1);
            }
            return full;
        }
    }

    static BufferedReader getListOfAllClasses() {
        try {
            // final Process process = Runtime.getRuntime().exec("jar tf /usr/lib/j2se/1.4/jre/lib/rt.jar");
            final Process process = Runtime.getRuntime().exec("/home/joey/j/jsh memo jar tf /usr/lib/j2se/1.4/jre/lib/rt.jar");
            final BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            final PipedOutputStream out = new PipedOutputStream();
            final PipedInputStream in = new PipedInputStream(out);
            final BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out));
            new Thread() {
                public void run() {
                    try {
                        while (true) {
                            String line = reader.readLine();
                            if (line == null)
                                break;
                            if (line.endsWith(".class")) {
                                line = line.substring(0,line.length() - ".class".length());
                            }
                            int i;
                            while ((i = line.indexOf("/")) >= 0) {
                                line = line.substring(0,i) + "." + line.substring(i+1);
                            }
                            while ((i = line.indexOf("\\")) >= 0) {
                                line = line.substring(0,i) + "." + line.substring(i+1);
                            }
                            writer.write(line + "\n");
                        }
                    } catch (IOException e) {
                        e.printStackTrace(System.err);
                    }
                }
            }.start();
            return new BufferedReader(new InputStreamReader(in));
        } catch (IOException e) {
            e.printStackTrace(System.err);
        }
        return null;
    }

    static JMenu buildClassMenu() {
        ClassMenu menu = new ClassMenu("");
        BufferedReader in = getListOfAllClasses();
        try {
            while (true) {
                String line = in.readLine();
                if (line == null)
                    break;
                menu.addClass(line);
            }
        } catch (IOException e) {
            if (!e.getMessage().equals("Write end dead")) {
                e.printStackTrace(System.err);
            }
        }
        return menu;
    }

}
