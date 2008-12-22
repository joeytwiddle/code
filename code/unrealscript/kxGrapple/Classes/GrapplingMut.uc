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
var config bool bCanGrappleWithoutSelection; // AKA RemoteControl

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
  // if (bReplaceTranslocator && Other.IsA('Translocator') && !Other.IsA('GrappleGun')) {
  if (bReplaceTranslocator && Translocator(Other)!=None && GrappleGun(Other)==None) {
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
  local String command,args;
  local GrappleGun gun;
  local GrapplingHook hook;
  // TODO: These mutate options are probably redundant given the exec functi_ns.
  //       NOT SO!  exec commands only work when switched to the weapon.  If we get these commands working, they could be used to grapple at any time without selecting it.  Admin should be able to disable that ability.
  //       NOT SO!  exec commands DO work when switched to OTHER weapons!
  // We allow players to fire or retract their grappling hook without having it selected as a weapon!
  if (InStr(MutateString," ")>=0) {
    command = Left(MutateString,InStr(MutateString," "));
    args = Right(MutateString,InStr(MutateString," ")+1);
  } else {
    command = MutateString;
    args = "";
  }
  if (command ~= "HELP") {
    Sender.ClientMessage("Grappling commands: ABV [0|1] | AutoBehindView [on|off] | FTW [0|1] | FireToWinch [on|off] | status"); // | AttachHook | ReleaseHook | FireHook");
    Sender.ClientMessage("Grappling mutate commands: mutate help | FireHook | ReleaseHook | ToggleHook | status | ABV | FTW"); // | GrappleJump
  }
  // DONE: Move this into an exec functi0n in the weapon.
  // TODO: this here is now redundant
  // NO! Keep it, it is telling me that server and client values are not in agreement!
  // Maybe the problem is that they are config vars.
  if (command ~= "STATUS") {
    // hook = Spawn(class'GrapplingHook'); // FAILed!
    // hook = Spawn(class'kxGrapple.GrapplingHook'); // FAILed!
    // foreach AllActors(class'GrapplingHook',hook) {
      // break;
    // }
    gun = GetGrappleLauncher(Sender);
    if (gun!=None)
      hook = gun.GrapplingHook;
    if (hook == None) {
      Sender.ClientMessage("I can only show kxGrapple settings when there is a grappling hook deployed!");
    } else {
      //// Well the server default values do appears to match the current server config values.
      //// If only this was a simulated functi_n, we could check the client's values also, to see if replication is working properly.
      // Sender.ClientMessage("kxGrapple defaults: Speed="$hook.default.Speed$" GrappleSpeed="$hook.default.GrappleSpeed$" bSwingPhysics="$hook.default.bSwingPhysics$" bLinePhysics="$hook.default.bLinePhysics$" bPrimaryWinch="$hook.default.bPrimaryWinch$" bCrouchReleases="$hook.default.bCrouchReleases);
      Sender.ClientMessage("Server GrappleGun status: bDisableAutoBehindview="$gun.bDisableAutoBehindview$" bFireToWinch="$gun.bFireToWinch);
      Sender.ClientMessage("Server GrapplingHook status:   Speed="$hook.Speed$" GrappleSpeed="$hook.GrappleSpeed$" bSwingPhysics="$hook.bSwingPhysics$" bLinePhysics="$hook.bLinePhysics$" bPrimaryWinch="$hook.bPrimaryWinch$" bCrouchReleases="$hook.bCrouchReleases);
      // TODO: Wait a second, we only grabbed the first hook we could fine, we should have grabbed the one belonging to Sender!
    }
  }
  // TODO: These fnctions should be passed String extra
  if (command~="ABV" || command~="AutoBehindView") {
    GetGrappleLauncher(Sender).AutoBehindView(args);
  }
  if (command~="FTW" || command~="FireToWinch") {
    GetGrappleLauncher(Sender).FireToWinch(args);
  }
  if (command~="GrappleJump") {
    GetGrappleLauncher(Sender).DoubleJump();
  }
  if (bCanGrappleWithoutSelection) {
    if (command~="FireHook" || command~="ToggleHook") {
      gun = GetGrappleLauncher(Sender);
      if (gun != None) {
        if (gun.GrapplingHook == None) {
           // gun.Fire(1.0); // Works but only when we have GrappleGun selected.  (Occasionally fires without selection.)
           gun.PlaySound(class'GrappleGun'.default.ThrowSound,SLOT_Interface,2.0);
           // gun.GrapplingHook = Spawn(class'GrapplingHook',Sender,,Sender.Location+64*Vector(Sender.Rotation),Sender.Rotation);
           // gun.ProjectileFire(gun.ProjectileClass, gun.ProjectileSpeed, gun.bWarnTarget);
           gun.GrapplingHook = GrapplingHook(gun.ProjectileFire(gun.ProjectileClass,class'GrapplingHook'.default.Speed,gun.bWarnTarget));
           // Fires grapple fine, but grappling line does not respond.  Ah we needed to SetMaster.
           gun.GrapplingHook.SetMaster(gun);
           gun.Instigator = Sender;
           gun.GrapplingHook.AmbientSound = class'GrapplingHook'.default.ReleaseSound;
           gun.AmbientSound = class'GrapplingHook'.default.ReleaseSound;
        } else {
           if (command~="ToggleHook") {
              gun.GrapplingHook.Destroy();
           }
        }
      }
    }
    // TODO: Now that I can ungrapple just by hitting a button, I don't need GrappleJump anymore - in fact I don't want it!
    if (command~="KillHook" || command~="ReleaseHook") {
      gun = GetGrappleLauncher(Sender);
      if (gun!=None && gun.GrapplingHook!=None) {
        gun.GrapplingHook.Destroy();
      }
    }
  }
  Super.Mutate(MutateString,Sender);
}

function bool MutatorTeamMessage(Actor Sender, Pawn Receiver, PlayerReplicationInfo PRI, coerce string Msg, name Type, optional bool bBeep) {
	if (Sender == Receiver && Pawn(Sender)!=None) {
		if (Msg~="ABV" || Msg~="AutoBehindView") {
			GetGrappleLauncher(Sender).AutoBehindView();
		}
		if (Msg~="FTW" || Msg~="FireToWinch") {
			GetGrappleLauncher(Sender).FireToWinch();
		}
	}
	return Super.MutatorTeamMessage(Sender,Receiver,PRI,Msg,Type,bBeep);
}

defaultproperties {
    // DefaultWeapon=Class'GrappleGun'
    // Doing this used to cause the ImpactHammer to be replaced:
    bReplaceTranslocator=True // You probably want this True for CTF, False for FairLMS/Siege.
    bGiveWeapon=True // Whether to always give the player this weapon on spawn.
    // bCanGrappleWithoutSelection=True // TODO: I expect most servers will want this default to False.
}

