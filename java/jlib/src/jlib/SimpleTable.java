package jlib;

import javax.swing.table.AbstractTableModel;
import java.util.Map;
import java.util.HashMap;
import java.awt.Point;
import javax.swing.JTable;

public class SimpleTable extends JTable {

  public SimpleTable(int x,int y) {
    super(new SimpleTableModel(x,y));
  }

  public void clearRow(int row) {
    for (int i=0;i<getColumnCount();i++)
      setValueAt(null,row,i);
  }

}