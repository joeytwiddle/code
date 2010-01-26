//================================================================================
// GrappleGun.
//================================================================================

// TODO: We should offer exec commands WinchGrapplingLine and ReleaseGrapplingLine, or ShortenLine and LengthenLine.

// class GrappleGun expands Translocator config(GrapplingHook); // Tried doing this so that the player's translocator bind would work automatically, but it didn't!
class GrappleGun expands TournamentWeapon config(kxGrapple);

#exec AUDIO IMPORT FILE="Sounds\Pull.wav" NAME="Pull" // grindy windy one from ND
#exec AUDIO IMPORT FILE="Sounds\greset.wav" NAME="Slurp" // metallic slurp from ND
#exec AUDIO IMPORT FILE="Sounds\hit1g.wav" NAME="hit1g" // From UnrealI.GasBag
// #exec AUDIO IMPORT FILE="Sounds\SoftPull.wav" NAME="SoftPull" // softer pull from ND (more robotic)
// #exec AUDIO IMPORT FILE="Sounds\End.wav" NAME="KrrChink" // kchink when grapple hits target

// Server config:
var config bool bLogging,bShowInfo;
var config Sound FailSound,ThrowSound;
// Could move to client:
var config bool bAutoDrop; // bAutoRetract?
var config bool bIdenticalButtons; // TODO: Not working!

// Client config:
var config bool bDisableAutoBehindview;
var config bool bFireToWinch;
var config int BehindViewFOV;

var Weapon PreviousWeapon;
var GrapplingHook GrapplingHook;
var GrapplingMut GrapplingMut;

var String NextCommand; // Used as RPC mechanism from server to client
var float LastFOV; // Used only on the client

replication {
  reliable if (Role==ROLE_Authority)
    bLogging,bShowInfo,bIdenticalButtons,bAutoDrop,FailSound,ThrowSound,NextCommand;
  // WARN CHECK TODO: If we don't somehow sync these between client and server, things might start acting weird: bAutoDrop,bIdenticalButtons
  // Client vars (may want to send to server):
  //   bAutoDrop,bIdenticalButtons,bDisableAutoBehindview,BehindViewFOV;
  //// We do NOT want to replicate bDisableAutoBehindview - we want the client to see his version and not be updated from the server.
  //// Blimey.  We need to replicate these player configs in entirely different ways, because of how they get picked up.
  reliable if (Role==ROLE_Authority) // Get changed and used on the client
    bDisableAutoBehindview,BehindViewFOV;
  reliable if (Role==ROLE_Authority)
    ABV,AutoBehindView;
  reliable if (Role!=ROLE_Authority) // Gets changed on the client, but used on the server.
    bFireToWinch;
  unreliable if (Role!=ROLE_Authority)
    FTW,FireToWinch;
  reliable if (Role<4)
    DoubleJump;
}

