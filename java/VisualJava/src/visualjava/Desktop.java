package visualjava;

import javax.swing.*;
import java.awt.*;
import java.lang.reflect.Method;

/** joey Nov 1, 2004 2:11:42 AM */
public class Desktop extends JDesktopPane {

    JFrame parent;

    public Desktop(JFrame _parent) {
        parent = _parent;
        // putClientProperty("JDesktopPane.dragMode","outline");
//        setLayout(new FlowLayout());
        // ImageIcon icon = new ImageIcon("/usr/share/pixmaps/gnome-gmush.png","Mushroom");
        // JLabel anIcon = new JLabel(icon);
        ObjectIcon anIcon = new ObjectIcon(this, "Hello");
        anIcon.setLocation(getWidth()/2,getHeight()/2);
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
    }

    public void displayMethod(Method m, Object obj) {
        MethodFrame methodFrame = new MethodFrame(m,obj);
        methodFrame.setLocation(getWidth()/2,getHeight()/2);
        methodFrame.setSize(methodFrame.getPreferredSize());
        add(methodFrame);
        methodFrame.setVisible(true);
        parent.validate();
        // parent.pack();
    }

}
