package visualjava;

import java.awt.Component;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JMenu;
import javax.swing.JMenuItem;

import org.neuralyte.Logger;
import org.neuralyte.common.swing.jmenus.DetachableJMenu;
import org.neuralyte.common.swing.jmenus.LazyJMenu;
import org.neuralyte.common.swing.jmenus.SplittingJMenu;



/** joey Nov 2, 2004 1:18:49 AM
 * @todo Should rename this to PackageMenu 
 * Mmmm well it's a mix of packages, expanding into Classes, and then into Methods. */
// public class ClassMenu extends SplittingJMenu { // DO NOT USE SplittingJMenu: it shows nothing!  Use LazyJMenu instead.
public class ClassMenu extends LazyJMenu {

    // final static ImageIcon packageIcon = new ImageIcon("/usr/share/pixmaps/gnome-default-dlg.png");
    final static ImageIcon packageIcon = new ImageIcon("src/visualjava/package-15x15.png");

    String packageName;

    List<String> tempClassList = new Vector<String>();
    
    Map childMenus; // Only sub-packages, no classes - all type ClassMenu.
    
    public int countTotalContainedClasses = 0;

    public ClassMenu(String _packageName) {
        super(( _packageName.length() > 0 ? ClassMenuBuilder.getLastInPath(_packageName) : "Class" ));
        setIcon(packageIcon);
        packageName = _packageName;
        childMenus = new java.util.HashMap();
    }

    // public void addClass(Class c) {
    public void addClass(final String c) {
    	countTotalContainedClasses++;
    	tempClassList.add(c);
    }
    
