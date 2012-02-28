package org.neuralyte.gui;

import java.awt.Component;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;

import org.neuralyte.Logger;


// TODO The label needs to be left-aligned in the Panel.
// TODO +/- folding Icons

public class FoldingNode extends JPanel {
    
    public final static ImageIcon closedFold = new ImageIcon("src/visualjava/bluearrow.png");
    public final static ImageIcon openFold = new ImageIcon("src/visualjava/bluearrowdown.png");
    
    // TODO: Could be better to accept a ComponentBuilder rather than a Component,
    //       so that the component can be generated lazily only if/when it is needed.
    // pff laziness should be built in to language :P
    
    public FoldingNode(String labelText, final Component contentComp) {
        super();
        final JLabel label = new JLabel(labelText);
        this.add(label);
        contentComp.setVisible(false);
        label.setIcon(closedFold);
        this.add(contentComp);
        label.addMouseListener(
                new MouseAdapter() {
                    public void mouseClicked(MouseEvent e) {
                        // Logger.log("Toggling visibility of "+contentComp);
                        contentComp.setVisible(!contentComp.isVisible());
                        label.setIcon( contentComp.isVisible() ? openFold : closedFold );
                        // validate();
                    }
                }
        );
    }

}
