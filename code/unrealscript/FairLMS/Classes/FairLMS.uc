// TODO: We didn't really need to give the player those weapons, since LMS will probably suit them up anyway.
//       But maybe we aren't in the LMS gametype.  In which case, we should probably remove weapons and other pickup items from the map.
// TODO BUG: Were some of the armor+pads getting left invisible on the spawnpoints?  I kept spawning with 150, but maybe that was done by normal LMS.
// TODO: Anti-camper detection.
// TODO: I didn't hear the amp pickup sound (altho maybe it came the same time as "headshot")
// DONE: Sucks to get Armour then Armour again, etc.  :P
// TODO: Deemer
// DONE: I spawned with shieldbelt.
// My Invis refused to wear off.
// Redeemer fire works but not altfire.
// DONE: WarheadLauncher DM-Liandri.WarheadLauncher0 (Function Botpack.WarheadLauncher.AltFire:002C) Accessed None
// DONE: WarheadLauncher DM-Liandri.WarheadLauncher12 (Function Botpack.WarheadLauncher.RateSelf:0027) Accessed None
// LMS seems a bit stingy on initial ammo.
// TODO: It is sometimes a bit laggy to load a new external object (e.g. the siege jetpack) during play due to DynamicLoadObject() calls.
//       Since one is likely to spawn during the game anyway, have server and client pre-load the Powerups resources before the game starts.
// TODO: I think you only get given one WarheadLauncher per life.  This is a feature, so ensure it stays that way!
// TODO: Detect last 2 players, display it, and then stop giving powerups+health.
// DONE: Some errors from the bots - are they due to pathing on spawned items which they think are navigable to/through?  Seemed to be fixed with some more additions to GiveInventory().

class FairLMS expands Mutator config(FairLMS);

struct LMSBonus {
	// var String Name;  // Actually each item probably already has a Name available from its defaultproperties.
	var String Type;
	var Color Color;
	var String Sound;
};

var config bool bGiveWeapons;
var config int InitialArmour,InitialHealth;
var config float HealthLostPerSec,HealthGainedPerKill;
var config int FragsForPowerup;
var config bool bGivePowerups;
var config String InitialWeapon[20];
var config LMSBonus Powerup[20];
var config Sound PowerupSound;

var int KillsSinceSpawn[64];

function PostBeginPlay() {
	Super.PostBeginPlay();
	SetTimer(1,True);
	if (HealthLostPerSec>0)
		Level.Game.GameName = "Anti-Idle "$ Level.Game.GameName;
	if (bGivePowerups || HealthGainedPerKill>0)
		Level.Game.GameName = Level.Game.GameName $" with Bonuses Powerups";
	if (HealthLostPerSec>0)
		DeathMatchPlus(Level.Game).StartMessage = "Your are losing health!  Kill to stay alive!";
	// Turn redeemers into mini-redeemers.
	// TODO: Do these changes propogate over maps, messing with other types of game?
	class'WarShell'.default.Speed = 400;
	class'WarShell'.default.Damage = 400;
	class'WarShell'.default.DrawScale = class'WarShell'.default.DrawScale * 0.6;
	class'WarShell'.default.MomentumTransfer = 1000;
}

event Timer() {
	local Pawn p;
	Super.Timer();
	foreach AllActors(class'Pawn',p) {
		if (PlayerPawn(p)!=None || Bot(p)!=None) {
			p.Health -= HealthLostPerSec;
			if (p.Health<1)
				p.Health = 1;
			// Average death 10 seconds after health runs out, provided timer stays at 1.0.
			if (p.Health <= 1 && FRand()<0.1) { // TODO: or was hit recently (may be about to be finished off)
				// Level.Game.
				p.Died(None, 'Suicided', p.Location);
			}
			// CONSIDER TODO: when player first reaches 0, we could add some effect to him (skull above his head) to show that he requires only 1 damage hit to die.
			//                maybe fairer not to warn other players, but to warn the player who is in danger!
		}
	}
}

function ModifyPlayer(Pawn p) {
	Super.ModifyPlayer(p);
	if (InitialHealth>0)
		p.Health = InitialHealth;
	// p.PlayerReplicationInfo.Armor = InitialArmour;
	GiveAllWeapons(p);
	if (p.PlayerReplicationInfo!=None) {
		KillsSinceSpawn[p.PlayerReplicationInfo.PlayerID%64] = 0;
	}
}

function ScoreKill(Pawn killer, Pawn other) {
	Super.ScoreKill(killer,other);
	if (killer != None) {
		killer.Health += HealthGainedPerKill;
		if (killer.Health > 199) killer.Health = 199;
		if (killer.PlayerReplicationInfo!=None) {
			KillsSinceSpawn[killer.PlayerReplicationInfo.PlayerID%64] += 1;
			if (KillsSinceSpawn[killer.PlayerReplicationInfo.PlayerID%64]%FragsForPowerup == 0 && bGivePowerups) {
				GiveRandomPowerup(killer);
			}
		}
	}
}

