package jlib.multiui;

import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import jlib.*;
import nuju.*;

public class Container extends Component {

  public List comps;

  public Container() {
    comps=new Vector();
  }

  public void add(Component c) {
    comps.add(c);
  }

	public String toString() {
		return getClass().getName()+" containing ["+comps+"]";
	}

}
