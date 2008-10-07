//================================================================================
// kxMutator.
//================================================================================

class kxMutator extends Mutator Config(kxGrapple);

var bool bDoneInit;
var DeathMatchPlus DM;
var config bool bRemoveTranslocator,bGiveToPlayers;
var config bool ExtraJumpBoots;
var() name WeaponName;
var() name AmmoName;
var() string WeaponString;
var() string AmmoString;
var kx_GrappleLauncher cl;

replication {
  unreliable if (Role == 4)
    cl;
}

function PostBeginPlay () {
  local int i;
  if (bDoneInit) {
    Log(Level.TimeSeconds$" "$Self$".PostBeginPlay() was called a SECOND time!  Dropping out.");
    return;
  }
  bDoneInit = True;
  // DefaultWeapon = Class'Enforcer'; // I set this but I still spawned with the GrappleLauncher selected.
  // DefaultWeapon = Class'ImpactHammer';
  // Level.Game.RegisterDamageMutator(self);
}

function bool CheckReplacement (Actor Other, out byte bSuperRelevant) {
  // Replace the Translocator with the grappling hook?
  if (bRemoveTranslocator && Other.IsA('Translocator') && !Other.IsA('kx_GrappleLauncher')) {
    ReplaceWith(Other,"kxGrapple.kx_GrappleLauncher"); // This was supposed to replace the translocator but it only removes it!
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

function ModifyPlayer (Pawn Other) {
  local Inventory Inv;
  GiveWeaponsTo(Other);
  if (PlayerPawn(Other) != None) {
    // OnDeselect(PlayerPawn(Other));
    // TODO CONSIDER: Isn't this just GetGrappleLauncher(Other).OnDeselect() ?
    Inv = PlayerPawn(Other).FindInventoryType(class'kxGrapple.kx_GrappleLauncher');
    if (kx_GrappleLauncher(Inv)!=None) {
      kx_GrappleLauncher(Inv).OnDeselect();
    }
  }
  // This player/pawn has respawned, so any grapples which were hooked on it should now be dropped.
  // We could have just searched all kxGrapples.  :P
  foreach AllActors(class'kx_GrappleLauncher',cl) {
    if (cl.kxGrapple!=None && cl.kxGrapple.thing == Other) {
      cl.kxGrapple.Destroy();
    }
  }
  Super.ModifyPlayer(Other);
}

function GiveWeaponsTo (Pawn P) {
  local Inventory Inv;
  if (bGiveToPlayers) {
    //// Adding these did not change weapon priority.
    // DeathMatchPlus(Level.Game).GiveWeapon(P,"Botpack.ImpactHammer");
    // DeathMatchPlus(Level.Game).GiveWeapon(P,"Botpack.Enforcer");
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
      // if (bDefaultBehindView || bDefaultChangeFOV) {
        // P.ClientMessage("To disable the grapple's BehindView switching: autobehindview");
      // }
      P.ClientMessage("To toggle the grapple's automatic view switching: AutoBehindView");
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

function kx_GrappleLauncher GetGrappleLauncher (Actor Other) {
  local kx_GrappleLauncher thelauncher;
  foreach AllActors(Class'kx_GrappleLauncher',thelauncher) {
  // foreach AllActors(Class'kxGrapple.Translocator',thelauncher) {
    if ( thelauncher.Owner == Other ) {
      return thelauncher;
    }
  }
  return None;
}

function Mutate (string MutateString, PlayerPawn Sender) {
  // TODO: These mutate options are probably redundant given the exec functi_ns.
  //       NOT SO!  exec commands only work when switched to the weapon.  If we get these commands working, they could be used to grapple at any time without selecting it.  Admin should be able to disable that ability.
  //       NOT SO!  exec commands DO work when switched to OTHER weapons!
  // We allow players to fire or retract their grappling hook without having it selected as a weapon!
  local kxGrapple kxg;
  if ( MutateString ~= "FireGrapple" ) {
    cl = GetGrappleLauncher(Sender);
    if ( cl != None && cl.kxGrapple == None ) {
      /*
      // TODO: This is unfinished / not working properly:
      PlaySound(class'kx_GrappleLauncher'.default.ThrowSound,SLOT_Interface,2.0);
      cl.kxGrapple = Spawn(class'kxGrapple',Sender,,Sender.Location+64*Vector(Sender.Rotation),Sender.Rotation);
      cl.kxGrapple.SetMaster(cl);
      cl.Instigator = Sender;
      cl.kxGrapple.AmbientSound = class'kx_GrappleLauncher'.default.ReleaseSound;
      cl.AmbientSound = class'kx_GrappleLauncher'.default.ReleaseSound;
      */
      // Maybe better to do something like call cl.Fire()?
      cl.Fire(); // Maybe no good from the server
    }
  }
  if ( MutateString ~= "KillHook" ) {
    cl = GetGrappleLauncher(Sender);
    if ( cl != None ) {
      cl.kxGrapple.Destroy();
    }
  }
  if (MutateString ~= "HELP") {
    Sender.ClientMessage("kxGrapple commands: AutoBehindView [on|off] | AttachHook | ReleaseHook | FireHook | mutate FireGrapple | mutate KillHook | mutate status");
  }
  if (MutateString ~= "STATUS") {
    kxg = Spawn(class'kxGrapple');
    Sender.ClientMessage("kxGrapple defaults: Speed="$kxg.default.Speed$" GrappleSpeed="$kxg.default.GrappleSpeed);
    Sender.ClientMessage("kxGrapple status:   Speed="$kxg.Speed$" GrappleSpeed="$kxg.GrappleSpeed);
  }
  if (MutateString ~= "AutoBehindView") {
    // The player need not have typed "mutate" :P
    // This probably doesn't work:
    GetGrappleLauncher(Sender).AutoBehindView();
  }
  Super.Mutate(MutateString,Sender);
}

defaultproperties {
    WeaponName=kx_GrappleLauncher
    // WeaponName=Translocator
    AmmoName=DefaultAmmo
    WeaponString="kxGrapple.kx_GrappleLauncher"
    AmmoString="none"
    // Doing this causes the ImpactHammer to be replaced:
    // DefaultWeapon=Class'kx_GrappleLauncher'
    bRemoveTranslocator=True
    bGiveToPlayers=True // For some reason we need both of these!
}

