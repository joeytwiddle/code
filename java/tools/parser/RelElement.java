package tools.parser;
/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk */

import java.lang.*;
import java.util.*;
import java.io.PrintStream;

import java.awt.TextArea; // debug
import java.awt.Frame; // debug
import java.awt.FlowLayout; // debug
import java.awt.event.ActionListener; // debug
import java.awt.event.ActionEvent; // debug
import java.awt.Button; // debug

import jlib.Files;
import jlib.JString;
import jlib.JLib;
import jlib.strings.*;

import Grammar;
import RuleSet;
import grmGrm;

public class RelElement implements Type {
  char dir;
  String to;
  Type leaf;
  RelElement(char d,String t,Type l) {
    dir=d;
    to=t;
    leaf=l;
  }
  public Match match(SomeString s) {
    JLib.error("RelElement.match(): Not supposed to try to match, these are for output targetting!");
    return null;
  }
  public String toString() {
    return "RelElement("+to+dir+leaf+")";
  }
  public boolean replacementfor(Type o) {
/*    if (o instanceof Type) {
      Type v=(Type)o;
      if (name.equals(v.name))
        return true;
    } */
    return true;
  }
  public void render(Match btwyourdadis,String target,PrintStream out) {
    if (dir=='^') {
      Match m=btwyourdadis;
      while (m!=null) {
        if (m.type instanceof Atom) {
          if (((Atom)m.type).type.equals(to)) {
            // System.err.println("RelElem "+this+": "+target+" -> "+m);
            Vector unusedmatches=(Vector)m.matches.clone();
            m.renderIn(unusedmatches,leaf,target,out);
            return;
          }
        }
        m=m.parent;
      }
      // out.print("<rel8ive error didn't find "+this+">");
      /* for (int i=Parser.allMatches.size()-1;i>=0;i--) {
        Match m=(Match)Parser.allMatches.get(i);
        if (m.type instanceof Atom) {
          // System.out.println("Matching >"+((Atom)m.type)+"< to >"+to+"<");
          if (((Atom)m.type).type.equals(to)) {
            // System.out.println("Rendering >"+leaf.name+"< of "+m);
            // ********** added this new Vector():
            m.renderIn(new Vector(),leaf,target,out);
            return;
          }
        }
      } */
      // Going up tree doesn't work because what you want is probably inside a simpler match
      /* m=m.parent;
      while (m!=null) {
        if (m.type instanceof Atom) {
          System.out.println("Matching "+((Atom)m.type)+" to "+to);
          if (((Atom)m.type).equals(to))
            return m.render(null,leaf.name);
        }
        m=m.parent;
      } */
    // } else if (dir=='.') {
    } else {
      out.print("<rel8ive doesn't do . yet>");
    }
  }
}
