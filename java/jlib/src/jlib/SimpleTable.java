package jlib;

import javax.swing.table.*;
import java.util.Map;
import java.util.HashMap;
import java.awt.Point;
// import javax.swing.DefaultCellEditor;
// import javax.swing.DefaultTableCellRenderer;
import java.awt.*;
import javax.swing.*;
import javax.swing.JTable;

class MyTableCellRenderer implements TableCellRenderer {

	Component comp;

	MyTableCellRenderer(Object oo) {
		comp=(Component)oo;
	}

	public Component getTableCellRendererComponent(JTable t,Object v,boolean sel,boolean foc,int r,int c) {
		return comp;
	}

}

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

	public void set(Object o,int col,int row) {
		model.setValueAt(o,row,col);
		// TableColumn tc=getColumn(getColumnName(col));
		if (o instanceof Component) {
			TableColumn tc=getColumnModel().getColumn(col);
			// tc.setCellRenderer(new DefaultTableCellRenderer());
			tc.setCellRenderer(new MyTableCellRenderer(o));
		}
	}

	public Object get(int col,int row) {
		return model.getValueAt(row,col);
	}

}
