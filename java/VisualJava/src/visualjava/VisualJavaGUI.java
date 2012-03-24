/*
 * Created on Nov 1, 2006
 * 
 * TODO To change the template for this generated file go to Window -
 * Preferences - Java - Code Style - Code Templates
 */
package visualjava;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Vector;

import javax.swing.BoxLayout;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.JTextField;

import org.common.nap.Nap;
import org.neuralyte.Logger;
import org.neuralyte.common.ArrayUtils;
import org.neuralyte.common.swing.jmenus.SplittingJMenu;

import visualjava.guicomponents.ClassMenu;
import visualjava.guicomponents.Desktop;
import visualjava.guicomponents.VariableView;

import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.io.xml.DomDriver;

/**
 * @author joey
 */

public class VisualJavaGUI {

	Desktop	 desktop;
	StatusBar	statusBar;

	JFrame	 frame;

	public VisualJavaGUI() {
		// visualjava.VisualJavaGui.oldShowDeprecated(new String("hello test"));
		final JFrame frame = new JFrame("VisualJava");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setSize(800, 600);

		desktop = new Desktop(frame);
		desktop.setSize(800, 600);
		// f.setContentPane(desktop);
		frame.getContentPane().setLayout(
		      new BoxLayout(frame.getContentPane(), BoxLayout.Y_AXIS));
		frame.getContentPane().add(desktop);

		statusBar = new StatusBar();
		frame.getContentPane().add(statusBar);

		frame.setVisible(true);

		new Thread() {

			public void run() {
				addMenuBar(frame);
			}
		}.start();

		frame.validate();
		this.frame = frame;
	}

	JMenuBar	 menuBar;
	JMenu	    fileMenu, classpathMenu, options;	 // , classMenu;
	ClassMenu	classMenu;

	Vector	 classpathSelections	= new Vector();

