package jlib.multiui;
import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import jlib.*;


public class SwitchButton extends Button {

  public String text;
	public String[] options;

	public SwitchButton(String[] ss) {
		options=ss;
		text=options[0];
	}

	public void action() {
		int i=getIndex();
	}

	public int getIndex() {
		return JArray.find(options,text);
	}

}
