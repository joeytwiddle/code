//================================================================================
// kxMutator.
//================================================================================

class kxMutator extends Mutator Config(kxGrapple);

var bool Initialized;
var DeathMatchPlus DM;
var config bool UseBuiltinGrapple;
var config bool ExtraJumpBoots;
var config bool bDefaultBehindView,bDefaultChangeFOV;
var() name WeaponName;
var() name AmmoName;
var() string WeaponString;
var() string AmmoString;
var kx_GrappleLauncher cl;

var int bBehindView[64];
var int bChangeFOV[64];
var float PreviousFOV[64];

replication
{
  unreliable if ( Role == 4 ) cl;
}

function PostBeginPlay ()
{
  local int i;
  if ( Initialized )
  {
    return;
  }
  Initialized = True;
  DefaultWeapon = Class'enforcer';
  // DefaultWeapon = Class'ImpactHammer';
  // Level.Game.RegisterDamageMutator(self);
  for (i=0;i<64;i++) {
    if (bDefaultBehindView)
      bBehindView[i] = 1;
    if (bDefaultChangeFOV)
      bChangeFOV[i] = 1;
    PreviousFOV[i] = 90;
  }
}

function bool CheckReplacement (Actor Other, out byte bSuperRelevant) {
  // Replace the Translocator with the grappling hook?
  if (UseBuiltinGrapple && Other.IsA('Translocator') && !Other.IsA('kx_GrappleLauncher')) {
    ReplaceWith(Other,"kxGrapple.kx_GrappleLauncher");
    // ReplaceWith(Other,"kxGrapple.Translocator");
    // Log("[kxGrapple.kxMutator] CheckReplacement("$Other$") -> kx_GrappleLauncher");
    return False;
  }
  return Super.CheckReplacement(Other,bSuperRelevant);
  // if ( Other.IsA('Inventory') ) {
    // if ( Inventory(Other).myMarker == None ) {
      // return True;
    // }
  // }
  // return True;
}

function ModifyPlayer (Pawn Other)
{
  if (PlayerPawn(Other) != None)
    OnDeselect(PlayerPawn(Other));
  GiveWeaponsTo(Other);
  // This player/pawn has respawned, so any grapples which were hooked on it should now be dropped.
  foreach AllActors(class'kx_GrappleLauncher',cl) {
    if (cl.kxGrapple!=None && cl.kxGrapple.thing == Other) {
      cl.kxGrapple.Destroy();
    }
  }
  Super.ModifyPlayer(Other);
}

function GiveWeaponsTo (Pawn P)
{
  local Inventory Inv;
  if (UseBuiltinGrapple) {
    // // P.AddInventory(Spawn(class'kxGrapple.Translocator'));
    // P.AddInventory(Spawn(class'kxGrapple.kx_GrappleLauncher'));
    Inv = P.FindInventoryType(class'kxGrapple.kx_GrappleLauncher');
    if (Inv != None) {
      // Log("[kxMutator] "$P.getHumanName()$" already has a "$Inv$"!"); // I've seen this, so I guess CheckReplacement() is working.
      if (kx_GrappleLauncher(Inv).kxGrapple != None) {
        Log("[kxMutator] Retracted "$P.getHumanName()$"'s grapple.");
        kx_GrappleLauncher(Inv).kxGrapple.Destroy();
        kx_GrappleLauncher(Inv).kxGrapple = None;
      }
    } else {
      DeathMatchPlus(Level.Game).GiveWeapon(P,"kxGrapple.kx_GrappleLauncher");
    }
    if (P.PlayerReplicationInfo.Deaths==0) {
      // P.ClientMessage("You have a Grappling Hook.");
      if (bDefaultBehindView || bDefaultChangeFOV) {
        P.ClientMessage("To disable the grapple's BehindView switching: mutate BV off");
      }
    }
    // Remove any Translocator they might have:
    Inv = P.FindInventoryType(class'Botpack.Translocator');
    if (Inv != None && !Inv.IsA('kx_GrappleLauncher')) {
      Log("[kxMutator] Removing "$Inv$" from "$P.getHumanName());
      Inv.Destroy();
    }
  }
}

// TODO: I don't know what this is for!
function bool PreventDeath (Pawn Killed, Pawn Killer, name DamageType, Vector HitLocation)
{
  Killed.Weapon = None;
  return Super.PreventDeath(Killed,Killer,DamageType,HitLocation);
}

