package org.neuralyte.common.swing;

import visualjava.VisualJava;

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
        JMenuItem item = new JMenuItem("----- 8< -----");
        item.addActionListener(
            new ActionListener() {
                public void actionPerformed(ActionEvent e) {
        // item.addMouseListener(
            // new MouseAdapter() {
                // public void mouseClicked(MouseEvent e) {
                    /*
                    JPopupMenu test = new JPopupMenu("Test");
                    JMenuItem item = new JMenuItem("test1");
                    item.addActionListener(
                        new ActionListener() {
                            public void actionPerformed(ActionEvent e) {
                                System.out.println("OK");
                            }
                        }
                    );
                    test.add(item);
                    test.add(new JMenuItem("test2"));
                    JMenu menu2 = new JMenu("Inner menu");
                    menu2.add(new JMenuItem("nothing"));
                    test.add(menu2);
                    // test.setVisible(true);
                    test.show(VisualJava.desktop,0,0); // ,true);
                    */
                    final JFrame frame = new JFrame();
                    frame.getContentPane().setLayout(new BoxLayout(frame.getContentPane(),BoxLayout.Y_AXIS));
                    // frame.getContentPane().add(detachableJMenu);
                    // Note i=1 here is intended to avoid cloning the "detach" button which was just clicked
                    for (int i=1;i<getItemCount();i++) {
                        frame.getContentPane().add(cloneItem(getItem(i),true));
                    }
                    // final JPopupMenu clonedMenu = new JPopupMenu(getText());
                    // for (int i=1;i<getItemCount();i++) {
                    // clonedMenu.add(cloneItem(getItem(i),true));
                    // }
                    // frame.setLocation(e.getPoint());
                    // frame.setLocation(((Component)e.getSource()).getLocationOnScreen());
                    // frame.getContentPane().add(clonedMenu);
                    // frame.getContentPane().setSize(clonedMenu.getPreferredSize());
                    //  frame.setSize(clonedMenu.getPreferredSize());
                    // setVisible(true);
                    // frame.setLocation(getLocationOnScreen());
                    // setVisible(false);
                    frame.setSize(frame.getPreferredSize());
                    frame.setVisible(true);
                    // clonedMenu.show(VisualJava.desktop,0,0);
                    // clonedMenu.setLocation(frame.getContentPane().getLocationOnScreen());
                    // clonedMenu.setVisible(true);
                    // clonedMenu.show(frame.getContentPane(),0,0);
                    // clonedMenu.setVisible(true);
                    // frame.addComponentListener(
                    // new ComponentAdapter() {
                    // public void componentMoved(ComponentEvent e) {
                    // clonedMenu.setLocation(frame.getContentPane().getLocationOnScreen());
                    // clonedMenu.setVisible(true);
                    // }
                    // }
                // );
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
            JMenu itemMenu = (JMenu)item;
            System.out.println("Doing " + itemMenu.getText() + " with " + itemMenu.getItemCount() + " children.");
            if (menusAsButtons) {
                final JPopupMenu newMenu = new JPopupMenu(item.getText());
                for (int i=0;i<itemMenu.getItemCount();i++) {
                    JMenuItem subItem = itemMenu.getItem(i);
                    newMenu.add(cloneItem(subItem,false));
                }
                clone = new JMenuItem(item.getText());
                /*
                clone = new JMenuItem(item.getText()) {
                    public void doClick(int pressTime) {
                        newMenu.show(this,(int)this.getWidth(),0);
                        super.doClick(pressTime);
                    }

                    public void doClick() {
                        super.doClick();
                    }
                };
                */
                clone.addActionListener(
                    new ActionListener() {
                        Component last = null;
                        public void actionPerformed(ActionEvent e) {
                            // newMenu.setLocation(((Component)e.getSource()).getLocationOnScreen());
                            // newMenu.setVisible(!newMenu.isVisible());
                            // newMenu.show(newMenu,0,0);
                            Component src = (Component)e.getSource();
                            /*
                            if (src == last) {
                                newMenu.setVisible(false);
                                last = null;
                            } else {
                                newMenu.show(src,(int)src.getWidth(),0);
                                last = src;
                            }
                            */
                            newMenu.show(src,(int)src.getWidth(),0);
                        }
                    }
                );
                /*
                clone.addMouseListener(
                    new MouseAdapter() {
                        Component last = null;
                        public void mouseClicked(MouseEvent e) {
                            Component src = (Component)e.getSource();
                            // newMenu.show(src,10,10);
                            // newMenu.show(src,(int)e.getPoint().getX()-6,(int)e.getPoint().getY()-6);
                            if (src == last) {
                                newMenu.setVisible(false);
                                last = null;
                            } else {
                                newMenu.show(src,(int)src.getWidth(),0);
                                last = src;
                            }
                        }
                    }
                );
                */
              } else {
                JMenu newMenu = new JMenu(item.getText());
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
        //if (!(clone instanceof JMenu)) {
            ActionListener[] listeners = item.getActionListeners();
            for (int i=0;i<listeners.length;i++) {
                clone.addActionListener(listeners[i]);
            }
        //}
        return clone;
    }

}
