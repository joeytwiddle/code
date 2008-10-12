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
// TODO: Rather than watch health go down 2 points each second, set the timer frequency calculated to remove 1hp each call to Timer().
// TODO: zeroping weapons not working, piglet wants "+50" txt, or happy with healthsound instead.
//       initial armour and health got set by LMS.
//       Player is Out.

class FairLMS expands Mutator config(FairLMS);

struct LMSBonus {
	var String Type;
	var Color Color;
	var String Name;  // If not set, defaults to item's ItemName.
	var String Sound; // If not set, defaults to item's PickupSound.
};

var config bool bGiveWeapons;
var config int InitialArmour,InitialHealth;
var config float HealthLostPerSec,HealthGainedPerKill;
var config bool bGivePowerups;
var config int FragsForPowerup;
var config String InitialWeapon[20];
var config LMSBonus Powerup[20];
var config Sound PowerupSound;
var config Color MessageColor;

var int KillsSinceSpawn[64];
var bool bGameStarted,bTwoPlayersLeft;

function PostBeginPlay() {
	Super.PostBeginPlay();
	Level.Game.BaseMutator.AddMutator(Self); // It was likely to happen later anyway.  This way we can get destroyed if neccessary, before setting any of this stuff up twice!
	SetTimer(1.0/HealthLostPerSec,True);
	if (HealthLostPerSec>0 || HealthGainedPerKill>0)
		Level.Game.GameName = "Anti-Hide "$ Level.Game.GameName;
	if (bGivePowerups)
		Level.Game.GameName = Level.Game.GameName $" with Bonus Powerups";
	if (HealthLostPerSec>0)
		DeathMatchPlus(Level.Game).StartMessage = "Your are losing health!  Kill to stay alive!";

	// TESTING: Turn redeemers into mini-redeemers.
	// TODO: Do these changes propogate over maps, messing with other types of game?
	// CONSIDER: If setting defaults doesn't work, change values after it has spawned.  (CheckReplacement() / IsRelevant()?)
	class'WarheadLauncher'.default.ItemName = "Mini Redeemer";
	class'WarShell'.default.DrawScale = class'WarShell'.default.DrawScale * 0.6;
	class'WarShell'.default.Speed = 400;
	class'WarShell'.default.Damage = 400;
	class'WarShell'.default.MomentumTransfer = 1000;
	class'GuidedWarShell'.default.DrawScale = class'GuidedWarShell'.default.DrawScale * 0.6;
	class'GuidedWarShell'.default.Speed = 400;
	class'GuidedWarShell'.default.Damage = 400;
	class'GuidedWarShell'.default.MomentumTransfer = 1000;
	class'WarExplosion'.default.DrawScale = class'WarExplosion'.default.DrawScale * 0.6;
	class'WarExplosion2'.default.DrawScale = class'WarExplosion2'.default.DrawScale * 0.6;
}

function AddMutator(Mutator m) {
	if (m.class == Self.class) {
		if (m != Self)
			m.Destroy();
		Log(Self$".AddMutator() Not adding mutator "$m$" because I already exist!");
	} else {
		Super.AddMutator(m);
	}
}

event Timer() {
	local Pawn p;
	local PlayerPawn pp;
	local int aliveCount;
	local String players;
	Super.Timer();
	foreach AllActors(class'Pawn',p) {
		if (!bGameStarted && p.FindInventoryType(class'Weapon')!=None) {
			bGameStarted = True;
		}
		if ((PlayerPawn(p)!=None || Bot(p)!=None) && Spectator(p)==None) {
			if (!bTwoPlayersLeft) {
				p.Health -= 1;
				if (p.Health == 0)
					FlashMessage(p,"You are about to die!  Kill to survive!",MessageColor);
				if (p.Health <= -15) {
					p.Died(None, 'Suicided', p.Location);
				}
				// CONSIDER DONE: when player first reaches 0, we could add some effect to him (skull above his head) to show that he requires only 1 damage hit to die.
				//                maybe fairer not to warn other players, but to warn the player who is in danger!
			}
		}
		if (p.PlayerReplicationInfo.Score>0) {
			aliveCount++;
			if (players == "")
				players = p.getHumanName() $ " v";
			else
				players = players $ " " $ p.getHumanName();
		}
	}
	if (bGameStarted && LastManStanding(Level.Game)!=None && aliveCount==2 && !bTwoPlayersLeft) {
		foreach AllActors(class'PlayerPawn',pp) {
			FlashMessage(pp,"Two players left: ",MessageColor,3,true);
			FlashMessage(pp,players,MessageColor,4);
		}
		bTwoPlayersLeft = True;
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
			if (KillsSinceSpawn[killer.PlayerReplicationInfo.PlayerID%64]%FragsForPowerup == 0 && bGivePowerups && !bTwoPlayersLeft) {
				GiveRandomPowerup(killer);
			}
		}
	}
}

function GiveArmor(Pawn p) {
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

		// OK we have created the powerup, we can give it to the player:
		GiveInventory(p,inv);
		col = Powerup[i].Color;
		if (col.R==0 && col.G==0 && col.B==0) {
			col.R=255; col.G=255; col.B=16; col.A=16;
		}
		if (Powerup[i].Name == "")
			Powerup[i].Name = inv.ItemName;
		FlashMessage(p,"-+- "$Caps(Powerup[i].Name)$" -+-",col);
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
		// TODO: maybe remove something from his inventory and retry?  Then at least he could get a fresh one.
	}

}

function FlashMessage(Pawn p, String msg, Color col, optional int line, optional bool bMoreComing) {
	if (PlayerPawn(p)==None)
		return;
	if (line == 0) line=4;
	// msg = Caps(msg) $ "!";
	if (!bMoreComing)
		PlayerPawn(p).ClearProgressMessages();
	PlayerPawn(p).SetProgressTime(3.0);
	PlayerPawn(p).SetProgressColor(col,line);
	PlayerPawn(p).SetProgressMessage(msg,line);
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
	Powerup(2)=(Type="Botpack.UDamage",Color=(R=192,G=31,B=192,A=32))
	Powerup(3)=(Type="Botpack.UT_Stealth",Color=(R=31,G=31,B=190,A=48))
	Powerup(4)=(Type="Botpack.UT_ShieldBelt",Color=(R=255,G=255,B=31,A=32))
	Powerup(5)=(Type="Botpack.UT_JumpBoots",Color=(R=91,G=255,B=255,A=32))
	Powerup(6)=(Type="Botpack.WarheadLauncher",Color=(R=255,G=31,B=31,A=32))
	Powerup(7)=(Type="SiegeXXL2e.JetPack",Color=(R=91,G=192,B=255,A=32))
	Powerup(8)=(Type="kxGrapple.GrappleGun",Color=(R=91,G=50,B=12,A=32))
	Powerup(9)=(Type="kxDoubleJump.DoubleJumpBoots",Color=(R=91,G=255,B=255,A=32))
	MessageColor=(R=255,G=255,B=31,A=0)
}

