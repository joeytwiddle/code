//  edu.isi.gamebots.clients.BotRunnerApp
//  Copyright 2000, University of Southern California,
//                  Information Science Institute
//
//  Personal and Educational use is hereby granted.
//  Permission required for commercial use and redistribution.


package edu.isi.gamebots.client;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;
//import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.lang.*;
import java.net.*;
import java.util.*;

import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;

import us.ca.la.anm.gui.*;
import us.ca.la.anm.util.io.*;

import edu.isi.gamebots.client.*;
import edu.isi.gamebots.examples.*;


/**
 *  This application is a test platform for any Bot or VizTool implementation.
 *  It provides a simple user interface for loading bot implementations, setting
 *  the server address and port, and connecting and disconnecting the clients.
 *
 *  <h3>Future Goals:</h3>
 *  <ul>
 *    <li>Finish the Bot menu.  Allow for individual Bot connection,
 *        reconnection, and disconnection, as well as the biligty to unload a
 *        bot from the team.
 *    <li>Add support for serializable Bots (load Bot from file, save Bot).
 *  </ul>
 *
 *  @author <a href="mailto:amarshal#gamebots@isi.edu">Andrew n marshall</a>
 */
public class BotRunnerApp implements GamebotsConstants {
  // Private Constants
  ///////////////////////////////////////////////////////////////////////////
  protected static final int MAX_CONNECTION_ATTEMPTS = 2;
  protected static final long TIMEOUT = 10000; // 10 seconds

  protected static final String DATA_CARD = "data";  // For AddBotDialog
  protected static final String ERROR_CARD = "error";  // For AddBotDialog

  protected static final String BOT = "Bot";
  protected static final String VIZ = "VizTool";

  // Private Data
  ///////////////////////////////////////////////////////////////////////////
  protected JTextAreaLog log = new JTextAreaLog();

  protected JFrame frame;
  protected JMenu botMenu;
  protected JTabbedPane tabbedPane;
  protected JPanel serverPanel;
  protected JTextArea text;
  protected JLabel status;

  protected JTextAreaLog curLogForText = log;

  protected Hashtable tabManagers = new Hashtable();  // JCompopnet views to TabManagers
  protected int botNameCount = 0;
  protected int vizNameCount = 0;
  protected int tabCount = 0;
  protected int connectionCount = 0;

  protected JTextField serverField;
  protected InetAddress serverAddress = null;
  protected JTextField botPortField;
  protected int serverBotPort = GamebotsClient.DEFAULT_BOT_PORT;
  protected JTextField vizPortField;
  protected int serverVizPort = GamebotsClient.DEFAULT_VIZ_PORT;

  protected Thread[] connectionEventThreads;
  protected Vector connectionEventQueue;

  // Public Methods
  ///////////////////////////////////////////////////////////////////////////
  public static void main( String[] args ) {
    new BotRunnerApp( args );
  }

  public BotRunnerApp( String[] args ) {
    buildGUI();
    log.setJTextArea( text );

    connectionEventQueue = new Vector();
    connectionEventThreads = new Thread[ MAX_CONNECTION_ATTEMPTS ];
    for( int i=0; i<MAX_CONNECTION_ATTEMPTS; i++ ) {
      connectionEventThreads[i] = new Thread( new ConnectionEventRunner(i) );
      connectionEventThreads[i].start();
    }

    frame.show();
  }

