import TrawletManager;

import java.awt.*;
import java.awt.event.*;
import java.awt.Frame;
import java.awt.TextArea;
import java.awt.FlowLayout;
import java.awt.Dimension;
import javax.swing.JTable;
import javax.swing.table.TableColumn;
import javax.swing.JScrollPane;
import java.lang.String;
import java.util.List;

import jlib.SimpleTable;
import jlib.SimpleTableModel;
import jlib.JMaths;

// import nuju.*;

public class TrawlerGUI extends Frame implements ActionListener {

  TrawletManager man;

  TextArea text;
  SimpleTable table;
  TextField field;

  TrawlerGUI(TrawletManager m) {
    super("Trawler");
    man=m;
    setSize(600,700);
    setLayout(new FlowLayout());

    text=new TextArea();
    text.setColumns(80);
    text.setRows(16);
//    text.setSize(800,400);
    add(text);
//    text.setSize(800,400);

    /*TableModel model = new DefaultTableModel() {
          public int getColumnCount() { return 10; }
          public int getRowCount() { return 10;}
          // public Object getValueAt(int row, int col) { return new Integer(row*col); }
    };
    model=new SimpleTableModel();*/
    table=new SimpleTable(5,800);
//    table.setSize(600,400);
    TableColumn tcm;
    tcm=table.getColumnModel().getColumn(0);
    tcm.setHeaderValue("URL");
    tcm.setPreferredWidth(500);
    tcm=table.getColumnModel().getColumn(1);
    tcm.setHeaderValue("Stage");
    tcm.setPreferredWidth(100);
    tcm=table.getColumnModel().getColumn(2);
    tcm.setHeaderValue("Depth");
    tcm.setPreferredWidth(30);
    tcm=table.getColumnModel().getColumn(3);
    tcm.setHeaderValue("Retrieved");
    tcm.setPreferredWidth(70);
    tcm=table.getColumnModel().getColumn(4);
    tcm.setHeaderValue("ConObj");
    tcm.setPreferredWidth(100);
//    Nuju.present(table.getColumnModel().getColumn(3));
//    table.getColumnModel().getColumn(4).setPreferredWidth(50);
    // add(table);
    JScrollPane scrollpane=new JScrollPane(table);
    scrollpane.setPreferredSize(new Dimension(580,400));
    add(scrollpane);

    Label label=new Label("Forbid:");
    add(label);
    field=new TextField("http://www.");
    field.setColumns(60);
    field.addActionListener(this);
    add(field);

    // setVisible(true);
    show();
  }

  public void actionPerformed(ActionEvent e) {
    String command=e.getActionCommand();
    man.strat.forbiddenStarts.add(command);
    for (int i=0;i<man.waiting.size();i++) {
      Trawlet t=(Trawlet)man.waiting.get(i);
      if (t.url.toString().startsWith(command)) {
        man.waiting.remove(t);
        i--;
      }
    }
    for (int i=0;i<man.open.size();i++) {
      Trawlet t=(Trawlet)man.open.get(i);
      if (t.url.toString().startsWith(command)) {
        man.timeout(t);
        i--;
      }
    }
  }

  public void redraw() {
    String s="";
    s+="Open --------------------------\n";
    s+=list(man.open);
    s+="Parsing -----------------------\n";
    s+=list(man.parsing);
    s+="Waiting -----------------------\n";
    s+=list(man.waiting);
    s+="Parsed ------------------------\n";
    s+=list(man.parsed);
    s+="Saved -------------------------\n";
    s+=list(man.saved);
    s+="Timed out ---------------------\n";
    s+=list(man.dodgy);
    float f=(float)text.getCaretPosition()/(float)text.getText().length();
    text.setText(s);
    // text.replaceRange(s,0,text.getText().length());
    text.setCaretPosition((int)(f*(float)text.getText().length()));

    int row=0;
    table.clearRow(row);

    table.setValueAt("Open",row,0);
    row++;
    table.clearRow(row);
    for (int i=0;i<man.open.size();i++) {
      showdata(table,row,man.open,i);
      row++;
      table.clearRow(row);
    }

    table.setValueAt("Parsing",row,0);
    row++;
    table.clearRow(row);
    for (int i=0;i<man.parsing.size();i++) {
      showdata(table,row,man.parsing,i);
      row++;
      table.clearRow(row);
    }

    table.setValueAt("Waiting",row,0);
    row++;
    table.clearRow(row);
    for (int i=0;i<man.waiting.size();i++) {
      showdata(table,row,man.waiting,i);
      row++;
      table.clearRow(row);
    }

    table.setValueAt("Timed out",row,0);
    row++;
    table.clearRow(row);
    for (int i=0;i<man.dodgy.size();i++) {
      showdata(table,row,man.dodgy,i);
      row++;
      table.clearRow(row);
    }

    if (JMaths.rnd(1,5)==1) {
      table.repaint();
    }

  }

  public void showdata(SimpleTable table,int row,List l,int i) {
      Trawlet t=(Trawlet)l.get(i);
      table.setValueAt(t.url,row,0);
      table.setValueAt(t.stage,row,1);
      table.setValueAt("("+t.depth+")",row,2);
      String cont;
      if (t.content==null)
        cont="...";
      else {
        if (t.contentLength==-1)
          cont=""+t.cnt;
        else
          cont=(int)(100.0*(float)t.cnt/(float)t.contentLength)+"%";
      }
      table.setValueAt(cont,row,3);
      table.setValueAt(t.conobj,row,4);
  }

  String list(List l) {
    String s="";
    for (int i=0;i<l.size();i++)
      s+=l.get(i)+"\n";
    return s;
  }

}