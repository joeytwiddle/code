package tools.parser;
import java.lang.String;
import java.util.Vector;
import java.util.Hashtable;
import java.util.Map;

public class RuleSet {
  String type;
  Vector rules;
  Map replacements;
  RuleSet(String t) {
    type=t;
    rules=new Vector();
    replacements=new Hashtable();
  }
  void add(Object o) {
    rules.add(o);
  }
/*  void addreplacementtype(String target) {
    map.put(target,new Vector());
  }
  void addreplacementbit(String target,Type t) {
    Vector rrs=map.get(target);
    rrs.add(t);
  }*/
}

