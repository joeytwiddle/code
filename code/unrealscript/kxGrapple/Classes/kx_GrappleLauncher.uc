//================================================================================
// kx_GrappleLauncher.
//================================================================================

// class kx_GrappleLauncher expands Translocator Config(kxGrapple); // Tried doing this so that the player's translocator bind would work automatically, but it didn't!
class kx_GrappleLauncher expands TournamentWeapon Config(kxGrapple);

// #exec AUDIO IMPORT FILE="Sounds\greset.wav" NAME="Slurp"

var config bool bAutoDrop;
var config bool bIdenticalButtons; // TODO: Not working!

var Weapon PreviousWeapon;
var kxGrapple kxGrapple;
var bool bManualShot;
var bool bShooting;
var kxMutator kxMutator;

exec function AttachHook ()
{
  PlayerPawn(Owner).ClientMessage("Trying to attachHook");
  if ( kxGrapple == None )
  {
    FireHook();
  }
}

exec function ReleaseHook ()
{
  PlayerPawn(Owner).ClientMessage("Trying to releaseHook");
  if ( kxGrapple != None )
  {
    kxGrapple.Destroy();
  }
}

simulated function ClientFireHook ()
{
  return;
}

exec function FireHook ()
{
  if ( kxGrapple != None )
  {
    return;
  }
  if ( Role < 4 )
  {
    ClientFireHook();
    return;
  }
  Fire();
  GotoState('NormalFire');
}

function Destroyed ()
{
  GetKXMutator().OnDeselect(PlayerPawn(Owner));
  if ( kxGrapple != None )
  {
    kxGrapple.Destroy();
    kxGrapple = None;
  }
  Super.Destroyed();
}

function DropFrom (Vector StartLocation)
{
  if ( kxGrapple != None )
  {
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

function Fire (optional float Value)
{
  GotoState('NormalFire');
  if ( kxGrapple == None )
  {
    if ( PlayerPawn(Owner) != None )
    {
      PlayerPawn(Owner).ShakeView(shaketime,shakemag,shakevert);
    }
    bPointing = True;
    PlayFiring();
    PlaySound(class'kxGrapple'.default.ThrowSound,SLOT_Interface,2.0);
    // AmbientSound = class'kxGrapple'.default.ThrowSound;
    // AmbientSound = Sound'Hidraul2';
    // AmbientSound = Sound'Slurp';
    kxGrapple = kxGrapple(ProjectileFire(ProjectileClass,2000.0,bWarnTarget));
    if (kxGrapple == None) {
      if (class'kxGrapple'.default.bDebugLogging) { Log(Self$".Fire() Failed to create kxGrapple!"); }
      // TODO: denied sound
    } else {
      kxGrapple.SetMaster(self);
      // TODO BUG: These sounds are not working.  It would be nice to hear the line flying out.
      kxGrapple.AmbientSound = class'kxGrapple'.default.ReleaseSound;
      AmbientSound = class'kxGrapple'.default.ReleaseSound;
    }
  } else if (bIdenticalButtons) {
    AltFire(Value);
  }
  if ( Owner.bHidden )
  {
    CheckVisibility();
  }
}

function AltFire (float Value)
{
  if ( kxGrapple != None )
  {
    AmbientSound = None;
    kxGrapple.Destroy();
    kxGrapple = None;
  } else if (bIdenticalButtons) {
    Fire(Value);
    // return; // BAD locks up all weapons till respawn :P
  }
  GotoState('AltFiring');
}

state NormalFire
{
  function Fire (float F)
  {
  }
  
  function AltFire (float F)
  {
  }
  
  function EndState ()
  {
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

state AltFiring
{
  function Fire (float F)
  {
  }
  
  function AltFire (float F)
  {
  }
  
  function EndState ()
  {
  }
  
  Begin:
    if ( kxGrapple != None )
    {
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

state Idle
{
  function AnimEnd ()
  {
    PlayIdleAnim();
  }
  
  function bool PutDown ()
  {
    GotoState('DownWeapon');
    return True;
  }
  
  Begin:
    bPointing = False;
    Disable('AnimEnd');
    PlayIdleAnim();
}

function Finish ()
{
  local Pawn PawnOwner;

  if ( bChangeWeapon )
  {
    GotoState('DownWeapon');
    return;
  }
  PawnOwner = Pawn(Owner);
  if ( PlayerPawn(Owner) == None )
  {
    PawnOwner.StopFiring();
    GotoState('Idle');
    return;
  } else {
    if ( PlayerPawn(Owner).bExtra1 != 0 )
    {
      Global.Fire(0.0);
    } else {
      if ( PawnOwner.bFire != 0 )
      {
        Global.Fire(0.0);
      } else {
        if ( PawnOwner.bAltFire != 0 )
        {
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

function SetHand (float hand)
{
  if ( hand != 2 )
  {
  if ( hand == 0 )
  {
    hand = 1.0;
  } else {
    hand *= -1;
  }
  if ( hand == -1 )
  {
    Mesh = Mesh(DynamicLoadObject("Botpack.TranslocR",Class'Mesh'));
  } else {
    Mesh = LodMesh'Transloc';
    }
  }
  Super.SetHand(hand);
}

simulated function PreBeginPlay() {
  local PlayerPawn P;
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
    return;
  }
  if (p.PlayerReplicationInfo.Deaths == 0) // Only check binds on first spawn.  More efficient but will not work if mutator is added mid-game, or Deaths is otherwise non-zero.  Alternatively, use something like bDoneCheck.
    CheckPlayerBinds(P);
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
    // P.ClientMessage("For example: SET INPUT Q GetWeapon Translocator | GetWeapon kx_GrapplingHook");
    // P.ClientMessage("You could make a keybind for your Translocator using the console, then reconnect.");
    P.ClientMessage("You should make a keybind for your Grappling Hook.");
    P.ClientMessage("Type in the console: SET INPUT Q SwitchWeapon 1");
  }
  return;
}

simulated function PlaySelect() {
  GetKXMutator().OnSelect(PlayerPawn(Owner));
  Super.PlaySelect();
}

state DownWeapon {
  function BeginState() {
    GetKXMutator().OnDeselect(PlayerPawn(Owner));
    Super.BeginState();
  }
}

function kxMutator GetKXMutator() {
  if (kxMutator!=None)
    return kxMutator;
  foreach AllActors(class'kxMutator',kxMutator) {
    break;
  }
  if (kxMutator==None) {
    Log("kx_GrappleLauncher is spawning one kxMutator to allow players to configure the weapon.");
    kxMutator = Spawn(class'kxMutator');
    Level.Game.BaseMutator.AddMutator(kxMutator);
  }
  return kxMutator;
}

defaultproperties
{
    bCanThrow=False
    FireOffset=(X=115.00,Y=15.00,Z=2.00),
    ProjectileClass=Class'kxGrapple'
    AltProjectileClass=Class'kxGrapple'
    DeathMessage="%k removed %o's skeleton with a rusty hook."
    bRotatingPickup=False
    ItemName="kx Grapple Launcher"
    PlayerViewOffset=(X=5.00,Y=-4.00,Z=-7.00),
    StatusIcon=Texture'Botpack.Icons.UseTrans'
    Mass=10.00
    bAutoDrop=False
    SelectSound=sound'UnrealI.flak.load1'
    // SelectSound=sound'kxGrapple.Slurp'
    bIdenticalButtons=False // TODO: NOT working!
}

