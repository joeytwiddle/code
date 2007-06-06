/*-------------------------------------------------------------------*
 * Copyright (C) 1998 University of Southern California (USC) and
 * USC Information Sciences Institute.
 *
 * Class:     JCardStack
 * Abstract:  A hypercard style component.
 * ChangeLog: 27 Sept 99: Andrew Marshall, copied from CardStack
 *-------------------------------------------------------------------*/

package us.ca.la.anm.gui;

import java.lang.*;
import java.awt.*;
import java.util.*;
import javax.swing.*;

public class JCardStack extends JComponent {

// Private Data
  JCardLayout layout;
  Hashtable hash = new Hashtable();

// Constructor
  public JCardStack() {
    super();
    setLayout( layout = new JCardLayout( this ) );
  }

// Public Methods
  public boolean contains( Object s ) {
    return hash.containsKey( s );
  }

  public void add( Component c, Object s ) {
    super.add( c, s );
    hash.put( s, c );
  }

  public Component get( Object s ) {
    if( hash.containsKey( s ))
      return (Component)hash.get( s );
    return null;
  }

  public void first() {
    layout.first();
  }

  public void last() {
    layout.last();
  }

/*
  public void next() {
    layout.next();
  }

  public void previous() {
    layout.previous();
  }
*/

  public void show( String name ) {
    layout.show( name );
  }
}

