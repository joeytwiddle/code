package tools.parser;

import java.io.PrintStream;
import java.util.Vector;

import org.neuralyte.Logger;

import jlib.strings.SomeString;


public class GroupedDefn implements Type {

	private RuleSet ruleset;

   public GroupedDefn(RuleSet ruleset) {
   	this.ruleset = ruleset;
   }
   
	/** @deprecated **/
	public GroupedDefn(Vector<Type> justOneRule) {
		this.ruleset = new RuleSet("AnonymousRuleset");
		ruleset.rules.add(justOneRule);
   }

   /*
	public GroupedDefn(Vector<Vector<Type>> rules) {
		this.ruleset = new RuleSet("AnonymousRuleset");
		ruleset.rules = rules;
   }
   */

	// @Override
   public Match match(SomeString s, ParseContext ctx) {
		String type = "AnonymousType";
	   return Atom.matchAgainstRuleset(type, this, ruleset, s, ctx);
   }


	/* Only relevant in MagicType
   // @Override
   public boolean replacementfor(Type o) {
	   // TODO Auto-generated method stub
	   return o instanceof GroupedDefn;
   }
   */

	// @Override
   public String toString() {
	   return "(" + ruleset + ")";
   }

	public void renderMatchAs(OutputContext ctx, Match parentMatch, String target, PrintStream out) {
		Logger.error("Never expected to enter this method!");
   }

}
