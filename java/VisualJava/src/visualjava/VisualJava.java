package visualjava;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

/** joey Jul 11, 2004 3:26:31 AM */
public class VisualJava {

	public static void main(String[] args) {
		// visualjava.VisualJavaGui.show(new String("hello test"));
        JFrame f = new JFrame();
//        addMenuBar(f);
        f.setContentPane(new visualjava.Desktop());
        f.setVisible(true);
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

	// public static void addMenuBar(JFrame frame) {
    public static void addMenuBar(JMenu myMenu) {

		JMenuBar menuBar;
		JMenu menu, submenu;
		JMenuItem menuItem;
		JRadioButtonMenuItem rbMenuItem;
		JCheckBoxMenuItem cbMenuItem;

		//Create the menu bar.
		menuBar = new JMenuBar();

		//Build the first menu.
		menu = new JMenu("A Menu");
        menu = myMenu;
		menu.setMnemonic(KeyEvent.VK_A);
		menu.getAccessibleContext().setAccessibleDescription(
				"The only menu in this program that has menu items");
		menuBar.add(menu);

		//a group of JMenuItems
		menuItem = new JMenuItem("A text-only menu item",
				KeyEvent.VK_T);
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

	}


}
