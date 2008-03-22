package org.neuralyte.common.swing.draganddrop;

import java.awt.Component;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;

import javax.swing.JLayeredPane;

/** joey Nov 2, 2004 4:14:39 PM */
public class Moveability {

    static MoveabilityListener moveabilityListener = null;

    public static MoveabilityListener getMoveabilityListener() {
        if (moveabilityListener == null) {
            moveabilityListener = new MoveabilityListener();
        }
        return moveabilityListener;
    }

    public static void allowUserToMove(Component component) {
        component.addMouseListener(getMoveabilityListener());
        component.addMouseMotionListener(getMoveabilityListener());
    }

}

class MoveabilityListener extends MouseAdapter implements MouseMotionListener {

    /** bring_to_front works fine except D+D manager does not catch DropTarget enter/exit events.  :-(  See BUGS in TODO.txt for explanation */
    public final static boolean bring_to_front = false;

    Component componentBeingDragged = null;
    Point initialClickPoint = null;

    public void mousePressed(MouseEvent e) {
        initialClickPoint = e.getPoint();
        // componentBeingDragged = (Component)e.getSource();
        componentBeingDragged = e.getComponent();
        org.neuralyte.Logger.log("initialClickPoint="+initialClickPoint);
        if (bring_to_front && componentBeingDragged.getParent() instanceof JLayeredPane) {
            ((JLayeredPane)componentBeingDragged.getParent()).setLayer(componentBeingDragged,JLayeredPane.DRAG_LAYER.intValue());
        }
    }

    public void mouseReleased(MouseEvent e) {
        initialClickPoint = null;
        componentBeingDragged = null;
        // org.neuralyte.Logger.log("initialClickPoint=null");
        if (bring_to_front && componentBeingDragged.getParent() instanceof JLayeredPane) {
            ((JLayeredPane)componentBeingDragged.getParent()).setLayer(componentBeingDragged,JLayeredPane.DEFAULT_LAYER.intValue());
        }
    }

    public void mouseMoved(MouseEvent e) {
    }

    public void mouseDragged(MouseEvent e) {
        Point mousePosition = e.getPoint();
        double dx = mousePosition.getX() - initialClickPoint.getX();
        double dy = mousePosition.getY() - initialClickPoint.getY();
        Point at = componentBeingDragged.getLocation();
        // org.neuralyte.Logger.log("Mouse="+mousePosition + " Component=" + at + " ("+dx+","+dy+")");
        // at.move((int)dx,(int)dy);
        // componentBeingDragged.setLocation(at);
        componentBeingDragged.setLocation((int)(at.getX() + dx), (int)(at.getY() + dy));
        // initialClickPoint = mousePosition;
    }

}

