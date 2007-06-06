//  edu.isi.gamebots.clients.GamebotsConstants
//  Copyright 2000, University of Southern California,
//                  Information Science Institute
//
//  Personal and Educational use is hereby granted.
//  Permission required for commercial use and redistribution.


package edu.isi.gamebots.client;

import java.lang.*;
import java.awt.Color;


/**
 *  This interface defines several constants often used by
 *  {@link GamebotsClient}s and {@link Bot}s.
 *
 *  @author <a href="mailto:amarshal#gamebots@isi.edu">Andrew n marshall</a>
 */
public interface GamebotsConstants {
  //  Public Static Data
  ///////////////////////////////////////////////////////////////////////////
  public final static int DEFAULT_BOT_PORT = 3000;
  public final static int DEFAULT_VIZ_PORT = 3001;

  public final static int NODE_DOMINATION = 1; //[srs]
  public final static int NODE_NAVIGATION = 2; //[srs]
  public final static int NODE_MOVER = 3; //[srs]
  public final static int NODE_INVENTORY = 4; //[srs]


  // Generics
  public final static String TRUE = "True";
  public final static String FALSE = "False";

  public final static String LOCATION = "Location"; // 3D World coordinates
  public final static String ROTATION = "Rotation"; // A pitch,yaw,roll vector
  public final static String VELOCITY = "Velocity"; // 3D World coordinates

  // Asynchrous from server...
  public final static String INFO   = "NFO"; // Server info/comment
  public final static String SELF   = "SLF"; // Info about self
  public final static String SEE    = "SEE"; // See another player
  public final static String WALL   = "WAL"; // Hit a wall
  public final static String BUMP   = "BMP"; // Bumped another player
  public final static String DAMAGE = "DAM"; // Bumped another player
  public final static String DIE    = "DIE"; // Self Death Notification
  public final static String KILL   = "KIL"; // Player Death Notification
  public final static String FINISHED = "Fin"; // End of Game
  public final static String PONG = "Pong"; // Ping response

//[srs]:
  public final static String ITEM = "AIN"; //picked up item
  public final static String GAMESTATE = "GAM"; //game status message
  public final static String DOMPOINTS = "DomPoints";
  public final static String PATH = "PTH";
  public final static String GETPATH = "GetPath";

  // From server in the vision update...
  public final static String BEG = "BEG"; // Begin vision update
  public final static String NAV = "NAV"; // Navigation point
  public final static String DOM = "DOM"; //Domination point [srs]
  public final static String MOV = "MOV"; // world mobile [srs]
  public final static String PLAYER = "PLR"; // Other Player
  public final static String INV = "INV"; // Inventory item
  public final static String END = "END"; // End vision update


  // Client Message Types
  public final static String INIT = "Init"; // Enter Game
  public final static String RUNTO = "RunTo";
  public final static String TURNTO = "TurnTo";
  public final static String PING = "Ping";

  // Actor properties (for NAV, PLAYER, SELF, INV, ...)
  public final static String ACTOR_ID = "Id"; // Unique Actor identifier
  public final static String ACTOR_REACHABLE = "Reachable"; // Boolean, is actor reachable from current location?
  public final static String ACTOR_CLASS = "Class"; // Class name
  //[srs]
  public final static String ITEM_ID = "Id"; //weapon/item identifier
  public final static String OWNER = "Controller"; //dom point owner
  public final static String WEAPON_1 = "Bubble";
  public final static String WEAPON_2 = "Goo";

  // Player properties (for SELF, PLAYER, ane SEE)
  public final static String PLAYER_TEAM = "Team"; // Unique Actor identifier
  public final static String PLAYER_NAME = "Name";
  public final static String PLAYER_WEAPON = "Weapon";
  public final static String PLAYER_HEALTH = "Health";
  public final static String PLAYER_ARMOR = "Armor";
  public final static String PLAYER_AMMO = "CurrentAmmo";

  // Death properties (for KILL and DIE)
  public final static String DEATH_KILLER = "Killer"; // Who did the killing
  public final static String DEATH_DAMAGE_TYPE = "DamageType"; // How

  // Client Message Parameters
  public final static String ARG_TARGET = "Target";

  //[srs] Client Commands
  public final static String STOP_SHOOT = "StopShoot";
  public final static String SHOOT = "Shoot";

  // Team IDs
  public final static int TEAM_ANY = -1; //Only used by bot initialization
  public final static int TEAM_NONE = 255; //[srs]
  public final static int TEAM_RED = 0;
  public final static int TEAM_BLUE = 1;
  public final static int TEAM_GREEN = 2;
  public final static int TEAM_GOLD = 3;

  // Game mechanics constants.  No gaurantee these are correct.
  public final static double PLAYER_RADIUS =  17.0d;
  
  // Items below added by Ryan Rozich - Texas A&M University for completeness
  // Command Constant
  public final static String SETWALK = "SETWALK";
  public final static String STOP = "STOP";
  public final static String JUMP = "JUMP";
  public final static String STRAFE = "STRAFE";
  public final static String ROTATE = "ROTATE";
  public final static String CHANGEWEAPON = "CHANGEWEAPON";
  public final static String CHECKREACH = "CHECKREACH";
  public final static String MESSAGE = "MESSAGE";
  
  // Client Message Parameters
  public final static String ARG_WALK = "Walk";
  public final static String ARG_AMOUNT = "Amount";
  public final static String ARG_ALT = "Alt";
  public final static String ARG_TEXT = "Text";
  public final static String ARG_GLOBAL = "Global";


  //  Inner Classes
  //////////////////////////////////////////////////////////////////////////
  /**
   *  The following is a means for constants (liek Team IDs) to be mapped to
   *  useful objects.
   */
  public static abstract class TeamColorMap {
    public static String teamToName( int id ) {
      switch( id ) {
      case TEAM_ANY:
        return "Any";
      case TEAM_NONE:
        return "None";
      case TEAM_RED:
        return "Red";
      case TEAM_BLUE:
        return "Blue";
      case TEAM_GREEN:
        return "Green";
      case TEAM_GOLD:
        return "Gold";
      default:
        return null;
      }
    }

    public static Color teamToColor( int id ) {
      switch( id ) {
      case TEAM_RED:
        return Color.red;
      case TEAM_BLUE:
        return Color.blue;
      case TEAM_GREEN:
        return Color.green;
      case TEAM_GOLD:
        return Color.yellow.darker();
      default:
        return null;
      }
    }
  }
}