function GiveAllWeapons(Pawn p) {
	local Inventory Inv;
	local int i;
	local class<Weapon> type;

	// Give appropriate armour.
	// CONSIDER: Alternatively, give them normal armour, but adjust its Charge.
	if (InitialArmour<50) {
	} else if (InitialArmour>=50 && InitialArmour<100) {
		GivePickupType(p,class'ThighPads');
	} else if (InitialArmour>=100 && InitialArmour<150) {
		GivePickupType(p,class'Armor2');
	} else if (InitialArmour>=150) {
		GivePickupType(p,class'Armor2');
		GivePickupType(p,class'ThighPads');
	}

	if (bGiveWeapons) {
		for (i=0;i<20;i++) {
			if (InitialWeapon[i]!="") {
				type = class<Weapon>(DynamicLoadObject(InitialWeapon[i],class'class'));
				Inv = p.FindInventoryType(type);
				if (Inv == None) {
					DeathMatchPlus(Level.Game).GiveWeapon(p,InitialWeapon[i]);
				} else {
					Log(Self$".GiveAllWeapons() Warning! "$p.getHumanName()$" already had a "$type$" so re-using it "$Inv);
					// Maybe this is not needed.  My actual problem was adding a SniperRifle to a player that already had a zp_SniperRifle.
				}
			}
		}
	}

}

function Inventory GivePickupType(Pawn p, class<Inventory> t) {
	// TODO: Can we just grab the implementation in GiveRandomPowerup()?
	local Inventory Inv;
	Inv = p.FindInventoryType(t);
	if (Inv!=None) {
		// Log(Self$".GivePickupType() Warning! "$p.getHumanName()$" already had a "$t$" so re-using it "$Inv);
		Log(Self$".GivePickupType() "$p.getHumanName()$" already has a "$Inv$" - destroying it.");
		Inv.Destroy();
		Inv = None;
		// TODO: This might be a bummer if it destroys the weapon you are currently holding!
	}
	// In the case of amp at least, re-using is bad because you don't get the fresh charge!
	if (Inv==None) {
		// Log(Self$".GivePickupType() Spawning a new "$t$" for "$p.getHumanName());
		Inv = Spawn(t,p);
	}
	if (Inv==None) {
		Log(Self$".GivePickupType() Warning! Failed to spawn a "$t);
	} else {
		GiveInventory(p,Inv);
		// if (Inv.PickupSound != None) {
			// p.PlaySound(Inv.PickupSound,SLOT_Interface,3.0);
		// } else {
			// p.PlaySound(PowerupSound,SLOT_Interface,3.0);
		// }

		// Post-hacks:
		if (Weapon(Inv)!=None) {
			// Really for the redeemer.
			if (Weapon(Inv).AmmoType.AmmoAmount<1)
				Weapon(Inv).AmmoType.AmmoAmount = 1;
		}
	}
	return Inv;
}

function GiveInventory(Pawn p, Inventory inv) {
	local Weapon w;

	inv.bHeldItem=True;
	inv.RespawnTime=0;

	w = Weapon(inv);
	if (w!=None) {

		// Handle Weapon:
		w.Instigator = P;
		w.BecomeItem();
		P.AddInventory(w);
		// w.GiveTo(p);
		w.GiveAmmo(P);
		// Not for the redeemer (or other 1 ammo weapons):
		if (Weapon(inv)!=None && Weapon(inv).AmmoType!=None && Weapon(inv).AmmoType.AmmoAmount>1) {
			// Increase ammo x 3
			Weapon(inv).AmmoType.AmmoAmount = Weapon(inv).AmmoType.AmmoAmount * 3;
		}
		w.SetSwitchPriority(P);
		w.WeaponSet(P);
		w.AmbientGlow = 0;

		// DeathMatchPlus does this to weapons:
		if ( p.IsA('PlayerPawn') )
			w.SetHand(PlayerPawn(p).Handedness);
		else
			w.GotoState('Idle');

	} else {

		// Handle other Inventory item:
		inv.GiveTo(p);
		inv.Activate();

	}
	// DONE: Check out what DeathMatchPlus, and Translocator/GrappleGun do to initialise weapons correctly.
	//       We may be missing something we should do for weapons.  In a game with bots I got: WarheadLauncher DM-Liandri.WarheadLauncher3 (F_nction Botpack.WarheadLauncher.RateSelf:0027) Accessed None
}

