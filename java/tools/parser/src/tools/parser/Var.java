package tools.parser;

/*
 * This source code is freely distributable under the GNU public licence. I
 * would be delighted to hear if have made use of this code. If you make money
 * with this code, please give me some! If you find this code useful, or have
 * any queries, please feel free to contact me: pclark@cs.bris.ac.uk /
 * joey@neuralyte.org Paul "Joey" Clark, hacking for humanity, Feb 99
 * www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk
 */

import java.lang.*;
import java.util.*;

import java.awt.TextArea; // debug
import java.awt.Frame; // debug
import java.awt.FlowLayout; // debug
import java.awt.event.ActionListener; // debug
import java.awt.event.ActionEvent; // debug
import java.awt.Button; // debug

import org.neuralyte.Logger;

import jlib.Files;
import jlib.JString;
import jlib.strings.*;

import tools.parser.*;

public class Var implements Type {

	String name;
	String deny; // characters which are not allowed
	String accept;

	Var(String n) {
		name = n;
		deny = "";
	}

	Var(String n, String d) {
		name = n;
		deny = d;
	}

	Var(String n, String _, String a) {
		name = n;
		accept = a;
	}

	public Match match(SomeString s, ParseContext ctx) {
		int most = s.length();
		if (accept != null) {
			for (int i=0;i<s.length();i++) {
				int found = accept.indexOf(s.charAt(i));
				if (found == -1) {
					most = i;
					break;
				}
			}
		}
		else if (deny != null) {
			/* This method does not scale very well, if some of the deny chars appear far into s or not at all!
			for (int i = 0; i < deny.length(); i++) {
				int j = s.indexOf(deny.charAt(i));
				// System.out.println("Found '"+Atom.strip(""+deny.charAt(i))+"' at "+j+" of "+Atom.strip(""+s));
				if (j >= 0 && j < most) most = j;
			}
			*/
			for (int i=0;i<s.length();i++) {
				int found = deny.indexOf(s.charAt(i));
				if (found >= 0) {
					most = i;
					break;
				}
			}
		}
		if (most == 0) {
			// Failure!
			if (Parser.Debugging) {
				// Logger.log("Failed to match var "+name+" with accept="+accept+" and deny="+deny);
			}
			
			// Originally exclude vars would be valid even if they accepted 0 chars!
			// Disabling that behaviour did cause some old rules to break, e.g. in
			//   Comment = "//" <comment/"\n"> "\n"
			// a 0-length var could be valid.
			// Such rules may now need to become:
			//   Comment = "//" <comment/"\n"> "\n" | "//\n"
			
			// if (accept != null && deny == null) {
				return null;
			// }
				
		}
		return new Match(this, s.subString(0, most), s.subString(most));
	}

	public String toString() {
		return "<" + name + ">";
	}

	public boolean replacementfor(Type o) {
		if (o instanceof Var) {
			Var v = (Var) o;
			if (name.equals(v.name)) return true;
		}
		return false;
	}
}