  //  Private Methods
  ///////////////////////////////////////////////////////////////////////////
  protected void buildGUI() {
    JMenuBar menuBar;
    JMenu menu;
    JMenuItem item;
    JPanel panel, panel2;
    JScrollPane scroll;
    JSplitPane splitter;
    JLabel label;
    GridBagConstraints gbc;

    requestAddBot.putValue( Action.NAME, "Add "+BOT+"..." );
    requestAddViz.putValue( Action.NAME, "Add "+VIZ+"..." );
    connectAll.putValue( Action.NAME, "Connect All" );
      connectAll.setEnabled( false );
    reconnectAll.putValue( Action.NAME, "Reconnect All" );
      connectAll.setEnabled( false );
    disconnectAll.putValue( Action.NAME, "Disconnect All" );
      disconnectAll.setEnabled( false );
    requestExit.putValue( Action.NAME, "Exit" );
    handleExit.putValue( Action.NAME, "Force Exit" );

    frame = new JFrame( "Gamebots Java Client" );
      menuBar = new JMenuBar();
        menu = new JMenu( "Team" );
          menu.add( new JMenuItem( requestAddBot ) );
          menu.add( new JMenuItem( requestAddViz ) );
          menu.addSeparator();
          menu.add( new JMenuItem( connectAll ) );
          menu.add( new JMenuItem( reconnectAll ) );
          menu.add( new JMenuItem( disconnectAll ) );
          menu.addSeparator();
          menu.add( new JMenuItem( requestExit ) );
        menuBar.add( menu );

        botMenu = new JMenu( BOT );
          botMenu.setEnabled( false );
        menuBar.add( botMenu );
      frame.setJMenuBar( menuBar );

      panel = new JPanel( new BorderLayout() );
        splitter = new JSplitPaneFix( JSplitPane.VERTICAL_SPLIT );
          tabbedPane = new JTabbedPane();
            tabbedPane.addChangeListener( tabListener );

            serverPanel = new JPanel( new GridBagLayout() );
              serverPanel.setBorder( new EmptyBorder( 2, 2, 2, 2 ) );

              label = new JLabel( "Server:" );
              gbc = new GridBagConstraints();
                gbc.gridx = gbc.gridy = 0;
                gbc.weightx = 0.0;
                gbc.weighty = 0.6;
                gbc.fill = gbc.NONE;
                gbc.anchor = gbc.SOUTHEAST;
              serverPanel.add( label, gbc );

              serverField = new JTextField();
                serverField.addFocusListener( serverFocusListener );

                gbc.gridx++;
                gbc.weightx = 1.0;
                gbc.anchor = gbc.SOUTHWEST;
                gbc.fill = gbc.HORIZONTAL;
              serverPanel.add( serverField, gbc );

              label = new JLabel( BOT+" Port:", SwingConstants.TRAILING );
                gbc.gridx = 0;
                gbc.gridy++;
                gbc.weightx = gbc.weighty = 0.0;
                gbc.fill = gbc.NONE;
                gbc.anchor = gbc.EAST;
              serverPanel.add( label, gbc );

              botPortField = new JTextField( Integer.toString( serverBotPort ) );
                botPortField.addFocusListener( botPortFocusListener );

                gbc.gridx++;
                gbc.weightx = 1.0;
                gbc.fill = gbc.HORIZONTAL;
                gbc.anchor = gbc.EAST;
              serverPanel.add( botPortField, gbc );

              label = new JLabel( VIZ+" Port:", SwingConstants.TRAILING );
                gbc.gridx = 0;
                gbc.gridy++;
                gbc.weightx = gbc.weighty = 0.0;
                gbc.anchor = gbc.EAST;
                gbc.fill = gbc.NONE;
              serverPanel.add( label, gbc );

              vizPortField = new JTextField( Integer.toString( serverVizPort ) );
                vizPortField.addFocusListener( vizPortFocusListener );

                gbc.gridx++;
                gbc.anchor = gbc.WEST;
                gbc.fill = gbc.HORIZONTAL;
              serverPanel.add( vizPortField, gbc );

              panel2 = new JPanel( new FlowLayout( FlowLayout.TRAILING ) );
                panel2.add( new JButton( connectAll ) );
//                panel2.add( new JButton( reconnectAll ) );
                panel2.add( new JButton( disconnectAll ) );

                gbc.gridx = 0;
                gbc.gridy++;
                gbc.weightx = gbc.weighty = 1.0;
                gbc.gridwidth = 2;
                gbc.anchor = gbc.SOUTH;
                gbc.fill = gbc.HORIZONTAL;
              serverPanel.add( panel2, gbc );
            tabbedPane.add( serverPanel, "Server" );
          splitter.setTopComponent( tabbedPane );

          text = new JTextArea();
            text.setFont( new Font( "Monospaced", Font.PLAIN, 11 ) );
            text.setAutoscrolls( true );
            text.setEditable( false );
            text.setMargin( new Insets( 2, 2, 2, 2 ) );
          scroll = new JScrollPane( text );
            scroll.setPreferredSize( new Dimension( 480, 360 ) );
          splitter.setBottomComponent( scroll );

        panel.add( splitter, BorderLayout.CENTER );

        status = new JLabel( " " );
        panel.add( status, BorderLayout.SOUTH );
      frame.setContentPane( panel );

      frame.setDefaultCloseOperation( WindowConstants.DO_NOTHING_ON_CLOSE );
      frame.addWindowListener( new WindowAdapter() {
          public void windowClosing( WindowEvent event ) {
            requestExit.actionPerformed( null );
          }

          public void windowClosed( WindowEvent event ) {
            handleExit.actionPerformed( null );
          }
        } );
    frame.pack();
  }

