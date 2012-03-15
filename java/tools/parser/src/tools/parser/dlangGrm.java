package tools.parser;

import java.lang.String;
import java.util.Vector;

import tools.parser.*;

public class dlangGrm {
  public static void setupgrammar() {
    Vector rulesets=Grammar.rulesets;
    RuleSet ruleset;
    Vector rule;

    // DLang - A dumb language

    ruleset=new RuleSet("Main");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("DLangFile"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("DLangFile");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("DLangFileSome"));
      ruleset.add(rule);
    // Replacements

    // DLangFileSome = 1 or more DLangFileBits
    // Should it be 0 or more?

    ruleset=new RuleSet("DLangFileSome");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("DLangFileBit"));
        rule.add(new Atom("DLangFile"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("DLangFileBit"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("DLangFileBit");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("ClassDefinition"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("FunctionDefinition"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Statement"));
      ruleset.add(rule);
    // Replacements

    // WS = Optional WhiteSpace
    // RWS = Required WhiteSpace?

    // TODO: Parametrised classes (templates)
    ruleset=new RuleSet("ClassDefinition");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("class"));
        rule.add(new Atom("WS"));
        rule.add(new Var("className"," \n"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("NL"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("ClassBody"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("ClassBody");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("IndentedDLangFileSome"));
      ruleset.add(rule);
    // Replacements

    // TODO
    ruleset=new RuleSet("IndentedDLangFileBit");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("DLangFileSome"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("FunctionDefinition");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Var("fnname"," ("));
        rule.add(new Text("("));
        rule.add(new Atom("Args"));
        rule.add(new Text(")"));
        rule.add(new Atom("WS"));
        rule.add(new Text("="));
        rule.add(new Atom("WS"));
        rule.add(new Atom("FunctionBody"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("Args");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("Arg"));
        rule.add(new Atom("MoreArgs"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Arg"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("WS"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("Arg");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("WS"));
        rule.add(new Atom("VarName"));
        rule.add(new Atom("WS"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("MoreArgs");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text(","));
        rule.add(new Atom("Args"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("VarReference");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("<"));
        rule.add(new Var("varname","<>\n\"/ "));
        rule.add(new Text(">"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("Statement");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("Assignment"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("Assignment");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("NormalAssignment"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("SpecialAssignment"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("NormalAssignment");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("VarReference"));
        rule.add(new Atom("WS"));
        rule.add(new Text("="));
        rule.add(new Atom("Expression"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("SpecialAssignment");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("VarReference"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("SpecialOperator"));
        rule.add(new Atom("Expression"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("SpecialOperator");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("+="));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("-="));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("*="));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("/="));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("Expression");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("ExpressionBit"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("ExpressionBit");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("VarReference"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("BracketedExpression"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Algebra"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("BracketedExpression");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("("));
        rule.add(new Atom("WS"));
        rule.add(new Atom("ExpressionBit"));
        rule.add(new Atom("WS"));
        rule.add(new Text(")"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("Algebra");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("ExpressionBit"));
        rule.add(new Atom("Operator"));
        rule.add(new Atom("ExpressionBit"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("Operator");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("AlgebraicOperator"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("BooleanOperator"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OtherOperator"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("AlgebraicOperator");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("*"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("/"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("+"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("-"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("BooleanOperator");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("=="));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("!="));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(">"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("<"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(">="));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("<="));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("OtherOperator");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("<"));
        rule.add(new Var("varname"," \n\t,."));
        rule.add(new Text(">"));
      ruleset.add(rule);
    // Replacements



    // HTF will I deal with indentation?



    ruleset=new RuleSet("NL");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("WS");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("Whitespace"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("RWS");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("Whitespace"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("Whitespace");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("WhitespaceBit"));
        rule.add(new Atom("Whitespace"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("WhitespaceBit"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("WhitespaceBit");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("\n"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(" "));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("\t"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("\r"));
      ruleset.add(rule);
    // Replacements

  }
}
