package tools.parser;

/*
 * This source code is freely distributable under the GNU public licence. I
 * would be delighted to hear if have made use of this code. If you make money
 * with this code, please give me some! If you find this code useful, or have
 * any queries, please feel free to contact me: pclark@cs.bris.ac.uk /
 * joey@neuralyte.org Paul "Joey" Clark, hacking for humanity, Feb 99
 * www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk
 */

import java.io.PrintStream;
import java.lang.*;
import java.util.*;

import java.awt.TextArea; // debug
import java.awt.Frame; // debug
import java.awt.FlowLayout; // debug
import java.awt.event.ActionListener; // debug
import java.awt.event.ActionEvent; // debug
import java.awt.Button; // debug

import jlib.Files;
import jlib.JString;
import jlib.strings.*;

import tools.parser.*;

public class Text implements Type {

	String text;

	Text(String t) {
		text = t;
	}

	public Match match(SomeString s, ParseContext ctx) {
		// System.out.println("\""+text+"\"==\""+s+"\"? "+text.equals(s));
		if (s.startsWith(text)) { return new Match(this, s.subString(0, text
		      .length()), s.subString(text.length())); }
		return null;
	}

	public String toString() {
		return "\"" + Atom.strip(text) + "\"";
	}

	/* Only relevant in MagicType
	public boolean replacementfor(Type o) {
		System.out.println("Shouldn't be running Text.replacementfor()!");
		System.exit(1);
		return false;
	}
	*/

	public String rendertext() {
		// if (text.startsWith("\""))
		// text=text.substring(1);
		// if (text.endsWith("\""))
		// text=text.substring(0,text.length()-1);
		text = JString.replace(text, "!qt!", "\"");
		text = JString.replace(text, "\\n", "\n");
		return text;
	}

	public void renderMatchAs(OutputContext ctx, Match match, String target, PrintStream out) {
		out.print(rendertext());
	}
	
}
