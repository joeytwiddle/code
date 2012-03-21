package tools.parser;

/* This source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joey@neuralyte.org
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

// implementing ReplacementType requires redefinition of the rule builders in
// all existing grammars!
// (Replacement rules will differ from the rules found in matcher rulesets.)

public class ActiveReplacement implements /*Replacement*/Type {

	Match match;
	String target;
	List unusedmatches;

	public String replace() {
		return "Must be overwritten by grammar!";
	}

	public boolean replacementfor(Type o) {
		return true;
	}

	@Override
   public Match match(SomeString s, ParseContext ctx) {
	   JLib.error("ActiveReplacement.match(): Not supposed to try to match, should be for replacements only");
	   return null;
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
