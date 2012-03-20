package tools.parser;
import java.lang.String;
import java.util.Vector;

import tools.parser.*;

// @todo Replace the Vector with a Hashtable, so that later atom definitions can
// overwrite earlier ones.

public class Grammar {
  public static Vector rulesets=new Vector();
  public static RuleSet getrulesetforatom(String type) {
    for (int i=0;i<rulesets.size();i++) {
      RuleSet rs=(RuleSet)rulesets.get(i);
      if (rs.type.equals(type))
        return rs;
    }
    System.out.println("Grammar.getrulesetforatom("+type+"): not found!");
    System.exit(1);
    return null;
  }
}
