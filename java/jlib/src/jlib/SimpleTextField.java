package jlib;

import java.lang.String;
import java.awt.*;
import java.awt.event.*;

public class SimpleTextField extends TextField implements ActionListener {

  public SimpleTextField(String inittext,int columns) {
    super(inittext,columns);
    // setColumns(columns);
    addActionListener(this);
  }

  public void actionPerformed(ActionEvent e) {
    System.out.println(""+e);
    String s=e.getActionCommand();
    onEnter(s);
  }

  public void onEnter(String s) {
  }

}