simulated function PreBeginPlay() {
  local PlayerPawn P;
  local String nextState;
  Super.PreBeginPlay();
  if ( Level.NetMode == 1 ) {
    return;
  }
  foreach AllActors(Class'PlayerPawn',P) {
    if ( Viewport(P.Player) != None ) {
      break;
    } else {
      P = None;
    }
  }
  if ( P == None ) {
    Log("GrappleGun.PreBeginPlay() NO LOCAL PLAYERPAWN!");
    // But we actually take no action :P
    // I never see the server complain about this.  Ah we checked NetMode earlier.
    return;
  }

  if (BehindViewFOV==0)
    BehindViewFOV=110;

  // It is quite important that this runs at the start of each new player's game, or they might not have the binds everyone else has, which would be unfair!
  // But it's also important that it runs only once, since otherwise it will cause lag and spam messages during play.
  // FIXED: This was getting called for every ForceGun created in the scene, not only the one belonging to this player.  The ==Self check below fixed it.
  // I tried checking p==Owner or Instigator, but both were None.
  // FAILED: P.Weapon==None should at least check that the player has not already spawned.  He might get more than one message if other bots/players spawn at the same time, but he won't get any messages once he has finished spawning and has a weapon in hard.
  // if (p.PlayerReplicationInfo.Deaths == 0 && P.Weapon==None) {
  // FIXED now I hope.  We check if P has a grappling gun which is not this gun.  If his gun is this gun or he has none, and this is his first life, only then we message him.
	// TODO TESTING: probable bug - if they have died before getting the weapon (likely in some modes), then they won't get checked!!
	//       solution - change a client side default bool from false to true :)
  if (p.PlayerReplicationInfo.Deaths == 0 && P.FindInventoryType(class'GrappleGun')==None || P.FindInventoryType(class'GrappleGun')==Self) {
    if (bLogging) { Log(Self$" Checking binds for "$P$" while Owner="$owner$" Instigator="$Instigator$" Deaths="$p.PlayerReplicationInfo.Deaths$" P.Weapon="$P.Weapon$" gg="$P.FindInventoryType(class'GrappleGun')); }
    // Only check binds on first spawn.  More efficient but will not work if weapon is not available on first spawn, e.g. mutator is added mid-game, or Deaths is otherwise non-zero.  Alternatively, use something like GrapplingMut.bDoneCheck[].
    CheckPlayerBinds(P);
    // Only display grapple message on first spawn:
		if (bShowInfo) {
			// p.ClientMessage("To toggle the grappling hook's auto view switching, type: ABV");
			if (bDisableAutoBehindview)
				nextState="enable";
			else
				nextState="disable";
			// p.ClientMessage("You can use the AutoBehindView command to "$nextState$" the grappling hook's behind-view switching");
			// p.ClientMessage("To "$nextState$" the grappling hook's auto-behindview, type: ABV");
			// p.ClientMessage("To "$nextState$" the grappling hook's AutoBehindView, type: ABV");
			// P.ClientMessage("Toggle the grappling hook's features with ABV and FTW");
			// P.ClientMessage("Crouch to unwind, Jump off if holding another weapon, Fire affects winching.");
			P.ClientMessage("Grappling Hook!  Aim high and swing!  Use ABV and FTW to toggle features.");
			P.ClientMessage("Fire pauses the winch, Crouch unwinds, Jump releases the grapple if you have another weapon.");
		}
  } else {
    if (bLogging) { Log(Self$" Not checking binds for "$P$" while Owner="$owner$" Instigator="$Instigator$" Deaths="$p.PlayerReplicationInfo.Deaths$" P.Weapon="$P.Weapon$" gg="$P.FindInventoryType(class'GrappleGun')); }
  }

}

function bool ContainsCommand(String bindStr, String cmd) {
	return (
		(
			// bindStr contains cmd with a neat end
			Right(bindStr,Len(cmd))==cmd
			|| InStr(bindStr,cmd$" ")>=0
			|| InStr(bindStr,cmd$"|")>=0
		) && (
			// bindStr contains cmd with a neat start
			Left(bindStr,Len(cmd))==cmd
			|| InStr(bindStr," "$cmd)>=0
			|| InStr(bindStr,"|"$cmd)>=0
		)
	);
}

