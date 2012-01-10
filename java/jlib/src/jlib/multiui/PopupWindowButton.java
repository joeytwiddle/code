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


public class PopupWindowButton extends Button {

  private Page destinationPage;

  public PopupWindowButton(String t) {
    text=t;
		destinationPage=null;
  }

  public PopupWindowButton(String t,Page d) {
    text=t;
		destinationPage=d;
  }

  // public PopupWindowButton(Page d,String t) {
		// this(t,d);
  // }

	public Page destination() {
		return destinationPage;
	}

	public void action() {
		JLib.report(this,"Haven't decided how 2 do popup windows yet");
		appServer.moveToPage(destination());
	}

}