  protected void setStatus( String status ) {
    this.status.setText( status );
  }

  // Event Handlers
  protected Action requestAddBot = new AbstractAction() {
    public void actionPerformed( ActionEvent event ) {
      JDialog dialog = new AddTabDialog( BOT );
      dialog.show();
    }
  };

  protected Action requestAddViz = new AbstractAction() {
    public void actionPerformed( ActionEvent event ) {
      JDialog dialog = new AddTabDialog( VIZ );
      dialog.show();
    }
  };

  protected Action connectAll = new AbstractAction() {
    public void actionPerformed( ActionEvent event ) {
      if( serverAddress == null ) {
        log.logNote( "Cannot connect: No server defined." );
        log.flush();
        return;
      }

      synchronized( connectionEventQueue ) {
        synchronized( tabManagers ) {
          Iterator i = tabManagers.values().iterator();
          TabManager tab;
          while( i.hasNext() )
            connectionEventQueue.add( ((TabManager) i.next()).connectRunner );
        } // end tab sync

        connectionEventQueue.notifyAll();
      }
    }
  };

  protected Action reconnectAll = new AbstractAction() {
    public void actionPerformed( ActionEvent event ) {
      synchronized( connectionEventQueue ) {
        connectAll.actionPerformed( event );
        disconnectAll.actionPerformed( event );
      }
    }
  };

  protected Action disconnectAll = new AbstractAction() {
    public void actionPerformed( ActionEvent event ) {
      synchronized( connectionEventQueue ) {
        synchronized( tabManagers ) {
          Iterator i = tabManagers.values().iterator();
          TabManager tab;
          while( i.hasNext() )
            connectionEventQueue.add( ((TabManager) i.next()).disconnectRunner );
        } // end tab sync

        connectionEventQueue.notifyAll();
      }
    }
  };

  protected ChangeListener tabListener = new ChangeListener() {
    public void stateChanged( ChangeEvent event ) {
      Component selected = tabbedPane.getSelectedComponent();

      if( selected == serverPanel ) {
        if( curLogForText != log ) {
          curLogForText.setJTextArea( null );
          text.setText( "" );
          curLogForText = log;
          curLogForText.setJTextArea( text );

          botMenu.setEnabled( false );
        }
      } else {
        Object tab = tabManagers.get( selected );
        if( tab == null ) {
          log.logError( "Selected component <"+selected+"> is not recognized." );
        } else {
          JTextAreaLog newLog = ((TabManager) tab).tabLog;
          if( curLogForText != newLog ) {
            curLogForText.setJTextArea( null );
            text.setText( "" );
            curLogForText = newLog;
            curLogForText.setJTextArea( text );
          }
        }
      }
    }
  };

  protected FocusListener serverFocusListener = new FocusListener() {
    public void focusGained( FocusEvent event ) {
      serverField.setForeground( Color.black );
    }

    public void focusLost( FocusEvent event ) {
      // This doesn't work
      frame.setCursor( Cursor.getPredefinedCursor( Cursor.WAIT_CURSOR ) );

      try {
        InetAddress oldAddress = serverAddress;
        serverAddress = InetAddress.getByName( serverField.getText() );

        if( oldAddress == null ||
            !oldAddress.equals( serverAddress ) ) {
          synchronized( tabManagers ) {
            Iterator i = tabManagers.values().iterator();
            TabManager tab;
            while( i.hasNext() ) {
              tab = (TabManager) i.next();
              tab.client.setServerAddress( serverAddress );
            }
          }
        }
      } catch( UnknownHostException error ) {
        serverField.setForeground( Color.red );
      }
      frame.setCursor( Cursor.getDefaultCursor() );
    }
  };