function GiveRandomPowerup(Pawn p) {
	local int i,j;
	local class<Inventory> type;
	local Inventory inv;
	local Color col;
	local Sound resource;
	for (j=0;j<100;j++) {
		i = 20*FRand();
		if (Powerup[i].Type == "")
			continue;
		type = class<Inventory>( DynamicLoadObject(Powerup[i].Type,class'Class') );
		if (type == None) {
			Log("[FairLMS] Powerup #"$i$" \""$Powerup[i].Type$"\" does not exist!");
			continue;
		}
		inv = p.FindInventoryType(type);
		if (inv != None) {
			// A bit log spammy:
			// Log(Self$".GiveRandomPowerup() "$p.getHumanName()$" already has a "$inv);
			// Log(Self$".GiveRandomPowerup() maybe they don't want another - maybe we could upgrade it?");
			// It seems to me that items *are* being removed from a player's inventory when he loses them.
			// I don't know if warhead 2nd time around is working now.
			continue;
		}
		inv = Spawn(type,p);
		if (inv == None) {
			Log(Self$".GiveRandomPowerup() Failed to spawn "$type);
			continue;
		}
		GiveInventory(p,inv);
		col = Powerup[i].Color;
		if (col.R==0 && col.G==0 && col.B==0) {
			col.R=255; col.G=255; col.B=16; col.A=16;
		}
		FlashMessage(p,inv.ItemName,col);
		// DONE: Sound!
		resource = None;
		if (Powerup[i].Sound != "") {
			resource = Sound(DynamicLoadObject(Powerup[i].Sound,class'Sound'));
		}
		if (resource != None) {
			p.PlaySound(resource,SLOT_Interface,3.0);
		} else {
			if (Inv.PickupSound != None) {
				p.PlaySound(Inv.PickupSound,SLOT_Interface,3.0);
			} else {
				p.PlaySound(PowerupSound,SLOT_Interface,3.0);
			}
		}
		return;
	}
	if (j==100) {
		Log(Self$".GiveRandomPowerup() Tried 100 times but could not find a suitable powerup!  Maybe "$p.getHumanName()$" has everything already.");
		// TODO: maybe remove something from his inventory? ^^
	}

}

function FlashMessage(Pawn p, String msg, Color col) {
	if (PlayerPawn(p)==None)
		return;
	// msg = Caps(msg) $ "!";
	msg = "-+- "$ Caps(msg) $" -+-";
	PlayerPawn(p).ClearProgressMessages();
	PlayerPawn(p).SetProgressTime(3.0);
	PlayerPawn(p).SetProgressColor(col,4);
	PlayerPawn(p).SetProgressMessage(msg,4);
}

function Mutate(String msg, PlayerPawn Sender) {
	local String rep;
	local Inventory inv;
	local Sound snd;

	if (msg ~= "LISTINV") {

		rep = "";
		for (Inv=Sender.Inventory; Inv!=None; Inv=Inv.Inventory) {
			// rep = rep $ Inv $"("$ Inv.getHumanName() $") ";
			rep = rep $ Inv $" ";
			if (Len(rep)>1500) {
				rep = rep $ "...";
				break;
			}
		}
		Sender.ClientMessage("Your inventory: "$rep);

	} else if (Left(msg,10) ~= "TESTSOUND ") {

		snd = Sound(DynamicLoadObject(Mid(msg,10),class'Sound'));
		if (snd == None) {
			Sender.ClientMessage("Failed to load sound \""$ Mid(msg,10) $"\".");
		} else {
			Sender.PlaySound(snd,SLOT_Interface,3.0);
		}

	}
	Super.Mutate(msg,Sender);
}

defaultproperties {
	bGiveWeapons=False  // Handled by LMS
	InitialArmour=123   // Handled by LMS
	InitialHealth=123   // Handled by LMS?
	// InitialArmour=100
	HealthLostPerSec=2.0
	HealthGainedPerKill=50.0
	bGivePowerups=True
	FragsForPowerup=4
	InitialWeapon(0)="Botpack.ImpactHammer"
	InitialWeapon(1)="Botpack.Enforcer"
	InitialWeapon(2)="Botpack.UT_BioRifle"
	InitialWeapon(3)="Botpack.PulseGun"
	InitialWeapon(4)="Botpack.Minigun2"
	InitialWeapon(5)="Botpack.UT_FlakCannon"
	InitialWeapon(6)="Botpack.UT_EightBall"
	InitialWeapon(7)="Botpack.ShockRifle"
	InitialWeapon(8)="Botpack.SniperRifle"
	PowerupSound=Sound'Botpack.Pickups.BeltSnd'
	// PowerupSound=Sound'Botpack.Pickups.Sbelthe2'
	// PowerupSound=Sound'Botpack.Pickups.AmpOut'
	Powerup(0)=(Type="Botpack.HealthPack",Color=(R=131,G=255,B=131,A=32))
	Powerup(1)=(Type="Botpack.Armor2",Color=(R=255,G=131,B=91,A=32))
	Powerup(2)=(Type="Botpack.UDamage",Color=(R=255,G=31,B=31,A=32))
	Powerup(3)=(Type="Botpack.UT_Stealth",Color=(R=31,G=31,B=190,A=48))
	Powerup(4)=(Type="Botpack.UT_ShieldBelt",Color=(R=255,G=31,B=31,A=32))
	Powerup(5)=(Type="Botpack.UT_JumpBoots",Color=(R=91,G=255,B=255,A=32))
	Powerup(6)=(Type="Botpack.WarheadLauncher",Color=(R=255,G=31,B=31,A=32))
	Powerup(7)=(Type="SiegeXXL2e.JetPack",Color=(R=91,G=255,B=255,A=32))
}

