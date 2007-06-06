/*-------------------------------------------------------------------*
 * Copyright (C) 2000 University of Southern California (USC) and
 * USC Information Sciences Institute.
 *
 * Class:     JCardLayout
 * Abstract:  A hypercard style layout that works with Swing.
 * ChangeLog: 7 aug 2000: Andrew Marshall, copied from CardStack
 *-------------------------------------------------------------------*/

package us.ca.la.anm.gui;

import java.lang.*;
import java.awt.*;
import java.util.*;
import javax.swing.*;

public class JCardLayout implements LayoutManager {
  // Private Constants
  private static final Dimension ZERO_DIMENSION = new Dimension();
  private static final Point     ZERO_POINT = new Point();

  // Private Data
  protected Container parent;
  protected Hashtable nameToChild = new Hashtable();
  protected Component current = null;

 // Constructor
  public JCardLayout( Container parent ) {
    this.parent = parent;
  }

  // Public Methods
  public void addLayoutComponent( String name, Component comp) {
    Object old = nameToChild.get( name );
    if( old != null )
      throw new IllegalStateException("Child component \""+name+"\" already exisits" );
    nameToChild.put( name, comp );

    if( current == null )
      current = comp;
  }

  public void removeLayoutComponent( Component comp ) {
    if( current == comp )
      current = null;

    Iterator i = nameToChild.entrySet().iterator();
    Map.Entry entry;
    while( i.hasNext() ) {
      entry = (Map.Entry) i.next();
      if( entry.getValue() == comp ) {
        nameToChild.entrySet().remove( entry );
        // The component could only be added once.
        return;
      }
    }
  }

  public Dimension preferredLayoutSize(Container parent) {
    if( current != null )
      return current.getPreferredSize();
    else
      return ZERO_DIMENSION;
  }

  public Dimension minimumLayoutSize(Container parent) {
    if( current != null )
      return current.getMinimumSize();
    else
      return ZERO_DIMENSION;
  }

  public void layoutContainer( Container parent ) {
    Component[] comps = parent.getComponents();
    int len = comps.length;
    for( int i=0; i<len; i++ ) {
      if( comps[i] == current ) {
        current.setVisible( true );
        current.setLocation( ZERO_POINT );
        current.setSize( parent.getSize() );
      } else {
        comps[i].setVisible( false );
      }
    }
  }

  public Component get( Object s ) {
    return (Component) nameToChild.get( s );
  }

  public void first() {
    Component[] comps = parent.getComponents();
    if( comps.length > 0 &&
        current != comps[0] ) {
      current = comps[0];
      parent.validate();
    }
  }

  public void last() {
    Component[] comps = parent.getComponents();
    int i = comps.length-1;  // i stands for index
    if( comps.length > 0 &&
        current != comps[i] ) {
      current = comps[i];
      parent.validate();
    }
  }

/*
  public void next() {
    layout.next( this );
  }

  public void previous() {
    layout.previous( this );
  }
*/

  public void show( String name ) {
    Component newComp = (Component) nameToChild.get( name );
    if( current != newComp ) {
      current = newComp;
      parent.validate();
    }
  }
}

