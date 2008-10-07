//================================================================================
// kx_GrappleLauncher.
//================================================================================

// TODO: We should offer exec commands WinchGrapplingLine and ReleaseGrapplingLine, or ShortenLine and LengthenLine.

// class kx_GrappleLauncher expands Translocator Config(kxGrapple); // Tried doing this so that the player's translocator bind would work automatically, but it didn't!
class kx_GrappleLauncher expands TournamentWeapon Config(kxGrapple);

// #exec AUDIO IMPORT FILE="Sounds\greset.wav" NAME="Slurp"

var config bool bAutoDrop; // bAutoRetract?
var config bool bIdenticalButtons; // TODO: Not working!
var config bool bDisableAutoBehindview; // This is the only one we really want on the client config.  The rest are for the server (could go to another class, e.g. kxRules).  If we did that we could remove the Config(kxGrapple) above!
var config bool bLogging;
var config Sound FailSound,ThrowSound;

var Weapon PreviousWeapon;
var kxGrapple kxGrapple;
var kxMutator kxMutator;

var String NextCommand; // Used as RPC mechanism from server to client
var float LastFOV; // Used only on the client

replication {
  reliable if (Role==ROLE_Authority)
    bIdenticalButtons,bLogging,bAutoDrop,FailSound,ThrowSound,NextCommand;
  //// We do NOT want to replicate bDisableAutoBehindview - we want the client to see his version and not be updated from the server.
  // reliable if (Role!=ROLE_Authority)
    // bDisableAutoBehindview;
  reliable if (Role==ROLE_Authority)
    AutoBehindView; // In case it is called from kxMutator().
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
    Log("kx_GrappleLauncher.PreBeginPlay() NO LOCAL PLAYERPAWN!");
    // But we actually take no action :P
    return;
  }
  if (p.PlayerReplicationInfo.Deaths == 0) {
    // Only check binds on first spawn.  More efficient but will not work if mutator is added mid-game, or Deaths is otherwise non-zero.  Alternatively, use something like bDoneCheck.
    CheckPlayerBinds(P);
    // Only display grapple message on first spawn:
    // PlayerPawn(Owner).ClientMessage("To toggle the grappling hook's auto view switching, type: AutoBehindView");
    if (bDisableAutoBehindview)
      nextState="enable";
    else
      nextState="disable";
    PlayerPawn(Owner).ClientMessage("To "$nextState$" the grappling hook's auto-behindview, type: AutoBehindView");
    // TODO: This is NOT getting displayed!
  }
}

// Failed attempts to make it not the spawn weapon:
function float RateSelf(out int bUseAltMode) {
  return -2.0;
}
function SetSwitchPriority(Pawn Other) {
  AutoSwitchPriority=0;
}

simulated exec function AttachHook () {
  PlayerPawn(Owner).ClientMessage("Trying to attachHook");
  if ( kxGrapple == None ) {
    FireHook();
  }
}

simulated exec function ReleaseHook () {
  PlayerPawn(Owner).ClientMessage("Trying to releaseHook");
  if ( kxGrapple != None ) {
    kxGrapple.Destroy();
  }
}

