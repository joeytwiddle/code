//================================================================================
// kx_GrappleLauncher.
//================================================================================

// class kx_GrappleLauncher expands Translocator Config(kxGrapple); // Tried doing this so that the player's translocator bind would work automatically, but it didn't!
class kx_GrappleLauncher expands TournamentWeapon Config(kxGrapple);

#exec AUDIO IMPORT FILE="Sounds\greset.wav" NAME="Slurp"

var config bool bAutoDrop;

var Weapon PreviousWeapon;
var kxGrapple kxGrapple;
var bool bManualShot;
var bool bShooting;
var() config bool bUseExtra0;
var() config bool bUseExtra1;

exec function AttachHook ()
{
  PlayerPawn(Owner).ClientMessage("Trying to attachHook"); // 0x00000013 : 0x0000
  if ( kxGrapple == None ) // 0x00000034 : 0x002A
  {
    FireHook(); // 0x0000003C : 0x0035
  }
}

exec function ReleaseHook ()
{
  PlayerPawn(Owner).ClientMessage("Trying to releaseHook"); // 0x00000014 : 0x0000
  if ( kxGrapple != None ) // 0x00000036 : 0x002B
  {
    kxGrapple.Destroy(); // 0x0000003E : 0x0036
  }
}

simulated function ClientFireHook ()
{
  return; // 0x00000013 : 0x0000
}

exec function FireHook ()
{
  if ( kxGrapple != None ) // 0x00000013 : 0x0000
  {
    return; // 0x0000001B : 0x000B
  }
  if ( Role < 4 ) // 0x0000001D : 0x000D
  {
    ClientFireHook(); // 0x00000029 : 0x001B
    return; // 0x0000002C : 0x0021
  }
  Fire(); // 0x0000002E : 0x0023
  GotoState('NormalFire'); // 0x00000031 : 0x0029
}

function Destroyed ()
{
  if ( kxGrapple != None ) // 0x00000014 : 0x0000
  {
    kxGrapple.Destroy(); // 0x0000001C : 0x000B
    kxGrapple = None; // 0x00000025 : 0x0017
  }
  Super.Destroyed(); // 0x00000029 : 0x001E
}

function DropFrom (Vector StartLocation)
{
  if ( kxGrapple != None ) // 0x00000016 : 0x0000
  {
    kxGrapple.Destroy(); // 0x0000001E : 0x000B
    kxGrapple = None; // 0x00000027 : 0x0017
  }
  Super.DropFrom(StartLocation); // 0x0000002B : 0x001E
}

function BringUp ()
{
  PreviousWeapon = None; // 0x00000014 : 0x0000
  Super.BringUp(); // 0x00000018 : 0x0007
}

function RaiseUp (Weapon OldWeapon)
{
  if ( OldWeapon == self ) // 0x00000015 : 0x0000
  {
    PreviousWeapon = None; // 0x0000001D : 0x000B
  } else { // 0x00000021 : 0x0012
    PreviousWeapon = OldWeapon; // 0x00000024 : 0x0015
  }
  BringUp(); // 0x00000029 : 0x0020
}

function Fire (optional float Value)
{
  GotoState('NormalFire'); // 0x00000015 : 0x0000
  if ( kxGrapple == None ) // 0x00000019 : 0x0007
  {
    if ( PlayerPawn(Owner) != None ) // 0x00000021 : 0x0012
    {
      PlayerPawn(Owner).ShakeView(shaketime,shakemag,shakevert); // 0x0000002B : 0x0022
    }
    bPointing = True; // 0x00000040 : 0x0045
    PlayFiring(); // 0x00000046 : 0x004D
    // AmbientSound = Sound'Hidraul2'; // 0x0000004A : 0x0053
    AmbientSound = Sound'Slurp'; // 0x0000004A : 0x0053
    kxGrapple = kxGrapple(ProjectileFire(ProjectileClass,2000.0,bWarnTarget)); // 0x00000050 : 0x005E
    kxGrapple.SetMaster(self); // 0x00000065 : 0x007F
  }
  if ( Owner.bHidden ) // 0x0000006F : 0x008F
  {
    CheckVisibility(); // 0x0000007C : 0x00A1
  }
}

function AltFire (float Value)
{
  if ( kxGrapple != None ) // 0x00000015 : 0x0000
  {
    AmbientSound = None; // 0x0000001D : 0x000B
    kxGrapple.Destroy(); // 0x00000021 : 0x0012
    kxGrapple = None; // 0x0000002A : 0x001E
  }
  GotoState('AltFiring'); // 0x0000002E : 0x0025
}

// Had to hide states - it doesn't like overriding them.

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
    FinishAnim(); // 0x00000014 : 0x0000
    Sleep(0.1); // 0x00000017 : 0x0003
    Finish(); // 0x0000001F : 0x000B
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
    if ( kxGrapple != None ) // 0x00000014 : 0x0000
    {
      AmbientSound = None; // 0x0000001C : 0x000B
      kxGrapple.Destroy(); // 0x00000020 : 0x0012
      kxGrapple = None; // 0x00000029 : 0x001E
    }
    FinishAnim(); // 0x0000002D : 0x0025
    Sleep(0.1); // 0x00000030 : 0x0028
    Finish(); // 0x00000038 : 0x0030
}

