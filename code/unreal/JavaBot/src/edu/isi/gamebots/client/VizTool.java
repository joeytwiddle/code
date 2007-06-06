//  edu.isi.gamebots.clients.VizTool
//  Copyright 2000, University of Southern California,
//                  Information Science Institute
//
//  Personal and Educational use is hereby granted.
//  Permission required for commercial use and redistribution.


package edu.isi.gamebots.client;

import java.lang.*;
import javax.swing.JComponent;

import us.ca.la.anm.util.io.*;

import edu.isi.gamebots.client.*;


/**
 *  This is the interface for all Java vizualization clients.
 *
 *  @author <a href="mailto:amarshal#gamebots@isi.edu">Andrew n marshall</a>
 */
public interface VizTool extends GamebotsConstants {
  public void init( DebugLog log );
  public JComponent getView();
  public GamebotsClient.Listener getListener();
  public void destroy();
}
