package org.neuralyte.common.swing;

import java.awt.*;
import java.awt.event.*;

/** joey Nov 2, 2004 4:14:39 PM */
public class Moveability extends MouseAdapter implements MouseMotionListener {

    static Moveability moveabilityListener = null;

    Component componentBeingDragged = null;
    Point initialClickPoint = null;

    public static void allowUserToMove(Component component) {
        component.addMouseListener(getMoveabilityListener());
        component.addMouseMotionListener(getMoveabilityListener());
    }

    public static Moveability getMoveabilityListener() {
        if (moveabilityListener == null) {
            moveabilityListener = new Moveability();
        }
        return moveabilityListener;
    }

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
