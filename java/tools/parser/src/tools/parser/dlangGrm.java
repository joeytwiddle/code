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
    //
    // Export targets:
    //
    // dintj - runtime structures suitable for interpreting by Java dlang.interpreter.*

    // TODO:
    //   Array dereference (aka List shortcuts)
    //   Member dereference (fields and methods of objects).  Similar for statics...
    //   .. namespaces
    //   Constructor definition and invocation
    //   Templating (generics)
    //   Anonymous implementations?  Or first order functions...
    //   Shortcuts for hashmaps
    //   Loops

    // @OPTION rewrite_meaningful_indentation "{" "}"

    ruleset=new RuleSet("Main");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("DLangFile"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("DLangFile");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("DLangImports"));
        rule.add(new Atom("DLangFileSome"));
      ruleset.add(rule);
    // Replacements

    // DLangImports = DLangImport [ DLangImports ]

    ruleset=new RuleSet("DLangImports");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("DLangImport"));
        rule.add(new Atom("DLangImports"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("DLangImport"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("DLangImport");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("DLangImportProper"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Comment"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Space"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("DLangImportProper");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("import"));
        rule.add(new Atom("Space"));
        rule.add(new Var("import","\n"));
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements

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

    // Should it be 0 or more?

    ruleset=new RuleSet("DLangFileBit");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("Comment"));
      ruleset.add(rule);
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

    ruleset=new RuleSet("Comment");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("#"));
        rule.add(new Var("comment","\n"));
        rule.add(new Text("\n"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("//"));
        rule.add(new Var("comment","\n"));
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements

    // DLangFileSome = 1 or more DLangFileBits
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
        rule.add(new Atom("ArgumentSignatureList"));
        rule.add(new Text(")"));
        rule.add(new Atom("WS"));
        rule.add(new Text("="));
        rule.add(new Atom("WS"));
        rule.add(new Atom("FunctionBody"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Text("new Function(\""));
        rule.add(new Var("fnname"));
        rule.add(new Text("\","));
        rule.add(new Atom("ArgumentSignatureList"));
        rule.add(new Text(","));
        rule.add(new Atom("FunctionBody"));
        rule.add(new Text(")"));
    ruleset.replacements.put("dintj",rule);

    ruleset=new RuleSet("FunctionBody");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("NiceCode"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("MutableCode"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("NiceCode");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("Expression"));
        rule.add(new Atom("Space"));
        rule.add(new Text("where"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("NiceNEDefs"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Expression"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("NiceNEDefs");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("NamedExpression"));
        rule.add(new Atom("'\n'"));
        rule.add(new Atom("NiceNEDefs"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("NamedExpression"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("MutableCode");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("WS"));
        rule.add(new Atom("Statement"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("MutableCode"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("WS"));
        rule.add(new Atom("Statement"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("ArgumentSignatureList");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("ArgumentSignature"));
        rule.add(new Atom("MoreArgs"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("ArgumentSignature"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("WS"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("ArgumentSignature");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("TypedArg"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("UntypedArg"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("TypedArg");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("WS"));
        rule.add(new Atom("VarName"));
        rule.add(new Atom("WS"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("UntypedArg");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("WS"));
        rule.add(new Atom("Type"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("VarName"));
        rule.add(new Atom("WS"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("MoreArgs");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text(","));
        rule.add(new Atom("ArgumentSignatureList"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("VarName");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Var("varname","<>\n\". :^+-*/"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("VarReference");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("VarName"));
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
        rule.add(new Atom("VarOrMemberReference"));
        rule.add(new Atom("WS"));
        rule.add(new Text("="));
        rule.add(new Atom("Expression"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("SpecialAssignment");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("VarOrMemberReference"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("SpecialAssignmentOperator"));
        rule.add(new Atom("Expression"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("SpecialAssignmentOperator");
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
        rule.add(new Atom("Space"));
        rule.add(new Atom("ExpressionBit"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("VarOrMemberReference"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("BracketedExpression"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("FunctionCall"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Algebra"));
      ruleset.add(rule);
    // Replacements

    // TODO: There is no support here for calling the results of expressions (or their members) :F

    ruleset=new RuleSet("FunctionCall");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("MemberReferenceOrNot"));
        rule.add(new Text("("));
        rule.add(new Atom("ArgumentParameterList"));
        rule.add(new Text(")"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("MemberName");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("VarName"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("VarOrMemberReference");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("VarReference"));
        rule.add(new Text("."));
        rule.add(new Atom("MemberName"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("VarReference"));
      ruleset.add(rule);
    // Replacements

    //                      | VarName

    ruleset=new RuleSet("ArgumentParameterList");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("Expression"));
        rule.add(new Text(","));
        rule.add(new Atom("ArgumentParameterList"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Expression"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
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
        rule.add(new Atom("OperatorSymbol"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Text("new Operator("));
        rule.add(new Atom("OperatorSymbol"));
        rule.add(new Text(")"));
    ruleset.replacements.put("dintj",rule);

    ruleset=new RuleSet("OperatorSymbol");
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


    ruleset=new RuleSet("VarDeclaration");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("VarDeclarationAndAssignment"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("VarDeclarationWithoutAssignment"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("VarDeclarationWithoutAssignment");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("var"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("VarName"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Type"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("VarName"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("var"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("VarName"));
        rule.add(new Atom("WS"));
        rule.add(new Text(":"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("Type"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("VarName"));
        rule.add(new Atom("WS"));
        rule.add(new Text(":"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("Type"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("VarDeclarationAndAssignment");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("VarDeclarationWithoutAssignment"));
        rule.add(new Atom("WS"));
        rule.add(new Text("="));
        rule.add(new Atom("WS"));
        rule.add(new Atom("Expression"));
      ruleset.add(rule);
    // Replacements



    // HTF will I deal with indentation?



    // WS    = Optional WhiteSpace
    // Space = Required WhiteSpace

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

    ruleset=new RuleSet("Space");
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
