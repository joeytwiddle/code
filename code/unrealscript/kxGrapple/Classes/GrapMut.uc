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
var config bool bRemoveStuff;
var config bool ExtraJumpBoots;
var config int RJBoost;
var float RJBoostFactor;
var bool GiveArmor;
var bool GiveThighPads;
var() name WeaponName;
var() name AmmoName;
var() string WeaponString;
var() string AmmoString;
var XP_GrappleLauncher cl;

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

function bool CheckReplacement (Actor Other, out byte bSuperRelevant)
{
  if ( Other.IsA('Inventory') ) // 0x00000015 : 0x0000
  {
    if ( Inventory(Other).myMarker == None ) // 0x00000023 : 0x0014
    {
      return True; // 0x00000034 : 0x002D
    }
  }
  // Replace the Translocator with the grappling hook?
  if (UseBuiltinGrapple && Other.IsA('Translocator')) {
    ReplaceWith(Other,"kxGrapple.kx_GrappleLauncher");
    Log("[kxGrapple.kxMutator] CheckReplacement("$Other$") -> kx_GrappleLauncher");
    return False;
  }
  if (bRemoveStuff) {
    if ( Other.IsA('FlakAmmo') || Other.IsA('BladeHopper') || Other.IsA('BulletBox') || Other.IsA('RifleShell') || Other.IsA('BioAmmo') || Other.IsA('PAmmo') || Other.IsA('Miniammo') ) // 0x00000036 : 0x002F
    {
      ReplaceWith(Other,"BotPack.HealthVial"); // 0x000000AB : 0x00C7
      return False; // 0x000000C4 : 0x00E6
    }
    if ( Other.IsA('WarheadLauncher') || Other.IsA('WarHeadAmmo') || Other.IsA('ut_biorifle') || Other.IsA('minigun2') ) // 0x000000C6 : 0x00E8
    {
      ReplaceWith(Other,"BotPack.Enforcer"); // 0x00000107 : 0x013E
      return False; // 0x0000011E : 0x015B
    }
    if ( Other.IsA('UT_FlakCannon') || Other.IsA('ripper') ) // 0x00000120 : 0x015D
    {
      ReplaceWith(Other,"BotPack.ShockCore"); // 0x0000013F : 0x0187
      return False; // 0x00000157 : 0x01A5
    }
    if ( Other.IsA('UDamage') || Other.IsA('UT_invisibility') || Other.IsA('UT_Stealth') || Other.IsA('UT_ShieldBelt') ) // 0x00000159 : 0x01A7
    {
      ReplaceWith(Other,"BotPack.Armor2"); // 0x0000019B : 0x01FD
      return False; // 0x000001B0 : 0x0218
    }
  }
  if ( Other.IsA('Weapon') ) // 0x000001B2 : 0x021A
  {
    if ( ExtraJumpBoots == True ) // 0x000001C0 : 0x022E
    {
      ReplaceWith(Other,"BotPack.UT_Jumpboots"); // 0x000001CA : 0x023A
    } else { // 0x000001E5 : 0x025B
      ReplaceWith(Other,"BotPack.ThighPads"); // 0x000001E8 : 0x025E
    }
    return False; // 0x00000200 : 0x027C
  }
  return True; // 0x00000202 : 0x027E
}