    public void reallyAddClass(String c) {
        // String classPackage = c.getPackage().getName();
        String classPackage = ClassMenuBuilder.getPackageFromClass(c);
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
                Variable.addStaticsToMenu(item,Class.forName(c));
                // JMenu statics = new StaticsMenu(getLastInPath(c),Class.forName(c));
            } catch (Exception e) {
                org.neuralyte.Logger.error(e);
            }
            */
            JMenu statics = new StaticsMenu(ClassMenuBuilder.getLastInPath(c),c);
            // We used to avoid doing Class.forName(c) until late, to make building
            // the menu faster.  But now this method is only called when the package
            // is about to be viewed.
            statics.setIcon(new ImageIcon("src/visualjava/class-15x15.png"));
            try {
            	Class cl = Class.forName(c);
            	ImageIcon imageIcon = VisualJavaGUIStatics.getSmallImageIconForClass(cl);
                statics.setIcon(imageIcon);
            } catch (ClassNotFoundException e) {
                org.neuralyte.Logger.error(e);
            } catch (NoClassDefFoundError e) {
                org.neuralyte.Logger.error(e);
            } catch (UnsatisfiedLinkError e) {
                org.neuralyte.Logger.error(e);
            } catch (Throwable e) { // Maybe there are more errors we don't know about!
                org.neuralyte.Logger.error(e);
            }
            add((Component)statics);
        } else {
        	
        	/*
            if (!classPackage.startsWith(packageName)) {
                throw new Error("Expect classPackage (" + classPackage + ") to start with packageName (" + packageName + ")");
            }
            // Assertion.assertThat(packageName.length() > 0);
            String subPackage = classPackage.substring(packageName.length() + ( packageName.length() > 0 ? 1 : 0 ));
            int i = subPackage.indexOf(".");
            if (i>=0) {
                subPackage = subPackage.substring(0,i);
            }
            if (subPackage.length() == 0) {
            	org.neuralyte.Logger.error("Cannot create SubMenu with no title - might never end!");
            	org.neuralyte.Logger.error("Class was "+c+" and package is "+packageName);
                return;
            }
            ClassMenu subMenu = getSubMenu(subPackage);
            subMenu.addClass(c);
            */
            
            String[] subPackages = classPackage.split("\\.");
            ClassMenu target = this;
            for (int j=0;j<subPackages.length;j++) {
            	String part = subPackages[j];
            	target = target.getSubMenu(part);
            }
            target.addClass(c);
        }
    }

	ClassMenu getSubMenu(String subPackage) {
        Object got = childMenus.get(subPackage);
        ClassMenu subMenu;
        if (got == null) {
            subMenu = new ClassMenu(packageName + ( packageName.length() > 0 ? "." : "" ) + subPackage);
            // subMenu.setIcon(new ImageIcon("src/visualjava/package-15x15.png"));
            childMenus.put(subPackage,subMenu);
            add((Component)subMenu);
        } else {
            subMenu = (ClassMenu)got;
        }
        return subMenu;
    }
    
    public void removeAll() {
        // TODO Auto-generated method stub
        super.removeAll();
        childMenus.clear();
    }

	public void generateChildren() {
		// Deal with the laziness:
		if (tempClassList.size()>100)
			Logger.log("In ClassMenu \""+getText()+"\", tempClassList.size() = " + tempClassList.size());
		for (int i=0;i<tempClassList.size();i++) {
			reallyAddClass(tempClassList.get(i));
		}
		tempClassList.clear(); // tempClassList = null;
		
		// Split the menu:
		splitJMenuIfNeeded(this);
		
		// Add a detacher to the menu:
		this.insert(DetachableJMenu.getDetacherFor(this), 0);
		// this.insert(new JSeparator(), 1);
	}

	public static void splitJMenuIfNeeded(JMenu menu) {
		if (menu.getMenuComponentCount()>15) {
			
			int preferredSplits = 15;
			List items = Arrays.asList(menu.getMenuComponents());

			//// I don't know how, but the objects have lost their sorting!
			// It may be from using .add(JMenuItem) instead of .add(Component)
			// Collections.sort(items); // Fails due to mixture of ClassMenus and StaticsMenus.
			Vector itemsVector = new Vector();
			itemsVector.addAll(items);
			bubbleSortJMenuItems(itemsVector);
			items = itemsVector;

			if (items.size()>20) {
				org.neuralyte.Logger.warn("Splitting "+items.size()+" items "+preferredSplits+" ways in "+menu.getClass()+" \""+menu.getText()+"\"!");
				// org.neuralyte.Logger.logStackTrace();
			}
			int[] splitAt=new int[preferredSplits+1];
			// splitAt[0]=0;
			// splitAt[preferedSplits-1]=items.size()+1;
			for (int i=0;i<preferredSplits+1;i++) {
				splitAt[i]=items.size()*i/(preferredSplits);
			}
			Vector splits=new Vector();
			for (int i=0;i<preferredSplits;i++) {
				if (splitAt[i+1] - splitAt[i] == 1) {
					splits.add(items.get(splitAt[i]));
					// menu.remove((JMenuItem)items.get(splitAt[i]));
				} else {
					Vector tmp=new Vector();
					for (int j=splitAt[i];j<splitAt[i+1];j++) {
						tmp.add(items.get(j));
						// menu.remove((JMenuItem)items.get(j));
					}
					String first=((JMenuItem)tmp.get(0)).getText();
					String last=((JMenuItem)tmp.get(tmp.size()-1)).getText();
                    // SplittingJMenu sm=new SplittingJMenu("[ "+first+" ... "+last+" ]");
                    SplittingJMenu sm=new SplittingJMenu(SplittingJMenu.getTitle(first,last));
					for (int j=0;j<tmp.size();j++) {
						// sm.add((JMenuItem)tmp.get(j)); // Performs its own sorting!
						sm.add((Component)tmp.get(j)); // Keeps the sorting we already have.
					}
					splits.add((Component)sm);
					// tmp.clear(); tmp=null;
				}
			}
			// items.clear(); items=null;
			menu.removeAll();
			for (int i=0;i<splits.size();i++) {
				menu.add((Component)splits.get(i));
			}
			// splits.clear(); splits=null;
			
		}
	}
	
    public static void bubbleSortJMenuItems(Vector in) {
    	for (int i=in.size();i>=0;i--) {
    		for (int j=0;j<i-1;j++) {
    		// for (int j=i-2;j>=0;j--) {
    			JMenuItem above = (JMenuItem)in.get(j);
    			JMenuItem below = (JMenuItem)in.get(j+1);
    			if (
    					(above instanceof StaticsMenu && below instanceof ClassMenu) // put packages before classes 
    					|| (above.getClass().equals(below.getClass()) && above.getText().compareTo(below.getText())>0) // sort by name
    			) {
    				in.set(j, below);
    				in.set(j+1, above);
    			}
    		}
    	}
    }
    
}
