//================================================================================
// GrapplingMut.
//================================================================================

// The GrapplingMut is needed whenever a GrappleGun is present, because it deals with:
//   Reset of hook and of AutoBehindView when player dies.
//   Reset of hook when the pawn it is attached to dies.
//   

// DONE: Waiting for ModifyPlayer() before destroying a grapple attached to a player is too late!
//       Try in ScoreKill() instead?

// TODO: The message telling players about AutoBehindView would be better off displayed when they first switch to the weapon, rather than at the moment of their first spawn.

class GrapplingMut extends Mutator Config(kxGrapple);

var bool bDoneInit;
var config bool bReplaceTranslocator,bGiveWeapon;
var GrappleGun cl;

replication {
  unreliable if (Role == 4)
    cl;
}

function PostBeginPlay () {
  local int i;
  if (bDoneInit) {
    Log(Level.TimeSeconds$" "$Self$".PostBeginPlay() was called a SECOND TIME!  Dropping out.");
    return;
  }
  bDoneInit = True;
  // DefaultWeapon = Class'Enforcer'; // I set this but I still spawned with the GrappleLauncher selected.
  // DefaultWeapon = Class'ImpactHammer';
  // Level.Game.RegisterDamageMutator(self);
  Level.Game.RegisterMessageMutator(Self);
}

function bool MutatorTeamMessage(Actor Sender, Pawn Receiver, PlayerReplicationInfo PRI, coerce string Msg, name Type, optional bool bBeep) {
	local GrappleGun gg;
	if (Sender == Receiver) {
		if (Msg ~= "ABV" && Pawn(Sender)!=None) {
			gg = GrappleGun( Pawn(Sender).FindInventoryType(class'GrappleGun') );
			if (gg != None) {
				gg.ABV();
			}
		}
	}
	return Super.MutatorTeamMessage(Sender,Receiver,PRI,Msg,Type,bBeep);
}


function AddMutator(Mutator mut) {
	if (mut.class == Self.class) {
		if (mut != Self) {
			Log(Level.TimeSeconds$" "$Self$".AddMutator() Destroying second GrapplingMut: "$mut);
			mut.Destroy();
		}
		return; // Never add another GrapplingMut.
	}
	Super.AddMutator(mut);
}

