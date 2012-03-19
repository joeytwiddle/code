package tools.parser;

import java.lang.String;
import java.util.Vector;

import tools.parser.*;

public class dlangGrm {
  public static void setupgrammar() {
    Vector rulesets=Grammar.rulesets;
    RuleSet ruleset;
    Vector<Type> rule;

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

    // @IMPORT DExpression "dexpr.grm"
    // @OPTION wrap_meaningful_indentation "{" "}"

    ruleset=new RuleSet("Main");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("DLangFile"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("DLangFile");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("DLangHeader"));
        rule.add(new Atom("MutableCodeBlock"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("DLangHeader");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("DLangModuleBlock"));
        rule.add(new Atom("DLangImports"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("DLangModuleBlock");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Comment"));
        rule.add(new Atom("DLangModuleBlock"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Space"));
        rule.add(new Atom("DLangModuleBlock"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("DLangModule"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("DLangModule");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("package"));
        rule.add(new Atom("Space"));
        rule.add(new Var("module","\n;"));
        rule.add(new Atom("EOL"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("module"));
        rule.add(new Atom("Space"));
        rule.add(new Var("module","\n;"));
        rule.add(new Atom("EOL"));
      ruleset.add(rule);
    // Replacements


    // DLangImports = DLangImport [ DLangImports ]

    ruleset=new RuleSet("DLangImports");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("DLangImport"));
        rule.add(new Atom("DLangImports"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("DLangImport"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("WS"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("DLangImport");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("DLangImportProper"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Comment"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Space"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("DLangImportProper");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("import"));
        rule.add(new Atom("Space"));
        rule.add(new Var("import","\n"));
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements


    // Should it be 0 or more?

    ruleset=new RuleSet("Comment");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("#"));
        rule.add(new Var("comment","\n"));
        rule.add(new Text("\n"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("//"));
        rule.add(new Var("comment","\n"));
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("\n/*"));
        rule.add(new Var("comment"));
        rule.add(new Text("*/\n"));
    ruleset.replacements.put("dintj",rule);


    // TODO: Parametrised classes (templates)
    ruleset=new RuleSet("ClassDefinition");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("class"));
        rule.add(new Atom("Space"));
        rule.add(new Var("className"," \n"));
        rule.add(new Atom("OptClassMods"));
        rule.add(new Atom("NL"));
        rule.add(new Atom("ClassBody"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("new ClassDefinition(\""));
        rule.add(new Atom("className"));
        rule.add(new Text("\", "));
        rule.add(new Atom("ClassBody"));
        rule.add(new Text(", \""));
        rule.add(new Atom("OptClassMods"));
        rule.add(new Text("\")"));
    ruleset.replacements.put("dintj",rule);


    ruleset=new RuleSet("OptClassMods");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Space"));
        rule.add(new Text("extends"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("ClassRef"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Space"));
        rule.add(new Text("implements"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("ClassRef"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("OptHorizSpace"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("ClassBody");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("[]"));
    ruleset.replacements.put("dintj",rule);


    // ClassBody = IndentedDLangBlock

    // TODO: I guess it will be "{" OWS DLangFileSome OWS "}"
    ruleset=new RuleSet("IndentedDLangBlock");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("DLangFileSome"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("FunctionDefinition");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
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
    rule=new Vector<Type>();
        rule.add(new Text("new Function(\""));
        rule.add(new Var("fnname"));
        rule.add(new Text("\", {"));
        rule.add(new Atom("ArgumentSignatureList"));
        rule.add(new Text("}, "));
        rule.add(new Atom("FunctionBody"));
        rule.add(new Text(")"));
    ruleset.replacements.put("dintj",rule);


    ruleset=new RuleSet("FunctionBody");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("NiceCode"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("MutableCodeBlock"));
      ruleset.add(rule);
    // Replacements


    //# NiceCode swallowed the var from an assignment statement
    //# Should NiceCode demand that it ends neatly with an EOL?
    // FunctionBody = MutableCode

    // NiceCode = Expression Space "where" Space NiceNEDefs EOL
    //          | Expression EOL

    ruleset=new RuleSet("NiceCode");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Expression"));
        rule.add( new GroupedDefn((RuleSet) new Runner(){ Object run(){
          RuleSet ruleset = new RuleSet("Anonymous");
        Vector<Type> rule = new Vector<Type>();
        rule.add(new Atom("Space"));
        rule.add(new Text("where"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("NiceNEDefs"));
        rule.add(new Atom("EOL"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("EOL"));
        ruleset.add(rule);
          return ruleset;
        } }.run() ) );
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("NiceNEDefs");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("NamedExpression"));
        rule.add(new Atom("'\n'"));
        rule.add(new Atom("NiceNEDefs"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("NamedExpression"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("MutableCodeBlock");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("MutableCodeInner"));
        rule.set(rule.size()-1, new RepeatedRule((Type)rule.lastElement(),"*"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("new MutableCodeBlock({"));
        rule.add(new Atom("MutableCodeInner"));
        rule.add(new Text("})"));
    ruleset.replacements.put("dintj",rule);


    // Note we test FunctionDefinition before FunctionCall - they both look similar at the start!

    ruleset=new RuleSet("MutableCodeInner");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Space"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("NonStatement"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Statement"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("ArgumentSignatureList");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("ArgumentSignature"));
        rule.add(new Atom("MoreArgs"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("ArgumentSignature"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("WS"));
      ruleset.add(rule);
    // Replacements


    // ArgumentSignature = TypedArg | UntypedArg
    ruleset=new RuleSet("ArgumentSignature");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("UntypedArg"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("UntypedArg");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("WS"));
        rule.add(new Atom("VarName"));
        rule.add(new Atom("WS"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("TypedArg");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("WS"));
        rule.add(new Atom("Type"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("VarName"));
        rule.add(new Atom("WS"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("MoreArgs");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text(","));
        rule.add(new Atom("ArgumentSignatureList"));
      ruleset.add(rule);
    // Replacements


    // Identifier = <varname/"<>\n\". :^+-*()/">
    ruleset=new RuleSet("Identifier");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Var("varname",null,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$@0-9"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("VarName");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Identifier"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("ClassName");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Identifier"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("VarReference");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("VarName"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("new VarReference(\""));
        rule.add(new Atom("VarName"));
        rule.add(new Text("\")"));
    ruleset.replacements.put("dintj",rule);


    // Could also be called a PrimitiveLiteral
    ruleset=new RuleSet("ConstReference");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Number"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("String"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("new ConstReference("));
        rule.add(new Atom("Number"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("String"));
        rule.add(new Text(")"));
    ruleset.replacements.put("dintj",rule);


    ruleset=new RuleSet("Statement");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Assignment"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("FunctionCall"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Loop"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("NonStatement");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Comment"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("ClassDefinition"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("FunctionDefinition"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Assignment");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("NormalAssignment"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("SpecialAssignment"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("NormalAssignment");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("VarOrMemberReference"));
        rule.add(new Atom("OptHorizSpace"));
        rule.add(new Text("="));
        rule.add(new Atom("Expression"));
        rule.add(new Atom("EOL"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("new NormalAssignment("));
        rule.add(new Atom("VarOrMemberReference"));
        rule.add(new Text(","));
        rule.add(new Atom("Expression"));
        rule.add(new Text(")"));
    ruleset.replacements.put("dintj",rule);


    ruleset=new RuleSet("SpecialAssignment");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("VarOrMemberReference"));
        rule.add(new Atom("OptHorizSpace"));
        rule.add(new Atom("SpecialAssignmentOperator"));
        rule.add(new Atom("Expression"));
        rule.add(new Atom("EOL"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("new SpecialAssignment("));
        rule.add(new Atom("VarOrMemberReference"));
        rule.add(new Text(",\""));
        rule.add(new Atom("SpecialAssignmentOperator"));
        rule.add(new Text("\","));
        rule.add(new Atom("Expression"));
        rule.add(new Text(")"));
    ruleset.replacements.put("dintj",rule);


    ruleset=new RuleSet("SpecialAssignmentOperator");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("+="));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("-="));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("*="));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("/="));
      ruleset.add(rule);
    // Replacements


    // ConstReference must come first because ATM VarOrMemberReference swallows numbers!

    ruleset=new RuleSet("ExpressionTerminal");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("ConstReference"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("VarOrMemberReference"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Expression");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("ExpressionBit"));
        rule.add(new Atom("OptHorizSpace"));
      ruleset.add(rule);
    // Replacements


    // We have a problem with Algebra
    // If I let Algebra contain Expressions, then it calls its repeatedly!

    ruleset=new RuleSet("ExpressionBit");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("HorizSpace"));
        rule.add(new Atom("ExpressionBit"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Algebra"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("ExpressionAtom"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("ExpressionAtom");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("HorizSpace"));
        rule.add(new Atom("ExpressionAtom"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("FunctionCall"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("BracketedExpression"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("ExpressionTerminal"));
      ruleset.add(rule);
    // Replacements


    // TODO: There is no support here for calling the results of expressions (or their members) :F
    // If it is impossible to replace VarOrMemberReference with Expression because
    // of infinite recursion, then perhaps OptFinalCall should be available at the
    // end of any expression (or expression bit), as a postfix, the same way we
    // dealt with + and * in grm.

    ruleset=new RuleSet("FunctionCall");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("ConstructorCall"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("RealFunctionCall"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("RealFunctionCall");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("VarOrMemberReference"));
        rule.add(new Text("("));
        rule.add(new Atom("ArgumentParameterList"));
        rule.add(new Text(")"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("new FunctionCall("));
        rule.add(new Atom("VarOrMemberReference"));
        rule.add(new Text(", ["));
        rule.add(new Atom("ArgumentParameterList"));
        rule.add(new Text("])"));
    ruleset.replacements.put("dintj",rule);


    ruleset=new RuleSet("ConstructorCall");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("new"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("RealFunctionCall"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("new ConstructorCall("));
        rule.add(new Atom("RealFunctionCall"));
        rule.add(new Text(")"));
    ruleset.replacements.put("dintj",rule);


    ruleset=new RuleSet("MemberName");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("VarName"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("VarOrMemberReference");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("VarReference"));
        rule.add(new Text("."));
        rule.add(new Atom("MemberName"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("VarReference"));
      ruleset.add(rule);
    // Replacements


    //                      | VarName

    ruleset=new RuleSet("ArgumentParameterList");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Expression"));
        rule.add(new Text(","));
        rule.add(new Atom("ArgumentParameterList"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Expression"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("BracketedExpression");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("("));
        rule.add(new Atom("WS"));
        rule.add(new Atom("ExpressionBit"));
        rule.add(new Atom("WS"));
        rule.add(new Text(")"));
      ruleset.add(rule);
    // Replacements


    //# Can lock us in a cyclic loop ExpressionBit -> Algebra -> ExpressionBit !
    // Algebra = ExpressionBit Operator ExpressionBit

    ruleset=new RuleSet("Algebra");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("ExpressionAtom"));
        rule.add(new Atom("OptHorizSpace"));
        rule.add(new Atom("Operator"));
        rule.add(new Atom("OptHorizSpace"));
        rule.add(new Atom("Expression"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("new Operation("));
        rule.add(new Atom("ExpressionAtom"));
        rule.add(new Text(","));
        rule.add(new Atom("Operator"));
        rule.add(new Text(","));
        rule.add(new Atom("Expression"));
        rule.add(new Text(")"));
    ruleset.replacements.put("dintj",rule);


    ruleset=new RuleSet("TerminalAlgebraicOperation");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("ExpressionAtom"));
        rule.add(new Atom("OptHorizSpace"));
        rule.add(new Atom("Operator"));
        rule.add(new Atom("OptHorizSpace"));
        rule.add(new Atom("Expression"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Operator");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("OperatorSymbol"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("new Operator(\""));
        rule.add(new Atom("OperatorSymbol"));
        rule.add(new Text("\")"));
    ruleset.replacements.put("dintj",rule);


    // We can't do: Constants.getOperator("*") here because not all are defined by
    // default, i.e. there may be an operator with a custom name.  I suppose
    // context.lookupOperator("*") might work.  For the moment though, we refer to
    // it by string, which would allow for operators to be defined after their first
    // appearance in the file.

    ruleset=new RuleSet("OperatorSymbol");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("BooleanOperator"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("AlgebraicOperator"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("OtherOperator"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("AlgebraicOperator");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("*"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("/"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("+"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("-"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("^"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("<<"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(">>"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("&"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("|"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("BooleanOperator");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("=="));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("!="));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(">"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("<"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(">="));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("<="));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("&&"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("||"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("OtherOperator");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("<"));
        rule.add(new Var("varname"," \n\t,."));
        rule.add(new Text(">"));
      ruleset.add(rule);
    // Replacements



    ruleset=new RuleSet("VarDeclaration");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("VarDeclarationAndAssignment"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("VarDeclarationWithoutAssignment"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("VarDeclarationWithoutAssignment");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("var"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("VarName"));
      ruleset.add(rule);
    // Replacements


    //                                 | Type Space VarName
    //                                 | "var" Space VarName WS ":" WS Type
    //                                 | VarName WS ":" WS Type

    ruleset=new RuleSet("Type");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("TypeWithoutTemplate"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("TypeWithTemplate"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("TypeWithoutTemplate");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Identifier"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("TypeWithTemplate");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("TypeWithoutTemplate"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("Template"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Template");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("<"));
        rule.add(new Atom("TemplateBits"));
        rule.add(new Text(">"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("TemplateBits");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("WS"));
        rule.add(new Atom("TemplateBit"));
        rule.add(new Atom("WS"));
        rule.add(new Text(","));
        rule.add(new Atom("WS"));
        rule.add(new Atom("TemplateBits"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("WS"));
        rule.add(new Atom("TemplateBit"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("VarDeclarationAndAssignment");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("VarDeclarationWithoutAssignment"));
        rule.add(new Atom("WS"));
        rule.add(new Text("="));
        rule.add(new Atom("WS"));
        rule.add(new Atom("Expression"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Number");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Float"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Int"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Float");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Digits"));
        rule.add(new Text("."));
        rule.add(new Atom("Digits"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Int");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Digits"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Digits");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Digit"));
        rule.add(new Atom("Digits"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Digit"));
      ruleset.add(rule);
    // Replacements


    // Digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"

    ruleset=new RuleSet("Digit");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Var("number",null,"0123456789"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("String");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("\""));
        rule.add(new Var("string","\""));
        rule.add(new Text("\""));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Loop");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("ForNumLoop"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("ForListLoop"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("WhileLoop"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("ForNumLoop");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("for"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("VarName"));
        rule.add(new Atom("WS"));
        rule.add(new Text("="));
        rule.add(new Atom("WS"));
        rule.add(new Atom("LoopRange"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("ForBody"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("new ForNumLoop("));
        rule.add(new Atom("VarName"));
        rule.add(new Text(","));
        rule.add(new Atom("LoopRange"));
        rule.add(new Text(")"));
    ruleset.replacements.put("dintj",rule);

    // "{" ForBody "}"

    ruleset=new RuleSet("ForListLoop");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("for"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("VarName"));
        rule.add(new Atom("Space"));
        rule.add(new Text("in"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("Expression"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("ForBody"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("new ForListLoop("));
        rule.add(new Atom("VarName"));
        rule.add(new Text(","));
        rule.add(new Atom("Expression"));
        rule.add(new Text(")"));
    ruleset.replacements.put("dintj",rule);

    // "{" ForBody "}"

    ruleset=new RuleSet("ForBody");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("MutableCodeBlock"));
      ruleset.add(rule);
    // Replacements


    //# TODO: Gah - Expressions don't parse nicely here!  (They swallow the "to" or what?)
    // LoopRange = Expression Space "to" Space Expression

    ruleset=new RuleSet("LoopRange");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("ConstReference"));
        rule.add(new Atom("Space"));
        rule.add(new Text("to"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("Expression"));
        rule.add(new Atom("OptStep"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("new LoopRange("));
        rule.add(new Atom("ConstReference"));
        rule.add(new Text(","));
        rule.add(new Atom("Expression"));
        rule.add(new Text(")"));
    ruleset.replacements.put("dintj",rule);


    ruleset=new RuleSet("OptStep");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("WS"));
        rule.add(new Text("step"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("Expression"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("WhileLoop");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("while"));
        rule.add(new Atom("Space"));
        rule.add(new Atom("Condition"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Condition");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Expression"));
      ruleset.add(rule);
    // Replacements


    // We hope to handle significant indentation by pre-parsing the lines and
    // rewriting with "{" and "}"s which the grammar can easily detect.  This may
    // cause issues on multi-line statements which should not have "{" and "}"s!



    // WS    = Optional WhiteSpace
    // Space = Required WhiteSpace

    ruleset=new RuleSet("NL");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("WS");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Whitespace"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Space");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Whitespace"));
      ruleset.add(rule);
    // Replacements


    // Whitespace = WhitespaceBit Whitespace
               // | WhitespaceBit
    // 
    // WhitespaceBit = "\n"
                  // | " "
                  // | "\t"
                  // | "\r"

    ruleset=new RuleSet("Whitespace");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Var("whitespace",null," \t\n\r"));
      ruleset.add(rule);
    // Replacements


    // Expressions will eat into the next line if allowed to swallow "\n"s
    // They don't know when to stop!
    // So we only allow them to swallow HorizSpace

    // HorizSpace = " " | "\t"
    ruleset=new RuleSet("HorizSpace");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Var("horizspace",null," \t"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("OptHorizSpace");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("HorizSpace"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    // EOL = ";" | "\n"

    ruleset=new RuleSet("EOL");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements


  }
}
