package tools.parser;
/* This source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
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

import tools.parser.*;

/** A better name for this interface might be Matchable or MatchableGrammarRule. */

public interface Type {
	
  Match match(SomeString s);
  
  /**
   * This is used in Match.renderIn() to decide which matched type to use when
   * performing a replacement.  It is only needed a couple of "difficult" types
   * like Type and Var, most others can be rendered without this lookup.
   *
   * Method for matching the new structural types [...]* (...)+ are not yet
   * specified, but as ever leaving the output target unset will cause all
   * sub-matches to be shown.
   *
   * If we refer to them positionally (which is logical since they are unnamed)
   * then we could provide the neccessary extra options that these types need,
   * namely join for lists, e.g. $2.join(", ")
   * and maybe even conditionals for optionals, e.g. $2 ? $2 : "null"
  **/
  boolean replacementfor(Type o);
  
}