  protected FocusListener botPortFocusListener = new FocusListener() {
    public void focusGained( FocusEvent event ) {
      botPortField.setForeground( Color.black );
    }

    public void focusLost( FocusEvent event ) {
      try {
        int oldPort = serverBotPort;
        serverBotPort = Integer.parseInt( botPortField.getText() );

        if( serverBotPort <= 0 ) {
          botPortField.setForeground( Color.red );
        } else if( oldPort != serverBotPort ) {
          synchronized( tabManagers ) {
            Iterator i = tabManagers.values().iterator();
            TabManager tab;
            while( i.hasNext() ) {
              tab = (TabManager) i.next();
              if( tab instanceof BotManager )
                tab.client.setServerPort( serverBotPort );
            }
          }
        }
      } catch( NumberFormatException error ) {
        botPortField.setForeground( Color.red );
      }
    }
  };

  protected FocusListener vizPortFocusListener = new FocusListener() {
    public void focusGained( FocusEvent event ) {
      vizPortField.setForeground( Color.black );
    }

    public void focusLost( FocusEvent event ) {
      try {
        int oldPort = serverVizPort;
        serverVizPort = Integer.parseInt( vizPortField.getText() );

        if( serverVizPort <= 0 ) {
          vizPortField.setForeground( Color.red );
        } else if( oldPort != serverVizPort ) {
          synchronized( tabManagers ) {
            Iterator i = tabManagers.values().iterator();
            TabManager tab;
            while( i.hasNext() ) {
              tab = (TabManager) i.next();
              if( tab instanceof VizManager ) {
                tab.disconnect();
                tab.client.setServerPort( serverVizPort );
              }
            }
          }
        }
      } catch( NumberFormatException error ) {
        vizPortField.setForeground( Color.red );
      }
    }
  };

  protected Action requestExit = new AbstractAction() {
    public void actionPerformed( ActionEvent event ) {
      frame.setVisible( false );
      frame.dispose();
    }
  };

  protected Action handleExit = new AbstractAction() {
    public void actionPerformed( ActionEvent event ) {
      frame.setVisible( false );
      frame.dispose();

      for( int i=0; i<MAX_CONNECTION_ATTEMPTS; i++ )
        connectionEventThreads[i] = null;

      synchronized( tabManagers ) {
        Iterator i = tabManagers.values().iterator();
        while( i.hasNext() )
          ((TabManager) i.next()).destroy();
      } // end sync

      System.exit( 0 );
    }
  };


  //  Protected Inner Classes
  ///////////////////////////////////////////////////////////////////////////
  protected class ConnectionEventRunner implements Runnable {
    protected int number;

    public ConnectionEventRunner( int number ) {
      this.number = number;
    }

    public void run() {
      Thread thisThread = Thread.currentThread();

      Runnable runner = null;
      while( connectionEventThreads[number] == thisThread ) {
        synchronized( connectionEventQueue ) {
          if( connectionEventQueue.isEmpty() ) {
            try {
              connectionEventQueue.wait();
            } catch( InterruptedException error ) {
              // ignore...
            }
          } else {
            runner = (Runnable) connectionEventQueue.get( 0 );
            connectionEventQueue.remove( 0 );
          }
        }  //  end synch

        if( runner != null )
          runner.run();
      }
    }
  }

  protected class AddTabDialog extends JDialog {
    //  Private Data
    /////////////////////////////////////////////////////////////////////////
/*
    protected final JMenuItem ANY_TEAM_ITEM;
    protected final JMenuItem RED_TEAM_ITEM;
    protected final JMenuItem BLUE_TEAM_ITEM;
    protected final JMenuItem GREEN_TEAM_ITEM;
    protected final JMenuItem YELLOW_TEAM_ITEM;
    protected final JMenuItem NO_TEAM_ITEM;
*/

    protected String[] knownBots = {
      "edu.isi.gamebots.examples.ExampleBot",
      "edu.cmu.gamebots.CMU_JBot",
      "edu.tamu.gamebots.humanbot.HumanBot",
      "edu.tamu.agents.CAST_PM.unreal.AgentUnrealTournamentAdapter"
    };
    
    protected String[] knownVizTools ={
      "edu.isi.gamebots.examples.ExampleVizTool"
    };
    
    protected String panelType;

    protected JCardStack cards;
    protected JTextField nameField;
    protected JComboBox  teamField;
    protected JTextField classpathField;
    protected JComboBox classField;//changed by Ryan Rozich for user convienience
    protected JButton addButton, cancelButton;

