package tools.parser;

/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk */

import java.lang.*;
import java.io.*;
import java.util.*;

import java.awt.TextArea; // debug
import java.awt.Frame; // debug
import java.awt.FlowLayout; // debug
import java.awt.event.ActionListener; // debug
import java.awt.event.ActionEvent; // debug
import java.awt.Button; // debug

import jlib.JLib;
import jlib.Files;
import jlib.JString;
import jlib.strings.*;

import tools.parser.*;

public class ActiveReplacement implements Type {

	Match match;
	String target;
	List unusedmatches;

	public String replace() {
		return "Must be overwritten by grammar!";
	}

	public Match match(SomeString s) {
   JLib.error("ActiveReplacement.match(): Not supposed to try to match, should be for replacements only");
	 return null;
	}

	public boolean replacementfor(Type o) {
		return true;
	}

	public void render(List um,Match btwyourdadis,String t,PrintStream out) {
		try {
		unusedmatches=um;
		match=btwyourdadis;
		target=t;
		out.print(replace());
		} catch (Exception e) {
			// System.err.println("( ActiveReplacement: "+this+": "+e+" )");
/*			e.printStackTrace(); */
		}
	}
	
}