function ModifyPlayer (Pawn Other)
{
  Other.Health = InitialHealth; // 0x00000014 : 0x0000
  GiveWeaponsTo(Other); // 0x00000020 : 0x0014
  Other.ClientMessage("kxGrapple.kxMutator"); // 0x00000025 : 0x001F
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
  local Weapon weap;

  if ( UseBuiltinGrapple == True ) // 0x00000013 : 0x0000
  {
    P.AddInventory(Spawn(Class'kx_GrappleLauncher')); // 0x0000001C : 0x000C
    P.ClientMessage("You have a Grappling Hook.");
  }
  DM = DeathMatchPlus(Level.Game); // 0x0000002B : 0x0023
  if ( DM == None ) // 0x0000003A : 0x003C
  {
    return; // 0x00000042 : 0x0047
  }
  if (bGiveAllWeapons) {
    DM.GiveWeapon(P,"Botpack.PulseGun"); // 0x00000044 : 0x0049
    DM.GiveWeapon(P,"Botpack.ShockRifle"); // 0x00000061 : 0x006F
    DM.GiveWeapon(P,"BotPack.UT_FlakCannon"); // 0x00000080 : 0x0097
    if ( EnableHammer == True ) // 0x000000A2 : 0x00C2
    {
      DM.GiveWeapon(P,"Botpack.ImpactHammer"); // 0x000000AB : 0x00CE
    }
    if ( EnableMiniGun == True ) // 0x000000CC : 0x00F8
    {
      DM.GiveWeapon(P,"Botpack.Minigun2"); // 0x000000D5 : 0x0104
    }
    if ( EnableSniperGun == True ) // 0x000000F2 : 0x012A
    {
      DM.GiveWeapon(P,"Botpack.SniperRifle"); // 0x000000FB : 0x0136
    }
    if ( EnableBioRifle == True ) // 0x0000011B : 0x015F
    {
      DM.GiveWeapon(P,"Botpack.UT_BioRifle"); // 0x00000124 : 0x016B
    }
    DM.GiveWeapon(P,"Botpack.UT_EightBall"); // 0x00000144 : 0x0194
  }
  Inv = P.Inventory; // 0x00000165 : 0x01BE
  if ( Inv != None ) // 0x00000171 : 0x01D2
  {
    weap = Weapon(Inv); // 0x00000179 : 0x01DD
    if ( (weap != None) && (weap.AmmoType != None) ) // 0x00000180 : 0x01ED
    {
      WeaponSetup(weap); // 0x00000198 : 0x020E
    }
    Inv = Inv.Inventory; // 0x0000019D : 0x0219
    goto JL01D2; // 0x000001A9 : 0x022D
  }
  if ( GiveArmor ) // 0x000001AC : 0x0230
  {
    Inv = Spawn(Class'Armor2'); // 0x000001B5 : 0x023C
    if ( Inv != None ) // 0x000001BE : 0x024A
    {
      Inv.bHeldItem = True; // 0x000001C6 : 0x0255
JL01D2:
      Inv.RespawnTime = 0.0; // 0x000001D2 : 0x0266
      Inv.GiveTo(P); // 0x000001E1 : 0x027A
    }
  }
  if ( GiveThighPads == True ) // 0x000001EC : 0x028E
  {
    Inv = Spawn(Class'ThighPads'); // 0x000001F5 : 0x029A
    if ( Inv != None ) // 0x000001FE : 0x02A8
    {
      Inv.bHeldItem = True; // 0x00000206 : 0x02B3
      Inv.RespawnTime = 0.0; // 0x00000212 : 0x02C4
      Inv.GiveTo(P); // 0x00000221 : 0x02D8
    }
  }
}

function WeaponSetup (Weapon W)
{
  if ( W.IsA('XP_GrappleLauncher') ) // 0x00000013 : 0x0000
  {
    return; // 0x00000021 : 0x0014
  }
  if ( W.IsA('UT_Eightball') ) // 0x00000023 : 0x0016
  {
    UT_Eightball(W).bAlwaysInstant = True; // 0x00000032 : 0x002A
    UT_Eightball(W).FiringSpeed = 0.81; // 0x00000040 : 0x0040
    W.AmmoType.MaxAmmo = 32; // 0x00000051 : 0x0059
    W.AmmoType.AmmoAmount = 32; // 0x00000062 : 0x0073
    return; // 0x00000073 : 0x008D
  }
  if ( W.IsA('UT_FlakCannon') ) // 0x00000075 : 0x008F
  {
    W.AmmoType.MaxAmmo = 10; // 0x00000083 : 0x00A3
    W.AmmoType.AmmoAmount = 10; // 0x00000094 : 0x00BD
    return; // 0x000000A5 : 0x00D7
  }
  if ( W.IsA('minigun2') ) // 0x000000A7 : 0x00D9
  {
    W.AmmoType.MaxAmmo = 80; // 0x000000B5 : 0x00ED
    W.AmmoType.AmmoAmount = 80; // 0x000000C6 : 0x0107
    return; // 0x000000D7 : 0x0121
  }
  W.AmmoType.MaxAmmo = W.AmmoType.Default.MaxAmmo / 4; // 0x000000D9 : 0x0123
  W.AmmoType.AmmoAmount = W.AmmoType.MaxAmmo; // 0x000000FA : 0x0156
  return; // 0x00000117 : 0x0185
}

function XP_GrappleLauncher GetGrappleLauncher (Actor Other)
{
  local XP_GrappleLauncher thelauncher;

  foreach AllActors(Class'XP_GrappleLauncher',thelauncher) // 0x00000014 : 0x0000
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
      cl.XPGrapple.Destroy(); // 0x00000037 : 0x0030
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
    AmmoName=DefaultAmmo
    WeaponString="kxGrapple.kx_GrappleLauncher"
    AmmoString="none"
    DefaultWeapon=Class'kx_GrappleLauncher'
    UseBuiltinGrapple=True
    bGiveAllWeapons=False
    bRemoveStuff=False
}