    //  Public Methods
    /////////////////////////////////////////////////////////////////////////
    public AddTabDialog( String type ) {
/*
      if( type == BOT ) {
        ANY_TEAM_ITEM = new JMenuItem( "Any" );
        RED_TEAM_ITEM = new JMenuItem( "Red" );
          RED_TEAM_ITEM.setForeground( Color.red );
        BLUE_TEAM_ITEM = new JMenuItem( "Blue" );
          BLUE_TEAM_ITEM.setForeground( Color.blue );
        GREEN_TEAM_ITEM = new JMenuItem( "Green" );
          GREEN_TEAM_ITEM.setForeground( Color.green );
        YELLOW_TEAM_ITEM = new JMenuItem( "Yellow" );
          YELLOW_TEAM_ITEM.setForeground( Color.yellow.darker() );
        NO_TEAM_ITEM = new JMenuItem( "None" );
      } else {
        ANY_TEAM_ITEM = RED_TEAM_ITEM = BLUE_TEAM_ITEM = GREEN_TEAM_ITEM
          = YELLOW_TEAM_ITEM = NO_TEAM_ITEM = null;
      }
*/

      panelType = type;

      JPanel panel, panel2;
      JLabel label;
      JButton button;
      GridBagConstraints gbc;

      Container content = getContentPane();
        cards = new JCardStack();
          panel = new JPanel( new GridBagLayout() );
            panel.setBorder( new EmptyBorder( 2, 2, 2, 2 ) );

            label = new JLabel( "Name:" );
            gbc = new GridBagConstraints();
              gbc.gridx = gbc.gridy = 0;
              gbc.weightx = 0.0;
              gbc.weighty = 0.3;
              gbc.anchor = gbc.SOUTHEAST;
            panel.add( label, gbc );

            if( panelType == BOT )
              nameField = new JTextField( panelType+(++botNameCount) );
            else // assume VIZ
              nameField = new JTextField( panelType+(++vizNameCount) );

              gbc.gridx++;
              gbc.weightx = 1.0;
              gbc.anchor = gbc.SOUTHWEST;
              gbc.fill = gbc.HORIZONTAL;
            panel.add( nameField, gbc );

            if( panelType == BOT ) {
              label = new JLabel( "Team:", SwingConstants.TRAILING );
                gbc.gridx++;
                gbc.weightx = 0.0;
                gbc.fill = gbc.NONE;
                gbc.anchor = gbc.SOUTHEAST;
              panel.add( label, gbc );

                teamField = new JComboBox();
                  teamField.addItem( GamebotsConstants.TeamColorMap.teamToName( TEAM_ANY ) );
                  teamField.addItem( GamebotsConstants.TeamColorMap.teamToName( TEAM_NONE ) );
                  teamField.addItem( GamebotsConstants.TeamColorMap.teamToName( TEAM_RED ) );
                  teamField.addItem( GamebotsConstants.TeamColorMap.teamToName( TEAM_BLUE ) );
                  teamField.addItem( GamebotsConstants.TeamColorMap.teamToName( TEAM_GREEN ) );
                  teamField.addItem( GamebotsConstants.TeamColorMap.teamToName( TEAM_GOLD ) );
                  teamField.setEditable( false );

                gbc.gridx++;
                gbc.anchor = gbc.SOUTHWEST;
                gbc.fill = gbc.HORIZONTAL;
              panel.add( teamField, gbc );
            }

/*
            label = new JLabel( "Classpath:", SwingConstants.TRAILING );
              gbc.gridx = 0;
              gbc.gridy++;
              gbc.weightx = gbc.weighty = 0.0;
              gbc.anchor = gbc.EAST;
            panel.add( label, gbc );

              classpathField = new JTextField();

              gbc.gridx++;
              gbc.gridwidth = 3;
              gbc.weightx = 1.0;
              gbc.anchor = gbc.WEST;
              gbc.fill = gbc.HORIZONTAL;
            panel.add( classpathField, gbc );
*/

            label = new JLabel( "Class:", SwingConstants.TRAILING );
              gbc.gridx = 0;
              gbc.gridy++;
              gbc.gridwidth = 1;
              gbc.weightx = 0.0;
              gbc.weighty = 1.0;
              gbc.anchor = gbc.NORTHEAST;
            panel.add( label, gbc );

            if( panelType == BOT )
              classField = new JComboBox( knownBots );
            else // assume VIZ
              classField = new JComboBox( knownVizTools );
            
            classField.setEditable(true);

              gbc.gridx++;
              gbc.gridwidth = 3;
              gbc.weightx = 1.0;
              gbc.anchor = gbc.NORTHWEST;
              gbc.fill = gbc.HORIZONTAL;
            panel.add( classField, gbc );
          cards.add( panel, DATA_CARD );
        content.add( cards, BorderLayout.CENTER );

        panel = new JPanel( new FlowLayout( FlowLayout.TRAILING ) );
          addButton = new JButton( "Add "+panelType );
          addButton.addActionListener( new ActionListener() {
              public void actionPerformed( ActionEvent event ) {
                addTab();
              }
            } );
          panel.add( addButton );

          cancelButton = new JButton( "Cancel" );
          cancelButton.addActionListener( new ActionListener() {
              public void actionPerformed( ActionEvent event ) {
                hide();
                dispose();
              }
            } );
          panel.add( cancelButton );
        content.add( panel, BorderLayout.SOUTH );
      pack();
    }

