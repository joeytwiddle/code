package visualjava;

import javax.swing.*;
import java.awt.*;

/** joey Nov 1, 2004 2:11:42 AM */
public class Desktop extends JDesktopPane {

    public Desktop() {
        setLayout(new FlowLayout());
        // ImageIcon icon = new ImageIcon("/usr/share/pixmaps/gnome-gmush.png","Mushroom");
        // JLabel anIcon = new JLabel(icon);
        Icon anIcon = new Icon("Hello");
        add(anIcon);
//        JMenu menu = new JMenu("Menu");
        // menu.setIcon(anIcon.getIcon());
        // add(menu);
        // JLabel anotherIcon = new JLabel("test");
        // add(anotherIcon);
//        JInternalFrame.JDesktopIcon dIcon = new JInternalFrame.JDesktopIcon(null);
    }

}
