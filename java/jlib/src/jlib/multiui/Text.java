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


public class Text extends Component {

  public String text;
	// Yes this (and everything else!) should be kept public:
	// It might be refered to by the app after it has been written.

  public Text(String t) {
    text=t;
  }

	public String renderHTML() {
		return JString.toHTML(text);
	}

}
