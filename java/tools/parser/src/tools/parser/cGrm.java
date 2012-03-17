package tools.parser;

import java.lang.String;
import java.util.Vector;

import tools.parser.*;

public class cGrm {
  public static void setupgrammar() {
    Vector rulesets=Grammar.rulesets;
    RuleSet ruleset;
    Vector rule;

    //#### Joey's grammar for C++ parsing
    //## Provided a single c file,
    //## targets "h" and "c" should output the correct split.


    //#### Main

    ruleset=new RuleSet("Main");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("CSrc"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("CSrc");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("CSrcBit"));
        rule.add(new Atom("CSrc"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("CSrcBit");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("WS"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Special"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Comment"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("HashCom"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("ClassDecl"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("ClassDefn"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Method"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("VariableDecl"));
      ruleset.add(rule);
    // Replacements

    // Note: Methods given priority because VariableDecl tries to catch eg. "operator==" !

    // Doesn't appear to be picking up in joeylib.c

    ruleset=new RuleSet("Special");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("/*special c*/"));
        rule.add(new Atom("EndSpecial"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("/*special h*/"));
        rule.add(new Atom("EndSpecial"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("EndSpecial");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Var("stuff","/"));
        rule.add(new Atom("ContSpecial"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("ContSpecial");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("/*end special*/"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("/"));
        rule.add(new Atom("EndSpecial"));
      ruleset.add(rule);
    // Replacements

    //#### Simple comments and #commands

    ruleset=new RuleSet("Comment");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("/*"));
        rule.add(new Atom("EndCom"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("//"));
        rule.add(new Var("stuff","\n"));
        rule.add(new Atom("NL"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("//"));
        rule.add(new Atom("NL"));
      ruleset.add(rule);
    // Replacements

    // Hack for /* */ to deal with eg. " ... /* blah foo*bar */ ... "
    ruleset=new RuleSet("EndCom");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Var("stuff","*"));
        rule.add(new Text("*/"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Var("stuff","*"));
        rule.add(new Text("*"));
        rule.add(new Atom("EndCom"));
      ruleset.add(rule);
    // Replacements

    // Although the repeated RHS are unaesthetic,
    // they are OK when parsing, since the RHS
    // is only reached when the LHS is satisfied.
    ruleset=new RuleSet("HashCom");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("#include "));
        rule.add(new Atom("HashIncludeLib"));
        rule.add(new Atom("OptSpc"));
        rule.add(new Atom("NL"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("#include "));
        rule.add(new Atom("HashIncludeFile"));
        rule.add(new Atom("OptSpc"));
        rule.add(new Atom("NL"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("#def"));
        rule.add(new Var("more","\n"));
        rule.add(new Atom("NL"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("#ifdef"));
        rule.add(new Var("more","\n"));
        rule.add(new Atom("NL"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("#"));
        rule.add(new Var("other","\n"));
        rule.add(new Atom("NL"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("HashIncludeLib");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("<"));
        rule.add(new Var("file","."));
        rule.add(new Text(".c>"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("<"));
        rule.add(new Var("file","."));
        rule.add(new Text(".h>"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Text("<"));
        rule.add(new Var("file"));
        rule.add(new Text(".h>"));
    ruleset.replacements.put("h",rule);

    ruleset=new RuleSet("HashIncludeFile");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("\""));
        rule.add(new Var("file","."));
        rule.add(new Text(".c\""));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("\""));
        rule.add(new Var("file","."));
        rule.add(new Text(".h\""));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Text("\""));
        rule.add(new Var("file"));
        rule.add(new Text(".h\""));
    ruleset.replacements.put("h",rule);



    //#### Classes

    // However, the class declaration vs. definition is
    // inefficient, since the LHS is searched in both cases.
    // However, it is neccessary to classify this LHS
    // data according the the resulting match on the RHS.

    ruleset=new RuleSet("ClassDecl");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptClassTemplate"));
        rule.add(new Text("class"));
        rule.add(new Atom("WS"));
        rule.add(new Var("name"," =(){};"));
        rule.add(new Text(";"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Atom("OptClassTemplate"));
        rule.add(new Text(" class "));
        rule.add(new Var("name"));
        rule.add(new Text(";"));
    ruleset.replacements.put("h",rule);

    rule=new Vector();
        rule.add(new Text("// Class declaration of existence: "));
        rule.add(new Var("name"));
        rule.add(new Text("\n"));
    ruleset.replacements.put("c",rule);

    ruleset=new RuleSet("ClassDefn");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("TemplateClass"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Class"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("TemplateClass");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("ClassTemplate"));
        rule.add(new Atom("Class"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Text("// Template class appears in original format:\n"));
      rule.add( new ActiveReplacement() { public String replace() {  jlib.strings.FakeOutputStream out=new jlib.strings.FakeOutputStream(); match.render(null,"none",new java.io.PrintStream(out)); return ""+out.store;  } } );
        rule.add(new Text("// End template class\n"));
    ruleset.replacements.put("h",rule);

    rule=new Vector();
        rule.add(new Text("// Template class must appear in .h file\n"));
    ruleset.replacements.put("c",rule);

    ruleset=new RuleSet("Class");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptClassMods"));
        rule.add(new Text("class "));
        rule.add(new Var("name","\n ;{}()"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("OptMoreClassMods"));
        rule.add(new Text("{"));
        rule.add(new Atom("ClassInside"));
        rule.add(new Text("};"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Atom("OptClassMods"));
        rule.add(new Text("class "));
        rule.add(new Var("name"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("OptMoreClassMods"));
        rule.add(new Text("{"));
        rule.add(new Atom("ClassInside"));
        rule.add(new Text("};"));
    ruleset.replacements.put("h",rule);

    rule=new Vector();
        rule.add(new Text("// Starts of class "));
        rule.add(new Var("name"));
        rule.add(new Text("\n"));
        rule.add(new Atom("ClassInside"));
        rule.add(new Text("// End class "));
        rule.add(new Var("name"));
        rule.add(new Text("\n"));
    ruleset.replacements.put("c",rule);

    ruleset=new RuleSet("OptMoreClassMods");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Var("moremods","\n;{}()"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("OptClassMods");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("ClassMod"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("OptClassMods"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("ClassMod");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("public"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("private"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("ClassInside");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("Publicity"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("ClassInside"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("CSrcBit"));
        rule.add(new Atom("ClassInside"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("Publicity");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("public:"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("private:"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Text(""));
    ruleset.replacements.put("c",rule);

    ruleset=new RuleSet("OptClassTemplate");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("ClassTemplate"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("ClassTemplate");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("Template"));
      ruleset.add(rule);
    // Replacements



    //#### Templates

    // 1) Templates used to declare a template class or method

    ruleset=new RuleSet("OptTemplate");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("Template"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("Template");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
        rule.add(new Text("template"));
        rule.add(new Atom("OptWS"));
        rule.add(new Text("<"));
        rule.add(new Atom("TemplateSuperInside"));
        rule.add(new Text(">"));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("TemplateSuperInside");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
        rule.add(new Text("class"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("TemplateInside"));
      ruleset.add(rule);
    // Replacements

    // 2) Templates used to declare a template object instance

    ruleset=new RuleSet("OptTemplateType");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("TemplateType"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("TemplateType");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
        rule.add(new Var("temptype"," (){}<>,"));
        rule.add(new Text("<"));
        rule.add(new Atom("TemplateTypeInner"));
        rule.add(new Atom("OptPtr"));
        rule.add(new Atom("OptWS"));
        rule.add(new Text(">"));
      ruleset.add(rule);
    // Replacements

    // Note: recursion to deal with eg. " List< List<int> > listOfLists(); "
    ruleset=new RuleSet("TemplateTypeInner");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("TemplateType"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("TemplateInside"));
      ruleset.add(rule);
    // Replacements

    // Took comma out to deal with eg. "template <class A,class B>"
    ruleset=new RuleSet("TemplateInside");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Var("inside","<>(){}"));
      ruleset.add(rule);
    // Replacements



    //#### Methods

    // Second case is constructor (no type)
    ruleset=new RuleSet("MDecl");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptTemplate"));
        rule.add(new Atom("OptVarMods"));
        rule.add(new Atom("OptWS"));
        rule.add(new Atom("VarType"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("OptPtr"));
        rule.add(new Atom("OptLater"));
        rule.add(new Atom("MethName"));
        rule.add(new Atom("OptWS"));
        rule.add(new Text("("));
        rule.add(new Atom("OptArgs"));
        rule.add(new Text(")"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptTemplate"));
        rule.add(new Atom("OptPtr"));
        rule.add(new Atom("OptLater"));
        rule.add(new Atom("MethName"));
        rule.add(new Atom("OptWS"));
        rule.add(new Text("("));
        rule.add(new Atom("OptArgs"));
        rule.add(new Text(")"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Atom("OptTemplate"));
        rule.add(new Atom("OptVarMods"));
        rule.add(new Atom("VarType"));
        rule.add(new Text(" "));
        rule.add(new Atom("OptPtr"));
        rule.add(new Atom("OptLater"));
        rule.add(new Atom("MethName"));
        rule.add(new Text("("));
        rule.add(new Atom("OptArgs"));
        rule.add(new Text(")"));
    ruleset.replacements.put("h",rule);

    rule=new Vector();
        { Vector realrule=rule; rule=new Vector();         rule.add(new Atom("ClassTemplate"));
 realrule.add(new RelElement('^',"TemplateClass",(Type)rule.get(0))); rule=realrule; }
        rule.add(new Atom("OptTemplate"));
        rule.add(new Atom("OptVarMods"));
        rule.add(new Atom("VarType"));
        rule.add(new Text(" "));
        rule.add(new Atom("OptPtr"));
        { Vector realrule=rule; rule=new Vector();         rule.add(new Var("name"));
 realrule.add(new RelElement('^',"Class",(Type)rule.get(0))); rule=realrule; }
      rule.add( new ActiveReplacement() { public String replace() {  Match m=match.getUp("Class").getDown("TemplateClass").getDown("TemplateSuperInside").getDown("TemplateInside"); return "<"+m.string+">";  } } );
      rule.add( new ActiveReplacement() { public String replace() {  return ( match.getUp("Class")==null ? "" : "::" );  } } );
        rule.add(new Atom("OptLater"));
        rule.add(new Atom("MethName"));
        rule.add(new Text("("));
        rule.add(new Atom("OptArgs"));
        rule.add(new Text(")"));
    ruleset.replacements.put("c",rule);

    // ` return ( match.getUp("Class")==null ? "extern " : "" ); ` 
    // c: Class^OptTemplate OptTemplate OptVarMods VarType " " OptPtr Class^<name> ` Match m=match.getUp("Class").getDown("ClassOptTemplate").getDown("TemplateSuperInside").getDown("TemplateInside"); return "<"+m.string+">"; ` ` return ( match.getUp("Class")==null ? "/* no class */" : "::" ); ` OptLater MethName "(" OptArgs ")"
    // h: ` if ( ! match.getDown("OptLater").string.toString().equals("") ) { unusedmatches.clear(); return "/* Skipping >"+match.getDown("OptLater").string+"< */\n"; } return "/* =) */"; ` OptTemplate OptVarMods VarType " " MethName "(" OptArgs ")"

    ruleset=new RuleSet("OptPtr");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
        rule.add(new Text("*"));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("OptLater");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("MethName"));
        rule.add(new Text("::"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements

    // Method names can contain, eg. operator=
    ruleset=new RuleSet("MethName");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("operator"));
        rule.add(new Var("op"," ()"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Var("varname"," =:;(){}"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("Method");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("MethodWithCode"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("MethodWithoutCode"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
      rule.add( new ActiveReplacement() { public String replace() {  return ( match.getDown("OptLater").string.toString().equals("") ? "" : "/* Skipping later :: " );  } } );
        rule.add(new Atom("MethodWithCode"));
        rule.add(new Atom("MethodWithoutCode"));
      rule.add( new ActiveReplacement() { public String replace() {  return ( match.getDown("OptLater").string.toString().equals("") ? "" : "*/\n" );  } } );
    ruleset.replacements.put("h",rule);

    ruleset=new RuleSet("MethodWithCode");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("MDecl"));
        rule.add(new Atom("OptWS"));
        rule.add(new Text("{"));
        rule.add(new Atom("Code"));
        rule.add(new Text("}"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Atom("MDecl"));
        rule.add(new Text("; // Method\n"));
    ruleset.replacements.put("h",rule);

    ruleset=new RuleSet("MethodWithoutCode");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("MDecl"));
        rule.add(new Atom("OptWS"));
        rule.add(new Text(";"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Atom("MDecl"));
        rule.add(new Text("; // Method\n"));
    ruleset.replacements.put("h",rule);

    rule=new Vector();
        rule.add(new Text("/* Skipping declared method "));
        rule.add(new Atom("MDecl"));
        rule.add(new Text(" */\n"));
    ruleset.replacements.put("c",rule);



    //#### Code - not yet fully parsed

    ruleset=new RuleSet("Code");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Var("code","{}\"/"));
        rule.add(new Atom("RestCode"));
        rule.add(new Atom("Code"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Var("other","{}"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("RestCode");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("String"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Comment"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("/"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("InnerCode"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("InnerCode");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("{"));
        rule.add(new Atom("Code"));
        rule.add(new Text("}"));
      ruleset.add(rule);
    // Replacements



    //#### Variables

    ruleset=new RuleSet("OptVarMods");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptVarMod"));
        rule.add(new Atom("OptWS"));
        rule.add(new Atom("OptVarMods"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptVarMod"));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements

    // fillin is one of my #defs, abstract may be Java only, not C
    // These should really be split up for methods, variables etc.
    ruleset=new RuleSet("OptVarMod");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("public"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("private"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("static"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("virtual"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("abstract"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("extern"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("unsigned"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("signed"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("const"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("static"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("struct"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Fillin"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Fillup"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
      rule.add( new ActiveReplacement() { public String replace() {  String val=match.string.toString(); if ( val.equals("virtual") || val.equals("static") || val.equals("fillin") /* && match.getUp("Class")==null */ ) return ""; else return ""+match.string;  } } );
    ruleset.replacements.put("c",rule);

    //          | "fillin"
    //          | "fillup"

    ruleset=new RuleSet("Fillin");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("fillin"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Text("// fillin"));
    ruleset.replacements.put("c",rule);

    ruleset=new RuleSet("Fillup");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("fillup"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Text("// fillup"));
    ruleset.replacements.put("h",rule);

    ruleset=new RuleSet("VarBeing");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
        rule.add(new Text("="));
        rule.add(new Atom("OptWS"));
        rule.add(new Var("varbeing",";"));
      ruleset.add(rule);
    // Replacements

    // Method: Second option is a method declaration

    ruleset=new RuleSet("VariableDecl");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("VariableDeclBeing"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("VariableDeclExist"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("VariableDeclExist");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptVarMods"));
        rule.add(new Atom("VarType"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("VarNames"));
        rule.add(new Text(";"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Atom("OptVarMods"));
      rule.add( new ActiveReplacement() { public String replace() {  return ( match.getUp("Class")==null ? "extern " : "" );  } } );
        rule.add(new Atom("VarType"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("VarNames"));
        rule.add(new Text("; // Exists"));
    ruleset.replacements.put("h",rule);

    rule=new Vector();
      rule.add( new ActiveReplacement() { public String replace() {  return ( match.getUp("Class")==null ? match.renderString("normal") : "// Variable declared in .h file" );  } } );
    ruleset.replacements.put("c",rule);

    // NB: If you try putting OptVarMods after `...` you may end up with "extern // fillup ..." in the output :-(

    ruleset=new RuleSet("VariableDeclBeing");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptVarMods"));
        rule.add(new Atom("VarType"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("VarNames"));
        rule.add(new Atom("VarBeing"));
        rule.add(new Text(";"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Atom("OptVarMods"));
      rule.add( new ActiveReplacement() { public String replace() {  return ( match.getUp("Class")==null ? "extern " : "" );  } } );
        rule.add(new Atom("VarType"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("VarNames"));
        rule.add(new Text("; // Variable initialised in .c file"));
    ruleset.replacements.put("h",rule);

    rule=new Vector();
        rule.add(new Atom("OptVarMods"));
        rule.add(new Atom("VarType"));
        rule.add(new Atom("WS"));
        { Vector realrule=rule; rule=new Vector();         rule.add(new Var("name"));
 realrule.add(new RelElement('^',"Class",(Type)rule.get(0))); rule=realrule; }
      rule.add( new ActiveReplacement() { public String replace() {  Match cls=match.getUp("Class"); return ( cls==null ? "" : "::" );  } } );
        rule.add(new Atom("VarNames"));
        rule.add(new Atom("VarBeing"));
        rule.add(new Text(";"));
    ruleset.replacements.put("c",rule);

    ruleset=new RuleSet("VarType");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("ArgType"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("VarNames");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("VarName"));
        rule.add(new Atom("OptWS"));
        rule.add(new Text(","));
        rule.add(new Atom("OptWS"));
        rule.add(new Atom("VarNames"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("VarName"));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
    // Replacements



    //#### Arguments and types

    ruleset=new RuleSet("ArgType");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptArgMods"));
        rule.add(new Atom("OptWS"));
        rule.add(new Atom("TemplateType"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptArgMods"));
        rule.add(new Atom("OptWS"));
        rule.add(new Var("vartype"," <>\n;(){},"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("OptArgMods");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptVarMods"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements

    // need to avoid = or it will be read!
    ruleset=new RuleSet("VarName");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("ArgName"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("OptArgs");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("Args"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("Args");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("DoubleArg"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Arg"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("DoubleArg");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("Arg"));
        rule.add(new Text(","));
        rule.add(new Atom("Args"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Atom("Arg"));
        rule.add(new Text(","));
        rule.add(new Atom("Args"));
    ruleset.replacements.put("h",rule);

    rule=new Vector();
        rule.add(new Atom("Arg"));
        rule.add(new Text(","));
        rule.add(new Atom("Args"));
    ruleset.replacements.put("c",rule);

    ruleset=new RuleSet("Arg");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
        rule.add(new Atom("ArgA"));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("ArgA");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("ArgType"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("ArgName"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("ArgType"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("..."));
      ruleset.add(rule);
    // Replacements

    // h: <argtype> " " ArgName
    // c: <argtype> " " ArgName

    // ArgType = VarType OptTemplateType

    ruleset=new RuleSet("ArgName");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("("));
        rule.add(new Var("argname"," ;=(){},<>\n"));
        rule.add(new Text(")"));
        rule.add(new Atom("OptWS"));
        rule.add(new Text("("));
        rule.add(new Atom("OptArgs"));
        rule.add(new Text(")"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Var("argname"," ;=(){},<>\n"));
        rule.add(new Atom("OptWS"));
        rule.add(new Text("("));
        rule.add(new Atom("OptArgs"));
        rule.add(new Text(")"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Var("argname"," ;=(){},<>\n"));
      ruleset.add(rule);
    // Replacements



    //#### And everything else

    ruleset=new RuleSet("String");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("\""));
        rule.add(new Var("str","\""));
        rule.add(new Text("\""));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("OptWS");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("WS"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("WS");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("NL"));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Comment"));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("SpcBit"));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("NL");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("\r\n"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("OptSpc");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("SpcBit"));
        rule.add(new Atom("OptSpc"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements

    ruleset=new RuleSet("SpcBit");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text(" "));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("\t"));
      ruleset.add(rule);
    // Replacements


  }
}
