package tools.parser;

import java.util.Vector;

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

	@Override
	public Match match(SomeString strIn) {
		Vector<Match> matches = new Vector<Match>();
		SomeString togo = strIn;
		int charsMatched = 0;
		while (true) {
			Match m = type.match(togo);
			if (m == null) {
				break;
			} else {
				togo = m.left;
				matches.add(m);
				charsMatched += m.string.length();
				if (maxMatches > -1 && matches.size() >= maxMatches) {
					break;
				}
			}
		}
		if (matches.size() < minMatches) {
			return null;
		}
		SomeString matchedStr = strIn.subString(0, charsMatched);
		return new Match(type, matchedStr, matches, togo);
	}

	@Override
	public boolean replacementfor(Type o) {
		// TODO Auto-generated method stub
		return false;
	}

}