simulated function CheckPlayerBinds(PlayerPawn P) {
  local int i;
  local string toAdd;
  local string keyName,keyVal,keyValCaps;
  local bool bBindExists;
  toAdd = "GetWeapon GrappleGun";
  for (i=0;i<256;i++) {
    keyName = p.ConsoleCommand("keyname "$i);
    keyVal = p.ConsoleCommand("keybinding "$keyName);
    keyValCaps = Caps(keyVal);
    // if (InStr(keyValCaps,Caps(toAdd))>=0) {
      // return; // We have found an existing key bound to this weapon.  To save time, stop searching!
    // }
    if (
			// InStr(keyValCaps,"GETWEAPON TRANSLOCATOR")>=0
			// || Right(keyValCaps,14)=="SWITCHWEAPON 1"
			// || InStr(keyValCaps,"SWITCHWEAPON 1 ")>=0
			// || InStr(keyValCaps,"SWITCHWEAPON 1|")>=0
			ContainsCommand(keyValCaps,"GETWEAPON TRANSLOCATOR")
			|| ContainsCommand(keyValCaps,"SWITCHWEAPON 1")
		) {
      if (InStr(keyValCaps,Caps(toAdd))==-1) {
        // Add a binding to this key!
        p.ConsoleCommand("SET INPUT "$keyName$" "$keyVal$"| "$toAdd);
        p.ClientMessage("* Grappling hook now available on your ["$keyName$"] key.");
        // Continue to search for other binds we could attach to.
      }
      bBindExists = True;
    }
    if (
			// InStr(keyValCaps,"JUMP")>=0
			ContainsCommand(keyValCaps,"JUMP")
			&& InStr(keyValCaps,"MUTATE GRAPPLEJUMP")==-1
		) {
      // Add a binding to this key!
      p.ConsoleCommand("SET INPUT "$keyName$" "$keyVal$"| Mutate GrappleJump");
      // p.ClientMessage("* Grappling jump now available on your ["$keyName$"] key."); // They don't care :P
      // Continue to search for other binds we could attach to.
      // bBindExists = True;
    }
  }
  Log("GrappleGun.CheckPlayerBinds() Finished checking all "$p.getHumanName()$"'s keybinds.");
  if (!bBindExists) {
    // P.ClientMessage("You should make a keybind for the Translocator and Grappling Hook weapons using your console.");
    // P.ClientMessage("For example: SET INPUT Q GetWeapon Translocator | GetWeapon GrappleGun");
    // P.ClientMessage("You could make a keybind for your Translocator using the console, then reconnect.");
    // P.ClientMessage("You should make a keybind for your Grappling Hook.");
    // P.ClientMessage("Type in the console: SET INPUT Q GetWeapon GrappleGun");
    P.ClientMessage("# You need to set a keybind in the console: SET INPUT Q GetWeapon GrappleGun");
    Log("Somehow creation of GrapplingHook keybinds failed.");
  }
  return;
}

simulated function Destroyed () {
  OnDeselect();
  if ( GrapplingHook != None ) {
    GrapplingHook.Destroy();
    GrapplingHook = None;
  }
  Super.Destroyed();
}

function float RateSelf(out int bUseAltMode) {
  if (bUseAltMode==0) {
    if (GrapplingHook==None)
      return 0.1;
    else
      return -0.1;
  } else {
    if (GrapplingHook!=None)
      return 0.2;
    else
      return -0.5;
  }
}

// Stops GrappleGun from being the spawn weapon (IF called when weapon is created!):
// I'm not sure this works :P
function SetSwitchPriority(Pawn Other) {
  AutoSwitchPriority=0;
}

// Old inherited functions from Expert100 which we don't really need?
simulated exec function AttachHook () {
  PlayerPawn(Owner).ClientMessage("Trying to attachHook");
  if ( GrapplingHook == None ) {
    FireHook();
  }
}
simulated exec function ReleaseHook () {
  PlayerPawn(Owner).ClientMessage("Trying to releaseHook");
  if ( GrapplingHook != None ) {
    GrapplingHook.Destroy();
  }
}
// We tried using this in "Mutate FireHook" but it didn't work all the time.
simulated exec function FireHook () {
  if ( GrapplingHook != None ) {
    return;
  }
  if ( Role < 4 ) {
    ClientFireHook();
    return;
  }
  Fire();
  GotoState('NormalFire');
}

simulated function ClientFireHook () {
  return;
}

function DropFrom (Vector StartLocation) {
  if ( GrapplingHook != None ) {
    GrapplingHook.Destroy();
    GrapplingHook = None;
  }
  Super.DropFrom(StartLocation);
}

function BringUp () {
  PreviousWeapon = None;
  Super.BringUp();
}

function RaiseUp (Weapon OldWeapon) {
  if ( OldWeapon == self ) {
    PreviousWeapon = None;
  } else {
    PreviousWeapon = OldWeapon;
    // Log("GrappleGun.RaiseUp() Set PreviousWeapon = "$PreviousWeapon);
  }
  Super.BringUp();
}

function ReturnToPreviousWeapon() {
  if ( (PreviousWeapon == None) ||
       ((PreviousWeapon.AmmoType != None) && (PreviousWeapon.AmmoType.AmmoAmount <=0))
  ) {
    Pawn(Owner).SwitchToBestWeapon();
  } else {
    Pawn(Owner).PendingWeapon = PreviousWeapon;
    PutDown();
  }
}

