package jlib;

import javax.swing.table.AbstractTableModel;
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
    int x=minx;
    Object[] keys=map.keySet().toArray();
    for (int i=0;i<keys.length;i++) {
      Point p=(Point)keys[i];
      if (p.x>x)
        x=p.x;
    }
    return x;
  }

  public int getRowCount() {
    int y=miny;
    Object[] keys=map.keySet().toArray();
    for (int i=0;i<keys.length;i++) {
      Point p=(Point)keys[i];
      if (p.y>y)
        y=p.y;
    }
    return y;
  }

  public Object getValueAt(int row, int col) {
    Point p=new Point(col,row);
    Object o=map.get(p);
    return o;
  }

  public void setValueAt(Object o,int row,int col) {
    map.put(new Point(col,row),o);
  }

}