package tools.parser;

import java.util.Vector;

import org.neuralyte.Logger;

import jlib.strings.SomeString;

public class RepeatedRule implements Type {

	Type type;
	int minMatches;
	int maxMatches;  // -1 means infinite

	/**
	 * @param type The rule that may be repeated.
	 * @param ruleStr The type of rule as a symbol, "*" or "+".
	 */
	public RepeatedRule(Type type, String ruleStr) {
		super();
		this.type = type;
		if (ruleStr.equals("*")) {
			this.minMatches = 0;
			this.maxMatches = -1;
		} else if (ruleStr.equals("+")) {
			this.minMatches = 1;
			this.maxMatches = -1;
		} else {
			throw new Error("Incorrect ruleStr argument to RepeatedRule: '"+ruleStr+"' should be '*' or '+'.");
		}
	}
	
	/**
	 * @param type The rule that may be repeated
	 * @param min The minimum number of times it must appear for a match.
	 * @param max The maximum number of times it may be parsed before the match is complete.
	 */
	public RepeatedRule(Type type, int min, int max) {
		this.type = type;
		minMatches = min;
		maxMatches = max;
	}

	// @Override
	public Match match(SomeString strIn, ParseContext ctx) {
		Vector<Match> matches = new Vector<Match>();
		SomeString togo = strIn;
		int charsMatched = 0;
		while (true) {
			Match m = type.match(togo, ctx);
			if (m == null) {
				break;
			} else {
				togo = m.left;
				matches.add(m);
				charsMatched += m.string.length();
				if (maxMatches > -1 && matches.size() >= maxMatches) {
					break;
				}
				// @todo If 0 chars were matched, we probably don't want to repeat!
				// It means the grammar author has foolishly put a 0-length match
				// inside a * rule!
				// Otoh it might be legal, e.g. if it's a 0 to 1 match, it will happily break out with "" above.
				// We only really need to worry if maxMatches == -1.
				if (m.string.length() == 0 && maxMatches == -1) {
					Logger.warn("Zero-length match made in unbounded RepeatedRule!  (Contents = "+type+".)  Breaking out of loop.");
					break;
				}
			}
		}
		if (matches.size() < minMatches) {
			return null;
		}
		SomeString matchedStr = strIn.subString(0, charsMatched);
		// @todo I want to replace type with this, but then grm.grm fails to build!
		Match repeatedMatch = new Match(type, matchedStr, matches, togo);
		// Logger.debug("Created repeat match "+repeatedMatch+" length "+matches.size()+" with type "+type+" with this="+this);
		return repeatedMatch;
	}

	// @Override
	public boolean replacementfor(Type o) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
   public String toString() {
	   return "[ " + type + " ]"; 
   }
	
	

}
