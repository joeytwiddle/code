package org.neuralyte.common.swing;

import visualjava.HasObject;
import visualjava.CanAcceptDroppedObject;

import javax.swing.*;
import javax.swing.plaf.ComponentUI;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseEvent;

/** joey Nov 3, 2004 5:15:30 PM */
public class DragAndDropManager {

    static DroppableObjectListener droppableObjectListener = null;
    static DropTargetListener dropTargetListener = null;

    public static DroppableObjectListener getDroppableObjectListener() {
        if (droppableObjectListener == null) {
            droppableObjectListener = new DroppableObjectListener();
        }
        return droppableObjectListener;
    }

    public static DropTargetListener getDropTargetListener() {
        if (dropTargetListener == null) {
            dropTargetListener = new DropTargetListener();
        }
        return dropTargetListener;
    }

    public static void hasObjectCanBeDropped(Component component) { // Should also implement HasObject
        org.neuralyte.common.Assertion.assertThat(component instanceof HasObject);
        Moveability.allowUserToMove(component);
        component.addMouseListener(getDroppableObjectListener());
        component.addMouseMotionListener(getDroppableObjectListener());
    }

    public static void canAcceptDroppedObject(Component component) {
        org.neuralyte.common.Assertion.assertThat(component instanceof CanAcceptDroppedObject);
        component.addMouseListener(getDropTargetListener());
    }
}

class DroppableObjectListener extends MouseAdapter implements MouseMotionListener {
    Component dragged = null;
    Object dragging = null;
    Point startPoint = null;
    // public void mousePressed(MouseEvent e) {
        // startPoint = e.getPoint();
    // }
    public void mouseDragged(MouseEvent e) {
        if (dragging == null) {
            // startPoint = e.getPoint();
            startPoint = e.getComponent().getLocation();
        }
        dragged = e.getComponent();
        dragging = ((HasObject)e.getComponent()).getObject();
        /*
        if (DragAndDropManager.getDropTargetListener().overATarget()) {
            if (((CanAcceptDroppedObject)DragAndDropManager.getDropTargetListener().currentTarget).isAcceptable(dragging)) {
                DragAndDropManager.getDropTargetListener().currentTarget.setBackground(Color.green);
            } else {
                DragAndDropManager.getDropTargetListener().currentTarget.setBackground(Color.red);
            }
        }
        */
    }
    public void mouseReleased(MouseEvent e) {
        // Assertion.assert(dragging != null);
        if (dragged == null) {
            System.out.println("I don't appear to be carrying anything!");
        } else {
            if (DragAndDropManager.getDropTargetListener().overATarget()) {
                DragAndDropManager.getDropTargetListener().tryToDrop(dragging);
                // If the user tried to drop the object,
                // then let it remain where it was on the desktop!
                dragged.setLocation(startPoint);
            }
        }
        dragged = null;
        dragging = null;
        startPoint = null;
    }
    public void mouseMoved(MouseEvent e) { }
}

class DropTargetListener extends MouseAdapter {
    Component currentTarget = null;
    Color previousBgColor = null;
    Color previousFgColor = null;
    public void mouseEntered(MouseEvent e) {
        if (currentTarget != null) {
            System.out.println("Has entered multiple droppable components!");
        }
        currentTarget = e.getComponent();
        System.out.println("Entered drop target " + currentTarget);
        if (Moveability.moveabilityListener.componentBeingDragged != null) {
            /*
            Component toClean = currentTarget;
            // while (toClean != null) {
            for (int i=0;i<10 && toClean!=null;i++) {
                try {
                    ComponentUI ui = (ComponentUI)toClean.getClass().getMethod("getUI",new Class[0]).
                        invoke(toClean,new Object[0]);
                    ui.uninstallUI((JComponent)toClean);
                } catch (Exception ex) { }
                toClean = toClean.getParent();
            }
            */
            previousBgColor = currentTarget.getBackground();
            previousFgColor = currentTarget.getForeground();
            /** @todo Make this line look nicer and your Objects will be better! **/
            if (((CanAcceptDroppedObject)currentTarget).isAcceptable(
                    ((HasObject)Moveability.moveabilityListener.componentBeingDragged).getObject()
            )) {
                currentTarget.setBackground(Color.green);
                currentTarget.setForeground(Color.green);
            } else {
                currentTarget.setBackground(Color.red);
                currentTarget.setForeground(Color.red);
            }
        }
    }
    public void mouseExited(MouseEvent e) {
        System.out.println("Exited drop target " + currentTarget);
        if (previousBgColor != null) {
            // I couldn't set the JLabel's background successfully,
            // until I had uninstalled its UI and its parent's UI.
            currentTarget.setBackground(previousBgColor);
            currentTarget.setForeground(previousFgColor);
            previousBgColor = null;
        }
        currentTarget = null;
    }
    public boolean overATarget() {
        return (currentTarget != null);
    }
    public boolean tryToDrop(Object obj) {
        CanAcceptDroppedObject acceptor = (CanAcceptDroppedObject)currentTarget;
        if (acceptor.isAcceptable(obj)) {
            System.out.println("Acceptable: " + obj);
            return acceptor.acceptObject(obj);
        } else {
            System.out.println("Not acceptable: " + obj);
            return false;
        }
    }
}

