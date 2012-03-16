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
import java.io.PrintStream;
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
import Type;
import Atom;
import Var;
import Text;

public class Match {
  Type type;
  SomeString string;
  Vector matches; // sub-matches making up this one
  Match parent; // parent match of which this is a sub-match
  SomeString left=null; // new RealString("error: left not initialised!");
  Match(Type t,SomeString s) {
    type=t;
    string=s;
    matches=null; // new Vector();
  }
  Match(Type t,SomeString s,Vector ms) {
    type=t;
    string=s;
    matches=ms;
  }
  Match(Type t,SomeString s,Vector ms,SomeString ll) {
    type=t;
    string=s;
    matches=ms;
    left=ll;
  }
  Match(Type t,SomeString s,SomeString ll) {
    type=t;
    string=s;
    matches=null;
    left=ll;
  }
  public String toString() {
    return "Match.toString(): gone to render(PrintStream,int)";
  }
  public void render(PrintStream out,String ind) {
//    System.out.println("Generating Match "+type+" with "+(matches==null?"no match":""+matches.size()));
    out.print(ind+type+" = "+"\""+Atom.strip(""+string)+"\"");
    if (matches!=null) {
      out.print(" with: [");
      for (int i=0;i<matches.size();i++) {
        out.print("\n"+ind);
        ((Match)matches.get(i)).render(out,ind+" ");
      }
//      System.out.println("a");
//      tmp=JString.replace(tmp,"\n","\n  ");
//      System.out.println("b");
      String weirdind=(ind+ind);
      if (weirdind.length()>0)
        weirdind=weirdind.substring(1);
      out.print("\n"+weirdind+"] leaving \""+Atom.strip(""+left)+"\"");
    }
  }
  public Vector rulefrommatch() {
    Vector v=new Vector();
    for (int i=0;i<matches.size();i++)
      v.add(((Match)matches.get(i)).type);
    return v;
  }
  /** render needs to learn to deal with RelElements. **/
  public void render(Match btwyourdadis,String target,PrintStream out) {
    if (parent==null)
      parent=btwyourdadis;
    Atom a=(Atom)type;
    RuleSet ruleset=Grammar.getrulesetforatom(a.type);
    Object tmp=ruleset.replacements.get(target);
    // if (tmp!=null || target.length()==0)
//    if (tmp==null)
//      return "";
    Vector rs=( tmp==null ? rulefrommatch() : (Vector)tmp );
//    out.print("    // Rendering "+Atom.strip(""+this)+" against "+rs+"\n";
//    out.print("    // Rendering "+a.type+": "+rs+"\n";
    Vector unusedmatches=(Vector)matches.clone();
		for (int i=0;i<rs.size();i++) {
      Type t=(Type)rs.get(i);
      renderIn(unusedmatches,t,target,out);
    }
  }
  public void renderIn(Vector unusedmatches,Type t,String target,PrintStream out) {
      if (t instanceof Text) {
        out.print(((Text)t).rendertext());
      } else if (t instanceof RelElement) {
        ((RelElement)t).render(target,out);
      } else {
        for (int j=0;j<unusedmatches.size();j++) {
        // for (int j=0;j<matches.size();j++) {
          // Match m=(Match)matches.get(j);
          Match m=(Match)unusedmatches.get(j);
          Type couldbe=(Type)m.type;
          if (t.replacementfor(couldbe)) {
            if (t instanceof Atom) {
              m.render(this,target,out);
              // Remove the match, so that subsequent Atoms of same type get the subsequent matches
              // matches.remove(m);
            } else if ((t instanceof Var)) { // *** needed if you don't want all vars printing! && (tmp!=null || target.length()==0)) {
              out.print(Parser.decode(""+m.string));
              // We don't expect two vars the same!
            }
						unusedmatches.remove(j);
            break;
          }
        }
      }
    // out.print("\n<Match.renderIn failed "+t+" on \n"+this+">");
  }
}
