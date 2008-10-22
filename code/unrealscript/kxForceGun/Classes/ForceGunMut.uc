//================================================================================
// ForceGunMut
//================================================================================

// TODO: bRemoveHammer does remove the hammer, but Pure throws some Accessed None errors!
// TODO: Secondary fire with nothing held = boost off nearest wall?  Against floor, should could gentle hover. ^^

class ForceGunMut extends Mutator Config(kxForceGun);

var config bool bRemoveHammer,bUseForceGun;

replication {
  // unreliable if (Role == 4)
    // cl;
}

function PostBeginPlay() {
  // Level.Game.RegisterDamageMutator(self);
  Super.PostBeginPlay();
}

function AddMutator(Mutator mut) {
	if (mut.class == Self.class) {
		if (mut != Self) {
			Log(Level.TimeSeconds$" "$Self$".AddMutator() Destroying second ForceGunMut: "$mut);
			mut.Destroy();
		}
		return;
	}
	Super.AddMutator(mut);
}

function bool CheckReplacement (Actor Other, out byte bSuperRelevant) {
  // Replace the Translocator with the grappling hook?
  if (bRemoveHammer && Other.IsA('ImpactHammer') && !Other.IsA('ForceGun')) {
    //// TODO: Even commented out, this still causes the errors with Pure: ST_Mutator DM-Liandri.ST_Mutator0 (F_nction PureStat7G.ST_Mutator.GetReplacementWeapon:03AC) Accessed None
    ////       But at least it doesn't leave pickups sitting around.
    ReplaceWith(Other,String(class'ForceGun')); // This was supposed to replace the hammer but it only removes it!
    Log("[kxForceGun.ForceGun] CheckReplacement("$Other$") -> ForceGun");
    return False;
  }
  return Super.CheckReplacement(Other,bSuperRelevant);
}

function ModifyPlayer (Pawn Other) {
  GiveWeaponsTo(Other);
  Super.ModifyPlayer(Other);
}

function bool PreventDeath (Pawn Victim, Pawn Killer, name DamageType, Vector HitLocation) {
  return Super.PreventDeath(Victim,Killer,DamageType,HitLocation);
}

function ScoreKill(Pawn Killer, Pawn Victim) {
  Super.ScoreKill(Killer,Victim);
}

function GiveWeaponsTo (Pawn P) {
  local Inventory Inv;
  local Weapon w;
  if (bUseForceGun) {
    Inv = P.FindInventoryType(class'ForceGun');
    if (Inv != None) {
      Log("[ForceGunMut] "$P.getHumanName()$" already has a "$Inv$"!"); // I've seen this, so I guess CheckReplacement() is working.
    } else {
      w = Spawn(class'ForceGun',P);
      if (w == None) {
         Log(Self$".GiveWeaponsTo("$P.getHumanName()$") Warning! Failed to spawn ForceGun!");
      } else {
         w.bHeldItem=True;
         w.RespawnTime=0;
         // w.bRotatingPickup = False; // otherwise it appears at the spawnpoint!  This did not work.  Maybe we are given the weapon elsewhere.
         w.Instigator = P;
         w.BecomeItem();
         P.AddInventory(w);
         w.GiveAmmo(P);
         w.SetSwitchPriority(P);
         w.WeaponSet(P);
         // Log(Self$".GiveWeaponsTo() Gave "$w$" to "$P);
      }
    }
    if (P.PlayerReplicationInfo.Deaths==0) {
    }
    // Remove any Translocator they might have:
    if (bRemoveHammer) {
      Inv = P.FindInventoryType(class'Botpack.ImpactHammer');
      if (Inv != None && !Inv.IsA('ForceGun')) {
        Log("[ForceGunMut] Removing "$Inv$" from "$P.getHumanName());
        Inv.Destroy();
      }
    }
  }
}

function Mutate (string MutateString, PlayerPawn Sender) {
  if (MutateString ~= "HELP") {
    // Sender.ClientMessage("Grappling commands: ABV [0|1] | AutoBehindView [on|off] | AttachHook | ReleaseHook | FireHook | mutate FireGrapple | mutate KillHook | status | mutate status");
  }
  Super.Mutate(MutateString,Sender);
}

defaultproperties {
    bRemoveHammer=True
    bUseForceGun=True // For some reason we need both of these!
}

