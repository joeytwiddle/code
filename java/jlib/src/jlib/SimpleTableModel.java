package jlib; 
import javax.swing.table.*;
import java.util.Map;
import java.util.HashMap;
import java.awt.Point;

public class SimpleTableModel extends AbstractTableModel {

	Map map=new HashMap();
	int minx,miny;

	SimpleTableModel(int x,int y) {
		minx=x;
		miny=y;
	}

	public int getColumnCount() {
		Object[] keys=map.keySet().toArray();
		for (int i=0;i<keys.length;i++) {
			Point p=(Point)keys[i];
			System.out.println(""+p+" : "+minx);
			if (p.x>minx)
				minx=p.x;
		}
		return minx;
	}

	public int getRowCount() {
		Object[] keys=map.keySet().toArray();
		for (int i=0;i<keys.length;i++) {
			Point p=(Point)keys[i];
			if (p.y>miny)
				miny=p.y;
		}
		return miny;
	}

	public Object getValueAt(int row, int col) {
		Point p=new Point(col,row);
		Object o=map.get(p);
		return o;
	}

	public void setValueAt(Object o,int row,int col) {
		map.put(new Point(col,row),o);
	}

	public Class getColumnClass(int c) {
		Object o=getValueAt(0, c);
		// System.out.println(""+o);
		if (o==null)
			return new String("").getClass();
		else
			return o.getClass();
	}

}
