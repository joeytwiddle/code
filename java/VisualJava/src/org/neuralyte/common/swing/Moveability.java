package org.neuralyte.common.swing;

import java.awt.*;
import java.awt.event.*;

/** joey Nov 2, 2004 4:14:39 PM */
public class Moveability extends MouseAdapter implements MouseMotionListener {

    static Moveability moveabilityListener = null;

    Component lastClickedComponent = null;
    Point lastClicked = null;
    Point mousePosition = null;

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
        lastClicked = e.getPoint();
        lastClickedComponent = (Component)e.getSource();
        System.out.println("lastClicked="+lastClicked);
    }

    public void mouseReleased(MouseEvent e) {
        lastClicked = null;
        System.out.println("lastClicked=null");
    }

    public void mouseMoved(MouseEvent e) {
    }

    public void mouseDragged(MouseEvent e) {
        mousePosition = e.getPoint();
        double dx = mousePosition.getX() - lastClicked.getX();
        double dy = mousePosition.getY() - lastClicked.getY();
        Point at = lastClickedComponent.getLocation();
        System.out.println("Mouse="+mousePosition + " Component=" + at + " ("+dx+","+dy+")");
        // at.move((int)dx,(int)dy);
        // lastClickedComponent.setLocation(at);
        lastClickedComponent.setLocation((int)(at.getX() + dx), (int)(at.getY() + dy));
        // lastClicked = mousePosition;
    }

}
