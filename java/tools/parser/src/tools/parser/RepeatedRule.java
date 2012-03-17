package tools.parser;

import java.util.Vector;

import jlib.strings.SomeString;

public class RepeatedRule implements Type {

	Type type;
	int minMatches;

	/**
	 * @param type
	 * @param rule
	 */
	public RepeatedRule(Type type, String ruleStr) {
		super();
		this.type = type;
		if (ruleStr.equals("*")) {
			this.minMatches = 0;
		} else if (ruleStr.equals("+")) {
			this.minMatches = 1;
		} else {
			throw new Error("Incorrect ruleStr argument to RepeatedRule: '"+ruleStr+"' should be '*' or '+'.");
		}
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
