package visualjava;

import javax.swing.*;
import java.util.Map;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

/** joey Nov 2, 2004 1:18:49 AM */
public class ClassMenu extends JMenu {
    String packageName;
    Map childMenus;
    public ClassMenu(String _packageName) {
        super(( _packageName.length() > 0 ? getLastInPath(_packageName) : "Class" ));
        packageName = _packageName;
        childMenus = new java.util.HashMap();
    }
    public static String getLastInPath(String fullPackage) {
        int i = fullPackage.lastIndexOf(".");
        if (i >= 0) {
            return fullPackage.substring(i+1);
        } else {
            return fullPackage;
        }
    }
    public static String getPackageFromClass(String className) {
        int i = className.lastIndexOf(".");
        return className.substring(0,i);
    }
    // public void addClass(Class c) {
    public void addClass(final String c) {
        // String classPackage = c.getPackage().getName();
        String classPackage = getPackageFromClass(c);
        if (classPackage.equals(packageName)) {
            // JMenuItem item = new JMenuItem(VisualJava.getSimpleClassName(c));
            // JMenuItem item = new JMenuItem(c.substring(classPackage.length()+1));
            // add(item);
            /*
            item.addActionListener(
                    new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                            try {
                                Object instance = Class.forName(c).newInstance();
                                VisualJava.desktop.showObject(instance);
                            } catch (Exception e2) {
                                e2.printStackTrace(System.out);
                            }
                        }
                    }
            );
            */
            /*
            try {
                JMenu item = new JMenu(c.substring(classPackage.length()+1));
                ObjectIcon.addStaticsToMenu(item,Class.forName(c));
                // JMenu statics = new StaticsMenu(getLastInPath(c),Class.forName(c));
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
            */
            JMenu statics = new StaticsMenu(getLastInPath(c),c);
            add(statics);
        } else {
            if (!classPackage.startsWith(packageName)) {
                throw new Error("Expect classPackage (" + classPackage + ") to start with packageName (" + packageName + ")");
            }
            String subPackage = classPackage.substring(packageName.length() + ( packageName.length() > 0 ? 1 : 0 ));
            int i = subPackage.indexOf(".");
            if (i>=0) {
                subPackage = subPackage.substring(0,i);
            }
            ClassMenu subMenu = getSubMenu(subPackage);
            subMenu.addClass(c);
        }
    }
    ClassMenu getSubMenu(String subPackage) {
        Object got = childMenus.get(subPackage);
        ClassMenu subMenu;
        if (got == null) {
            subMenu = new ClassMenu(packageName + ( packageName.length() > 0 ? "." : "" ) + subPackage);
            childMenus.put(subPackage,subMenu);
            add(subMenu);
        } else {
            subMenu = (ClassMenu)got;
        }
        return subMenu;
    }
}