simulated exec function FireHook () {
  if ( kxGrapple != None ) {
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

simulated function Destroyed () {
  OnDeselect();
  if ( kxGrapple != None ) {
    kxGrapple.Destroy();
    kxGrapple = None;
  }
  Super.Destroyed();
}

function DropFrom (Vector StartLocation) {
  if ( kxGrapple != None ) {
    kxGrapple.Destroy();
    kxGrapple = None;
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
    // Log("kx_GrappleLauncher.RaiseUp() Set PreviousWeapon = "$PreviousWeapon);
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
  if ( kxGrapple == None ) {
    // AmbientSound = class'kxGrapple'.default.ThrowSound;
    // AmbientSound = Sound'Hidraul2';
    // AmbientSound = Sound'Slurp';
    kxGrapple = kxGrapple(ProjectileFire(ProjectileClass,2000.0,bWarnTarget));
    if (kxGrapple == None) {
      // if (bLogging) { Log(Self$".Fire() Failed to create kxGrapple!"); }
      // DONE: denied sound
      // TODO: bug - the sounds play rapidy repeating until the button is released - just once, or once a second would be enough
      //       this is really a problem with the fire mechanism retrying, instead of waiting for release and then a second press
      PlaySound(ThrowSound,SLOT_None,0.8); // quiet failed throw
      PlaySound(FailSound,SLOT_Interface,3.0);
    } else {
      kxGrapple.SetMaster(self);
      if ( PlayerPawn(Owner) != None ) {
        PlayerPawn(Owner).ShakeView(shaketime,shakemag,shakevert);
      }
      bPointing = True;
      PlayFiring();
      PlaySound(ThrowSound,SLOT_Interface,1.6);
      // TODO BUG: These sounds are not working.  It would be nice to hear the line flying out.
      kxGrapple.AmbientSound = class'kxGrapple'.default.ReleaseSound;
      AmbientSound = class'kxGrapple'.default.ReleaseSound;
    }
  } else if (bIdenticalButtons) {
    AltFire(Value);
  }
  if ( Owner.bHidden ) {
    CheckVisibility();
  }
}

function AltFire (float Value) {
  if ( kxGrapple != None ) {
    AmbientSound = None;
    kxGrapple.Destroy();
    kxGrapple = None;
  } else if (bIdenticalButtons) {
    Fire(Value);
    // return; // BAD locks up all weapons till respawn :P
  } else {
    PlaySound(FailSound,SLOT_Interface,2.0);
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
    if ( kxGrapple != None ) {
      AmbientSound = None;
      kxGrapple.Destroy();
      kxGrapple = None;
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
            if ( kxGrapple != None ) {
              AmbientSound = None;
              kxGrapple.Destroy();
              kxGrapple = None;
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

simulated function CheckPlayerBinds(PlayerPawn P) {
  local int i;
  local string toAdd;
  local string keyName,keyVal,keyValCaps;
  local bool bBindExists;
  toAdd = "GetWeapon kx_GrappleLauncher";
  for (i=0;i<256;i++) {
    keyName = p.ConsoleCommand("keyname "$i);
    keyVal = p.ConsoleCommand("keybinding "$keyName);
    keyValCaps = Caps(keyVal);
    if (InStr(keyValCaps,Caps(toAdd))>=0) {
      return; // We have found an existing key bound to this weapon.  To save time, stop searching!
    }
    if (InStr(keyValCaps,"GETWEAPON TRANSLOCATOR")>=0 || (Right(keyValCaps,14)=="SWITCHWEAPON 1" || InStr(keyValCaps,"SWITCHWEAPON 1 ")>=0 && InStr(keyValCaps,"SWITCHWEAPON 1|")>=0)) {
      // Add a binding to this key!
      p.ConsoleCommand("SET INPUT "$keyName$" "$keyVal$" | "$toAdd);
      p.ClientMessage("Grappling hook now available on your ["$keyName$"] key.");
      // Continue to search for other binds we could attach to.
      bBindExists = True;
    }
  }
  Log("kx_GrappleLauncher.CheckPlayerBinds() Finished checking all "$p.getHumanName()$"'s keybinds.");
  if (!bBindExists) {
    // P.ClientMessage("You should make a keybind for the Translocator and Grappling Hook weapons using your console.");
    // P.ClientMessage("For example: SET INPUT Q GetWeapon Translocator | GetWeapon kx_GrappleLauncher");
    // P.ClientMessage("You could make a keybind for your Translocator using the console, then reconnect.");
    P.ClientMessage("You should make a keybind for your Grappling Hook.");
    P.ClientMessage("Type in the console: SET INPUT Q SwitchWeapon 1");
  }
  return;
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



// AutoBehindView

simulated exec function AutoBehindView(optional String extra) {
  if (extra=="0" || extra~="OFF" || extra~="NO" || extra~="False") {
    bDisableAutoBehindview = True;
  } else if (extra=="1" || extra~="ON" || extra~="YES" || extra~="True") {
    bDisableAutoBehindview = False;
  } else {
    bDisableAutoBehindview = !bDisableAutoBehindview;
  }
  SaveConfig();
  if (bDisableAutoBehindview) extra="DISABLED"; else extra="ENABLED";
  PlayerPawn(Owner).ClientMessage("The grappling hook's auto-behindview has been "$extra$", your settings were saved.");
  if (bActive) {
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

// The functions OnSelect() and OnDeselect() are sometimes called
// non-simulated, but we require that the response actions are called
// simulated, so we replicate a command to the client here, and the command
// should be run simulated in the next call to Tick().

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
  if (bLogging) { Log(Level.TimeSeconds$" "$Self$".OnSelectCheck() called with Role="$Role); }
  if (!bDisableAutoBehindview) {
    DoBehindview();
 }
}

simulated function OnDeselectCheck() {
  if (bLogging) { Log(Level.TimeSeconds$" "$Self$".OnDeselectCheck() called with Role="$Role); }
  if (!bDisableAutoBehindview) {
    UndoBehindview();
 }
}

simulated function DoBehindview() {
  LastFOV = PlayerPawn(Owner).DesiredFOV;
  // PlayerPawn(Owner).ConsoleCommand("FOV 110");
  PlayerPawn(Owner).DesiredFOV = 110;
  PlayerPawn(Owner).ConsoleCommand("BehindView 1");
}

simulated function UndoBehindview() {
  PlayerPawn(Owner).ConsoleCommand("FOV "$LastFOV);
  PlayerPawn(Owner).ConsoleCommand("BehindView 0");
}



defaultproperties {
    bCanThrow=False
    FireOffset=(X=115.00,Y=15.00,Z=2.00),
    ProjectileClass=Class'kxGrapple'
    AltProjectileClass=Class'kxGrapple'
    // DeathMessage="%k removed %o's skeleton with a rusty hook."
    // DeathMessage="%k ripped %o into chunks with a grappling hook!"
    // DeathMessage="%k tore %o into chunks with a grappling hook!"
    // DeathMessage="%k tried to climb %o but tore him into chunks!"
    DeathMessage="%k chopped %o into chunks with a grappling hook!"
    bRotatingPickup=False
    ItemName="kx Grappling Hook"
    PlayerViewOffset=(X=5.00,Y=-4.00,Z=-7.00),
    StatusIcon=Texture'Botpack.Icons.UseTrans'
    Mass=25.00
    bAutoDrop=False
    SelectSound=Sound'UnrealI.flak.load1'
    // These should really be FireSound and AltFireSound:
    ThrowSound=Sound'Botpack.Translocator.ThrowTarget'
    FailSound=Sound'Botpack.Translocator.ReturnTarget'
    // SelectSound=sound'kxGrapple.Slurp'
    // bIdenticalButtons=True // TODO: NOT working!
    NetPriority=2.95 // I was hoping here to make replication of NextCommand as fast as possible.

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
    // bDisableAutoBehindview=True // I fear setting a default might override the client value.
    LastFOV=90 // In case we accidentally read it before writing it!
}

