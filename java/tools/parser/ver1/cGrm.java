import java.lang.String;
import java.util.Vector;

import Grammar;
import RuleSet;

public class cGrm {
  public static void setupgrammar() {
    Vector rulesets=Grammar.rulesets;
    RuleSet ruleset;
    Vector rule;

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
        rule.add(new Atom("CSrcBit"));
      ruleset.add(rule);
    // Replacements



    //     | Class "};" CSrc
    //     | CSrcBit "\n" CSrc
    //     | WS CSrc
    //     | CSrcBit CSrc
    //     | CSrcBit
    //     | "\n"
    //     | <iffytypestuff> CSrc

    ruleset=new RuleSet("CSrcBit");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("WS"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Comment"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Include"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Method"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("VariableDecl"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Class"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("ClassDecl"));
      ruleset.add(rule);
    // Replacements



    // Include = "#include" <stuff/"\n">
    // can be define as wel
    ruleset=new RuleSet("Include");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("#"));
        rule.add(new Var("stuff","\n"));
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Text(""));
    ruleset.replacements.put("c",rule);



    ruleset=new RuleSet("ClassDecl");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptTemplate"));
        rule.add(new Text("class"));
        rule.add(new Atom("OptWS"));
        rule.add(new Var("name","{};"));
        rule.add(new Text(";"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Text("// Class declaration of existence: "));
        rule.add(new Var("name"));
        rule.add(new Text("\n"));
    ruleset.replacements.put("c",rule);

    rule=new Vector();
        rule.add(new Atom("OptTemplate"));
        rule.add(new Text(" class "));
        rule.add(new Var("name"));
        rule.add(new Text(";"));
    ruleset.replacements.put("h",rule);



    ruleset=new RuleSet("Class");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
        rule.add(new Atom("OptTemplate"));
        rule.add(new Atom("ClassMods"));
        rule.add(new Text("class"));
        rule.add(new Atom("WS"));
        rule.add(new Var("name","\n ;{}()"));
        rule.add(new Atom("WS"));
        rule.add(new Var("moremods","\n;{}()"));
        rule.add(new Text("{"));
        rule.add(new Atom("ClassInside"));
        rule.add(new Atom("OptWS"));
        rule.add(new Text("};"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
        rule.add(new Atom("ClassMods"));
        rule.add(new Text("class "));
        rule.add(new Var("name","\n ;{}()"));
        rule.add(new Text(" "));
        rule.add(new Var("moremods","\n;{}()"));
        rule.add(new Text("{"));
        rule.add(new Atom("ClassInside"));
        rule.add(new Atom("OptWS"));
        rule.add(new Text("};"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Atom("OptTemplate"));
        rule.add(new Atom("ClassMods"));
        rule.add(new Text(" class "));
        rule.add(new Var("name"));
        rule.add(new Var("moremods"));
        rule.add(new Text("{\n"));
        rule.add(new Atom("ClassInside"));
        rule.add(new Text("};\n"));
    ruleset.replacements.put("h",rule);

    rule=new Vector();
        rule.add(new Text("// Start of class "));
        rule.add(new Var("name"));
        rule.add(new Text("\n"));
        rule.add(new Atom("ClassInside"));
    ruleset.replacements.put("c",rule);



    ruleset=new RuleSet("ClassMods");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("ClassMod"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("ClassMods"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("ClassMod"));
        rule.add(new Atom("WS"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
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



    ruleset=new RuleSet("OptTemplate");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
        rule.add(new Text("template"));
        rule.add(new Atom("OptWS"));
        rule.add(new Text("<"));
        rule.add(new Atom("TemplateInside"));
        rule.add(new Text(">"));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements



    // h: OptTemplate " " OptVarMods " " VarType " " VarName OptVarBeing ";\n"
    // c: "// Variable " OptTemplate " " VarType " " VarName " declared in .h file\n"

    ruleset=new RuleSet("TemplateInside");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Var("inside","<>(){}"));
      ruleset.add(rule);
    // Replacements



    // Took comma out of above to deal with eg. "template <class A,class B>"

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
        rule.add(new Atom("OptWS"));
        rule.add(new Text(">"));
      ruleset.add(rule);
    // Replacements



    ruleset=new RuleSet("TemplateTypeInner");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("TemplateType"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("TemplateInside"));
      ruleset.add(rule);
    // Replacements



    // Second case is constructor (no type)
    ruleset=new RuleSet("MDecl");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptTemplate"));
        rule.add(new Atom("OptVarMods"));
        rule.add(new Atom("OptWS"));
        rule.add(new Atom("VarType"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("MethName"));
        rule.add(new Atom("OptWS"));
        rule.add(new Text("("));
        rule.add(new Atom("OptArgs"));
        rule.add(new Text(")"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptTemplate"));
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
        rule.add(new Atom("MethName"));
        rule.add(new Text("("));
        rule.add(new Atom("OptArgs"));
        rule.add(new Text(")"));
    ruleset.replacements.put("h",rule);

    rule=new Vector();
        rule.add(new Atom("OptTemplate"));
        rule.add(new Atom("OptVarMods"));
        rule.add(new Atom("VarType"));
        rule.add(new Text(" "));
        { Vector realrule=rule; rule=new Vector();         rule.add(new Var("name"));
 realrule.add(new RelElement('^',"Class",(Var)rule.get(0))); rule=realrule; }
        rule.add(new Text("::"));
        rule.add(new Atom("MethName"));
        rule.add(new Text("("));
        rule.add(new Atom("OptArgs"));
        rule.add(new Text(")"));
    ruleset.replacements.put("c",rule);



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
        rule.add(new Text("fillin"));
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
    // Replacements



    // The above should really be split up for methods, variables etc.

    // h: <vartype>
    // c: <vartype>

    // Method names can contain, eg. operator=
    ruleset=new RuleSet("MethName");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Var("varname"," ;(){}"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Var("varname"));
    ruleset.replacements.put("h",rule);

    rule=new Vector();
        rule.add(new Var("varname"));
    ruleset.replacements.put("c",rule);



    ruleset=new RuleSet("OptVarBeing");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
        rule.add(new Text("="));
        rule.add(new Atom("OptWS"));
        rule.add(new Var("varbeing",";"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements



    // VarStuff = <vartype/"\n;{}() ">
    //          | <varstuff/"\n;{}() "> " " <vartype/"\n;{}() ">

    // VarBeing = <varname/"\n;(){} =">
    //          | <varname/"\n;(){} ="> WS "=" <varinit/"\n;">

    //               <vartype/"\n ;{}()"> " " <varname/"=">
    //             | <varstuff/"\n;{}()"> " " <vartype/"\n ;{}()"> " " <varname/"=">
    //             | <vartype/"\n ;{}()"> " " <varname/"="> "=" <varinit/";">
    //             | <varstuff/"\n;{}()"> " " <vartype/"\n ;{}()"> " " <varname/"="> "=" <varinit/";">

    ruleset=new RuleSet("Comment");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("/*"));
        rule.add(new Atom("EndCom"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("//"));
        rule.add(new Var("stuff","\n"));
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Text("//"));
        rule.add(new Var("stuff"));
        rule.add(new Text("\n"));
    ruleset.replacements.put("h",rule);



    // h: "/* " <stuff> EndCom "*/\n"

    // Hack for /* */ since we might have /* blah foo*bar */

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
    rule=new Vector();
        rule.add(new Text(""));
    ruleset.replacements.put("h",rule);



    ruleset=new RuleSet("ClassInside");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptPublicity"));
        rule.add(new Atom("Comment"));
        rule.add(new Atom("ClassInside"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptPublicity"));
        rule.add(new Atom("Method"));
        rule.add(new Atom("ClassInside"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptPublicity"));
        rule.add(new Atom("Include"));
        rule.add(new Atom("ClassInside"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptPublicity"));
        rule.add(new Atom("VariableDecl"));
        rule.add(new Atom("ClassInside"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptPublicity"));
        rule.add(new Atom("Class"));
        rule.add(new Atom("ClassInside"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
    // Replacements



    // Note: Methods given priority because VariableDecl tries to catch operator== !

    // ClassInside = OptPublicity Variables OptPublicity Methods
    //             | OptPublicity Methods

    // Variables = Comment OptWS Variables
    //           | VariableDecl OptWS Variables
    //           | VariableDecl

    // Methods = OptWS Comment Methods
    //         | OptWS Include Methods
    //         | OptWS Method Methods
    //         | OptWS Method

    ruleset=new RuleSet("OptPublicity");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
        rule.add(new Atom("PublicColon"));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
        rule.add(new Atom("PrivateColon"));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
    // Replacements



    ruleset=new RuleSet("PublicColon");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("public:"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Text(""));
    ruleset.replacements.put("c",rule);

    rule=new Vector();
        rule.add(new Text("public:"));
    ruleset.replacements.put("h",rule);



    // c: "public:"

    ruleset=new RuleSet("PrivateColon");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("private:"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Text(""));
    ruleset.replacements.put("c",rule);

    rule=new Vector();
        rule.add(new Text("private:"));
    ruleset.replacements.put("h",rule);



    // c:"private:"

    // Method: Second option is a method declaration

    ruleset=new RuleSet("Method");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("MDecl"));
        rule.add(new Atom("OptWS"));
        rule.add(new Text("{"));
        rule.add(new Atom("Code"));
        rule.add(new Text("}"));
      ruleset.add(rule);
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
        rule.add(new Atom("MDecl"));
        rule.add(new Text(" {"));
        rule.add(new Atom("Code"));
        rule.add(new Text("}\n"));
    ruleset.replacements.put("c",rule);



    // hreplacement: MDecl ";"
    // creplacement: MDecl<stuff> Class<name>::MDecl<name> "(" MDecl<args> ") {" Code "}"
    //             | MDecl "{" Code "}"

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

    ruleset=new RuleSet("ArgType");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("VarType"));
      ruleset.add(rule);
    // Replacements



    ruleset=new RuleSet("VariableDecl");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Atom("OptVarMods"));
        rule.add(new Atom("VarType"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("VarNames"));
        rule.add(new Atom("OptVarBeing"));
        rule.add(new Text(";"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Text("// Variable "));
        rule.add(new Var("VarNames"));
        rule.add(new Text(" declared in .h file"));
    ruleset.replacements.put("c",rule);

    rule=new Vector();
        rule.add(new Atom("OptVarMods"));
        rule.add(new Atom("VarType"));
        rule.add(new Atom("WS"));
        rule.add(new Atom("VarNames"));
        rule.add(new Atom("OptVarBeing"));
        rule.add(new Text("; // Variable declared"));
    ruleset.replacements.put("h",rule);



    // Surely should be: VariableDecl = OptTemplateType OptWS OptVarMods VarType " " VarName OptVarBeing ";"

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



    ruleset=new RuleSet("VarType");
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



    // h: <argname> OptArgs
    // c: <argname> OptArgs

    ruleset=new RuleSet("Code");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Var("out","{}\"/"));
        rule.add(new Atom("RestCode"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Var("other","{}"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Var("out"));
        rule.add(new Atom("RestCode"));
        rule.add(new Var("other"));
    ruleset.replacements.put("c",rule);



    ruleset=new RuleSet("RestCode");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("\""));
        rule.add(new Var("str","\""));
        rule.add(new Text("\""));
        rule.add(new Atom("Code"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Comment"));
        rule.add(new Atom("Code"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("/"));
        rule.add(new Atom("Code"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("InnerCode"));
        rule.add(new Atom("Code"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector();
        rule.add(new Atom("Comment"));
        rule.add(new Atom("InnerCode"));
        rule.add(new Atom("Code"));
    ruleset.replacements.put("c",rule);



    ruleset=new RuleSet("InnerCode");
      rulesets.add(ruleset);
      rule=new Vector();
        rule.add(new Text("{"));
        rule.add(new Atom("Code"));
        rule.add(new Text("}"));
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
        rule.add(new Text("\n"));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text(" "));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Text("\t"));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
      rule=new Vector();
        rule.add(new Atom("Comment"));
        rule.add(new Atom("OptWS"));
      ruleset.add(rule);
    // Replacements




  }
}