	public void addMenuBar(final JFrame frame) {
		// public static void addMenuBar(JMenu myMenu) {

		// JMenu menu, submenu;
		JMenuItem menuItem;
		JRadioButtonMenuItem rbMenuItem;
		JCheckBoxMenuItem cbMenuItem;

		// Create the menu bar.
		menuBar = new JMenuBar();

		frame.setJMenuBar(menuBar);

		fileMenu = new JMenu("File");
		menuItem = new JMenuItem("Open");
		menuItem.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				JFileChooser chooser = new JFileChooser(new File("VisualJava.xml"));
				int result = chooser.showOpenDialog(frame);
				if (result == JFileChooser.APPROVE_OPTION) {
					File file = chooser.getSelectedFile();
					try {
						Object obj = new XStream(new DomDriver())
						      .fromXML(new FileReader(file));
						VisualJava.showObject(obj);
					} catch (Exception e2) {
						Logger.logStackTrace(e2);
					}
				}
			}
		});
		fileMenu.add(menuItem);
		menuItem = new JMenuItem("Save");
		menuItem.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				JFileChooser chooser = new JFileChooser(new File("VisualJava.xml"));
				int result = chooser.showOpenDialog(frame);
				if (result == JFileChooser.APPROVE_OPTION) {
					File file = chooser.getSelectedFile();
					try {
						Vector variables = new Vector();
						Component[] components = desktop.getComponents();
						for (int i = 0; i < components.length; i++) {
							if (components[i] instanceof VariableView) {
								VariableModel variable = ((VariableView) components[i]).variable;
								try {
									String dummyResult = new XStream(new DomDriver())
									      .toXML(variable);
									variables.add(variable);
								} catch (Exception e2) {
									Logger.warn("Could not convert to XML " + variable
									      + " because " + e2);
								}
							}
						}
						// Object obj = this;
						Object obj = variables;
						Logger.info("Saving " + variables + " now.");
						new XStream(new DomDriver()).toXML(obj, new FileWriter(file));
						Logger.info("Save done.");
					} catch (Throwable e2) {
						Logger.logStackTrace(e2);
					}
				}
			}
		});
		fileMenu.add(menuItem);
		menuItem = new JMenuItem("Exit");
		menuItem.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				System.exit(0);
			}
		});
		fileMenu.add(menuItem);
		menuBar.add(fileMenu);

		options = new JMenu("Options");
		menuItem = new JCheckBoxMenuItem("Auto-exec no-parameter methods");
		ActionListener actionListener = new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				Logger.info("Action on " + e.getActionCommand() + ": " + e);
				if (e.getSource() instanceof JCheckBoxMenuItem) {
					VisualJavaOptions.autoExecNoParameterMethods = ((JCheckBoxMenuItem) e
					      .getSource()).isSelected();
					if (!VisualJavaOptions.autoExecNoParameterMethods) {
						// TODO: setSelected(false) and setEnabled(false) on the other
						// option
					} else {
						// TODO: setEnabled(true) on the other option
					}
				}
			}
		};
		menuItem.addActionListener(actionListener);
		menuItem.setSelected(VisualJavaOptions.autoExecNoParameterMethods);
		options.add(menuItem);
		menuItem = new JCheckBoxMenuItem("Auto-close no-parameter methods");
		actionListener = new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				Logger.info("Action on " + e.getActionCommand() + ": " + e);
				if (e.getSource() instanceof JCheckBoxMenuItem) {
					VisualJavaOptions.autoCloseNoParameterMethods = ((JCheckBoxMenuItem) e
					      .getSource()).isSelected();
				}
			}
		};
		menuItem.addActionListener(actionListener);
		menuItem.setSelected(VisualJavaOptions.autoCloseNoParameterMethods);
		options.add(menuItem);
		menuBar.add(options);

		// One if not both needed:
		frame.validate();
		frame.repaint();

		classpathMenu = buildClasspathMenu();
		menuBar.add(classpathMenu);

		// One if not both needed:
		frame.validate();
		frame.repaint();

		classMenu = new ClassMenu("");
		// buildClassMenu((ClassMenu)classMenu);
		rebuildClassMenu();
		menuBar.add(classMenu);

		// One if not both needed:
		frame.validate();
		frame.repaint();

		/*
		 * 
		 * //Build the first menu. menu = new JMenu("Class"); menu = myMenu;
		 * menu.setMnemonic(KeyEvent.VK_C);
		 * menu.getAccessibleContext().setAccessibleDescription
		 * ("Create a new Object or find a Class"); menuBar.add(menu);
		 * 
		 * //a group of JMenuItems menuItem = new
		 * JMenuItem("A text-only menu item",KeyEvent.VK_T);
		 * menuItem.setAccelerator(KeyStroke.getKeyStroke( KeyEvent.VK_1,
		 * ActionEvent.ALT_MASK));
		 * menuItem.getAccessibleContext().setAccessibleDescription(
		 * "This doesn't really do anything"); menu.add(menuItem);
		 * 
		 * menuItem = new JMenuItem("Both text and icon", new
		 * ImageIcon("images/middle.gif")); menuItem.setMnemonic(KeyEvent.VK_B);
		 * menu.add(menuItem);
		 * 
		 * menuItem = new JMenuItem(new ImageIcon("images/middle.gif"));
		 * menuItem.setMnemonic(KeyEvent.VK_D); menu.add(menuItem);
		 * 
		 * //a group of radio button menu items menu.addSeparator(); ButtonGroup
		 * group = new ButtonGroup(); rbMenuItem = new
		 * JRadioButtonMenuItem("A radio button menu item");
		 * rbMenuItem.setSelected(true); rbMenuItem.setMnemonic(KeyEvent.VK_R);
		 * group.add(rbMenuItem); menu.add(rbMenuItem);
		 * 
		 * rbMenuItem = new JRadioButtonMenuItem("Another one");
		 * rbMenuItem.setMnemonic(KeyEvent.VK_O); group.add(rbMenuItem);
		 * menu.add(rbMenuItem);
		 * 
		 * //a group of check box menu items menu.addSeparator(); cbMenuItem = new
		 * JCheckBoxMenuItem("A check box menu item");
		 * cbMenuItem.setMnemonic(KeyEvent.VK_C); menu.add(cbMenuItem);
		 * 
		 * cbMenuItem = new JCheckBoxMenuItem("Another one");
		 * cbMenuItem.setMnemonic(KeyEvent.VK_H); menu.add(cbMenuItem);
		 * 
		 * //a submenu menu.addSeparator(); submenu = new JMenu("A submenu");
		 * submenu.setMnemonic(KeyEvent.VK_S);
		 * 
		 * menuItem = new JMenuItem("An item in the submenu");
		 * menuItem.setAccelerator(KeyStroke.getKeyStroke( KeyEvent.VK_2,
		 * ActionEvent.ALT_MASK)); submenu.add(menuItem);
		 * 
		 * menuItem = new JMenuItem("Another item"); submenu.add(menuItem);
		 * menu.add(submenu);
		 * 
		 * //Build second menu in the menu bar. menu = new JMenu("Another Menu");
		 * menu.setMnemonic(KeyEvent.VK_N);
		 * menu.getAccessibleContext().setAccessibleDescription(
		 * "This menu does nothing"); menuBar.add(menu);
		 * 
		 * // frame.setJMenuBar(menuBar);
		 */

	}

	public JMenu buildClasspathMenu() {
		final JMenu menu = new SplittingJMenu("Classpath");

		final String classPathSeparator = (File.separatorChar == '\\' ? ";" : ":");
		String classPath = System.getProperty("java.class.path");

		// Add libs:
		File jrelibdir = new File(System.getProperty("java.home"), "lib");
		// No good adding all libs; since they aren't neccessarily on the
		// classpath! (We get ClassNotFound exceptions when we try to access
		// them.)
		/*
		 * File[] children = jrelibdir.listFiles(); for (int
		 * i=0;i<children.length;i++) { if (children[i].isFile() &&
		 * children[i].getName().toLowerCase().endsWith(".jar")) { classPath +=
		 * classPathSeparator + children[i].getAbsolutePath(); } }
		 */
		// But we do want rt.jar: (although it may already be there, but it isn't
		// for me under Windows)
		classPath += classPathSeparator
		      + new File(jrelibdir, "rt.jar").getAbsolutePath();

		org.neuralyte.Logger.log("Examining classpath: " + classPath);
		String[] classPaths = classPath.split(classPathSeparator);
		Logger.log("BEFORE: "+Nap.toString(classPaths));
		classPaths = (String[]) ArrayUtils.removeDuplicates(classPaths, new String[0]);
		Logger.log("AFTER: "+Nap.toString(classPaths));
		for (int i = 0; i < classPaths.length; i++) {
			String aClassPath = classPaths[i];
			if ("".equals(aClassPath)) {
				continue;
			}
			JCheckBoxMenuItem menuItem = buildClasspathSelection(aClassPath);
			menu.add(menuItem);
		}

		JMenuItem addButton = new JMenuItem("Add to classpath...");
		addButton.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent arg0) {
				JFileChooser chooser = new JFileChooser();
				chooser.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);
				chooser.setMultiSelectionEnabled(true);
				int result = chooser.showOpenDialog(frame);
				if (result == JFileChooser.APPROVE_OPTION) {
					File[] files = chooser.getSelectedFiles();
					for (int i = 0; i < files.length; i++) {
						File file = files[i];

						// TODO BUG: the property changes but the classes are not
						// accessible to java :S
						String classpath = System.getProperty("java.class.path");
						classpath += classPathSeparator + file.getAbsolutePath();
						System.setProperty("java.class.path", classpath);

						System.out.println("SystemClassloader = "
						      + ClassLoader.getSystemClassLoader());
						VisualJava.showObject(ClassLoader.getSystemClassLoader());

						JCheckBoxMenuItem menuItem = buildClasspathSelection(file
						      .getAbsolutePath());
						menu.add(menuItem);
						// menu.validate();
						// menu.repaint();
					}
					rebuildClassMenu();
				}
			}
		});
		menu.add(addButton);

		return menu;
	}

	private JCheckBoxMenuItem buildClasspathSelection(String aClassPath) {
		JCheckBoxMenuItem menuItem = new JCheckBoxMenuItem("" + aClassPath);
		classpathSelections.add(menuItem);
		// menuItem.setEnabled(false);
		ActionListener verboseActionListener = new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				Logger.info("Action on " + e.getActionCommand() + ": " + e);
				if (e.getSource() instanceof JCheckBoxMenuItem) {
					// if (((JCheckBoxMenuItem)e.getSource()).isSelected()) {
					rebuildClassMenu();
					// }
				}
			}
		};
		menuItem.addActionListener(verboseActionListener);
		// menuItem.setSelected(false);
		menuItem.setSelected(true);
		return menuItem;
	}

	public/* JMenu */void rebuildClassMenu() {

		new Thread() {

			public void run() {

				if (statusBar != null) {
					statusBar.setStatus("Building class menu....");
				}

				if (classMenu == null) {
				} else {
					classMenu.removeAll();
					menuBar.remove(classMenu);
					/*
					 * menuBar.validate(); desktop.validate(); frame.validate();
					 * frame.pack();
					 */
					frame.repaint();
					/**
					 * TODO This does not seem to be working! I see two "Class" menu
					 * dropdowns.
					 **/
				}
				classMenu = new ClassMenu("");

				/*
				 * BufferedReader in = getListOfAllClasses(); try {
				 * Thread.currentThread().setPriority(1); while (true) {
				 * Thread.yield(); String line = in.readLine(); if (line == null)
				 * break; if (line.length() == 0) {org.neuralyte.Logger.error(
				 * "VisualJavaGUIStatics.buildClassMenu(): Got class = \"\"!"); }
				 * classMenu.addClass(line); } } catch (IOException e) { // if
				 * (!e.getMessage().equals("Write end dead")) {
				 * org.neuralyte.Logger.error(e); // } }
				 */

				try {
					printClassesTo(new PrintWriter(new File("/tmp/tmp.xyz")) {

						public void println(String line) {
							// org.neuralyte.Logger.log("Adding: "+line);
							classMenu.addClass(line);
							super.println(line);
						}
					});
				} catch (Exception e) {
					org.neuralyte.Logger.error(e);
				}

				// JMenuItem filter = new JMenuItem("Filter");
				// filter.add(new JTextField("*"));
				// classMenu.insert(filter,1);

				// classMenu.generateChildren(); // Doesn't tell itself that caching
				// has been done!
				classMenu.ensurePopulated();

				menuBar.add(classMenu);

				org.neuralyte.Logger.log("Class menu built.");

				// // A nice touch: starts to split/populate the menu in the
				// background (because it is a slow operation!)
				// // Strangely if I wrap the above population reading in this
				// Thread, the menu still does not appear.
				// classMenu.ensurePopulated();

				/*
				 * new LoudThread("ensurePopulated") { public synchronized void
				 * run() { menu.ensurePopulated(); } }.start();
				 */
				if (statusBar != null) {
					statusBar.setStatus("Class menu built.");
					statusBar.setStatus(classMenu.countTotalContainedClasses
					      + " classes available.");
				}

			}
		}.start();

		// return classMenu;
	}

	public void printClassesTo(final PrintWriter writer) {
		/*
		 * PipedOutputStream out = new PipedOutputStream(); PipedInputStream in =
		 * null; try { in = new PipedInputStream(out); } catch (IOException e) {
		 * org.neuralyte.Logger.error(e); // If we can't create the
		 * PipedOutputStream (never happens) } final PrintWriter writer = new
		 * PrintWriter(new OutputStreamWriter(out));
		 */

		// new Thread() {
		// public void run() {
		//    
		// Thread.currentThread().setPriority(1);
		String classPath = System.getProperty("java.class.path");

		/*
		 * if (classPath.indexOf(File.separatorChar + "rt.jar") == -1) { // a bit
		 * dodgy classPath += ":" + System.getProperty("java.home") +
		 * "/lib/rt.jar"; } org.neuralyte.Logger.log("Will parse classpath: " +
		 * classPath); int j; while (classPath.length() > 0) { j =
		 * classPath.indexOf(":"); String jarOrDir; if (j >= 0) { jarOrDir =
		 * classPath.substring(0,j); classPath = classPath.substring(j+1); } else
		 * { jarOrDir = classPath; classPath = ""; } if (statusBar != null) {
		 * statusBar.setStatus("Parsing class names from " + jarOrDir + " (" +
		 * classPath.split(":").length + " remaining on classpath) ..."); } String
		 * command; printClassListFromResource(writer, jarOrDir); }
		 */

		/*
		 * String[] resources = classPath.split(":"); for (int
		 * i=0;i<resources.length;i++) { String resource = resources[i]; }
		 */

		/*
		 * for (int i=0;i<classpathMenu.getComponentCount();i++) {
		 * JCheckBoxMenuItem checkBoxMenuItem =
		 * (JCheckBoxMenuItem)classpathMenu.getComponent(i); if
		 * (checkBoxMenuItem.isSelected()) { printClassListFromResource(writer,
		 * checkBoxMenuItem.getLabel()); } }
		 */

		for (int i = 0; i < classpathSelections.size(); i++) {
			JCheckBoxMenuItem checkBoxMenuItem = (JCheckBoxMenuItem) classpathSelections
			      .get(i);
			if (checkBoxMenuItem.isSelected()) {
				ClassMenuBuilder.printClassListFromResource(writer,
				      checkBoxMenuItem.getLabel());
			}
		}

		writer.flush();
		writer.close();
		org.neuralyte.Logger.log("Class list sent.");

		// }
		//
		// }.start();

		// return new BufferedReader(new InputStreamReader(in));
	}

	class StatusBar extends JLabel {

		public StatusBar() {
			super();
		}

		public void setStatus(String newStatus) {
			setText(newStatus);
			// repaint();
		}
	}

}
