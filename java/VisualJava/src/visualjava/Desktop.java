package visualjava;

import javax.swing.*;
import java.awt.*;
import java.lang.reflect.Method;
import java.lang.reflect.Constructor;

/** joey Nov 1, 2004 2:11:42 AM */
public class Desktop extends JDesktopPane {

    JFrame parent;

    int whereToAddX = 0;
    int whereToAddY = 0;

    public Desktop(JFrame _parent) {
        parent = _parent;
        // putClientProperty("JDesktopPane.dragMode","outline");
//        setLayout(new FlowLayout());
        // ImageIcon icon = new ImageIcon("/usr/share/pixmaps/gnome-gmush.png","Mushroom");
        // JLabel anIcon = new JLabel(icon);
    }

    public String showObject(Object object) {
        String name = VisualJavaStatics.getSimpleClassName(object.getClass());
        name = Character.toLowerCase(name.charAt(0)) + name.substring(1);
        System.out.println("[Desktop] Showing " + VisualJavaStatics.getSimpleClassName(object.getClass()) + " \"" + object + "\"");
        Variable anIcon = new Variable(this,name,object);
        // anIcon.setLocation(getWidth()/2,getHeight()/2);
        placeAnItem(anIcon);
        anIcon.setSize(anIcon.getPreferredSize());
        add(anIcon,JLayeredPane.DEFAULT_LAYER);
        anIcon.setVisible(true);
//        JMenu menu = new JMenu("Menu");
        // menu.setIcon(anIcon.getIcon());
        // add(menu);
        // JLabel anotherIcon = new JLabel("test");
        // add(anotherIcon);
//        JInternalFrame.JDesktopIcon dIcon = new JInternalFrame.JDesktopIcon(null);
        validate();
        return anIcon.name;
    }

    public void placeAnItem(Component component) {
        component.setLocation(whereToAddX + 10, whereToAddY + 10);
        whereToAddY += 30;
//        if (isVisible()) { // Doesn't get confused if not yet displayed
            if (whereToAddY > getHeight() - 50) {
                whereToAddY = 0;
                whereToAddX += 50;
                if (whereToAddX > getWidth() - 50) {
                    whereToAddX = whereToAddY = 0;
                }
            }
//        }
    }

    public void displayMethod(Method m, Object obj) {
        System.out.println("displayMethod("+m+","+obj+");");
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
        parent.validate();
        // parent.pack();
    }

}
