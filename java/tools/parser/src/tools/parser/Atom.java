package tools.parser;

/*
 * This source code is freely distributable under the GNU public licence. I
 * would be delighted to hear if have made use of this code. If you make money
 * with this code, please give me some! If you find this code useful, or have
 * any queries, please feel free to contact me: pclark@cs.bris.ac.uk /
 * joey@neuralyte.org Paul "Joey" Clark, hacking for humanity, Feb 99
 * www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk
 */

import jlib.Profile;

import java.lang.*;
import java.util.*;

import java.awt.TextArea; // debug
import java.awt.Frame; // debug
import java.awt.FlowLayout; // debug
import java.awt.event.ActionListener; // debug
import java.awt.event.ActionEvent; // debug
import java.awt.Button; // debug

import org.neuralyte.Logger;
import org.neuralyte.common.ArrayUtils;
import org.neuralyte.common.StringUtils;

import jlib.Files;
import jlib.JString;
import jlib.strings.*;

import tools.parser.*;

public class Atom implements Type {

	public static int depth = 0;
	public static Vector path = new Vector();
	
	String type;

	/* Feature!  If all the line numbers are above 100, they align nicely in the log. :P
	
	    Fold this comment block away in your editor, or delete it if you don't need this feature.
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	*/
	
	public Atom(String t) {
		type = t;
	}

	public static String strip(SomeString s) {
		return strip(s.toString());
	}

	public static String strip(String s) {
		int max = 20;
		if (s.length() > max)
		   s = JString.left(s, max) + "..." + (s.length() - max);
		s = JString.replace(s, "\n", "\\n");
		return s;
	}

	public Match match(SomeString s) {
		
		// This is bad, it does not scale, because our grammars often use
		// concatenation. We would need to implement tail-recursion, or make more
		// horizontal grammars, e.g. introduce ( ... | ... | ... )*, to get an
		// accurate stack depth reading.
		// It is better to wait for the stack-trace, and use -debug for logging.
		/* if (depth > 499) {
			Logger.error("Breaking out because depth="+depth);
			return null;
		} */
		
		RuleSet rs = Grammar.getrulesetforatom(type);
		
		return matchAgainstRuleset(type, this, rs, s);
	}

	public static Match matchAgainstRuleset(String type, Type realType, RuleSet rs, SomeString s) {
		String profileName = "Atom.matchAgainstRuleset()"; 
		Profile.start(profileName);
	   if (Parser.Debugging) {
			Logger.log(indent()+" Entered atom " + type + " with " + rs.rules.size()
			      + " rules...");
		}
		for (int i = 0; i < rs.rules.size(); i++) {
//			Profile.start("Atom.match: Outside loop");
			Vector rules = (Vector) rs.rules.get(i);
			if (rules.size() == 0) {
				System.err.println("rulesetforatom(" + type + ") number " + i
				      + " is empty!");
				System.exit(1);
			}

			if (Parser.Debugging) {
				// This is often not enough info when a match is failing. The
				// grammar writer is more interested in the stack of trial matches
				// we recently tried.
				// Also it is costly to construct, and not usually needed, only
				// during failed parses.
				String head = headSome(s); 
				Parser.lastMatchAttempt = "Trying to match " + rules + " against: "
				      + head
				      + "..";
				Logger.log(indent()+" "+Parser.lastMatchAttempt);
			}
			Vector ms = new Vector();
			SomeString rest = s;
			boolean failure = false;
			depth++;
			path.add(type);
			for (int j = 0; j < rules.size() && !failure; j++) {
//				Profile.start("Atom.match: Inside loop");
				Type t = (Type) rules.get(j);
//				Profile.start("Atom.match: inner inner");
				// Profile.start(t.getClass().getName()+".match()"); // heavy
				Match m = t.match(rest);
				// Profile.stop(t.getClass().getName()+".match()"); // heavy
//				Profile.stop("Atom.match: inner inner");
				if (m == null) {
					failure = true;
					if (Parser.Debugging) {
						String lastFailure = "Failed to match "+j+" of " + rules + " against: "
						      + headSome(s) + "..";
						Logger.log(indent()+" "+lastFailure);
					}
				} else {
					ms.add(m);
					// System.out.println("  Original: "+strip(left));
					rest = m.left;
					// System.out.println("       New: "+strip(left));
				}
//				Profile.stop("Atom.match: Inside loop");
			}
			depth--;
			path.remove(path.size()-1);
			if (!failure) {
				SomeString matchedString = s.subString(0, s.length() - rest.length());
				if (Parser.Debugging) {
					Logger.log(indent()+" Succeeded: " + rules +" swallows: "+headSome(matchedString)+" ["+matchedString.length()+" chars]");
				}
				Match m = new Match(realType, matchedString, ms, rest);
//				Profile.stop("Atom.match: Outside loop");
//				Profile.stop("Atom.match: Elsewhere");
				return m;
			} else {
			}
//			Profile.stop("Atom.match: Outside loop");
		}
		Profile.stop(profileName);
		return null;
   }

	public String toString() {
		return type;
	}

	public boolean replacementfor(Type o) {
		if (o instanceof Atom) {
			Atom a = (Atom) o;
			if (type.equals(a.type)) return true;
		}
		return false;
	}
	
	public static String indent() {
		// return JString.repeat(">", depth);
		// return path.toString();
		return JString.repeat(">", depth) + (path.size()>0 ? path.lastElement() : "");
	}

	public static String headSome(SomeString s) {
	   int lastchar = s.length() < 40 ? s.length() : 40;
	   String head = StringUtils.escapeSpecialChars(s.substring(0, lastchar));
	   return head;
   }

}
