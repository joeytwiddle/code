package org.neuralyte.common.swing;

import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;

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
                    final JFrame frame = new JFrame();
                    frame.getContentPane().setLayout(new BoxLayout(frame.getContentPane(),BoxLayout.Y_AXIS));
                    // frame.getContentPane().add(detachableJMenu);
                    // Note i=1 here is intended to avoid cloning the "detach" button which was just clicked
                    // for (int i=1;i<getItemCount();i++) {
                        // frame.getContentPane().add(cloneItem(getItem(i),true));
                    final JPopupMenu clonedMenu = new JPopupMenu(getText());
                    for (int i=1;i<getItemCount();i++) {
                        clonedMenu.add(cloneItem(getItem(i),true));
                    }
                    frame.getContentPane().add(clonedMenu);
                    frame.setSize(clonedMenu.getPreferredSize());
                    frame.setVisible(true);
                    clonedMenu.setLocation(frame.getContentPane().getLocationOnScreen());
                    clonedMenu.setVisible(true);
                    frame.addComponentListener(
                        new ComponentAdapter() {
                            public void componentMoved(ComponentEvent e) {
                                clonedMenu.setLocation(frame.getContentPane().getLocationOnScreen());
                            }
                        }
                    );
                    // clonedMenu.setSelected(true);
                    // clonedMenu.updateUI();
                }
            }
        );
        super.add(item);
    }

    Component cloneItem(JMenuItem item, boolean menusAsButtons) {
        if (item == null)
            return new JSeparator();
        AbstractButton clone;
        if (item instanceof JMenu) {
            if (menusAsButtons) {
                final JPopupMenu newMenu = new JPopupMenu(item.getText());
                JMenu itemMenu = (JMenu)item;
                for (int i=0;i<itemMenu.getItemCount();i++) {
                    JMenuItem subItem = itemMenu.getItem(i);
                    newMenu.add(cloneItem(subItem,false));
                }
                clone = new JMenuItem(item.getText());
                clone.addActionListener(
                    new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                            newMenu.setLocation(((Component)e.getSource()).getLocationOnScreen());
                            newMenu.setVisible(!newMenu.isVisible());
                        }
                    }
                );
            } else {
                JMenu newMenu = new JMenu(item.getText());
                JMenu itemMenu = (JMenu)item;
                for (int i=0;i<itemMenu.getItemCount();i++) {
                    JMenuItem subItem = itemMenu.getItem(i);
                    newMenu.add(cloneItem(subItem,false));
                }
                clone = newMenu;
            }
        } else {
            // if (menusAsButtons) {
                // clone = new JButton(item.getText());
            // } else {
                clone = new JMenuItem(item.getText());
            // }
        }
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
