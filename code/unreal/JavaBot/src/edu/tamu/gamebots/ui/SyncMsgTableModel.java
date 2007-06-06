/*
 * SyncMsgTableModel.java
 *
 * Created on February 21, 2002, 11:52 AM
 */

package edu.tamu.gamebots.ui;
import edu.isi.gamebots.client.*;

/**
 * <p>
 * A table model for displaying vision messages from the server based on a
 * MessageBlock object. Columns:
 * <ul>
 * <li>Column 1: Type</li>
 * <li>Column 2: ID</li>
 * <li>Column 3: Location</li>
 * <li>Column 4: Reachable (Boolean)</li>
 * <ul>
 * </p>
 * @author rtr7684
 */
public class SyncMsgTableModel extends javax.swing.table.AbstractTableModel {
    
    private MessageBlock msgBlock;
    private String properties[] = { "Type", "Id", "Location", "Reachable"};

    /** 
     * <p>
     * Creates a new instance of SyncMsgTableModel
     * </p>
     * @param block The <CODE>MessageBlock</CODE>
     */
    public SyncMsgTableModel(MessageBlock block) {
      msgBlock = block.onlyNodeMessages();
    }

    /**
     * <p>
     * Returns the vlaue at a row and column
     * </p>
     * @param row The row
     * @param column THe column
     * @return The object at the given position
     */    
    public Object getValueAt(int row, int column) {
      Message msg = msgBlock.getMessageAt(row);
      if(column==0) return msg.getType();
      else if(column==3) return Boolean.valueOf(msg.getProperty(properties[column]));
      else return msg.getProperty(properties[column]);
    }
    
    /**
     * <p>
     * Given a column number, returns the name of the column.
     * </p>
     * @param column The column number
     * @return The name of the column
     */    
    public String getColumnName(int column) {
      return properties[column];
    }
    
    /**
     * <p>
     * Returns the number of rows
     * </p>
     * @return The number of visible objects displayed in the table
     */    
    public int getRowCount() {
      return msgBlock.getNumMessages();
    }
    
    /**
     * <p>
     * False, no cells are editable
     * </p>
     * @param param row number
     * @param param1 column number
     * @return false
     */    
    public boolean isCellEditable(int param, int param1) {
        return false;
    }
    
    /**
     * <p>
     * Set the value of a cell to an object
     * </p>
     * @param obj The object to set to
     * @param param row number
     * @param param2 column number
     */    
    public void setValueAt(Object obj, int param, int param2) {
    }
    
    /**
     * <p>
     * Returns the class of the data in a given column
     * </p>
     * @param col The Column numober
     * @return the class of that column
     */    
    public Class getColumnClass(int col) {
      if(col == 3) return Boolean.TRUE.getClass();
      else return (new String("")).getClass();
    }
    
    /**
     * <p>
     * Returns the number of columns in the table
     * </p>
     * @return The number of columns
     */    
    public int getColumnCount() {
      return properties.length;
    }
    
}
