package tools.parser;

import java.io.PrintStream;
import java.util.Vector;

import org.neuralyte.Logger;

import jlib.JLib;
import jlib.strings.SomeString;


public class ArgReplacement implements Type {

	private int argNum;

	public ArgReplacement(int argNum) {
		this.argNum = argNum - 1;
	}
	
	public boolean replacementfor(Type o) {
		return o instanceof ArgReplacement;
	}

	// @Override
   public Match match(SomeString s, ParseContext ctx) {
	   JLib.error("ActiveReplacement.match(): Not supposed to try to match, should be for replacements only");
	   return null;
   }

   public void renderMatchAs(Match match, String target, PrintStream out) {
		// Logger.debug("Doing matches.get("+argNum+") inside "+match.type+" with size "+match.matches.size());
		match.matches.get(argNum).render(match,target,out);
   }

}
