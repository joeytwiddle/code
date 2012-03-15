package visualjava.guicomponents;

import java.awt.Component;
import java.util.Collection;

import javax.swing.JFrame;
import javax.swing.JInternalFrame;
import javax.swing.JLabel;

import org.neuralyte.genericgui.CollectionViewer;
import org.neuralyte.gui.FoldingNode;

import visualjava.VariableModel;


public class InspectionWindow {

    public Desktop desktop;
    public JInternalFrame frame;
    
    public InspectionWindow(VariableModel underInspection, Desktop desktop) {
        frame = new JInternalFrame(""+underInspection,true,true,false,true);
        
        // What follows should actually be performed by the component
        // which the Factory chooses to build.
        
        /** @todo
         *  
         * Get preferred type of InspectionComponent from Factory.
         *
         * The default generic viewer for any Java class type variable, could
         * show the object's toString, and show each of its visible properties.
         * 
         * User should be able to expand the properties to investigate them.
         * 
         * We might wish child components to be InspectionWindows also.
         * But if this is not handled lazily, we will need to stop at some maxDepth!
         * 
         * Everything should be made drag+droppable.
         * And context menus should be added.
         * 
        **/

        Object value = underInspection.getValue();
        
        Component comp;
        if (value instanceof Collection) {
            comp = new CollectionViewer((Collection)value);
        } else {
            comp = new JLabel("\""+value+"\"");
            comp = new FoldingNode(underInspection.getType()+" "+underInspection.getName(),comp);
        }
        
        // frame.getContentPane().add(comp);
        frame.getContentPane().add(comp);
    }
    
    // Our foldable component could simply be a JPanel containing:
    // 1) A JLabel with a [+] or [-] icon and a name.
    // 2) The expanded component, lazy, sometimes hidden.
    
}
