package jumpgate;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;

public class JumpGate extends PageCreator {

	private static final String[] options={"Show options","Hide options"};

  public Component optionsOn=new SwitchButton(options);

  // All other fields are dynamically generated from data.

  public void renderPage(Page p) {

/*    DB db=new DB("jumpgatedb","entries");

    List typesOfJGData=db.getAllTypes();

    List categories=db.getAll("JGCategory"); 

    // get a list of all the fields.
    // those that appear in >1 entry
    //   (eg. "description", "button", "name", "link") [aren't latter two merged?]
    // will be globally off/onable.

    List globaloptions; // make a different to some entries, but do globally

    List entries=db.getObjectsImplementing("JGEntry");

    for (int i=0;i<entries.size();i++) {
      JGEntry e=(JGEntry)entries.get(i);
      List localoptions=e.getnonGlobalOptions();
      // ...
    } */
    
  }
}
