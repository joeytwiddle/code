package tools.parser;

import java.lang.String;
import java.util.Arrays;
import java.util.Vector;
import org.neuralyte.literal.LiteralMap;

import tools.parser.*;
import tools.parser.extensions.*;

public class cGrm extends GrammarHelper {
  public static Grammar setupgrammar() {
    Grammar grammar = new Grammar( new LiteralMap()    //#### Joey's grammar for C++ parsing
    //## The content of code blocks is not currently classified, and probably only a subset of the whole language can currently be matched.
    //## Furthermore it may no longer work since the 2011/12 parser changes!
    //## Its current usage is to split a C class defined in one file
    //## into two separate .h and a .c files, using the "h" and "c" targets.


    //#### Main

    .with("Main", new RuleSet("Main", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("CSrc") } )
      )
    ))

    .with("CSrc", new RuleSet("CSrc", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("CSrcBit"), new Atom("CSrc") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("CSrcBit", new RuleSet("CSrcBit", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("WS") } ),
      Arrays.asList( new Type[]{ new Atom("Special") } ),
      Arrays.asList( new Type[]{ new Atom("Comment") } ),
      Arrays.asList( new Type[]{ new Atom("HashCom") } ),
      Arrays.asList( new Type[]{ new Atom("ClassDecl") } ),
      Arrays.asList( new Type[]{ new Atom("ClassDefn") } ),
      Arrays.asList( new Type[]{ new Atom("Method") } ),
      Arrays.asList( new Type[]{ new Atom("VariableDecl") } )
      )
    ))

    // Note: Methods given priority because VariableDecl tries to catch eg. "operator==" !

    // Doesn't appear to be picking up in joeylib.c

    .with("Special", new RuleSet("Special", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("/*special c*/"), new Atom("EndSpecial") } ),
      Arrays.asList( new Type[]{ new Text("/*special h*/"), new Atom("EndSpecial") } )
      )
    ))

    .with("EndSpecial", new RuleSet("EndSpecial", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("stuff","/"), new Atom("ContSpecial") } )
      )
    ))

    .with("ContSpecial", new RuleSet("ContSpecial", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("/*end special*/") } ),
      Arrays.asList( new Type[]{ new Text("/"), new Atom("EndSpecial") } )
      )
    ))

    //#### Simple comments and #commands

    .with("Comment", new RuleSet("Comment", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("/*"), new Atom("EndCom") } ),
      Arrays.asList( new Type[]{ new Text("//"), new Var("stuff","\n"), new Atom("NL") } ),
      Arrays.asList( new Type[]{ new Text("//"), new Atom("NL") } )
      )
    ))

    // Hack for /* */ to deal with eg. " ... /* blah foo*bar */ ... "
    .with("EndCom", new RuleSet("EndCom", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("stuff","*"), new Text("*/") } ),
      Arrays.asList( new Type[]{ new Var("stuff","*"), new Text("*"), new Atom("EndCom") } )
      )
    ))

    // Although the repeated RHS are unaesthetic,
    // they are OK when parsing, since the RHS
    // is only reached when the LHS is satisfied.
    .with("HashCom", new RuleSet("HashCom", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("#include "), new Atom("HashIncludeLib"), new Atom("OptSpc"), new Atom("NL") } ),
      Arrays.asList( new Type[]{ new Text("#include "), new Atom("HashIncludeFile"), new Atom("OptSpc"), new Atom("NL") } ),
      Arrays.asList( new Type[]{ new Text("#def"), new Var("more","\n"), new Atom("NL") } ),
      Arrays.asList( new Type[]{ new Text("#ifdef"), new Var("more","\n"), new Atom("NL") } ),
      Arrays.asList( new Type[]{ new Text("#"), new Var("other","\n"), new Atom("NL") } )
      )
    ))

    .with("HashIncludeLib", new RuleSet("HashIncludeLib", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("<"), new Var("file","."), new Text(".c>") } ),
      Arrays.asList( new Type[]{ new Text("<"), new Var("file","."), new Text(".h>") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("h", Arrays.asList(new Text("<") , new Var("file") , new Text(".h>")))

    ))

    .with("HashIncludeFile", new RuleSet("HashIncludeFile", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("\""), new Var("file","."), new Text(".c\"") } ),
      Arrays.asList( new Type[]{ new Text("\""), new Var("file","."), new Text(".h\"") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("h", Arrays.asList(new Text("\"") , new Var("file") , new Text(".h\"")))

    ))



    //#### Classes

    // However, the class declaration vs. definition is
    // inefficient, since the LHS is searched in both cases.
    // However, it is neccessary to classify this LHS
    // data according the the resulting match on the RHS.

    .with("ClassDecl", new RuleSet("ClassDecl", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptClassTemplate"), new Text("class"), new Atom("WS"), new Var("name"," =(){};"), new Text(";") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("h", Arrays.asList(new Atom("OptClassTemplate") , new Text(" class ") , new Var("name") , new Text(";")))

        .with("c", Arrays.asList(new Text("// Class declaration of existence: ") , new Var("name") , new Text("\n")))

    ))

    .with("ClassDefn", new RuleSet("ClassDefn", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("TemplateClass") } ),
      Arrays.asList( new Type[]{ new Atom("Class") } )
      )
    ))

    .with("TemplateClass", new RuleSet("TemplateClass", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ClassTemplate"), new Atom("Class") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("h", Arrays.asList(new Text("// Template class appears in original format:\n") , new ActiveReplacement() { public String replace(OutputContext ctx) {  jlib.strings.FakeOutputStream out=new jlib.strings.FakeOutputStream(); match.render(ctx,null,"none",new java.io.PrintStream(out)); return ""+out.store;  } } , new Text("// End template class\n")))

        .with("c", Arrays.asList(new Text("// Template class must appear in .h file\n")))

    ))

    .with("Class", new RuleSet("Class", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptClassMods"), new Text("class "), new Var("name","\n ;{}()"), new Atom("WS"), new Atom("OptMoreClassMods"), new Text("{"), new Atom("ClassInside"), new Text("};") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("h", Arrays.asList(new Atom("OptClassMods") , new Text("class ") , new Var("name") , new Atom("WS") , new Atom("OptMoreClassMods") , new Text("{") , new Atom("ClassInside") , new Text("};")))

        .with("c", Arrays.asList(new Text("// Starts of class ") , new Var("name") , new Text("\n") , new Atom("ClassInside") , new Text("// End class ") , new Var("name") , new Text("\n")))

    ))

    .with("OptMoreClassMods", new RuleSet("OptMoreClassMods", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("moremods","\n;{}()") } )
      )
    ))

    .with("OptClassMods", new RuleSet("OptClassMods", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ClassMod"), new Atom("WS"), new Atom("OptClassMods") } ),
      Arrays.asList( new Type[]{ new Atom("OptWS") } )
      )
    ))

    .with("ClassMod", new RuleSet("ClassMod", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("public") } ),
      Arrays.asList( new Type[]{ new Text("private") } )
      )
    ))

    .with("ClassInside", new RuleSet("ClassInside", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Publicity"), new Atom("WS"), new Atom("ClassInside") } ),
      Arrays.asList( new Type[]{ new Atom("CSrcBit"), new Atom("ClassInside") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("Publicity", new RuleSet("Publicity", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("public:") } ),
      Arrays.asList( new Type[]{ new Text("private:") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("c", Arrays.asList(new Text("")))

    ))

    .with("OptClassTemplate", new RuleSet("OptClassTemplate", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ClassTemplate") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("ClassTemplate", new RuleSet("ClassTemplate", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Template") } )
      )
    ))



    //#### Templates

    // 1) Templates used to declare a template class or method

    .with("OptTemplate", new RuleSet("OptTemplate", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Template") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("Template", new RuleSet("Template", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptWS"), new Text("template"), new Atom("OptWS"), new Text("<"), new Atom("TemplateSuperInside"), new Text(">"), new Atom("OptWS") } )
      )
    ))

    .with("TemplateSuperInside", new RuleSet("TemplateSuperInside", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptWS"), new Text("class"), new Atom("WS"), new Atom("TemplateInside") } )
      )
    ))

    // 2) Templates used to declare a template object instance

    .with("OptTemplateType", new RuleSet("OptTemplateType", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("TemplateType") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("TemplateType", new RuleSet("TemplateType", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptWS"), new Var("temptype"," (){}<>,"), new Text("<"), new Atom("TemplateTypeInner"), new Atom("OptPtr"), new Atom("OptWS"), new Text(">") } )
      )
    ))

    // Note: recursion to deal with eg. " List< List<int> > listOfLists(); "
    .with("TemplateTypeInner", new RuleSet("TemplateTypeInner", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("TemplateType") } ),
      Arrays.asList( new Type[]{ new Atom("TemplateInside") } )
      )
    ))

    // Took comma out to deal with eg. "template <class A,class B>"
    .with("TemplateInside", new RuleSet("TemplateInside", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("inside","<>(){}") } )
      )
    ))



    //#### Methods

    // Second case is constructor (no type)
    .with("MDecl", new RuleSet("MDecl", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptTemplate"), new Atom("OptVarMods"), new Atom("OptWS"), new Atom("VarType"), new Atom("WS"), new Atom("OptPtr"), new Atom("OptLater"), new Atom("MethName"), new Atom("OptWS"), new Text("("), new Atom("OptArgs"), new Text(")") } ),
      Arrays.asList( new Type[]{ new Atom("OptTemplate"), new Atom("OptPtr"), new Atom("OptLater"), new Atom("MethName"), new Atom("OptWS"), new Text("("), new Atom("OptArgs"), new Text(")") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("h", Arrays.asList(new Atom("OptTemplate") , new Atom("OptVarMods") , new Atom("VarType") , new Text(" ") , new Atom("OptPtr") , new Atom("OptLater") , new Atom("MethName") , new Text("(") , new Atom("OptArgs") , new Text(")")))

        .with("c", Arrays.asList(new RelElement('^',"TemplateClass",new Atom("ClassTemplate")) , new Atom("OptTemplate") , new Atom("OptVarMods") , new Atom("VarType") , new Text(" ") , new Atom("OptPtr") , new RelElement('^',"Class",new Var("name")) , new ActiveReplacement() { public String replace(OutputContext ctx) {  Match m=match.getUp("Class").getDown("TemplateClass").getDown("TemplateSuperInside").getDown("TemplateInside"); return "<"+m.string+">";  } } , new ActiveReplacement() { public String replace(OutputContext ctx) {  return ( match.getUp("Class")==null ? "" : "::" );  } } , new Atom("OptLater") , new Atom("MethName") , new Text("(") , new Atom("OptArgs") , new Text(")")))

    ))

    // ` return ( match.getUp("Class")==null ? "extern " : "" ); ` 
    // c: Class^OptTemplate OptTemplate OptVarMods VarType " " OptPtr Class^<name> ` Match m=match.getUp("Class").getDown("ClassOptTemplate").getDown("TemplateSuperInside").getDown("TemplateInside"); return "<"+m.string+">"; ` ` return ( match.getUp("Class")==null ? "/* no class */" : "::" ); ` OptLater MethName "(" OptArgs ")"
    // h: ` if ( ! match.getDown("OptLater").string.toString().equals("") ) { unusedmatches.clear(); return "/* Skipping >"+match.getDown("OptLater").string+"< */\n"; } return "/* =) */"; ` OptTemplate OptVarMods VarType " " MethName "(" OptArgs ")"

    .with("OptPtr", new RuleSet("OptPtr", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptWS"), new Text("*"), new Atom("OptWS") } ),
      Arrays.asList( new Type[]{ new Atom("OptWS") } )
      )
    ))

    .with("OptLater", new RuleSet("OptLater", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("MethName"), new Text("::") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    // Method names can contain, eg. operator=
    .with("MethName", new RuleSet("MethName", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("operator"), new Var("op"," ()") } ),
      Arrays.asList( new Type[]{ new Var("varname"," =:;(){}") } )
      )
    ))

    .with("Method", new RuleSet("Method", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("MethodWithCode") } ),
      Arrays.asList( new Type[]{ new Atom("MethodWithoutCode") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("h", Arrays.asList(new ActiveReplacement() { public String replace(OutputContext ctx) {  return ( match.getDown("OptLater").string.toString().equals("") ? "" : "/* Skipping later :: " );  } } , new Atom("MethodWithCode") , new Atom("MethodWithoutCode") , new ActiveReplacement() { public String replace(OutputContext ctx) {  return ( match.getDown("OptLater").string.toString().equals("") ? "" : "*/\n" );  } }))

    ))

    .with("MethodWithCode", new RuleSet("MethodWithCode", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("MDecl"), new Atom("OptWS"), new Text("{"), new Atom("Code"), new Text("}") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("h", Arrays.asList(new Atom("MDecl") , new Text("; // Method\n")))

    ))

    .with("MethodWithoutCode", new RuleSet("MethodWithoutCode", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("MDecl"), new Atom("OptWS"), new Text(";") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("h", Arrays.asList(new Atom("MDecl") , new Text("; // Method\n")))

        .with("c", Arrays.asList(new Text("/* Skipping declared method ") , new Atom("MDecl") , new Text(" */\n")))

    ))



    //#### Code - not yet fully parsed

    .with("Code", new RuleSet("Code", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("code","{}\"/"), new Atom("RestCode"), new Atom("Code") } ),
      Arrays.asList( new Type[]{ new Var("other","{}") } )
      )
    ))

    .with("RestCode", new RuleSet("RestCode", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("String") } ),
      Arrays.asList( new Type[]{ new Atom("Comment") } ),
      Arrays.asList( new Type[]{ new Text("/") } ),
      Arrays.asList( new Type[]{ new Atom("InnerCode") } )
      )
    ))

    .with("InnerCode", new RuleSet("InnerCode", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("{"), new Atom("Code"), new Text("}") } )
      )
    ))



    //#### Variables

    .with("OptVarMods", new RuleSet("OptVarMods", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptVarMod"), new Atom("OptWS"), new Atom("OptVarMods") } ),
      Arrays.asList( new Type[]{ new Atom("OptVarMod"), new Atom("OptWS") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    // fillin is one of my #defs, abstract may be Java only, not C
    // These should really be split up for methods, variables etc.
    .with("OptVarMod", new RuleSet("OptVarMod", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("public") } ),
      Arrays.asList( new Type[]{ new Text("private") } ),
      Arrays.asList( new Type[]{ new Text("static") } ),
      Arrays.asList( new Type[]{ new Text("virtual") } ),
      Arrays.asList( new Type[]{ new Text("abstract") } ),
      Arrays.asList( new Type[]{ new Text("extern") } ),
      Arrays.asList( new Type[]{ new Text("unsigned") } ),
      Arrays.asList( new Type[]{ new Text("signed") } ),
      Arrays.asList( new Type[]{ new Text("const") } ),
      Arrays.asList( new Type[]{ new Text("static") } ),
      Arrays.asList( new Type[]{ new Text("struct") } ),
      Arrays.asList( new Type[]{ new Atom("Fillin") } ),
      Arrays.asList( new Type[]{ new Atom("Fillup") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("c", Arrays.asList(new ActiveReplacement() { public String replace(OutputContext ctx) {  String val=match.string.toString(); if ( val.equals("virtual") || val.equals("static") || val.equals("fillin") /* && match.getUp("Class")==null */ ) return ""; else return ""+match.string;  } }))

    ))

    //          | "fillin"
    //          | "fillup"

    .with("Fillin", new RuleSet("Fillin", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("fillin") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("c", Arrays.asList(new Text("// fillin")))

    ))

    .with("Fillup", new RuleSet("Fillup", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("fillup") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("h", Arrays.asList(new Text("// fillup")))

    ))

    .with("VarBeing", new RuleSet("VarBeing", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptWS"), new Text("="), new Atom("OptWS"), new Var("varbeing",";") } )
      )
    ))

    // Method: Second option is a method declaration

    .with("VariableDecl", new RuleSet("VariableDecl", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("VariableDeclBeing") } ),
      Arrays.asList( new Type[]{ new Atom("VariableDeclExist") } )
      )
    ))

    .with("VariableDeclExist", new RuleSet("VariableDeclExist", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptVarMods"), new Atom("VarType"), new Atom("WS"), new Atom("VarNames"), new Text(";") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("h", Arrays.asList(new Atom("OptVarMods") , new ActiveReplacement() { public String replace(OutputContext ctx) {  return ( match.getUp("Class")==null ? "extern " : "" );  } } , new Atom("VarType") , new Atom("WS") , new Atom("VarNames") , new Text("; // Exists")))

        .with("c", Arrays.asList(new ActiveReplacement() { public String replace(OutputContext ctx) {  return ( match.getUp("Class")==null ? match.renderString(ctx,"normal") : "// Variable declared in .h file" );  } }))

    ))

    // NB: If you try putting OptVarMods after `...` you may end up with "extern // fillup ..." in the output :-(

    .with("VariableDeclBeing", new RuleSet("VariableDeclBeing", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptVarMods"), new Atom("VarType"), new Atom("WS"), new Atom("VarNames"), new Atom("VarBeing"), new Text(";") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("h", Arrays.asList(new Atom("OptVarMods") , new ActiveReplacement() { public String replace(OutputContext ctx) {  return ( match.getUp("Class")==null ? "extern " : "" );  } } , new Atom("VarType") , new Atom("WS") , new Atom("VarNames") , new Text("; // Variable initialised in .c file")))

        .with("c", Arrays.asList(new Atom("OptVarMods") , new Atom("VarType") , new Atom("WS") , new RelElement('^',"Class",new Var("name")) , new ActiveReplacement() { public String replace(OutputContext ctx) {  Match cls=match.getUp("Class"); return ( cls==null ? "" : "::" );  } } , new Atom("VarNames") , new Atom("VarBeing") , new Text(";")))

    ))

    .with("VarType", new RuleSet("VarType", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ArgType") } )
      )
    ))

    .with("VarNames", new RuleSet("VarNames", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("VarName"), new Atom("OptWS"), new Text(","), new Atom("OptWS"), new Atom("VarNames") } ),
      Arrays.asList( new Type[]{ new Atom("VarName"), new Atom("OptWS") } )
      )
    ))



    //#### Arguments and types

    .with("ArgType", new RuleSet("ArgType", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptArgMods"), new Atom("OptWS"), new Atom("TemplateType") } ),
      Arrays.asList( new Type[]{ new Atom("OptArgMods"), new Atom("OptWS"), new Var("vartype"," <>\n;(){},") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("OptArgMods", new RuleSet("OptArgMods", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptVarMods") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    // need to avoid = or it will be read!
    .with("VarName", new RuleSet("VarName", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ArgName") } )
      )
    ))

    .with("OptArgs", new RuleSet("OptArgs", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Args") } ),
      Arrays.asList( new Type[]{ new Atom("OptWS") } )
      )
    ))

    .with("Args", new RuleSet("Args", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("DoubleArg") } ),
      Arrays.asList( new Type[]{ new Atom("Arg") } )
      )
    ))

    .with("DoubleArg", new RuleSet("DoubleArg", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Arg"), new Text(","), new Atom("Args") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("h", Arrays.asList(new Atom("Arg") , new Text(",") , new Atom("Args")))

        .with("c", Arrays.asList(new Atom("Arg") , new Text(",") , new Atom("Args")))

    ))

    .with("Arg", new RuleSet("Arg", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptWS"), new Atom("ArgA"), new Atom("OptWS") } )
      )
    ))

    .with("ArgA", new RuleSet("ArgA", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ArgType"), new Atom("WS"), new Atom("ArgName") } ),
      Arrays.asList( new Type[]{ new Atom("ArgType") } ),
      Arrays.asList( new Type[]{ new Text("...") } )
      )
    ))

    // h: <argtype> " " ArgName
    // c: <argtype> " " ArgName

    // ArgType = VarType OptTemplateType

    .with("ArgName", new RuleSet("ArgName", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("("), new Var("argname"," ;=(){},<>\n"), new Text(")"), new Atom("OptWS"), new Text("("), new Atom("OptArgs"), new Text(")") } ),
      Arrays.asList( new Type[]{ new Var("argname"," ;=(){},<>\n"), new Atom("OptWS"), new Text("("), new Atom("OptArgs"), new Text(")") } ),
      Arrays.asList( new Type[]{ new Var("argname"," ;=(){},<>\n") } )
      )
    ))



    //#### And everything else

    .with("String", new RuleSet("String", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("\""), new Var("str","\""), new Text("\"") } )
      )
    ))

    .with("OptWS", new RuleSet("OptWS", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("WS") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("WS", new RuleSet("WS", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("NL"), new Atom("OptWS") } ),
      Arrays.asList( new Type[]{ new Atom("Comment"), new Atom("OptWS") } ),
      Arrays.asList( new Type[]{ new Atom("SpcBit"), new Atom("OptWS") } )
      )
    ))

    .with("NL", new RuleSet("NL", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("\r\n") } ),
      Arrays.asList( new Type[]{ new Text("\n") } )
      )
    ))

    .with("OptSpc", new RuleSet("OptSpc", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("SpcBit"), new Atom("OptSpc") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("SpcBit", new RuleSet("SpcBit", Arrays.asList(
        Arrays.asList( new Type[]{ new Text(" ") } ),
      Arrays.asList( new Type[]{ new Text("\t") } )
      )
    ))



    );
    return grammar;
  }
}
