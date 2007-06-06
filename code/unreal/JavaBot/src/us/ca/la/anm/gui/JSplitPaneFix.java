//  us.ca.la.anm.gui.JSplitPaneFix
//  Copyright 2000, Andrew n. marshall
//
//  Personal and Educational use is hereby granted.
//  Permission required for commercial use and redistribution.


package us.ca.la.anm.gui;

import java.lang.*;

import javax.swing.JSplitPane;

import us.ca.la.anm.gui.*;


/**
 *  Fixes an error in {@link JSplitPane} when {@link #setDividerLocation( double )}
 *  is set before the split pane is validated.  In these cases, the height and
 *  width of the JSplitPane is zero, and any precentage of zero is still zero.
 *  JSplitPaneFix saves the percentage value in these cases until after it has
 *  been validated.
 *
 *  @author <a href="mailto:amarshal#JSplitPane@isi.edu">Andrew n marshall</a>
 */
public class JSplitPaneFix extends JSplitPane {
  protected Double _newDividerLocation = null;

  public JSplitPaneFix() {
    super();
  }

  public JSplitPaneFix( int orientation ) {
    super( orientation );
  }

  public void setDividerLocation( double dividerLocation ) {
    if( isValid() ) {
      super.setDividerLocation( dividerLocation );
    } else {
      _newDividerLocation = new Double( dividerLocation );
    }
  }

  public void setBounds( int x, int y, int width, int height ) {
    super.setBounds( x, y, width, height );

    if( _newDividerLocation != null ) {
      if( getOrientation() == JSplitPane.HORIZONTAL_SPLIT ) {
        if( height != 0 ) {
          super.setDividerLocation( _newDividerLocation.doubleValue() );
          _newDividerLocation = null;
        }
      } else {
        // Vertical Split
        if( width != 0 ) {
          super.setDividerLocation( _newDividerLocation.doubleValue() );
          _newDividerLocation = null;
        }
      }
    }
  }
}
