package jlib.multiui;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import jlib.*;
import nuju.*;

public class Form extends Container {

	public String action="Submit";

	public void receive(Map params) {
	
	}

	public String renderHTML() {
		String s="";
		s+="<form blah blah blah>\n";
		for (int i=0;i<comps.size();i++)
			s+=((Component)comps.get(i)).renderHTML();
		s+="<input type=\"submit\" value=\""+action+"\">\n";
		s+="</form>\n";
		return s;
	}

}
