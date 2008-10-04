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
  if ( Initialized ) // 0x00000013 : 0x0000
  {
    return; // 0x00000019 : 0x0009
  }
  Initialized = True; // 0x0000001B : 0x000B
  // DefaultWeapon = Class'enforcer'; // 0x00000020 : 0x0013
  // Level.Game.RegisterDamageMutator(self); // 0x00000027 : 0x001E
  for (i=0;i<64;i++) {
    if (bDefaultBehindView)
      bBehindView[i] = 1;
    if (bDefaultChangeFOV)
      bChangeFOV[i] = 1;
    PreviousFOV[i] = 90;
  }
}

// function string GetInventoryClassOverride(string InventoryClassName) {
  // if (UseBuiltinGrapple && InventoryClassName ~= "Botpack.Translocator") {
    // return "kxGrapple.Translocator";
  // }
  // return Super.GetInventoryClassOverride(InventoryClassName);
// }

function bool CheckReplacement (Actor Other, out byte bSuperRelevant)
{
  // Replace the Translocator with the grappling hook?
  if (UseBuiltinGrapple && Other.IsA('Translocator') && !Other.IsA('kx_GrappleLauncher')) {
    ReplaceWith(Other,"kxGrapple.kx_GrappleLauncher");
    // ReplaceWith(Other,"kxGrapple.Translocator");
    // Log("[kxGrapple.kxMutator] CheckReplacement("$Other$") -> kx_GrappleLauncher");
    return False;
  }
  return Super.CheckReplacement(Other,bSuperRelevant);
  /*
  if ( Other.IsA('Inventory') ) // 0x00000015 : 0x0000
  {
    if ( Inventory(Other).myMarker == None ) // 0x00000023 : 0x0014
    {
      return True; // 0x00000034 : 0x002D
    }
  }
  return True; // 0x00000202 : 0x027E
  */
}

function ModifyPlayer (Pawn Other)
{
  if (PlayerPawn(Other) != None)
    OnDeselect(PlayerPawn(Other));
  GiveWeaponsTo(Other); // 0x00000020 : 0x0014
  // if (UseBuiltinGrapple && Other.IsA('PlayerPawn') && Other.PlayerReplicationInfo.Deaths == 0) {
    // CheckPlayerBinds(PlayerPawn(Other));
  // }
  if ( NextMutator != None ) // 0x0000003D : 0x003D
  {
    NextMutator.ModifyPlayer(Other); // 0x00000045 : 0x0048
  }
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
        P.ClientMessage("Disable auto-behindview with: mutate BV off");
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
  Killed.Weapon = None; // 0x00000016 : 0x0000
  return Super.PreventDeath(Killed,Killer,DamageType,HitLocation); // 0x00000022 : 0x0010
}

function kx_GrappleLauncher GetGrappleLauncher (Actor Other)
{
  local kx_GrappleLauncher thelauncher;

  foreach AllActors(Class'kx_GrappleLauncher',thelauncher) // 0x00000014 : 0x0000
  // foreach AllActors(Class'kxGrapple.Translocator',thelauncher) // 0x00000014 : 0x0000
  {
    if ( thelauncher.Owner == Other ) // 0x0000001E : 0x0010
    {
      return thelauncher; // 0x0000002F : 0x0029
    }
  } // 0x00000033 : 0x0030
  return None; // 0x00000034 : 0x0031
}

function Mutate (string MutateString, PlayerPawn Sender)
{
  if ( MutateString == "KillHook" ) // 0x00000015 : 0x0000
  {
    cl = GetGrappleLauncher(Sender); // 0x00000027 : 0x0014
    if ( cl != None ) // 0x0000002F : 0x0025
    {
      cl.kxGrapple.Destroy(); // 0x00000037 : 0x0030
    }
  }
  if (MutateString ~= "HELP") {
    Sender.ClientMessage("kxGrapple commands: mutate BV off | mutate BV on");
  }
  if (MutateString ~= "BV off") {
    bBehindView[Sender.PlayerReplicationInfo.PlayerID%64]=0;
    bChangeFOV[Sender.PlayerReplicationInfo.PlayerID%64]=0;
  }
  if (MutateString ~= "BV on") {
    bBehindView[Sender.PlayerReplicationInfo.PlayerID%64]=1;
    bChangeFOV[Sender.PlayerReplicationInfo.PlayerID%64]=1;
  }
  if ( NextMutator != None ) // 0x00000046 : 0x0045
  {
    NextMutator.Mutate(MutateString,Sender); // 0x0000004E : 0x0050
  }
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
    // WeaponString="kxGrapple.Translocator"
    AmmoString="none"
    DefaultWeapon=Class'kx_GrappleLauncher'
    // DefaultWeapon=Class'kxGrapple.Translocator'
    UseBuiltinGrapple=True
    bDefaultBehindView=True
    bDefaultChangeFOV=True
}

