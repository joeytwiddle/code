package org.neuralyte.common.swing;

import javax.swing.*;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.*;

/** joey Nov 2, 2004 7:35:11 PM */
public class DetachableJMenu extends JMenu {

    public DetachableJMenu(String name) {
        super(name,false);
        init();
    }

    void init() {
        final DetachableJMenu detachableJMenu = this;
        JMenuItem item = new JMenuItem("----- >8 -----");
        item.addActionListener(
            new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    JFrame frame = new JFrame();
                    frame.getContentPane().setLayout(new BoxLayout(frame.getContentPane(),BoxLayout.Y_AXIS));
                    // frame.getContentPane().add(detachableJMenu);
                    // Note i=1 here is intended to avoid cloning the "detach" button which was just clicked
                    for (int i=1;i<getItemCount();i++) {
                        frame.getContentPane().add(cloneItem(getItem(i)));
                    }
                    frame.setSize(frame.getPreferredSize());
                    frame.setVisible(true);
                }
            }
        );
        super.add(item);
    }

    Component cloneItem(JMenuItem item) {
        if (item == null)
            return new JSeparator();
        // JMenuItem clone = new JMenuItem(item.getText());
        AbstractButton clone;
        if (item instanceof JMenu) {
            clone = new JMenu(item.getText());
            JMenu itemMenu = (JMenu)item;
            for (int i=0;i<itemMenu.getItemCount();i++) {
                JMenuItem subItem = itemMenu.getItem(i);
                ((JMenu)clone).add(cloneItem(subItem));
            }
        }
        clone = new JButton(item.getText());
        if (item.getIcon() != null) {
            clone.setIcon(item.getIcon());
        }
        ActionListener[] listeners = item.getActionListeners();
        for (int i=0;i<listeners.length;i++) {
            clone.addActionListener(listeners[i]);
        }
        return clone;
    }

}