function Fire (optional float Value) {
  GotoState('NormalFire');
  if ( GrapplingHook == None ) {
    // AmbientSound = class'GrapplingHook'.default.ThrowSound;
    // AmbientSound = Sound'Hidraul2';
    // AmbientSound = Sound'Slurp';
    GrapplingHook = GrapplingHook(ProjectileFire(ProjectileClass,1,bWarnTarget));
    if (GrapplingHook == None) {
      //// Hook failed to spawn - probably we are standing right in front of a wall, so there is no space for it!
      // if (bLogging) { Log(Self$".Fire() Failed to create GrapplingHook!"); }
      // DONE: denied sound
      // TODO: bug - the sounds play rapidy repeating until the button is released.  Just once, or once a second would be enough
      //       this is really a problem with the fire mechanism retrying, instead of waiting for release and then a second press.
      //       Well I got used to this sound by now.  At least it makes it clear that the Grapple is trying to fire and failing.
      PlaySound(ThrowSound,SLOT_None,0.8); // quiet failed throw
      PlaySound(FailSound,SLOT_Interface,3.0);
    } else {
      GrapplingHook.SetMaster(self);
      if ( PlayerPawn(Owner) != None ) {
        PlayerPawn(Owner).ShakeView(shaketime,shakemag,shakevert);
      }
      bPointing = True;
      PlayFiring();
      PlaySound(ThrowSound,SLOT_Interface,1.6);
      // TODO BUG: These sounds are not working.  It would be nice to hear the line flying out.
      GrapplingHook.AmbientSound = class'GrapplingHook'.default.ReleaseSound;
      AmbientSound = class'GrapplingHook'.default.ReleaseSound;
    }
  } else if (bIdenticalButtons) {
    AltFire(Value);
  }
  if ( Owner.bHidden ) {
    CheckVisibility();
  }
}

function AltFire (float Value) {
  if ( GrapplingHook != None ) {
    AmbientSound = None;
    GrapplingHook.Destroy();
    GrapplingHook = None;
  } else if (bIdenticalButtons) {
    Fire(Value);
    // return; // BAD locks up all weapons till respawn :P
  } else {
    // PlaySound(FailSound,SLOT_Interface,2.0);
  }
  GotoState('AltFiring');
}

state NormalFire {
  function Fire (float F) {
  }

  function AltFire (float F) {
  }

  function EndState () {
  }

  Begin:
    FinishAnim();
    Sleep(0.1);
    // From Botpack.Translocator:
    if ( (Pawn(Owner).bFire != 0) && (Pawn(Owner).bAltFire != 0) ) {
      ReturnToPreviousWeapon();
    }
    //
    Finish();
}

state AltFiring {
  function Fire (float F) {
  }

  function AltFire (float F) {
  }

  function EndState () {
  }

  Begin:
    if ( GrapplingHook != None ) {
      AmbientSound = None;
      GrapplingHook.Destroy();
      GrapplingHook = None;
    }
    FinishAnim();
    Sleep(0.1);
    // From Botpack.Translocator:
    if ( (Pawn(Owner).bFire != 0) && (Pawn(Owner).bAltFire != 0) ) {
      ReturnToPreviousWeapon();
    }
    //
    Finish();
}

state Idle {
  function AnimEnd () {
    PlayIdleAnim();
  }

  simulated function bool PutDown () {
    GotoState('DownWeapon');
    return True;
  }

  Begin:
    bPointing = False;
    Disable('AnimEnd');
    PlayIdleAnim();
}

