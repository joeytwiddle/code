package tools.parser;

import java.lang.String;
import java.util.Arrays;
import java.util.Vector;
import org.neuralyte.literal.LiteralMap;

import tools.parser.*;
import tools.parser.extensions.*;

public class dlangGrm extends GrammarHelper {
  public static Grammar setupgrammar() {
    Grammar grammar = new Grammar( new LiteralMap()    // DLang - A dumb language
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
    //   Define terminator symbol, e.g. "\n" | ";"

    // This is not exactly minimal, but we'd like a nicer way to access the last
    // element in an array than l[l.length-1]

    // @OPTION wrap_meaningful_indentation "{" "}"
    // @todo grammar.addPreProcessor(new WrapMeaningfulIndentation("{","}"));    // Wrapping indented blocks in {...} is one way to make them easily visible to the grammar/parse.  Another way would be to detect and mark only OUTDENTs.  These could be used to catch the end of a block when expected, or ignored when uninteresting (e.g. sometimes long expressions are broken onto lines and indented).

    // Many languages suffer from stuff that is irrelevant.  E.g. whitespace and comments.  Usually wherever we accept one we also accept the other.  So WS searches are in fact incorrect.  Recommend creating a minimalist gramelle for this: _ = (WS | Comment)+
    // We could attempt to swallow unneccessary braces "{" in multi-line expressions "}" using _e = (WS | Comment | RedundantBrace)*

    // @todo grammar.importNamespace("DExpression","dexpr.grm");

    .with("Main", new RuleSet("Main", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("DLangFile") } )
      )
    ))

    .with("DLangFile", new RuleSet("DLangFile", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("DLangHeader"), new Atom("DLangFileMain") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("package dlang.parsed;\n\nimport java.lang.String;\nimport java.util.Vector;\n\nimport dlang.types.*;\n\npublic class DLangFile {\n  public static void build() {\n    \n\n") , new Atom("DLangHeader") , new Atom("DLangFileMain") , new Text("  }\n}\n")))

    ))

    // DLangHeader = DLangModuleBlock DLangImports

    // DLangModuleBlock = Comment DLangModuleBlock
    //                  | Space DLangModuleBlock
    //                  | DLangModule
    //                  | ""

    .with("DLangHeader", new RuleSet("DLangHeader", Arrays.asList(
        Arrays.asList( new Type[]{ new RepeatedRule(new Atom("Comment"),0,1), new RepeatedRule(new Atom("DLangModule"),0,1), new RepeatedRule(new Atom("Comment"),0,1), new RepeatedRule(new Atom("DLangImports"),0,1) } )
      )
    ))
    // DLangHeader = (WS Comment)* WS [DLangModule] (WS | Comment | DLangImportProper)*

    .with("DLangModule", new RuleSet("DLangModule", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("package"), new Atom("Space"), new Var("module","\n;"), new Atom("EOL") } ),
      Arrays.asList( new Type[]{ new Text("module"), new Atom("Space"), new Var("module","\n;"), new Atom("EOL") } )
      )
    ))

    // DLangImports = DLangImport [ DLangImports ]

    // Bad defn but gets caught - try it!
    // DLangImports = ( DLangImport | WS )*

    .with("DLangImports", new RuleSet("DLangImports", Arrays.asList(
        Arrays.asList( new Type[]{ new RepeatedRule(new GroupedDefn(new RuleSet("Anonymous", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("DLangImport") } ),
      Arrays.asList( new Type[]{ new Atom("Space") } )
      ))),"*") } )
      )
    ))

    .with("DLangImport", new RuleSet("DLangImport", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("DLangImportProper") } ),
      Arrays.asList( new Type[]{ new Atom("Comment") } ),
      Arrays.asList( new Type[]{ new Atom("Space") } )
      )
    ))

    .with("DLangImportProper", new RuleSet("DLangImportProper", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("import"), new Atom("Space"), new Var("import","\n"), new Text("\n") } )
      )
    ))



    // TODO: Comments might perceivably be 0 chars long, but <var> demands > 0
    .with("Comment", new RuleSet("Comment", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("#"), new Var("comment","\n"), new Text("\n") } ),
      Arrays.asList( new Type[]{ new Text("//"), new Var("comment","\n"), new Text("\n") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("\n/*") , new Var("comment") , new Text(" */\n")))

    ))

    // TODO: Parametrised classes (templates)
    .with("ClassDefinition", new RuleSet("ClassDefinition", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("class"), new Atom("Space"), new Var("className"," \n"), new Atom("OptClassMods"), new Atom("NL"), new Atom("ClassBody") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("new ClassDefinition(\"") , new Atom("className") , new Text("\", ") , new Atom("ClassBody") , new Text(", \"") , new Atom("OptClassMods") , new Text("\")")))

    ))

    .with("OptClassMods", new RuleSet("OptClassMods", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Space"), new Text("extends"), new Atom("Space"), new Atom("ClassRef") } ),
      Arrays.asList( new Type[]{ new Atom("Space"), new Text("implements"), new Atom("Space"), new Atom("ClassRef") } ),
      Arrays.asList( new Type[]{ new Atom("OptHorizSpace") } )
      )
    ))

    .with("ClassBody", new RuleSet("ClassBody", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("[]")))

    ))

    // ClassBody = IndentedDLangBlock

    // TODO: I guess it will be "{" OWS DLangFileSome OWS "}"
    .with("IndentedDLangBlock", new RuleSet("IndentedDLangBlock", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("DLangFileSome") } )
      )
    ))

    .with("FunctionDefinition", new RuleSet("FunctionDefinition", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("fnname"," ("), new Text("("), new Atom("ArgumentSignatureList"), new Text(")"), new Atom("WS"), new Text("="), new Atom("WS"), new Atom("FunctionBody") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("new Function(\"") , new Var("fnname") , new Text("\", {") , new Atom("ArgumentSignatureList") , new Text("}, ") , new Atom("FunctionBody") , new Text(")")))

    ))

    .with("FunctionBody", new RuleSet("FunctionBody", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("NiceCode") } ),
      Arrays.asList( new Type[]{ new Atom("MutableCodeBlock") } )
      )
    ))

    //# NiceCode swallowed the var from an assignment statement
    //# Should NiceCode demand that it ends neatly with an EOL?
    // FunctionBody = MutableCode

    // NiceCode = Expression Space "where" Space NiceNEDefs EOL
    //          | Expression EOL

    .with("NiceCode", new RuleSet("NiceCode", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Expression"), new GroupedDefn(new RuleSet("Anonymous", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Space"), new Text("where"), new Atom("Space"), new Atom("NiceNEDefs"), new Atom("EOL") } ),
      Arrays.asList( new Type[]{ new Atom("EOL") } )
      ))) } )
      )
    ))

    .with("NiceNEDefs", new RuleSet("NiceNEDefs", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("NamedExpression"), new Text("\n"), new Atom("NiceNEDefs") } ),
      Arrays.asList( new Type[]{ new Atom("NamedExpression") } )
      )
    ))

    .with("ArgumentSignatureList", new RuleSet("ArgumentSignatureList", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ArgumentSignature"), new Atom("MoreArgs") } ),
      Arrays.asList( new Type[]{ new Atom("ArgumentSignature") } ),
      Arrays.asList( new Type[]{ new Atom("WS") } )
      )
    ))

    // ArgumentSignature = TypedArg | UntypedArg
    .with("ArgumentSignature", new RuleSet("ArgumentSignature", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("UntypedArg") } )
      )
    ))

    .with("UntypedArg", new RuleSet("UntypedArg", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("WS"), new Atom("VarName"), new Atom("WS") } )
      )
    ))

    .with("TypedArg", new RuleSet("TypedArg", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("WS"), new Atom("Type"), new Atom("Space"), new Atom("VarName"), new Atom("WS") } )
      )
    ))

    .with("MoreArgs", new RuleSet("MoreArgs", Arrays.asList(
        Arrays.asList( new Type[]{ new Text(","), new Atom("ArgumentSignatureList") } )
      )
    ))

    // Identifier = <varname/"<>\n\". :^+-*()/">
    .with("Identifier", new RuleSet("Identifier", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("varname",null,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$@0-9") } )
      )
    ))

    .with("VarName", new RuleSet("VarName", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Identifier") } )
      )
    ))

    .with("ClassName", new RuleSet("ClassName", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Identifier") } )
      )
    ))

    .with("VarReference", new RuleSet("VarReference", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("VarName") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("new VarReference(\"") , new Atom("VarName") , new Text("\")")))

    ))

    // Could also be called a PrimitiveLiteral
    .with("ConstReference", new RuleSet("ConstReference", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Number") } ),
      Arrays.asList( new Type[]{ new Atom("String") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("Constants.getConstRef(") , new Atom("Number") , new Atom("String") , new Text(")")))

    ))



    .with("DLangFileMain", new RuleSet("DLangFileMain", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("MutableCodeBlock") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("MutableCodeBlock allTheCode = ") , new Atom("MutableCodeBlock") , new Text(";")))

    ))

    .with("MutableCodeBlock", new RuleSet("MutableCodeBlock", Arrays.asList(
        Arrays.asList( new Type[]{ new RepeatedRule(new Atom("MutableCodeInner"),"*") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("new MutableCodeBlock( newList()") , new ArgReplacement(1) , new Text(" )")))

    ))
    // dintj: "new MutableCodeBlock({" MutableCodeInner "})"
    // dintj: "new MutableCodeBlock( newList()" MutableCodeInner " )"

    // Note we test FunctionDefinition before FunctionCall - they both look similar at the start!
    // This now means NonStatement must be tested before Statement.

    .with("MutableCodeInner", new RuleSet("MutableCodeInner", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Space") } ),
      Arrays.asList( new Type[]{ new Atom("Comment") } ),
      Arrays.asList( new Type[]{ new Atom("RelevantCodeBit") } )
      )
    ))

    .with("RelevantCodeBit", new RuleSet("RelevantCodeBit", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("NonStatement") } ),
      Arrays.asList( new Type[]{ new Atom("Statement") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("      .with(") , new ArgReplacement(1) , new Text(")\n")))

    ))
    // dintj: Space NonStatement Statement ", "
    // dintj: ".concat(" NonStatement | Statement ")"

    .with("Statement", new RuleSet("Statement", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Assignment") } ),
      Arrays.asList( new Type[]{ new Atom("FunctionCall") } ),
      Arrays.asList( new Type[]{ new Atom("Loop") } )
      )
    ))

    .with("NonStatement", new RuleSet("NonStatement", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ClassDefinition") } ),
      Arrays.asList( new Type[]{ new Atom("FunctionDefinition") } )
      )
    ))



    .with("Assignment", new RuleSet("Assignment", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("NormalAssignment") } ),
      Arrays.asList( new Type[]{ new Atom("SpecialAssignment") } )
      )
    ))

    .with("NormalAssignment", new RuleSet("NormalAssignment", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("VarOrMemberReference"), new Atom("OptHorizSpace"), new Text("="), new Atom("Expression"), new Atom("EOL") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("new NormalAssignment(") , new Atom("VarOrMemberReference") , new Text(",") , new Atom("Expression") , new Text(")")))

    ))

    .with("SpecialAssignment", new RuleSet("SpecialAssignment", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("VarOrMemberReference"), new Atom("OptHorizSpace"), new Atom("SpecialAssignmentOperator"), new Atom("Expression"), new Atom("EOL") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("new SpecialAssignment(") , new Atom("VarOrMemberReference") , new Text(",\"") , new Atom("SpecialAssignmentOperator") , new Text("\",") , new Atom("Expression") , new Text(")")))

    ))

    .with("SpecialAssignmentOperator", new RuleSet("SpecialAssignmentOperator", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("+=") } ),
      Arrays.asList( new Type[]{ new Text("-=") } ),
      Arrays.asList( new Type[]{ new Text("*=") } ),
      Arrays.asList( new Type[]{ new Text("/=") } )
      )
    ))

    // ConstReference must come first because ATM VarOrMemberReference swallows numbers!

    .with("ExpressionTerminal", new RuleSet("ExpressionTerminal", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ConstReference") } ),
      Arrays.asList( new Type[]{ new Atom("VarOrMemberReference") } )
      )
    ))

    .with("Expression", new RuleSet("Expression", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ExpressionBit"), new Atom("OptHorizSpace") } )
      )
    ))

    // We have a problem with Algebra
    // If I let Algebra contain Expressions, then it calls its repeatedly!

    .with("ExpressionBit", new RuleSet("ExpressionBit", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("HorizSpace"), new Atom("ExpressionBit") } ),
      Arrays.asList( new Type[]{ new Atom("Algebra") } ),
      Arrays.asList( new Type[]{ new Atom("ExpressionAtom") } )
      )
    ))

    .with("ExpressionAtom", new RuleSet("ExpressionAtom", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("HorizSpace"), new Atom("ExpressionAtom") } ),
      Arrays.asList( new Type[]{ new Atom("FunctionCall") } ),
      Arrays.asList( new Type[]{ new Atom("BracketedExpression") } ),
      Arrays.asList( new Type[]{ new Atom("ExpressionTerminal") } )
      )
    ))

    // TODO: There is no support here for calling the results of expressions (or their members) :F
    // If it is impossible to replace VarOrMemberReference with Expression because
    // of infinite recursion, then perhaps OptFinalCall should be available at the
    // end of any expression (or expression bit), as a postfix, the same way we
    // dealt with + and * in grm.

    .with("FunctionCall", new RuleSet("FunctionCall", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ConstructorCall") } ),
      Arrays.asList( new Type[]{ new Atom("RealFunctionCall") } )
      )
    ))

    .with("RealFunctionCall", new RuleSet("RealFunctionCall", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("VarOrMemberReference"), new Text("("), new Atom("ArgumentParameterList"), new Text(")") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("new FunctionCall(") , new Atom("VarOrMemberReference") , new Text(", [") , new Atom("ArgumentParameterList") , new Text("])")))

    ))

    .with("ConstructorCall", new RuleSet("ConstructorCall", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("new"), new Atom("Space"), new Atom("RealFunctionCall") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("new ConstructorCall(") , new Atom("RealFunctionCall") , new Text(")")))

    ))

    .with("MemberName", new RuleSet("MemberName", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("VarName") } )
      )
    ))

    // Could be renamed: CompoundReference
    //
    // NOTE: The dot (".") dereference "operator" may conflict with the general
    // customizable "." operator.
    // Unless we specify that the dereferencer may not be surrounded by space,
    // whilst general operators may.
    //
    // TODO: Both when calling functions, and making assignments, the target may be
    // the result of any expression, although for assignments it must end in a var
    // or property or indexed item, no?
    //
    //   e.g. field1.getCows( \(cow) -> cow.age < 2 ).health++;
    //
    //   but not field1.getCows( \(cow) -> cow.age < 2 ) = Farm.getFreshCowList();
    //
    // Better to create instead TargettableReference for assignments, and ensure it ends in a property/varref.
    //
    // FunctionCall is difficult to parse, if we say the fn can be an expression.
    // Hmm...  maybe not so hard, we must just check for () in/after Expression matches.

    .with("VarOrMemberReference", new RuleSet("VarOrMemberReference", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("VarReference"), new Text("."), new Atom("VarOrMemberReference") } ),
      Arrays.asList( new Type[]{ new Atom("VarReference") } )
      )
    ))

    //                      | VarName

    .with("ArgumentParameterList", new RuleSet("ArgumentParameterList", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Expression"), new Text(","), new Atom("ArgumentParameterList") } ),
      Arrays.asList( new Type[]{ new Atom("Expression") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("BracketedExpression", new RuleSet("BracketedExpression", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("("), new Atom("WS"), new Atom("ExpressionBit"), new Atom("WS"), new Text(")") } )
      )
    ))

    //# Can lock us in a cyclic loop ExpressionBit -> Algebra -> ExpressionBit !
    // Algebra = ExpressionBit Operator ExpressionBit

    .with("Algebra", new RuleSet("Algebra", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ExpressionAtom"), new Atom("OptHorizSpace"), new Atom("Operator"), new Atom("OptHorizSpace"), new Atom("Expression") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("new Operation(") , new Atom("ExpressionAtom") , new Text(",") , new Atom("Operator") , new Text(",") , new Atom("Expression") , new Text(")")))

    ))

    .with("TerminalAlgebraicOperation", new RuleSet("TerminalAlgebraicOperation", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ExpressionAtom"), new Atom("OptHorizSpace"), new Atom("Operator"), new Atom("OptHorizSpace"), new Atom("Expression") } )
      )
    ))

    .with("Operator", new RuleSet("Operator", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OperatorSymbol") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("Operators.getOperator(\"") , new Atom("OperatorSymbol") , new Text("\")")))

    ))
    // dintj: "new Operator(\"" OperatorSymbol "\")"

    // We can't do: Constants.getOperator("*") here because not all are defined by
    // default, i.e. there may be an operator with a custom name.  I suppose
    // context.lookupOperator("*") might work.  For the moment though, we refer to
    // it by string, which would allow for operators to be defined after their first
    // appearance in the file.

    .with("OperatorSymbol", new RuleSet("OperatorSymbol", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("BooleanOperator") } ),
      Arrays.asList( new Type[]{ new Atom("AlgebraicOperator") } ),
      Arrays.asList( new Type[]{ new Atom("OtherOperator") } )
      )
    ))

    .with("AlgebraicOperator", new RuleSet("AlgebraicOperator", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("*") } ),
      Arrays.asList( new Type[]{ new Text("/") } ),
      Arrays.asList( new Type[]{ new Text("+") } ),
      Arrays.asList( new Type[]{ new Text("-") } ),
      Arrays.asList( new Type[]{ new Text("^") } ),
      Arrays.asList( new Type[]{ new Text("<<") } ),
      Arrays.asList( new Type[]{ new Text(">>") } ),
      Arrays.asList( new Type[]{ new Text("&") } ),
      Arrays.asList( new Type[]{ new Text("|") } )
      )
    ))

    .with("BooleanOperator", new RuleSet("BooleanOperator", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("==") } ),
      Arrays.asList( new Type[]{ new Text("!=") } ),
      Arrays.asList( new Type[]{ new Text(">") } ),
      Arrays.asList( new Type[]{ new Text("<") } ),
      Arrays.asList( new Type[]{ new Text(">=") } ),
      Arrays.asList( new Type[]{ new Text("<=") } ),
      Arrays.asList( new Type[]{ new Text("&&") } ),
      Arrays.asList( new Type[]{ new Text("||") } )
      )
    ))

    .with("OtherOperator", new RuleSet("OtherOperator", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("<"), new Var("varname"," \n\t,."), new Text(">") } )
      )
    ))


    .with("VarDeclaration", new RuleSet("VarDeclaration", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("VarDeclarationAndAssignment") } ),
      Arrays.asList( new Type[]{ new Atom("VarDeclarationWithoutAssignment") } )
      )
    ))

    .with("VarDeclarationWithoutAssignment", new RuleSet("VarDeclarationWithoutAssignment", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("var"), new Atom("Space"), new Atom("VarName") } )
      )
    ))

    //                                 | Type Space VarName
    //                                 | "var" Space VarName WS ":" WS Type
    //                                 | VarName WS ":" WS Type

    .with("Type", new RuleSet("Type", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("TypeWithoutTemplate") } ),
      Arrays.asList( new Type[]{ new Atom("TypeWithTemplate") } )
      )
    ))

    .with("TypeWithoutTemplate", new RuleSet("TypeWithoutTemplate", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Identifier") } )
      )
    ))

    .with("TypeWithTemplate", new RuleSet("TypeWithTemplate", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("TypeWithoutTemplate"), new Atom("WS"), new Atom("Template") } )
      )
    ))

    .with("Template", new RuleSet("Template", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("<"), new Atom("TemplateBits"), new Text(">") } )
      )
    ))

    .with("TemplateBits", new RuleSet("TemplateBits", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("WS"), new Atom("TemplateBit"), new Atom("WS"), new Text(","), new Atom("WS"), new Atom("TemplateBits") } ),
      Arrays.asList( new Type[]{ new Atom("WS"), new Atom("TemplateBit") } )
      )
    ))

    .with("VarDeclarationAndAssignment", new RuleSet("VarDeclarationAndAssignment", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("VarDeclarationWithoutAssignment"), new Atom("WS"), new Text("="), new Atom("WS"), new Atom("Expression") } )
      )
    ))

    .with("Number", new RuleSet("Number", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Float") } ),
      Arrays.asList( new Type[]{ new Atom("Int") } )
      )
    ))

    .with("Float", new RuleSet("Float", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Digits"), new Text("."), new Atom("Digits") } )
      )
    ))

    .with("Int", new RuleSet("Int", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Digits") } )
      )
    ))

    .with("Digits", new RuleSet("Digits", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Digit"), new Atom("Digits") } ),
      Arrays.asList( new Type[]{ new Atom("Digit") } )
      )
    ))

    // Digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"

    .with("Digit", new RuleSet("Digit", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("number",null,"0123456789") } )
      )
    ))

    .with("String", new RuleSet("String", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("\""), new Var("string","\""), new Text("\"") } )
      )
    ))

    .with("Loop", new RuleSet("Loop", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ForNumLoop") } ),
      Arrays.asList( new Type[]{ new Atom("ForListLoop") } ),
      Arrays.asList( new Type[]{ new Atom("WhileLoop") } )
      )
    ))

    .with("ForNumLoop", new RuleSet("ForNumLoop", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("for"), new Atom("Space"), new Atom("VarName"), new Atom("WS"), new Text("="), new Atom("WS"), new Atom("LoopRange"), new Atom("WS"), new Atom("ForBody") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("new ForNumLoop(") , new Atom("VarName") , new Text(",") , new Atom("LoopRange") , new Text(")")))

    ))
    // "{" ForBody "}"

    .with("ForListLoop", new RuleSet("ForListLoop", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("for"), new Atom("Space"), new Atom("VarName"), new Atom("Space"), new Text("in"), new Atom("Space"), new Atom("Expression"), new Atom("WS"), new Atom("ForBody") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("new ForListLoop(") , new Atom("VarName") , new Text(",") , new Atom("Expression") , new Text(")")))

    ))
    // "{" ForBody "}"

    .with("ForBody", new RuleSet("ForBody", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("MutableCodeBlock") } )
      )
    ))

    //# TODO: Gah - Expressions don't parse nicely here!  (They swallow the "to" or what?)
    // LoopRange = Expression Space "to" Space Expression

    .with("LoopRange", new RuleSet("LoopRange", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ConstReference"), new Atom("Space"), new GroupedDefn(new RuleSet("Anonymous", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("to") } ),
      Arrays.asList( new Type[]{ new Text("..") } )
      ))), new Atom("Space"), new Atom("Expression"), new Atom("OptStep") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("dintj", Arrays.asList(new Text("new LoopRange(") , new Atom("ConstReference") , new Text(",") , new Atom("Expression") , new Text(")")))

    ))

    .with("OptStep", new RuleSet("OptStep", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("WS"), new GroupedDefn(new RuleSet("Anonymous", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("step") } ),
      Arrays.asList( new Type[]{ new Text("by") } )
      ))), new Atom("Space"), new Atom("Expression") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("WhileLoop", new RuleSet("WhileLoop", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("while"), new Atom("Space"), new Atom("Condition") } )
      )
    ))

    .with("Condition", new RuleSet("Condition", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Expression") } )
      )
    ))

    // We hope to handle significant indentation by pre-parsing the lines and
    // rewriting with "{" and "}"s which the grammar can easily detect.  This may
    // cause issues on multi-line statements which should not have "{" and "}"s!



    // WS    = Optional WhiteSpace
    // Space = Required WhiteSpace

    .with("NL", new RuleSet("NL", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("\n") } )
      )
    ))

    .with("WS", new RuleSet("WS", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Whitespace") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("Space", new RuleSet("Space", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Whitespace") } )
      )
    ))

    // Whitespace = WhitespaceBit Whitespace
               // | WhitespaceBit
    // 
    // WhitespaceBit = "\n"
                  // | " "
                  // | "\t"
                  // | "\r"

    .with("Whitespace", new RuleSet("Whitespace", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("whitespace",null," \t\n\r") } )
      )
    ))

    // Expressions will eat into the next line if allowed to swallow "\n"s
    // They don't know when to stop!
    // So we only allow them to swallow HorizSpace

    // HorizSpace = " " | "\t"
    .with("HorizSpace", new RuleSet("HorizSpace", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("horizspace",null," \t") } )
      )
    ))

    .with("OptHorizSpace", new RuleSet("OptHorizSpace", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("HorizSpace") } ),
      Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    // EOL = ";" | "\n"

    .with("EOL", new RuleSet("EOL", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("\n") } )
      )
    ))


    );
    return grammar;
  }
}
