package org.neuralyte.common.swing;

import javax.swing.*;

/**
 * This lazy JMenu does not need to generate its child menu items
 * until it is actually accessed.
 * <P>
 * This is useful if you want to provide large menu trees and
 * it would be costly to generate all the nodes in advance.
 * <P>
 * A LazyJMenu's child items are generated when they are needed by implementing generateChildren().
 * <P>
 * By default a LazyJMenu will only generate it's children once, when they are first needed.
 * But a LazyJMenu may be made <I>dynamic</I>, in which case it will regenerate its child items
 * every time the menu is accessed.  (This behaviour assumes the menu's children should only undergo change when the menu is not open!)
 * <P>
 * Any JMenuItems added to a LazyJMenu outside of generateChildren are liable to be removed!  (Now this is only true in dynamic mode =)
 * <P>
 * (I'm sure Java Swing menu's used to be lazy!)
 * <P>
 * (Thanks to Liu Yang.)
 */

public abstract class LazyJMenu extends LargeCapacityJMenu {

    boolean dynamic;
    boolean generated;

    public LazyJMenu(String name) {
        this(name,false);
    }

    /** If the Menu is dynamic, then generateChildren() will be called every time
     *  the Menu is re-selected by the user (whenever it re-appears).
     *  Otherwish generateChildren() is called only the first time the menu is reached.
     */
    public LazyJMenu(String name, boolean _dynamic) {
        super(name);
        dynamic = _dynamic;
        generated = false;
    }

    public void setSelected(boolean selected) {
        if (selected && (!generated || dynamic)) {
            if (dynamic) {
                this.removeAll();
            }
            this.generateChildren();
            generated = true;
        }
        super.setSelected(selected);
    }

    public abstract void generateChildren();

}