simulated function Finish () {
  local Pawn PawnOwner;

  if ( bChangeWeapon ) {
    GotoState('DownWeapon');
    return;
  }
  PawnOwner = Pawn(Owner);
  if ( PlayerPawn(Owner) == None ) {
    PawnOwner.StopFiring();
    GotoState('Idle');
    return;
  } else {
    if ( PlayerPawn(Owner).bExtra1 != 0 ) {
      Global.Fire(0.0);
    } else {
      if ( PawnOwner.bFire != 0 ) {
        Global.Fire(0.0);
      } else {
        if ( PawnOwner.bAltFire != 0 ) {
          Global.AltFire(0.0);
        } else {
          GotoState('Idle');
          // I guess this was primary-fire release-button ev_nt.
          if (bAutoDrop) {
            // If you release primary fire, your grapple retracts
            if ( GrapplingHook != None ) {
              AmbientSound = None;
              GrapplingHook.Destroy();
              GrapplingHook = None;
            }
          } else {
            // Hook continues to fly until you force it to release and retract.
          }
        }
      }
    }
  }
}

/*
function SetHand (float hand) {
  Mesh = LodMesh'Transloc'; // Upper hand
  // Mesh = LodMesh'TranslocR'; // Upper hand
  Mesh = Mesh(DynamicLoadObject("Botpack.TranslocR",Class'Mesh'));
  // // Mesh = Mesh(DynamicLoadObject("Botpack.Transloc",Class'Mesh'));
}
*/

function SetHand (float hand) {
  if ( hand != 2 ) {
    if ( hand == 0 ) {
      hand = 1.0;
    } else {
      hand *= -1;
    }
    if ( hand == -1 ) {
      Mesh = Mesh(DynamicLoadObject("Botpack.TranslocR",Class'Mesh'));
      // Mesh = Mesh(DynamicLoadObject("Botpack.Transloc",Class'Mesh'));
    } else {
      Mesh = PlayerViewMesh;
    }
  }
  Super.SetHand(hand);
}

simulated function PlaySelect() {
  OnSelect();
  Super.PlaySelect(); // Avoids errors thrown by missing meshes.
}

state DownWeapon {
  function BeginState() {
    OnDeselect(); // Was not being called simulated, so moved it to the GotoState() calls, but still not.  So made it work non-replicated.
    Super.BeginState();
  }
}

// This makes jumping off the line more reliable, 
// WARN: I suspected that sometimes DoubleJumpUT was swallowing jump presses before they could reach the GrapplingHook.  I generally tested with the DoubleJumpUT mutator listed after the GrapplingMut in the mutator list.
// It is fired on the client and the call is replicated on the server, where the action happens.
// Or if not, it should be done by the mutator call in GrapplingMut, which should have been bound to all of the player's Jump keys in CheckPlayerBinds() earlier.
// TODO: Test can we do it without the mutate?
// TODO: Test fully and analyse then document before removing: do we need to be simulated, do we need to be exec?
simulated exec function GrappleJump() {
  // if (Role == ROLE_Authority) { // Only acts on the server
    // PlayerPawn(Owner).ClientMessage("GrappleGun is processing DoubleJump call.");
    if (GrapplingHook==None) {
      // PlayerPawn(Owner).ClientMessage("Your GrappleGun has no GrapplingHook!");
    } else {
      if (PlayerPawn(Owner)!=None && GrappleGun(PlayerPawn(Owner).Weapon)==None) {
        // TODO: Ideally only log client-side role - server is expected.
        if (bLogging) { PlayerPawn(Owner).ClientMessage("Forcing un-grapple through GrappleGun.GrappleJump() with ROLE="$Role); }
        // CONSIDER: We could refactor this into DoGrappleJump()
        GrapplingHook.Destroy();
      } else {
        // bLogging? PlayerPawn(Owner).ClientMessage("Not ungrappling, weapon="$PlayerPawn(Owner).Weapon);
      }
    }
  // }
}

// TOTEST: I'm not convinced this is working at all when DoubleJump is present anywhere in the mutator chain.
// TODO: Rename this - we are calling it from GrapplingMut now, rather than receiving the DoubleJump call directly.
simulated exec function DoubleJump() {
   GrappleJump();
}

simulated exec function Jump() {
  if (bLogging) { PlayerPawn(Owner).ClientMessage("GrappleGun.JUMP() was called!"); }
}



// AutoBehindView options

