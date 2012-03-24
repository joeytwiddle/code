package visualjava;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JSeparator;

import org.neuralyte.Logger;
import org.neuralyte.common.swing.jmenus.SplittingJMenu;

public class MenuBuilder {

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
	    menuItem.setIcon(VisualJavaGUIStatics.getSmallImageIconForClass(c));
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
	    menuItem.setIcon(VisualJavaGUIStatics.getSmallImageIconForClass(m.getReturnType()));
	    menu.add(menuItem);
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
	                try {
	                    addFieldToMenu(f, statics, null);
	                } catch (Exception e) {
	                	org.neuralyte.Logger.error(e);
	                    // I once got: java.lang.IllegalAccessException: Class visualjava.VisualJavaGUIStatics can not access a member of class org.neuralyte.common.swing.MoveabilityListener with modifiers "public static final"
	                }
	            }
	        }
	    } catch (Exception e) {
	        e.printStackTrace(System.err);
	    }
	}

	/** Do not pass Variable, pass actual Object!  Why?  Variable is used for Code and Dyno.  Did I mean the other way around? **/
	public static void addFieldToMenu(final Field f, JMenu menu, final VariableModel variable) {
		final Object obj = (variable == null ? null : variable.value);
		try {
			Object value = f.get(obj);
			JMenuItem menuItem = new JMenuItem(
					VisualJavaStatics.getSimpleClassName(f.getType())
					+ " " + f.getName() + " = " + VisualJavaStatics.niceString(value) + "?" // Added "?" since value may change!
			);
			menuItem.addActionListener(
					new ActionListener() {
						public void actionPerformed(ActionEvent e) {
							try {
								Object value = f.get(obj);
								String name = VisualJava.desktop.showObject(value);
								Logger.log("[CODE] "+name+" = "+VisualJava.getVariableName(variable)+"."+f.getName());
								VisualJava.passToDyno(variable, f, null);
							} catch (Exception ex) {
								VisualJava.desktop.showObject(ex);
							}
						}
					}
			);
		    menuItem.setIcon(VisualJavaGUIStatics.getSmallImageIconForClass(f.getType()));
			menu.add(menuItem);
		} catch (Exception e) {
			// org.neuralyte.Logger.log("VisualJavaGUIStatics.addFieldToMenu(): cannot work on field \""+f+"\" because: "+e);
		}
	}
	
	/*
	public static JMenu getOrCreateSubMenu(JMenu menu, String text) {
		JMenu child = getChildWithText(menu, text);
		if (child == null) {
			child = new SplittingJMenu(text);
			menu.add(child);
			Logger.log("Created child "+child+" for menu "+menu);
		}
		return child;
	}
	
	public static JMenu getChildWithText(JMenu menu, String text) {
		for (int i=0;i<menu.getMenuComponentCount();i++) {
			Component subMenu = menu.getMenuComponent(i); 
			if (subMenu instanceof JMenu && subMenu.getName().equals(text)) {
				return (JMenu)subMenu;
			}
		}
		return null;
	}
	*/

}