    //  Private Methods
    /////////////////////////////////////////////////////////////////////////
    protected void addTab() {
      JLabel label;

      try {
        if( panelType == BOT ) {
          Class botClass = Class.forName( ((String)classField.getSelectedItem()).trim() );
          Bot bot = (Bot) botClass.newInstance();
          int team;
          String teamName = teamField.getSelectedItem().toString();
          if( teamName.equals("Any") )
            team = TEAM_ANY;
          else if( teamName.equals("Red") )
            team = TEAM_RED;
          else if( teamName.equals("Blue") )
            team = TEAM_BLUE;
          else if( teamName.equals("Green") )
            team = TEAM_GREEN;
          else if( teamName.equals("Gold") )
            team = TEAM_GOLD;
          else // if( teamName.equals("None") )
            team = TEAM_NONE;

          new BotManager( bot, nameField.getText().trim(), team );

        } else if( panelType == VIZ ) {
          Class vizClass = Class.forName( ((String)classField.getSelectedItem()).trim() );
          VizTool viz = (VizTool) vizClass.newInstance();
          new VizManager( viz, nameField.getText().trim() );
        }

        // Success, go ahead and clear the dialog
        hide();
        dispose();
      } catch( Exception error ) {
        label = new JLabel( "<html><font color='black'><b>Unable to create "+panelType+":</b><p>"+error.toString() );
        cards.add( label, ERROR_CARD );
        cards.show( ERROR_CARD );
        addButton.setVisible( false );
      }
    }
  }

  protected abstract class TabManager extends GamebotsClient.Adapter {
    //  Private Data
    ///////////////////////////////////////////////////////////////////////
    protected final Object      clientLock = new Object();
    public final GamebotsClient client;

    public final JTextAreaLog   tabLog;

    protected String            title;

    //  Protected Constructor
    ///////////////////////////////////////////////////////////////////////
    protected TabManager( String title ) {
      this.title = title;
      tabLog = new JTextAreaLog();
      client = new GamebotsClient();

      client.setLog( tabLog );
      client.addListener( this );

      if( serverAddress != null )
        client.setServerAddress( serverAddress );
    }

    //  Public Methods
    ///////////////////////////////////////////////////////////////////////
    public void connect() {
//      tabLog.logNote( "Attempting Connection: Server: "+client.getServerAddress()+":"+client.getServerPort() );
      try {
        synchronized( clientLock ) {
          client.connect();
          long remaining = TIMEOUT;
          long endTime = System.currentTimeMillis() + remaining;
          while( !client.isConnected() ) {
            try {
              clientLock.wait( TIMEOUT );
            } catch( InterruptedException error ) {
              // ignore
            }
            remaining = endTime - System.currentTimeMillis();
            if( remaining < 0 ) {
              log.logNote( title+" <"+getClass().getName()+"> connection attempt timed out ("+new Date()+")" );
              break;
            }
          }
        } // end synch
      } catch ( IOException error ) {
        log.logError( title+" <"+getClass().getName()+"> could not connect ("+new Date()+")", error );
      } catch ( RuntimeException error ) {
        log.logError( title+" <"+getClass().getName()+"> could not connect ("+new Date()+")", error );
      }
    }