simulated exec function Status() {
	local GrapplingHook hook;
	foreach AllActors(class'GrapplingHook',hook) {
		break;
	}
	if (hook == None) {
		PlayerPawn(Owner).ClientMessage("I can only show the kxGrapple settings if there is a grappling hook deployed!");
	} else {
		//// Well the server default values do appears to match the current server config values.
		//// If only this was a simulated functi_n, we could check the client's values also, to see if replication is working properly.
		//// TODO: Move this into an exec functi0n in the weapon.
		PlayerPawn(Owner).ClientMessage("Client GrapplingGun status: bDisableAutoBehindview="$bDisableAutoBehindview$" bFireToWinch="$bFireToWinch);
		PlayerPawn(Owner).ClientMessage("Client GrapplingHook defaults: Speed="$hook.default.Speed$" GrappleSpeed="$hook.default.GrappleSpeed$" bSwingPhysics="$hook.default.bSwingPhysics$" bLinePhysics="$hook.default.bLinePhysics);
		PlayerPawn(Owner).ClientMessage("Client GrapplingHook status :  Speed="$hook.Speed$" GrappleSpeed="$hook.GrappleSpeed$" bSwingPhysics="$hook.bSwingPhysics$" bLinePhysics="$hook.bLinePhysics$" bPrimaryWinch="$hook.bPrimaryWinch);
		PlayerPawn(Owner).ClientMessage("Client GrapplingHook other: Owner="$hook.Owner$" Master="$hook.Master$" InstigatorRep="$hook.InstigatorRep$" LineSprite="$hook.LineSprite);
	}
}

simulated exec function AutoBehindView(optional String extra) {
  ABV(extra);
}
simulated exec function ABV(optional String extra) {
  if (extra=="0" || extra~="OFF" || extra~="NO" || extra~="False") {
    bDisableAutoBehindview = True;
  } else if (extra=="1" || extra~="ON" || extra~="YES" || extra~="True") {
    bDisableAutoBehindview = False;
  } else {
    bDisableAutoBehindview = !bDisableAutoBehindview;
  }
  SaveConfig();
  if (bDisableAutoBehindview) extra="DISABLED"; else extra="ENABLED";
  PlayerPawn(Owner).ClientMessage("The grappling hook's Auto-BehindView has been "$extra$", your settings were saved.");
  // if (bActive) { // TODO: FAIL!
  if (Pawn(Owner).Weapon == Self) {
    // The grapple is the current weapon.  We probably need to switch view since behaviour has just changed.
    if (bDisableAutoBehindview) {
      UndoBehindview(); // Without checking our setting, undo the view change we probably made earlier.
    } else {
      DoBehindview(); // Set the view as desired.
    }
  } else {
    // We go not have the grapple selected.  We should be in the correct view style already.
  }
}

simulated exec function FireToWinch(optional String extra) {
  FTW(extra);
}
simulated exec function FTW(optional String extra) {
  if (extra=="0" || extra~="OFF" || extra~="NO" || extra~="False") {
    bFireToWinch = False;
  } else if (extra=="1" || extra~="ON" || extra~="YES" || extra~="True") {
    bFireToWinch = True;
  } else {
    bFireToWinch = !bFireToWinch;
  }
  SaveConfig();
  if (bFireToWinch) extra="ENABLED"; else extra="DISABLED";
  PlayerPawn(Owner).ClientMessage("The grappling hook's Fire-To-Winch has been "$extra$", your settings were saved.");
  // Take immediate effect:
  if (GrapplingHook != None) {
    GrapplingHook.bPrimaryWinch = bFireToWinch;
  }
}



// AutoBehindView event handling

// The functions OnSelect() and OnDeselect() are sometimes called
// non-simulated, but we require that the response actions are called
// simulated, so we replicate a command to the client here, and the command
// should be run simulated in the next call to Tick().
// This is not a queue; there should only be one happening at a time.

function OnSelect() {
  NextCommand = "onselect";
}

function OnDeselect() {
  NextCommand = "ondeselect";
}

simulated event Tick(float DeltaTime) {
  Super.Tick(DeltaTime);
  if (Role==ROLE_Authority) // We only want to execute these commands on the client.
    return;
  if (NextCommand=="")
    return;
  if (NextCommand=="onselect") {
    OnSelectCheck();
  } else if (NextCommand=="ondeselect") {
    OnDeselectCheck();
  } else {
    Log(Self$".Tick() Error! NextCommand=\""$NextCommand$"\" is an unknown command!");
  }
  NextCommand="";
}

