package org.neuralyte.genericgui;

import java.awt.Color;
import java.awt.Component;
import java.util.Collection;
import java.util.EventObject;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JComponent;
import javax.swing.JEditorPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.event.CellEditorListener;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableCellRenderer;
import javax.swing.text.Document;


public class CollectionViewer extends JPanel {

    Collection collection;
    
    JTable table;
    
    public CollectionViewer(Collection collection) {
        this.collection = collection;
        table = new JTable(1,1);

        // Check for common ancestor, and count its fields - that's num columns.
        
        for (Object o : collection) {
            Object[] row = new Object[1];
            row[0] = o;
            ((DefaultTableModel) table.getModel()).addRow(row);
        }
        setDoubleBuffered(false);
        table.setDoubleBuffered(true);
        table.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
        table.setRowSelectionAllowed(true);
        // table.setDragEnabled(true);
        add(new JScrollPane(table));
        table.repaint();
    }
    
}


/*
class CustomTableCellRenderer implements TableCellRenderer {

   Map specialColorCells = new HashMap();

   public CustomTableCellRenderer() {
      // setOpaque(true); //MUST do this for background to show up.
   }

   public void setColorOfCell(int row, int column, Color color) {
      specialColorCells.put(row+":"+column,color);
   }

   public Component getTableCellRendererComponent(JTable table, Object value,
         boolean isSelected, boolean hasFocus, int row, int column) {
      DefaultTableCellRenderer tcr = new DefaultTableCellRenderer();
      // TableCellRenderer tcr = table.getCellRenderer(row,column);

       Component component;

       // If this cell contains an HTML document, use a JEditorPane as the component
       // otherwise use the default component.
       if (value instanceof Document) {
           JEditorPane jeditorPane = new JEditorPane("text/html",((Document)value).toString());
           jeditorPane.setDocument((Document)value);
           component = jeditorPane;
           // component = new JScrollPane(jeditorPane);
       } else {
           component = tcr.getTableCellRendererComponent(table,value,isSelected,hasFocus,row,column);
       }

      // If we have been asked to colourise this cell, then colourise the component.
      String key = row+":"+column;
      Object override = specialColorCells.get(key);
      if (override != null) {
          if (component instanceof JComponent) {
              ((JComponent)component).setOpaque(true);
              ((JComponent)component).setBackground((Color)override);
          }
      }
      return component;
   }

}

class CustomTableCellEditor implements TableCellEditor {

    // @todo It is not closing the editor component when focus leaves the cell!
    // If we do get it closing, it might be preferable to keep the rowHeight expanded,
    // maybe even expand it again on the next edit.
    // But wait, keeping that JScrollBar scrolled to the user's desired position is probably a feature!
    // @todo We do get trouble if we try to edit the cell above one already edited; the former occludes the latter!

    public CustomTableCellEditor() {
    }

    public Object getCellEditorValue() {
        return null;
    }

    public boolean isCellEditable(EventObject anEvent) {
        return true;
    }

    public boolean shouldSelectCell(EventObject anEvent) {
        return true;
    }

    public boolean stopCellEditing() {
        return true;
    }

    public void cancelCellEditing() {
    }

    public void addCellEditorListener(CellEditorListener l) {
    }

    public void removeCellEditorListener(CellEditorListener l) {
    }

    public Component getTableCellEditorComponent(JTable table, Object value,
                                                 boolean isSelected,
                                                 int row, int column) {
        // Sometimes a non-diffed line may be too long to fit it's display,
        // and the user may be interested in it, so now I add a JScrollPane to both types.
        if (value instanceof Document) {
            // table.setRowHeight(row,table.getRowHeight()*2);
            table.setRowHeight(row,table.getRowHeight()*3); // 2 lines and space for a horizontal scrollbar if one is produced.
            JEditorPane jeditorPane = new JEditorPane("text/html",((Document)value).toString());
            jeditorPane.setDocument((Document)value);
            jeditorPane.setEditable(false);
            // table.setRowHeight(row,(int)jeditorPane.getPreferredSize().getHeight());
            // return new JScrollPane(jeditorPane);
            // return jeditorPane;
            return new JScrollPane(jeditorPane,JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        } else {
            // return new DefaultCellEditor().getTableCellEditorComponent(table,value,isSelected,row,column);
            // return new JTextArea(value.toString());
            table.setRowHeight(row,table.getRowHeight()*3); // 2 lines and space for a horizontal scrollbar if one is produced.
            JTextArea jtextArea = new JTextArea(value.toString());
            jtextArea.setEditable(false);
            // return jtextArea;
            return new JScrollPane(jtextArea,JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
            // return super.getTableCellEditorComponent(table,value,isSelected,row,column);
        }

    }
}
*/