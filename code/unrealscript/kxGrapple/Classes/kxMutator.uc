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
var config int InitialHealth;
var config int InitialArmor;
var config bool UseBuiltinGrapple;
var config bool bGiveAllWeapons;
var config bool ExtraJumpBoots;
var config int RJBoost;
var float RJBoostFactor;
var bool GiveArmor;
var bool GiveThighPads;
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
  switch (InitialHealth) // 0x00000048 : 0x004A
  {
    case 0: // 0x0000004C : 0x0051
    GiveThighPads = False; // 0x00000050 : 0x0055
    GiveArmor = False; // 0x00000055 : 0x005D
    break; // 0x0000005A : 0x0065
    case 50: // 0x0000005D : 0x0068
    GiveThighPads = True; // 0x00000062 : 0x006D
    GiveArmor = False; // 0x00000067 : 0x0075
    break; // 0x0000006C : 0x007D
    case 100: // 0x0000006F : 0x0080
    GiveThighPads = False; // 0x00000074 : 0x0085
    GiveArmor = True; // 0x00000079 : 0x008D
    break; // 0x0000007E : 0x0095
    case 150: // 0x00000081 : 0x0098
    GiveThighPads = False; // 0x00000086 : 0x009D
    GiveArmor = True; // 0x0000008B : 0x00A5
    break; // 0x00000090 : 0x00AD
    default: // 0x00000093 : 0x00B0
    break; // 0x00000096 : 0x00B3
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
  Other.Health = InitialHealth; // 0x00000014 : 0x0000
  GiveWeaponsTo(Other); // 0x00000020 : 0x0014
  // Other.ClientMessage("kxGrapple.kxMutator"); // 0x00000025 : 0x001F
  if ( NextMutator != None ) // 0x0000003D : 0x003D
  {
    NextMutator.ModifyPlayer(Other); // 0x00000045 : 0x0048
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

function GiveWeaponsTo (Pawn P)
{
  local Inventory Inv;
  if (UseBuiltinGrapple) {
    // Replace the default Translocator:
    Inv = P.FindInventoryType(class'Botpack.Translocator');
    if (Inv != None) {
      Inv.Destroy();
    }
    // P.AddInventory(Spawn(class'kxGrapple.Translocator'));
    P.AddInventory(Spawn(class'kxGrapple.kx_GrappleLauncher'));
    if (P.PlayerReplicationInfo.Deaths==0) P.ClientMessage("You have a Grappling Hook.");
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
    InitialHealth=120
    InitialArmor=100
    RJBoost=200
    GiveArmor=True
    WeaponName=kx_GrappleLauncher
    // WeaponName=Translocator
    AmmoName=DefaultAmmo
    WeaponString="kxGrapple.kx_GrappleLauncher"
    // WeaponString="kxGrapple.Translocator"
    AmmoString="none"
    DefaultWeapon=Class'kx_GrappleLauncher'
    // DefaultWeapon=Class'kxGrapple.Translocator'
    UseBuiltinGrapple=True
    bGiveAllWeapons=False
}
