package org.neuralyte.common.swing;

import visualjava.HasObject;
import visualjava.CanAcceptDroppedObject;

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
        org.neuralyte.common.Assertion.assert(component instanceof HasObject);
        Moveability.allowUserToMove(component);
        component.addMouseListener(getDroppableObjectListener());
        component.addMouseMotionListener(getDroppableObjectListener());
    }

    public static void canAcceptDroppedObject(Component component) {
        org.neuralyte.common.Assertion.assert(component instanceof CanAcceptDroppedObject);
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
    public void mouseEntered(MouseEvent e) {
        if (currentTarget != null) {
            System.out.println("Has entered multiple droppable components!");
        }
        currentTarget = e.getComponent();
        System.out.println("Entered drop target " + currentTarget);
    }
    public void mouseExited(MouseEvent e) {
        System.out.println("Exited drop target " + currentTarget);
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

