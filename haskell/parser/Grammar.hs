module Grammar where

data Type = Atom String | Var String | Str String
          | VarExcl String String
  deriving (Eq)

data Match = No | Yes Type String [Match] String
  deriving (Eq)

type RuleSet = [[Type]]

type Rule = ( Type , RuleSet , [Replacement] )

type Replacement = ( String , [Type])


rules = [   ( Atom "Main",[
    [ Atom "GrammarDefn" ]
    ] , [

  ] ) ,
  ( Atom "GrammarDefn",[
    [ Atom "Grm" ]
    ] , [
      ( "java" , [ Str "import java.lang.String;\nimport java.util.Vector;\n\nimport Grammar;\nimport RuleSet;\n\npublic class grmGrm {\n  public static void setupgrammar() {\n    Vector rulesets=Grammar.rulesets;\n    RuleSet ruleset;\n    Vector rule;\n\n", Atom "Grm", Str "  }\n}\n" ] ) ,
      ( "hugs" , [ Str "module Grammar where\n\ndata Type = Atom String | Var String | Str String\n          | VarExcl String String\n  deriving (Eq)\n\ndata Match = No | Yes Type String [Match] String\n  deriving (Eq)\n\ntype RuleSet = [[Type]]\n\ntype Rule = ( Type , RuleSet , [Replacement] )\n\ntype Replacement = ( String , [Type])\n\n\nrules = [ ", Atom "Grm", Str " ]\n" ] ) 
  ] ) ,
  ( Atom "Grm",[
    [ Atom "GrmBit", Atom "Grm"] ,
      [ Atom "GrmBit" ]
    ] , [

  ] ) ,
  ( Atom "GrmBit",[
    [ Str "\n"] ,
      [ Atom "Whitespace"] ,
      [ Atom "Comment"] ,
      [ Atom "Atom" ]
    ] , [

  ] ) ,
  ( Atom "Comment",[
    [ Str "#", VarExcl "comment" "\n", Str "\n" ]
    ] , [
      ( "java" , [ Str "    //", Var "comment", Str "\n" ] ) 
  ] ) ,
  ( Atom "Atom",[
    [ Atom "AtomWithout", Str "\n\n" ]
    ] , [

  ] ) ,
  ( Atom "AtomWithout",[
    [ VarExcl "atomname" "<>\n\" =", Str " = ", Atom "Defn", Atom "OptReplacements" ]
    ] , [
      ( "java" , [ Str "    ruleset=new RuleSet(\"", Var "atomname", Str "\");\n      rulesets.add(ruleset);\n      rule=new Vector();\n", Atom "Defn", Str "      ruleset.add(rule);\n", Atom "OptReplacements", Str "\n" ] ) ,
      ( "hugs" , [ Str "  ( Atom \"", Var "atomname", Str "\",[\n    [ ", Atom "Defn", Str " ]\n    ] , [\n", Atom "OptReplacements", Str "\n  ] ) ,\n" ] ) 
  ] ) ,
  ( Atom "OptReplacements",[
    [ Str "\n", Atom "Replacements"] ,
      [ Str "" ]
    ] , [
      ( "java" , [ Str "    // Replacements\n", Atom "Replacements" ] ) 
  ] ) ,
  ( Atom "Replacements",[
    [ Atom "TwoReplacements"] ,
      [ Atom "Replacement" ]
    ] , [

  ] ) ,
  ( Atom "TwoReplacements",[
    [ Atom "Replacement", Str "\n", Atom "Replacements" ]
    ] , [
      ( "hugs" , [ Atom "Replacement", Str ",\n", Atom "Replacements" ] ) 
  ] ) ,
  ( Atom "Replacement",[
    [ VarExcl "target" "<>\n\" :", Str ": ", Atom "Defn" ]
    ] , [
      ( "java" , [ Str "    rule=new Vector();\n", Atom "Defn", Str "    ruleset.replacements.put(\"", Var "target", Str "\",rule);\n" ] ) ,
      ( "hugs" , [ Str "      ( \"", Var "target", Str "\" , [ ", Atom "Defn", Str " ] ) " ] ) 
  ] ) ,
  ( Atom "Defn",[
    [ Atom "DefnOr"] ,
      [ Atom "DefnAnd"] ,
      [ Atom "DefnBit" ]
    ] , [

  ] ) ,
  ( Atom "DefnBit",[
    [ Atom "Variable"] ,
      [ Atom "Text"] ,
      [ Atom "AtomRef" ]
    ] , [

  ] ) ,
  ( Atom "DefnOr",[
    [ Atom "DefnBit", Atom "Whitespace", Str "| ", Atom "Defn" ]
    ] , [
      ( "java" , [ Atom "DefnBit", Str "      ruleset.add(rule);\n      rule=new Vector();\n", Atom "Defn" ] ) ,
      ( "hugs" , [ Atom "DefnBit", Str "] ,\n      [ ", Atom "Defn" ] ) 
  ] ) ,
  ( Atom "DefnAnd",[
    [ Atom "DefnBit", Str " ", Atom "Defn" ]
    ] , [
      ( "java" , [ Atom "DefnBit", Atom "Defn" ] ) ,
      ( "hugs" , [ Atom "DefnBit", Str ", ", Atom "Defn" ] ) 
  ] ) ,
  ( Atom "Variable",[
    [ Atom "Var"] ,
      [ Atom "VarDeny" ]
    ] , [

  ] ) ,
  ( Atom "Var",[
    [ Str "<", VarExcl "varname" "<>\n\"/ ", Str ">" ]
    ] , [
      ( "java" , [ Str "        rule.add(new Var(\"", Var "varname", Str "\"));\n" ] ) ,
      ( "hugs" , [ Str "Var \"", Var "varname", Str "\"" ] ) 
  ] ) ,
  ( Atom "VarDeny",[
    [ Str "<", VarExcl "varname" "<>\n\"/ ", Str "/\"", VarExcl "denied" "\"", Str "\">" ]
    ] , [
      ( "java" , [ Str "        rule.add(new Var(\"", Var "varname", Str "\",\"", Var "denied", Str "\"));\n" ] ) ,
      ( "hugs" , [ Str "VarExcl \"", Var "varname", Str "\" \"", Var "denied", Str "\"" ] ) 
  ] ) ,
  ( Atom "AtomRef",[
    [ VarExcl "atomtype" "<>\n\" " ]
    ] , [
      ( "java" , [ Str "        rule.add(new Atom(\"", Var "atomtype", Str "\"));\n" ] ) ,
      ( "hugs" , [ Str "Atom \"", Var "atomtype", Str "\"" ] ) 
  ] ) ,
  ( Atom "Text",[
    [ Str "\"", VarExcl "text" "\"", Str "\""] ,
      [ Str "\"\"" ]
    ] , [
      ( "java" , [ Str "        rule.add(new Text(\"", Var "text", Str "\"));\n" ] ) ,
      ( "hugs" , [ Str "Str \"", Var "text", Str "\"" ] ) 
  ] ) ,
  ( Atom "Whitespace",[
    [ Atom "WhitespaceBit", Atom "Whitespace"] ,
      [ Atom "WhitespaceBit" ]
    ] , [

  ] ) ,
  ( Atom "WhitespaceBit",[
    [ Str "\n"] ,
      [ Str " " ]
    ] , [

  ] )
 ]

