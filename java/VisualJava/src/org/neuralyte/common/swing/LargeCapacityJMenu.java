package org.neuralyte.common.swing;

import javax.swing.*;

/** joey Nov 2, 2004 2:29:51 PM */
public class LargeCapacityJMenu extends JMenu {

    // final static int maxComponents = 30;
    final static int maxComponents = 15; // Damn those big icons!  Ideally this class would know when it exceeds display size.

    public LargeCapacityJMenu(String name) {
        super(name);
    }

    public JMenuItem add(JMenuItem menuItem) {
        if (this.getMenuComponentCount() > maxComponents - 1) { // because max includes the "More..." menu if we need to add it!
            Object last = getMenuComponent(getMenuComponentCount() - 1);
            JMenu more;
            if (last != null && last instanceof JMenu && "More...".equals(((JMenu)last).getText())) {
                more = (JMenu)last;
            } else {
            // if (last == null || !(last instanceof JMenu) || !"More...".equals(((JMenu)last).getText())) {
                more = new LargeCapacityJMenu("More...");
                super.add(more);
            }
            // return ((JMenu)last).add(menuItem);
            return more.add(menuItem);
        }
        return super.add(menuItem);
    }
}
