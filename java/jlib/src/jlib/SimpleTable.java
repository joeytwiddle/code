package jlib;

import javax.swing.table.AbstractTableModel;
import java.util.Map;
import java.util.HashMap;
import java.awt.Point;
import javax.swing.JTable;

public class SimpleTable extends JTable {

	SimpleTableModel model;

  public SimpleTable(int x,int y) {
	 super(new SimpleTableModel(x,y));
	 model=(SimpleTableModel)dataModel;
  }

  public void clearRow(int row) {
    for (int i=0;i<getColumnCount();i++)
      setValueAt(null,row,i);
  }

  public void set(Object o,int row,int col) {
    model.setValueAt(o,row,col);
  }

}
