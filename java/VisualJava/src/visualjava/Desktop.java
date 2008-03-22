package visualjava;

import java.awt.Component;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;

import javax.swing.JDesktopPane;
import javax.swing.JFrame;
import javax.swing.JLayeredPane;

/** joey Nov 1, 2004 2:11:42 AM */
public class Desktop extends JDesktopPane {

	JFrame parent;

	int whereToAddX = 0;
	int whereToAddY = 0;

	public Desktop(JFrame _parent) {
		parent = _parent;
		// putClientProperty("JDesktopPane.dragMode","outline");
//		setLayout(new FlowLayout());
		// ImageIcon icon = new ImageIcon("/usr/share/pixmaps/gnome-gmush.png","Mushroom");
		// JLabel anIcon = new JLabel(icon);
	}

	public String showObject(Object object) {
		String name = VisualJavaStatics.getSimpleClassName(object.getClass());
		name = Character.toLowerCase(name.charAt(0)) + name.substring(1);
		if (name.endsWith("[]")) {
			name = name.substring(0,name.length()-2) + "Array";
		}
		// @todo Should check if an icon with this name already exists, and add a number until it's unique.

		org.neuralyte.Logger.log("Adding and showing " + VisualJavaStatics.getSimpleClassName(object.getClass()) + " \"" + object + "\"");
		VariableView anIcon = new VariableView(this,name,object);
		// anIcon.setLocation(getWidth()/2,getHeight()/2);
		placeAnItem(anIcon);
		anIcon.setSize(anIcon.getPreferredSize());
		add(anIcon,JLayeredPane.DEFAULT_LAYER);
		anIcon.setVisible(true);
//		JMenu menu = new JMenu("Menu");
		// menu.setIcon(anIcon.getIcon());
		// add(menu);
		// JLabel anotherIcon = new JLabel("test");
		// add(anotherIcon);
//		JInternalFrame.JDesktopIcon dIcon = new JInternalFrame.JDesktopIcon(null);
		validate();
		return anIcon.variable.name;
	}
	
	void getLastPos() {
		VariableView lastVar = null;
		int i = getComponentCount()-1;
		while (true) {
			if (i<0)
				break;
			Component component = getComponent(i);
			if (component instanceof VariableView) {
				lastVar = (VariableView)component;
				break;
			}
			i--;
		}
		if (lastVar != null) {
			// Logger.info("Getting pos of last var: "+lastVar.variable.name);
			whereToAddX = lastVar.getLocation().x - 10;
			whereToAddY = lastVar.getLocation().y + 30 - 10;
		}
	}

	public void placeAnItem(Component component) {
		getLastPos();
		int offsetX = (component instanceof MethodFrame ? 480 : 0);
		int destX = whereToAddX + 10 + offsetX;
		int destY = whereToAddY + 10;
		while (destY > getHeight()-50) {
			destY -= 8;
			destX += 15;
		}
		while (destX > getWidth() - 100) {
			destX -= 35;
		}
		component.setLocation(destX, destY);
		/*
		// Old method: keep track of next position:
		whereToAddY += 30;
//		if (isVisible()) { // Doesn't get confused if not yet displayed
		if (whereToAddY > getHeight() - 50) {
			// whereToAddY = 0;
			whereToAddY = getHeight() - 50;
			whereToAddX += 50;
			if (whereToAddX > getWidth() - 50) {
				whereToAddX = whereToAddY = 0;
			}
		}
//		}
		 */
	}

	public void displayMethod(Method m, Object obj) {
		org.neuralyte.Logger.log("displayMethod("+m+","+obj+");");
		MethodFrame methodFrame = new MethodFrame(m,obj);
		initMethodFrame(methodFrame);
	}
	public void displayConstructor(Constructor con) {
		MethodFrame methodFrame = new MethodFrame(con);
		initMethodFrame(methodFrame);
	}
	private void initMethodFrame(MethodFrame methodFrame) {
		// methodFrame.setLocation(getWidth()/2,getHeight()/2);
		placeAnItem(methodFrame);
		methodFrame.setSize(methodFrame.getPreferredSize());
		add(methodFrame);
		methodFrame.setVisible(true);
		
        if (VisualJavaOptions.autoExecNoParameterMethods && methodFrame.parameterTargets.length == 0) {
        	methodFrame.tryToInvoke();
        	if (VisualJavaOptions.autoCloseNoParameterMethods) {
        		remove(methodFrame);
        		methodFrame.setVisible(false);
        	}
        }

		parent.validate();
		// parent.pack();
	}

}
