package org.neuralyte.common.swing;

import visualjava.HasObject;
import visualjava.CanAcceptDroppedObject;

import java.awt.*;
import java.awt.event.*;

import org.neuralyte.common.Assertion;

/** joey Nov 2, 2004 4:14:39 PM */
public class Moveability {

    static MoveabilityListener moveabilityListener = null;
    static DroppableObjectListener droppableObjectListener = null;
    static DropTargetListener dropTargetListener = null;

    public static MoveabilityListener getMoveabilityListener() {
        if (moveabilityListener == null) {
            moveabilityListener = new MoveabilityListener();
        }
        return moveabilityListener;
    }

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

    public static void allowUserToMove(Component component) {
        component.addMouseListener(getMoveabilityListener());
        component.addMouseMotionListener(getMoveabilityListener());
    }

    public static void hasObjectCanBeDropped(Component component) { // Should also implement HasObject
        org.neuralyte.common.Assertion.assert(component instanceof HasObject);
        allowUserToMove(component);
        component.addMouseListener(getDroppableObjectListener());
        component.addMouseMotionListener(getDroppableObjectListener());
    }

    public static void canAcceptDroppedObject(Component component) {
        org.neuralyte.common.Assertion.assert(component instanceof CanAcceptDroppedObject);
        component.addMouseListener(getDropTargetListener());
    }

}


class DroppableObjectListener extends MouseAdapter implements MouseMotionListener {
    public void mouseMoved(MouseEvent e) { }
    Object dragging = null;
    public void mouseDragged(MouseEvent e) {
        dragging = ((HasObject)e.getComponent()).getObject();
    }
    public void mouseReleased(MouseEvent e) {
        // Assertion.assert(dragging != null);
        if (dragging == null) {
            System.out.println("Cannot drop onto null");
        } else {
            if (Moveability.getDropTargetListener().overATarget()) {
                Moveability.getDropTargetListener().tryToDrop(dragging);
            }
        }
        dragging = null;
    }
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

class MoveabilityListener extends MouseAdapter implements MouseMotionListener {

    Component componentBeingDragged = null;
    Point initialClickPoint = null;

    public void mousePressed(MouseEvent e) {
        initialClickPoint = e.getPoint();
        componentBeingDragged = (Component)e.getSource();
        System.out.println("initialClickPoint="+initialClickPoint);
    }

    public void mouseReleased(MouseEvent e) {
        initialClickPoint = null;
        // System.out.println("initialClickPoint=null");
    }

    public void mouseMoved(MouseEvent e) {
    }

    public void mouseDragged(MouseEvent e) {
        Point mousePosition = e.getPoint();
        double dx = mousePosition.getX() - initialClickPoint.getX();
        double dy = mousePosition.getY() - initialClickPoint.getY();
        Point at = componentBeingDragged.getLocation();
        // System.out.println("Mouse="+mousePosition + " Component=" + at + " ("+dx+","+dy+")");
        // at.move((int)dx,(int)dy);
        // componentBeingDragged.setLocation(at);
        componentBeingDragged.setLocation((int)(at.getX() + dx), (int)(at.getY() + dy));
        // initialClickPoint = mousePosition;
    }

}

