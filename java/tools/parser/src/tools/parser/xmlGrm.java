package tools.parser;

import java.lang.String;
import java.util.Vector;


public class xmlGrm {
  public static void setupgrammar() {
    Vector rulesets=Grammar.rulesets;
    RuleSet ruleset;
    Vector rule;

    ruleset=new RuleSet("XML");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("<"));
        rule.add(new Var("type"));
        rule.add(new Text(">"));
        rule.add(new Atom("Inside"));
        rule.add(new Text("</"));
        rule.add(new Var("type"));
        rule.add(new Text(">"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("<"));
        rule.add(new Var("type"));
        rule.add(new Atom("Param"));
        rule.add(new Text(">"));
        rule.add(new Atom("Inside"));
        rule.add(new Text("</"));
        rule.add(new Var("type"));
        rule.add(new Text(">"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("<"));
        rule.add(new Var("type"));
        rule.add(new Text("/>"));
      ruleset.add(rule);
    // Replacements



    ruleset=new RuleSet("Param");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text(" "));
        rule.add(new Var("param"));
        rule.add(new Text("="));
        rule.add(new Var("value"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Param"));
        rule.add(new Atom("Param"));
      ruleset.add(rule);
    // Replacements



  }
}
