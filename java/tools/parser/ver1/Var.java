/* This source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joey@neuralyte.org
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk */

import java.lang.*;
import java.util.*;

import java.awt.TextArea; // debug
import java.awt.Frame; // debug
import java.awt.FlowLayout; // debug
import java.awt.event.ActionListener; // debug
import java.awt.event.ActionEvent; // debug
import java.awt.Button; // debug

import jlib.Files;
import jlib.JString;
import jlib.strings.*;

import Grammar;
import RuleSet;
import grmGrm;

public class Var implements Type {
  String name;
  String deny; // characters which are not allowed
  Var(String n) {
    name=n;
    deny="";
  }
  Var(String n,String d) {
    name=n;
    deny=d;
  }
  public Match match(SomeString s) {
    int least=s.length();
    for (int i=0;i<deny.length();i++) {
      int j=s.indexOf(deny.charAt(i));
//      System.out.println("Found '"+Atom.strip(""+deny.charAt(i))+"' at "+j+" of "+Atom.strip(""+s));
      if (j>=0 && j<least)
        least=j;
    }
    return new Match(this,s.subString(0,least),s.subString(least));
  }
  public String toString() {
    return "<"+name+">";
  }
  public boolean replacementfor(Type o) {
    if (o instanceof Var) {
      Var v=(Var)o;
      if (name.equals(v.name))
        return true;
    }
    return false;
  }
}