function kx_GrappleLauncher GetGrappleLauncher (Actor Other)
{
  local kx_GrappleLauncher thelauncher;

  foreach AllActors(Class'kx_GrappleLauncher',thelauncher)
  // foreach AllActors(Class'kxGrapple.Translocator',thelauncher)
  {
    if ( thelauncher.Owner == Other )
    {
      return thelauncher;
    }
  }
  return None;
}

function Mutate (string MutateString, PlayerPawn Sender)
{
  // We allow players to fire or retract their grappling hook without having it selected as a weapon!
  if ( MutateString ~= "FireGrapple" ) {
    cl = GetGrappleLauncher(Sender);
    if ( cl != None && cl.kxGrapple == None ) {
      // TODO: This is unfinished / not working properly:
      PlaySound(class'kxGrapple'.default.ThrowSound,SLOT_Interface,2.0);
      cl.kxGrapple = Spawn(class'kxGrapple',Sender,,Sender.Location+64*Vector(Sender.Rotation),Sender.Rotation);
      cl.kxGrapple.SetMaster(cl);
      cl.Instigator = Sender;
      cl.kxGrapple.AmbientSound = class'kxGrapple'.default.ReleaseSound;
      cl.AmbientSound = class'kxGrapple'.default.ReleaseSound;
    }
  }
  if ( MutateString ~= "KillHook" ) {
    cl = GetGrappleLauncher(Sender);
    if ( cl != None ) {
      cl.kxGrapple.Destroy();
    }
  }
  if (MutateString ~= "HELP") {
    Sender.ClientMessage("kxGrapple commands: mutate BV off | mutate BV on");
  }
  if (MutateString ~= "BV off") {
    bBehindView[Sender.PlayerReplicationInfo.PlayerID%64]=0;
    bChangeFOV[Sender.PlayerReplicationInfo.PlayerID%64]=0;
    // Reset their settings.  BUG: Eek if they type this *before* using the grapple, their FOV will be changed to default 90.  And maybe they wanted to be in behindview, just not switching!
    Sender.ConsoleCommand("BehindView 0");
    Sender.ConsoleCommand("FOV "$PreviousFOV[Sender.PlayerReplicationInfo.PlayerID%64]);
  }
  if (MutateString ~= "BV on") {
    bBehindView[Sender.PlayerReplicationInfo.PlayerID%64]=1;
    bChangeFOV[Sender.PlayerReplicationInfo.PlayerID%64]=1;
  }
  Super.Mutate(MutateString,Sender);
}

function OnSelect(PlayerPawn p) {
  if (p == None)
    return;
  if (bBehindView[p.PlayerReplicationInfo.PlayerID%64]>0) {
    p.ConsoleCommand("BehindView 1");
  }
  if (bChangeFOV[p.PlayerReplicationInfo.PlayerID%64]>0) {
    PreviousFOV[p.PlayerReplicationInfo.PlayerID%64] = p.DesiredFOV;
    if (p.FOVAngle<110) {
      p.ConsoleCommand("FOV 110");
      //// This method didn't work even when I made the calling functi0ns "simulated":
      // p.DefaultFOV = 110;
      // p.DesiredFOV = 110;
      //// Well of course, we are in the mutator, although we were called by the weapon.
    }
  }
}

function OnDeselect(PlayerPawn p) {
  if (p == None)
    return;
  if (bBehindView[p.PlayerReplicationInfo.PlayerID%64]>0) {
    p.ConsoleCommand("BehindView 0");
  }
  if (bChangeFOV[p.PlayerReplicationInfo.PlayerID%64]>0) {
    p.ConsoleCommand("FOV "$PreviousFOV[p.PlayerReplicationInfo.PlayerID%64]);
    //// This method didn't work even when I made the calling functi0ns "simulated":
    // p.DesiredFOV = PreviousFOV[p.PlayerReplicationInfo.PlayerID%64];
    // p.DefaultFOV = PreviousFOV[p.PlayerReplicationInfo.PlayerID%64];
  }
}

defaultproperties
{
    WeaponName=kx_GrappleLauncher
    // WeaponName=Translocator
    AmmoName=DefaultAmmo
    WeaponString="kxGrapple.kx_GrappleLauncher"
    AmmoString="none"
    // Doing this causes the ImpactHammer to be replaced:
    // DefaultWeapon=Class'kx_GrappleLauncher'
    UseBuiltinGrapple=True
    bDefaultBehindView=True
    bDefaultChangeFOV=True
}