state Idle
{
  function AnimEnd ()
  {
    PlayIdleAnim(); // 0x00000013 : 0x0000
  }
  
  function bool PutDown ()
  {
    GotoState('DownWeapon'); // 0x00000016 : 0x0000
    return True; // 0x0000001A : 0x0007
  }
  
  Begin:
    bPointing = False; // 0x00000014 : 0x0000
    Disable('AnimEnd'); // 0x0000001A : 0x0008
    PlayIdleAnim(); // 0x0000001E : 0x000F
}



/*
function Fire(float Value) {
  Super.AltFire(Value);
}
*/

  /*
  if (!bAutoDrop) { // If you fire when it is already out, retract it:
    if ( kxGrapple != None ) // 0x0000009C : 0x00C2
    {
      AmbientSound = None; // 0x000000A4 : 0x00CD
      kxGrapple.Destroy(); // 0x000000A8 : 0x00D4
      kxGrapple = None; // 0x000000B1 : 0x00E0
      return;
    }
  }
  */

/*
function AltFire(float Value) {
  // Super.Fire(Value);
  Super.AltFire(Value);
}
*/


function Finish ()
{
  local Pawn PawnOwner;

  if ( bChangeWeapon ) // 0x00000014 : 0x0000
  {
    GotoState('DownWeapon'); // 0x0000001B : 0x0009
    return; // 0x0000001F : 0x0010
  }
  PawnOwner = Pawn(Owner); // 0x00000021 : 0x0012
  if ( PlayerPawn(Owner) == None ) // 0x00000028 : 0x0022
  {
    PawnOwner.StopFiring(); // 0x00000032 : 0x0032
    GotoState('Idle'); // 0x0000003C : 0x0041
    return; // 0x00000040 : 0x0048
  } else { // 0x00000042 : 0x004A
    if ( PlayerPawn(Owner).bExtra1 != 0 ) // 0x00000045 : 0x004D
    {
      Global.Fire(0.0); // 0x00000057 : 0x0067
    } else { // 0x0000005F : 0x0072
      if ( PawnOwner.bFire != 0 ) // 0x00000062 : 0x0075
      {
        Global.Fire(0.0); // 0x00000072 : 0x008A
      } else { // 0x0000007A : 0x0095
        if ( PawnOwner.bAltFire != 0 ) // 0x0000007D : 0x0098
        {
          Global.AltFire(0.0); // 0x0000008D : 0x00AD
        } else { // 0x00000095 : 0x00B8
          GotoState('Idle'); // 0x00000098 : 0x00BB
          if (bAutoDrop) { // If you release primary fire, your grapple retracts
            if ( kxGrapple != None ) // 0x0000009C : 0x00C2
            {
              AmbientSound = None; // 0x000000A4 : 0x00CD
              kxGrapple.Destroy(); // 0x000000A8 : 0x00D4
              kxGrapple = None; // 0x000000B1 : 0x00E0
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
  if ( hand != 2 ) // 0x00000015 : 0x0000
  {
  if ( hand == 0 ) // 0x0000001F : 0x000D
  {
    hand = 1.0; // 0x00000028 : 0x0019
  } else { // 0x00000030 : 0x0024
    hand *= -1; // 0x00000033 : 0x0027
  }
  if ( hand == -1 ) // 0x0000003D : 0x0034
  {
    Mesh = Mesh(DynamicLoadObject("Botpack.TranslocR",Class'Mesh')); // 0x0000004A : 0x0044
  } else { // 0x0000006B : 0x006D
    Mesh = LodMesh'Transloc'; // 0x0000006E : 0x0070
    }
  }
  Super.SetHand(hand); // 0x00000075 : 0x007B
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
  // Caused Access None: if (p.PlayerReplicationInfo.Deaths == 0) // Only check binds on first spawn.  More efficient but will not work if mutator is added mid-game.
  CheckPlayerBinds(P);
}

simulated function CheckPlayerBinds(PlayerPawn P) {
  local int i;
  local string toAdd;
  local string keyName,keyVal,keyValCaps;
  toAdd = "GetWeapon kx_GrappleLauncher";
  for (i=0;i<256;i++) {
    keyName = p.ConsoleCommand("keyname "$i);
    keyVal = p.ConsoleCommand("keybinding "$keyName);
    keyValCaps = Caps(keyVal);
    if (InStr(keyValCaps,Caps(toAdd))>=0) {
      Log("kxMutator.CheckPlayerBinds() Stopped searching at "$i$" on "$p.getHumanName());
      return; // We have found an existing key bound to this weapon.  To save time, stop searching!
    }
    if (InStr(keyValCaps,"GETWEAPON TRANSLOCATOR")>=0 || InStr(keyValCaps,"SWITCHWEAPON 0")>=0) {
      // Add a binding to this key!
      p.ConsoleCommand("SET INPUT "$keyName$" "$keyVal$" | "$toAdd);
      p.ClientMessage("Grappling hook bound to your ["$keyName$"] key.");
      Log("kxMutator.CheckPlayerBinds() Changed "$p.getHumanName()$"'s keybind for "$keyName$"="$keyVal$" | "$toAdd);
      // Continue to search for other binds we could attach to.
    }
  }
  Log("kxMutator.CheckPlayerBinds() Finished checking all "$p.getHumanName()$"'s keybinds.");
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
    bUseExtra1=True
    bAutoDrop=False
}