    public void disconnect() {
      try {
        synchronized( clientLock ) {
          if( client.isConnected() ) {
            client.disconnect();
            long remaining = TIMEOUT;
            long endTime = System.currentTimeMillis() + remaining;
            while( !client.isConnected() ) {
              try {
                clientLock.wait( TIMEOUT );
              } catch( InterruptedException error ) {
                // ignore
              }
              remaining = endTime - System.currentTimeMillis();
              if( remaining < 0 ) {
                log.logNote( title+" <"+getClass().getName()+"> disconnection attempt timed out ("+new Date()+")" );
                break;
              }
            }
          }
        } // end synch
      } catch ( RuntimeException error ) {
        log.logError( title+" <"+getClass().getName()+"> failed duing disconnect()", error );
      }
    }

    public void destroy() {
      // close log???

      client.disconnect();
    }

    // Event Handlers
    public void connected() {
      log.logNote( "Connection: \""+title+"\" ("+new Date()+")" );
      synchronized( tabManagers ) {
        ++connectionCount;

        disconnectAll.setEnabled( true );
        reconnectAll.setEnabled( true );
        connectAll.setEnabled( connectionCount != tabCount );
      }
      synchronized( clientLock ) {
        clientLock.notifyAll();
      }
    }

    public void disconnected() {
      log.logNote( "Disconnection: \""+title+"\" ("+new Date()+")" );
      synchronized( tabManagers ) {
        --connectionCount;

        connectAll.setEnabled( true );
        boolean moreConnections = connectionCount > 0;
        disconnectAll.setEnabled( moreConnections );
        reconnectAll.setEnabled( moreConnections );
      }
      synchronized( clientLock ) {
        clientLock.notifyAll();
      }
    }

    // Public Connection Event Runners
    public Runnable connectRunner = new Runnable() {
      public void run() {
        try {
          connect();
        } catch( RuntimeException error ) {
          tabLog.logError( "Unexpected error during connection ("+new Date()+")", error );
        }
      }
    };

    public Runnable disconnectRunner = new Runnable() {
      public void run() {
        disconnect();
      }
    };

    //  Private Methods
    ///////////////////////////////////////////////////////////////////////
    protected void addTab( JComponent view ) {
      synchronized( tabManagers ) {
        ++tabCount;
        tabbedPane.add( view, title );
        tabManagers.put( view, TabManager.this );

        connectAll.setEnabled( true );
      }
    }
  }

  protected class BotManager extends TabManager {
    public final Bot bot;

    public BotManager( Bot bot, String name, int team ) {
      super( name );

      this.bot = bot;

      client.setServerPort( serverBotPort );

      bot.init( name, client, team, tabLog );
      addTab( bot.getView() );
    }

    public void connect() {
      if( client.isConnected() )
        return;

      tabLog.logNote( "Attempting Connection: Server: "+client.getServerAddress()+":"+client.getServerPort() );
      try {
        synchronized( clientLock ) {
          bot.connect();
          long remaining = TIMEOUT;
          long endTime = System.currentTimeMillis() + remaining;
          while( !client.isConnected() ) {
            try {
              clientLock.wait( TIMEOUT );
            } catch( InterruptedException error ) {
              // ignore
            }
            remaining = endTime - System.currentTimeMillis();
            if( remaining < 0 ) {
              log.logNote( BOT+" \""+bot.getName()+"\" connection attempt timed out ("+new Date()+")" );
              break;
            }
          }
        }
      } catch ( RuntimeException error ) {
        log.logError( BOT+" \""+bot.getName()+"\" could not connect ("+new Date()+")", error );
      }
    }

    public void disconnect() {
      try {
        bot.disconnect();
      } catch ( RuntimeException error ) {
        log.logError( BOT+" \""+bot.getName()+"\" could not disconnect ("+new Date()+")", error );
      }
    }

    public void destroy() {
      try {
        bot.destroy();
      } catch ( RuntimeException error ) {
        log.logError( BOT+" \""+bot.getName()+"\" failed during disconnect()", error );
      }

      super.destroy();
    }
  }

  protected class VizManager extends TabManager {
    //  Private Data
    /////////////////////////////////////////////////////////////////////////
    protected final VizTool viz;

    //  Public Methods
    /////////////////////////////////////////////////////////////////////////
    public VizManager( VizTool viz, String name ) {
      super( name );

      this.viz = viz;

      client.setServerPort( serverVizPort );

      viz.init( tabLog );
      client.addListener( viz.getListener() );
      addTab( viz.getView() );
    }
  }
}
