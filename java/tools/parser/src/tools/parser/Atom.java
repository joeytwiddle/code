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

import java.io.PrintStream;
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

public class Atom implements MagicType {

	public static int depth = 0;
	public static Vector path = new Vector();
	
	String type;


	/* The following gap is a feature!

	   If all the line numbers are above 100, they align nicely in the log. :P

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

	public Match match(SomeString s, ParseContext ctx) {
		
		// This is bad, it does not scale, because our grammars often use
		// concatenation. We would need to implement tail-recursion, or make more
		// horizontal grammars, e.g. introduce ( ... | ... | ... )*, to get an
		// accurate stack depth reading.
		// It is better to wait for the stack-trace, and use -debug for logging.
		/* if (depth > 499) {
			Logger.error("Breaking out because depth="+depth);
			return null;
		} */
		
		// The ruleset could be locally cached, or even loaded in a post-load
		// stage, but what it can't be is loaded on instantiation, because the
		// grammar is not complete at that time.
		RuleSet rs = Grammar.getrulesetforatom(type, ctx);

		if (rs == null) {
			throw new Error("Failed to find ruleset for atom type \""+type+"\"");
		}
		
		return matchAgainstRuleset(type, this, rs, s, ctx);
	}

	public static Match matchAgainstRuleset(String type, Type realType, RuleSet rs, SomeString s, ParseContext ctx) {
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
			Vector<Match> ms = new Vector<Match>();
			SomeString rest = s;
			boolean failure = false;
			depth++;
			path.add(type);
			for (int j = 0; j < rules.size() && !failure; j++) {
//				Profile.start("Atom.match: Inside loop");
				Type t = (Type) rules.get(j);
//				Profile.start("Atom.match: inner inner");
				// Profile.start(t.getClass().getName()+".match()"); // heavy
				if (Parser.DebugPath) {
					ctx.path.add(t);
					Parser.dbdta.setText( "" + ctx.path /*+ "\n" + ctx.closestFailure*/ );
				}
				// @todo Perhaps we should ask ctx.parser to do the match here.
				// By going through one central function for all match attempts, it
				// would be easier to implement match caching.
				Match m = t.match(rest, ctx);
				if (Parser.DebugPath) {
					ctx.path.remove(ctx.path.size() - 1);
				}
				// Profile.stop(t.getClass().getName()+".match()"); // heavy
//				Profile.stop("Atom.match: inner inner");
				if (m == null) {
					failure = true;
					if (Parser.Debugging) {
						String lastFailure = "Failed to match "+j+" of " + rules + " against: "
						      + headSome(s) + "..";
						Logger.log(indent()+" "+lastFailure);
					}
					int charsRemaining = rest.length();
					if (charsRemaining < ctx.closestToEnd) {
						ctx.closestToEnd = charsRemaining;
						ctx.closestFailure = "Failed to match "+t+", argument "+(j+1)+" of "+type+" against "+headSome(s)+" to form "+ctx.path;
					}
				} else {
					ms.add(m);
					// System.out.println("  Original: "+strip(left));
					rest = m.left;
					// System.out.println("       New: "+strip(left));
				}
//				Profile.stop("Atom.match: Inside loop");
				if (Parser.DebuggingWin) {
					Parser.doing(Parser.all.length() - s.length(), Parser.all.length() - rest.length());
					// try { Thread.sleep(2); } catch (Exception e) { }
				}
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
	   // @todo See also Atom.strip() - these two should probably be merged
	   String head = StringUtils.escapeSpecialChars(s.substring(0, lastchar));
	   return head;
   }

	public void renderMatchAs(Match parentMatch, String target, PrintStream out) {
		Match m = parentMatch.grabUnusedMatchMatching(this);
		if (m == null) {
			// Logger.error("Could not find match for "+this+" in "+parentMatch);
			// This is pretty much allowed. Grammar authors can put a lot of atoms
			// in a replacement rule, since the ruleset may have collapsed in a
			// number of different ways; only those which were present in the final
			// match will be renderered.
			return;
		}
		m.render(parentMatch, target, out);
   }

}