function bool CheckReplacement (Actor Other, out byte bSuperRelevant) {
  // Replace the Translocator with the grappling hook?
  if (bReplaceTranslocator && Other.IsA('Translocator') && !Other.IsA('GrappleGun')) {
    ReplaceWith(Other,String(class'GrappleGun')); // This was supposed to replace the translocator but it only removes it!
    // Log("[GrapplingHook.GrapplingMut] CheckReplacement("$Other$") -> GrappleGun");
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

// ScoreKill, PreventDeath and ModifyPlayer deal with grappling hooks when a player dies or respawns.

function ModifyPlayer (Pawn Other) {
  local Inventory Inv;
  // In case the victim was grappling before death, we should undo the grapple's AutoBehindView:
  // This was in PreventDeath() for a while but didn't always work.
  if (PlayerPawn(Other) != None) {
    Inv = PlayerPawn(Other).FindInventoryType(class'GrappleGun');
    if (GrappleGun(Inv)!=None) {
      GrappleGun(Inv).OnDeselect();
    }
  }

  // Give the player his spawn weapons:
  GiveWeaponsTo(Other);
  Super.ModifyPlayer(Other);
}

function bool PreventDeath (Pawn Victim, Pawn Killer, name DamageType, Vector HitLocation) {
  local GrapplingHook hook;

  // BTW we are kind-of assuming here that the guy *is* going to die!

  // TODO: I don't know what this is for!
  Victim.Weapon = None;

  // Any grapples which were hooked into the victim should now be reset.
  foreach AllActors(class'GrapplingHook',hook) {
    if (hook.Thing == victim) {
      hook.Destroy();
    }
  }

  // TODO: If the victim had a hook, it should be destroyed.  But I think this already happens automatically when the weapon is destroyed.

  return Super.PreventDeath(Victim,Killer,DamageType,HitLocation);
}

function ScoreKill(Pawn Killer, Pawn Victim) {
  // DONE: These should probably be in PreventDeath.  That way they will happen before the player dies.
  Super.ScoreKill(Killer,Victim);
}

function GiveWeaponsTo (Pawn P) {
  local Inventory Inv;
  local Weapon w;
  if (bGiveWeapon) {
    //// Adding these did not change weapon priority.
    // DeathMatchPlus(Level.Game).GiveWeapon(P,"Botpack.ImpactHammer");
    // DeathMatchPlus(Level.Game).GiveWeapon(P,"Botpack.Enforcer");
    // // P.AddInventory(Spawn(class'GrapplingHook.Translocator'));
    // P.AddInventory(Spawn(class'GrapplingHook.GrappleGun'));
    Inv = P.FindInventoryType(class'GrappleGun');
    if (Inv != None) {
      // Log("[GrapplingMut] "$P.getHumanName()$" already has a "$Inv$"!"); // I've seen this, so I guess CheckReplacement() is working.
      if (GrappleGun(Inv).GrapplingHook != None) {
        Log("[GrapplingMut] Retracted "$P.getHumanName()$"'s grapple.");
        GrappleGun(Inv).GrapplingHook.Destroy();
        GrappleGun(Inv).GrapplingHook = None;
      }
    } else {
      // DeathMatchPlus(Level.Game).GiveWeapon(P,String(class'GrappleGun'));
      // This is how DeathMatchPlus sets up the translocator:
      // And it finally succeeded in making it not the default weapon.  (Which also fixed the fire-on-spawn problem.)
      w = Spawn(class'GrappleGun');
      if (w == None) {
         Log(Self$".GiveWeaponsTo("$P.getHumanName()$") Warning! Failed to spawn GrappleGun!");
      } else {
         w.Instigator = P;
         w.BecomeItem();
         P.AddInventory(w);
         w.GiveAmmo(P);
         w.SetSwitchPriority(P);
         w.WeaponSet(P);
      }
    }
    if (P.PlayerReplicationInfo.Deaths==0) {
      // P.ClientMessage("You have a Grappling Hook.");
      // if (bDefaultBehindView || bDefaultChangeFOV) {
        // P.ClientMessage("To disable the grapple's BehindView switching: autobehindview");
      // }
      // P.ClientMessage("To toggle the grapple's automatic behind-view switching: ABV");
      // This was moved to the gun itself.
    }
    // Remove any Translocator they might have:
    Inv = P.FindInventoryType(class'Botpack.Translocator');
    if (Inv != None && !Inv.IsA('GrappleGun')) {
      Log("[GrapplingMut] Removing "$Inv$" from "$P.getHumanName());
      Inv.Destroy();
    }
  }
}

function GrappleGun GetGrappleLauncher (Actor Other) {
  // TODO: We could just use the Pawn's FindInventoryType().  :P
  local GrappleGun thelauncher;
  foreach AllActors(Class'GrappleGun',thelauncher) {
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
  local GrapplingHook kxg;
  if ( MutateString ~= "FireGrapple" ) {
    cl = GetGrappleLauncher(Sender);
    if ( cl != None && cl.GrapplingHook == None ) {
      /*
      // TODO: This is unfinished / not working properly:
      PlaySound(class'GrappleGun'.default.ThrowSound,SLOT_Interface,2.0);
      cl.GrapplingHook = Spawn(class'GrapplingHook',Sender,,Sender.Location+64*Vector(Sender.Rotation),Sender.Rotation);
      cl.GrapplingHook.SetMaster(cl);
      cl.Instigator = Sender;
      cl.GrapplingHook.AmbientSound = class'GrappleGun'.default.ReleaseSound;
      cl.AmbientSound = class'GrappleGun'.default.ReleaseSound;
      */
      // Maybe better to do something like call cl.Fire()?
      cl.Fire(); // Maybe no good from the server
    }
  }
  if ( MutateString ~= "KillHook" ) {
    cl = GetGrappleLauncher(Sender);
    if ( cl != None ) {
      cl.GrapplingHook.Destroy();
    }
  }
  if (MutateString ~= "HELP") {
    Sender.ClientMessage("Grappling commands: ABV [0|1] | AutoBehindView [on|off] | AttachHook | ReleaseHook | FireHook | mutate FireGrapple | mutate KillHook | status | mutate status");
  }
  // DONE: Move this into an exec functi0n in the weapon.
  // TODO: this here is now redundant
  // NO! Keep it, it is telling me that server and client values are not in agreement!
  // Maybe the problem is that they are config vars.
  if (MutateString ~= "STATUS") {
    // kxg = Spawn(class'GrapplingHook'); // FAILed!
    // kxg = Spawn(class'kxGrapple.GrapplingHook'); // FAILed!
    foreach AllActors(class'GrapplingHook',kxg) {
      break;
    }
    if (kxg == None) {
      Sender.ClientMessage("I can only show kxGrapple settings when there is a grappling hook deployed!");
    } else {
      //// Well the server default values do appears to match the current server config values.
      //// If only this was a simulated functi_n, we could check the client's values also, to see if replication is working properly.
      // Sender.ClientMessage("kxGrapple defaults: Speed="$kxg.default.Speed$" GrappleSpeed="$kxg.default.GrappleSpeed$" bSwingPhysics="$kxg.default.bSwingPhysics$" bLinePhysics="$kxg.default.bLinePhysics$" bPrimaryWinch="$kxg.default.bPrimaryWinch$" bCrouchReleases="$kxg.default.bCrouchReleases);
      Sender.ClientMessage("kxGrapple status:   Speed="$kxg.Speed$" GrappleSpeed="$kxg.GrappleSpeed$" bSwingPhysics="$kxg.bSwingPhysics$" bLinePhysics="$kxg.bLinePhysics$" bPrimaryWinch="$kxg.bPrimaryWinch$" bCrouchReleases="$kxg.bCrouchReleases);
    }
  }
  if (MutateString ~= "AutoBehindView") {
    // The player need not have typed "mutate" :P
    // This probably doesn't work:
    GetGrappleLauncher(Sender).AutoBehindView();
  }
  Super.Mutate(MutateString,Sender);
}

defaultproperties {
    // DefaultWeapon=Class'GrappleGun'
    // Doing this used to cause the ImpactHammer to be replaced:
    bReplaceTranslocator=True // You probably want this True for CTF, False for FairLMS/Siege.
    bGiveWeapon=True // Whether to always give the player this weapon on spawn.
}

