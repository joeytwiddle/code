package jlib.multiui.convenience;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import jlib.*;



public class DeleteButton extends Button {

	public Vector vector;

  public DeleteButton(String t,Vector v,Object o) {
		text=t;
    carry=o;
		vector=v;
  }

	public void action() {
		vector.remove(carry);
	}

}
