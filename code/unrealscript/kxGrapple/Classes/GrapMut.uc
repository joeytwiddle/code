//================================================================================
// kxMutator.
//================================================================================

class kxMutator extends Mutator Config(kxGrapple);

var bool Initialized;
var DeathMatchPlus DM;
var config bool EnableMiniGun;
var config bool EnableSniperGun;
var config bool EnableBioRifle;
var config bool EnableHammer;
var config bool UseBuiltinGrapple;
var config bool ExtraJumpBoots;
var config int RJBoost;
var float RJBoostFactor;
var() name WeaponName;
var() name AmmoName;
var() string WeaponString;
var() string AmmoString;
var kx_GrappleLauncher cl;

replication
{
  unreliable if ( Role == 4 ) cl;
}

function PostBeginPlay ()
{
  if ( Initialized ) // 0x00000013 : 0x0000
  {
    return; // 0x00000019 : 0x0009
  }
  Initialized = True; // 0x0000001B : 0x000B
  DefaultWeapon = Class'enforcer'; // 0x00000020 : 0x0013
  Level.Game.RegisterDamageMutator(self); // 0x00000027 : 0x001E
  RJBoostFactor = RJBoost / 100.0; // 0x0000003A : 0x0037
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
    Log("[kxGrapple.kxMutator] CheckReplacement("$Other$") -> kx_GrappleLauncher");
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
      Log("[kxMutator] "$P.getHumanName()$" already has a "$Inv$"!");
      if (kx_GrappleLauncher(Inv).kxGrapple != None) {
        Log("[kxMutator] Retracted "$P.getHumanName()$"'s grapple.");
        kx_GrappleLauncher(Inv).kxGrapple.Destroy();
      }
    } else {
      DeathMatchPlus(Level.Game).GiveWeapon(P,"kxGrapple.kx_GrappleLauncher");
    }
    // if (P.PlayerReplicationInfo.Deaths==0) P.ClientMessage("You have a Grappling Hook.");
    // Remove any Translocator they might have:
    Inv = P.FindInventoryType(class'Botpack.Translocator');
    if (Inv != None && !Inv.IsA('kx_GrappleLauncher')) {
      Log("[kxMutator] Removing "$Inv$" from "$P.getHumanName());
      Inv.Destroy();
    }
  }
}

function bool PreventDeath (Pawn Killed, Pawn Killer, name DamageType, Vector HitLocation)
{
  Killed.Weapon = None; // 0x00000016 : 0x0000
  Super.PreventDeath(Killed,Killer,DamageType,HitLocation); // 0x00000022 : 0x0010
}

function MutatorTakeDamage (out int actualDamage, Pawn Victim, Pawn instigatedBy, out Vector HitLocation, out Vector Momentum, name DamageType)
{
  if ( (instigatedBy == Victim) && (DamageType == 'RocketDeath') ) // 0x00000014 : 0x0000
  {
    Momentum.Z *= RJBoostFactor; // 0x0000002B : 0x0020
    Momentum.X /= 2; // 0x00000035 : 0x0031
    Momentum.Y /= 2; // 0x0000003E : 0x0040
    actualDamage = 4; // 0x00000048 : 0x004F
  } else { // 0x0000004D : 0x0057
    if ( DamageType == 'jolted' ) // 0x00000050 : 0x005A
    {
      actualDamage = 95; // 0x0000005A : 0x0069
    } else { // 0x0000005F : 0x0071
      if ( DamageType == 'Fell' ) // 0x00000062 : 0x0074
      {
        actualDamage = 4; // 0x0000006C : 0x0083
      } else { // 0x00000071 : 0x008B
        actualDamage = Min(actualDamage,90); // 0x00000074 : 0x008E
      }
    }
    Momentum.Z /= 2; // 0x0000007D : 0x009D
    Momentum.X /= 2; // 0x00000087 : 0x00AC
    Momentum.Y /= 2; // 0x00000090 : 0x00BB
  }
  if ( NextDamageMutator != None ) // 0x0000009A : 0x00CA
  {
    NextDamageMutator.MutatorTakeDamage(actualDamage,Victim,instigatedBy,HitLocation,Momentum,DamageType); // 0x000000A3 : 0x00D5
  }
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
  if ( NextMutator != None ) // 0x00000046 : 0x0045
  {
    NextMutator.Mutate(MutateString,Sender); // 0x0000004E : 0x0050
  }
}

defaultproperties
{
    EnableSniperGun=True
    EnableHammer=True
    RJBoost=200
    WeaponName=kx_GrappleLauncher
    // WeaponName=Translocator
    AmmoName=DefaultAmmo
    WeaponString="kxGrapple.kx_GrappleLauncher"
    // WeaponString="kxGrapple.Translocator"
    AmmoString="none"
    DefaultWeapon=Class'kx_GrappleLauncher'
    // DefaultWeapon=Class'kxGrapple.Translocator'
    UseBuiltinGrapple=True
}
