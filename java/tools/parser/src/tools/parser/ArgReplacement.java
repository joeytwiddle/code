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


	/* Only relevant in MagicType
	public boolean replacementfor(Type o) {
		return o instanceof ArgReplacement;
	}
	*/

	// @Override
	public Match match(SomeString s, ParseContext ctx) {
		JLib.error("ActiveReplacement.match(): Not supposed to try to match, should be for replacements only");
		return null;
	}


	public Match getReferredMatch(OutputContext ctx, Match match) {
		if (argNum >= match.matches.size()) {
			if (ctx.allowOutOfRangeArguments) {
				return null;
			} else {
				throw new Error("Replacement tried to use $" + (argNum + 1)
				        + " but there were only " + match.matches.size()
				        + " matches. " + ctx + " " + match);
			}
		}
		return match.matches.get(argNum);
	}


	public void renderMatchAs(OutputContext ctx, Match parentMatch, String target,
	        PrintStream out) {
		// Logger.debug("Doing matches.get("+argNum+") inside "+match.type+" with size "+match.matches.size());
		Match m = getReferredMatch(ctx, parentMatch);
		if (m != null) {
			m.render(ctx, parentMatch, target, out);
		}
	}

}