simulated function OnSelectCheck() {
  // if (bLogging) { Log(Level.TimeSeconds$" "$Self$".OnSelectCheck() called with Role="$Role); }
  if (!bDisableAutoBehindview) {
    DoBehindview();
 }
}

simulated function OnDeselectCheck() {
  // if (bLogging) { Log(Level.TimeSeconds$" "$Self$".OnDeselectCheck() called with Role="$Role); }
  if (!bDisableAutoBehindview) {
    UndoBehindview();
 }
}

// TODO: If server has behindview disabled, this will have the effect of only changing their FOV, which is not really desirable.  We could try to detect whether the server has BV disabled, and if so, do nothing.
// TODO TEST: What happens if server has restricted FOV?  It might cause our FOV 110 to fail entirely because the server max is 105, in which case we should have gone for 105.

simulated function DoBehindview() {
  if (PlayerPawn(Owner)==None)
    return;
  LastFOV = PlayerPawn(Owner).DesiredFOV;
  // PlayerPawn(Owner).ConsoleCommand("FOV "$BehindViewFOV);
  PlayerPawn(Owner).DesiredFOV = BehindViewFOV;
  PlayerPawn(Owner).ConsoleCommand("BehindView 1");
}

simulated function UndoBehindview() {
  if (PlayerPawn(Owner)==None)
    return;
  PlayerPawn(Owner).ConsoleCommand("FOV "$LastFOV);
  PlayerPawn(Owner).ConsoleCommand("BehindView 0");
}



defaultproperties {
    bCanThrow=False
    FireOffset=(X=115.00,Y=15.00,Z=2.00),
    ProjectileClass=Class'GrapplingHook'
    AltProjectileClass=Class'GrapplingHook'
    // DeathMessage="%k removed %o's skeleton with a rusty hook."
    // DeathMessage="%k ripped %o into chunks with a grappling hook!"
    // DeathMessage="%k tore %o into chunks with a grappling hook!"
    // DeathMessage="%k tried to climb %o but tore him into chunks!"
    DeathMessage="%k chopped %o into chunks with a grappling hook!"
    bRotatingPickup=True
    ItemName="kx Grappling Hook"
    PlayerViewOffset=(X=5.00,Y=-4.00,Z=-7.00),
    StatusIcon=Texture'Botpack.Icons.UseTrans'
    Mass=75.00
    bAutoDrop=False
    SelectSound=Sound'UnrealI.flak.load1'
    // These should really be FireSound and AltFireSound:
    ThrowSound=Sound'Botpack.Translocator.ThrowTarget'
    FailSound=Sound'Botpack.Translocator.ReturnTarget'
    PickupSound=Sound'UnrealShare.Pickups.WeaponPickup'
    // PickupSound=Sound'UnrealI.Pickups.WeaponPickup'
    // SelectSound=sound'GrapplingHook.Slurp'
    // bIdenticalButtons=True // TODO: NOT working!
    NetPriority=2.95 // I was hoping here to make replication of NextCommand as fast as possible.

    ProjectileSpeed=3000

    // From Translocator.uc:
    AutoSwitchPriority=0
    InventoryGroup=0
    // AutoSwitchPriority=0
    // InventoryGroup=0
    FiringSpeed=1.0
    PickupAmmoCount=1
    PlayerViewMesh=Mesh'Botpack.TranslocR'
    PickupViewMesh=Mesh'Botpack.Trans3loc'
    ThirdPersonMesh=Mesh'Botpack.Trans3loc'
    StatusIcon=Texture'Botpack.Icons.UseTrans'
    Icon=Texture'Botpack.Icons.UseTrans'
    // Mesh=Mesh'Botpack.Trans3loc'
    // bDisableAutoBehindview=True   // Do not set a default for this - it will override the client's value!
    // bFireToWinch=False   // Likewise
    LastFOV=90 // In case we accidentally read it before writing it!
    bShowInfo=True
}

