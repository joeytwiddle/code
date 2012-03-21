package tools.parser;

/*
 * This source code is freely distributable under the GNU public licence. I
 * would be delighted to hear if have made use of this code. If you make money
 * with this code, please give me some! If you find this code useful, or have
 * any queries, please feel free to contact me: pclark@cs.bris.ac.uk /
 * joey@neuralyte.org Paul "Joey" Clark, hacking for humanity, Feb 99
 * www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk
 */

// import java.lang.*;
import java.util.*;

import java.io.PrintStream; // import java.io.PrintWriter;

/*
 * import java.awt.TextArea; // debug import java.awt.Frame; // debug import
 * java.awt.FlowLayout; // debug import java.awt.event.ActionListener; // debug
 * import java.awt.event.ActionEvent; // debug import java.awt.Button; // debug
 */

// import jlib.Files;
// import jlib.JString;
import jlib.strings.*;

public class Match {

	Type type;
	SomeString string;
	Vector<Match> matches; // sub-matches making up this one, if any (can be null).
	Match parent; // parent match of which this is a sub-match
	SomeString left = null; // new RealString("error: left not initialised!");

	/*
	
	// Other parts of the code assume left to exist, so hiding these constructors where it doesn't!

	Match(Type t, SomeString s) {
		type = t;
		string = s;
		matches = null; // new Vector();
	}

	Match(Type t, SomeString s, Vector<Match> ms) {
		type = t;
		string = s;
		matches = ms;
	}
	*/

	Match(Type t, SomeString s, SomeString ll) {
		type = t;
		string = s;
		matches = null;
		left = ll;
	}

	Match(Type t, SomeString s, Vector<Match> ms, SomeString ll) {
		type = t;
		string = s;
		matches = ms;
		left = ll;
	}

	public String toString() { // temporary (can be removed)
		return "Match: " + type + " = \"" + Atom.strip(string) + "\"";
		// FakeOutputStream out=new FakeOutputStream();
		// render(new PrintStream(out),"");
		// return out.store.toString();
		// return "Match.toString(): gone to render(PrintStream,int)";
	}

	// @todo Rename this: printParseTree
	public void render(PrintStream out, String ind) {
		// System.out.println("Generating Match "+type+" with "+(matches==null?"no match":""+matches.size()));
		out.print(ind + type + " = " + "\"" + Atom.strip("" + string) + "\"");
		if (matches != null) {
			out.print(" with: [");
			for (int i = 0; i < matches.size(); i++) {
				out.print("\n" + ind);
				matches.get(i).render(out, ind + " ");
			}
			// System.out.println("a");
			// tmp=JString.replace(tmp,"\n","\n  ");
			// System.out.println("b");
			String weirdind = (ind + ind);
			if (weirdind.length() > 0) weirdind = weirdind.substring(1);
			out.print("\n" + weirdind + "] leaving \"" + Atom.strip("" + left)
			      + "\"");
		}
	}

	public Vector<Type> rulefrommatch() {
		Vector<Type> v = new Vector<Type>();
		for (int i = 0; i < matches.size(); i++)
			v.add(matches.get(i).type);
		return v;
	}

	/** render needs to learn to deal with RelElements. **/
	public void render(Match btwyourdadis, String target, PrintStream out) {
		if (parent == null) parent = btwyourdadis;
		Atom a = (Atom) type;
		ParseContext ctx = new ParseContext();   /** @todo Should be passed in! **/
		RuleSet ruleset = Grammar.getrulesetforatom(a.type, ctx);
		Object tmp = ruleset.replacements.get(target);
		// if (tmp!=null || target.length()==0)
		// if (tmp==null)
		// return "";
		Vector rs = (tmp == null ? rulefrommatch() : (Vector) tmp);
		// out.print("    // Rendering "+Atom.strip(""+this)+" against "+rs+"\n";
		// out.print("    // Rendering "+a.type+": "+rs+"\n";
		Vector<Match> unusedmatches = (Vector<Match>) matches.clone();
		for (int i = 0; i < rs.size(); i++) {
			Type t = (Type) rs.get(i);
			renderIn(unusedmatches, t, target, out);
		}
	}

	public String renderString(String target) {
		FakeOutputStream out = new FakeOutputStream();
		render(null, target, new PrintStream(out));
		return out.store.toString();
	}

	public void renderIn(Vector<Match> unusedmatches, Type t, String target,
	      PrintStream out) {
		
		// TODO: Do we need all these instanceof checks?  Couldn't we instead
		// create a common interface for each Type to implement, e.g.
		// Type.renderIn(...) ?

		// TODO: I suspect we may want to do some error reporting somewhere here,
		// if the desired replacement is not found.
		
		if (t instanceof Text) {
			out.print(((Text) t).rendertext());
		} else if (t instanceof RelElement) {
			((RelElement) t).render(this, target, out);
		} else if (t instanceof ActiveReplacement) {
			((ActiveReplacement) t).render(unusedmatches, this, target, out);
		} else {
			for (int j = 0; j < unusedmatches.size(); j++) {
				// for (int j=0;j<matches.size();j++) {
				// Match m=(Match)matches.get(j);
				Match m = unusedmatches.get(j);
				Type couldbe = (Type) m.type;
				if (t.replacementfor(couldbe)) {
					if (t instanceof Atom) {
						m.render(this, target, out);
						// Remove the match, so that subsequent Atoms of same type get
						// the subsequent matches
						// matches.remove(m);
					} else if ((t instanceof Var)) { // *** needed if you don't want
						// all vars printing! &&
						// (tmp!=null ||
						// target.length()==0)) {
						out.print(Parser.decode("" + m.string));
						// We don't expect two vars the same!
					}
					unusedmatches.remove(j);
					break;
				}
			}
		}
		
		// TODO: If we have an Atom, we may want to descend into ()s []s *s and
		// +s, in case the replacement refers to atoms which appear to the in the
		// "flat" definition, but are actually hidden inside these types.
		
		// out.print("\n<Match.renderIn failed "+t+" on \n"+this+">");
	}

	/**
	 * At the moment only Atom searching is possible. Variable searching could be
	 * implemented, or you could make the variable into an atom in the grammar.
	 **/

	public Match getUp(String called) {
		if (type instanceof Atom)
		   if (((Atom) type).type.equals(called)) return this;
		if (parent == null) return null;
		return parent.getUp(called);
	}

	public Match getDown(String called) {
		// System.err.println("On "+this);
		if (type instanceof Atom)
		   if (((Atom) type).type.equals(called)) return this;
		if (matches != null) {
			for (int i = 0; i < matches.size(); i++) {
				Match m = matches.get(i);
				// System.err.println(i+"/"+matches.size()+" Match.getDown(): looking at "+m);
				Match res = m.getDown(called);
				// System.err.println("Got "+res);
				if (res != null) return res;
			}
		}
		return null;
	}

}
