// For convenience, FastBuildMut can be invoked by selecting the FastBuildGI gametype.  But adding the mutator to a classic Siege game has the same effect.

// Note FastBuild is only really for classic Siege, not for SiegeAnywhere.
// E.g. in SiegeCTF and SiegeAssault, players can make flagruns, or complete AS objectives, during the 3 minute period when weapons cannot harm them!

// Problems:
// At the start, people just run around creating containers, often in stupid places, just because they don't wanna waste RU.  Well I guess they will learn to get a better plan...  We could set the team MAX RU to start at 500, and each player to start at 0, so they don't get worried about their RU reaching capacity too soon.  We can also reduce FastBuildRUPerSecond.

// DONE: make it detect the gamestart!  (esp. for tournament)
// DONE: Our FALLBACKs require enemies meeting. :F  Can we make a FALLBACK in the timer, that does something like check player inventories / core age / RU>0 ?
// DONE: save and load buildings - see SiegeForever
// DONE: rename package to FastBuildXXL2dRC1.
// TODO: So that this mutator will work on ANY Siege version, can we change the implementation so that it uses string-matching on class types, and Get/SetPropertyText to update RU?  The toughest part will be finding the initialisation class types.

// Provide FastBuild to the first 3 minutes of any SiegeGame by adding this mutator.
// TODO: Allow non Siege gametypes, where players still have a sgPRI.  (You mean SiegeAnywhere?)

// DONE: +RU from kills/leeching can exceed MaxRU!  That was only happening with SiegeAnywhere - fixed there.

// TODO: Do not allow players into enemy base during the FastBuild.

class FastBuildMut extends Mutator;

var config Color colorGreen,colorYellow,colorRed,colorWhite;
var config float FastBuildMinutes;
var config int FastBuildRUPerSecond;
var config int FastBuildMaxRU;
// var config int PostbuildMaxRU;
var config bool bConfineToBase;

// Config vars but not yet committed to config, because I might rename or remove them, so I don't yet want them written in the .ini file.
var /*config*/ bool bMoreFlashing;

var Vector TeamOrigin[2];
var float MaxAllowedRadius;

var bool bGameStarted;
var bool bFastBuildOver;
var int SecondsToGo;

defaultproperties {
	FastBuildMinutes=3
	// FastBuildRUPerSecond=50
	FastBuildRUPerSecond=15
	FastBuildMaxRU=400 // We could just use SiegeGI.StartingMaxRU, unless we want a *different* setting for FastBuild + normal Siege.
	// PostbuildMaxRU=1000
	bConfineToBase=True

	bMoreFlashing=True

	bFastBuildOver=False
	// SecondsToGo=180

	colorGreen=(R=0,G=255,B=0,A=0)
	colorYellow=(R=255,G=255,B=0,A=0)
	colorRed=(R=255,G=0,B=0,A=0)
	colorWhite=(R=255,G=255,B=255,A=0)
}

function PostBeginPlay() {
	local int i;

	Super.PostBeginPlay();

	/*
	if (SiegeGI(Level.Game) == None) {
		Log("Gametype " $ Level.Game $ " is not a Siege type!  Destroying FastBuildMut.");
		Self.Destroy();
		return;
	}
	*/

	// SiegeGI(Level.Game).GameName = SiegeGI(Level.Game).GameName $ " - AS Mode";
	Level.Game.GameName = Int(FastBuildMinutes) $ " minute FastBuild on " $ Level.Game.GameName;

	Level.Game.RegisterDamageMutator(Self);

	bGameStarted = False;

	FindBoundaries();

	SetTimer(1,True);

}

function FindBoundaries() {
	local FlagBase Flag;
	foreach AllActors(class'FlagBase', Flag) {
		TeamOrigin[Flag.Team] = Flag.Location;
	}
	if (VSize(TeamOrigin[0]) == 0 || VSize(TeamOrigin[1]) == 0) { // This test is not really good - one of the FlagSpots really might be at (0,0,0).  Better to save the FlagBases rather than their Locations.
		// TODO: calculate TeamOrigin from PlayerStarts
		// If team PlayerStarts are not grouped, ensure MaxAllowedRadius at least encompasses all PlayerStarts (or make it huge).
		Log("FastBuild could not find both FlagBases - setting bConfineToBase=False.");
		bConfineToBase = False;
		return;
	}
	MaxAllowedRadius = VSize(TeamOrigin[0] - TeamOrigin[1]) / 2.0;
	if (MaxAllowedRadius < 2048)
		MaxAllowedRadius = 1024 * 256;
	// Some maps, e.g. CTF-Burning, have the flags quite close to each other, but the map expands away from them.
	// Good sanity check.  We could try to detect this, e.g. by looking at PlayerStarts, PathNodes, InventorySpots...
	// DONE: Or maybe we shouldn't use radius at all - just look for players crossing the plane which divides the to bases.
	// TODO: Even then, on some maps, you might cross that boundary without actually leaving your base.  E.g. Joust maps, where every PlayerStart will be *beyond* the boundary!
}

/* // Not working, possibly Siege fails to call ModifyPlayer().
function ModifyPlayer(Pawn p) {
	Log("FastBuildMut.ModifyPlayer(" $ p.getHumanName() $ ")");
	// This detects game start, when the first player spawns.
	// But will it work if the tournament mode lets players fight?!  Probably not!
	if (!bGameStarted) {
		DoGameStart();
	}
	Super.ModifyPlayer(p);
}
*/

/*
// Since ModifyPlayer() is not getting called, and in case DoGameStart() is never called from the FastBuildGI, we do a FALLBACK check on the first kill:
function bool PreventDeath(Pawn Killed, Pawn Killer, name damageType, vector HitLocation) {
	if (!bGameStarted)
		DoGameStart();
	Super.PreventDeath(Killed,Killer,damageType,HitLocation);
	// Actually I put another FALLBACK in MutatorTakeDamage().
}
*/

// Detect the start of the game.
// This is the FALLBACK we kept, in case we were not invoked from FastBuildGI.
function CheckForGameStart() {
	local PlayerPawn p;
	foreach AllActors(class'PlayerPawn', p) {
		// We could in fact check for any weapon type which we know will be a default, e.g. enforcer/piston.
		// if (p.FindInventoryType(class'sgConstructor') != None || p.FindInventoryType(class'SiegeAnywhereConstructor') != None) { // This was firing early - I blame AddSiegeToGameType.AddDefaultInventory()!
		if (p.FindInventoryType(class'ImpactHammer') != None) {
			Log("FastBuildMut.CheckForGameStart() Detected player " $ p.getHumanName() $ " who is holding a weapon - doing GameStart!");
			DoGameStart();
			return;
		}
	}
}

function DoGameStart() {
	local int i;

	Log("FastBuildMut.DoGameStart() Level.TimeSeconds="$Level.TimeSeconds$" Level.Game.ElapsedTime="$TeamGamePlus(Level.Game).ElapsedTime);
	bGameStarted = True;
	// TimeGameStarted = Level.TimeSeconds;
	bFastBuildOver = False;
	SecondsToGo = 60 * FastBuildMinutes;
	// SetStage();
	SetTimer(1,True);
	FlashToAllPlayers("FastBuild has begun!",colorWhite,2,0,True,False);

	// // SiegeGI(Level.Game).FreeBuild = False;
	// // SiegeGI(Level.Game).RUMax = 1000;
	//// Soddit, start from scratch!
	if (SiegeGI(Level.Game) != None) {
		for (i=0;i<SiegeGI(Level.Game).MaxTeams;i++) {
			SiegeGI(Level.Game).MaxRUs[i] = FastBuildMaxRU;
		} // If this doesn't work, try doing it earlier (PostBeginPlay)
	}
}

event Timer() {
	local String str;
	if (Level.Game.bGameEnded) {
		return;
	}
	if (!bGameStarted) {
		if (TeamGamePlus(Level.Game).countdown <= 1) {
			ClearAllProgressMessages();
			DoGameStart();
		} else {
			FlashToAllPlayers("Weapons will not work during the FastBuild",colorWhite,3,0,True,False);
			FlashToAllPlayers("Resource rate = "$FastBuildRUPerSecond$" RU/sec",colorWhite,4,0,True,False);
			CheckForGameStart();
			return;
		}
	}
	if (!bFastBuildOver) {
		if (SecondsToGo <= 0) {
			Log("FastBuildMut.Timer() FastBuild Over");
			// FlashToAllPlayers("Weapons are Live - Assault the Base!",colorRed,4);
			FlashToAllPlayers("FastBuild Over",colorGreen,2,0,False,True);
			if (bMoreFlashing) {
				FlashToAllPlayers("Weapons are LIVE!",colorRed,3,0,True,False);
				FlashToAllPlayers("Assault the Base!",colorYellow,4,0,True,False);
			}
			bFastBuildOver = True;
			// SecondsToGo = 330;
			// SetStage();
			// SetTimer(0,False);
		} else {

			AddRUGlobally(FastBuildRUPerSecond);

			if ((SecondsToGo % 60) == 0) {
				FlashToAllPlayers("You have " $ (SecondsToGo/60) $ " minutes to build your base!",colorWhite,2,0,False,True);
			}
			if (SecondsToGo == 60) {
				FlashToAllPlayers("Weapons go Live in 1 minute...",colorWhite,2,0,False,True);
			}
			if (SecondsToGo == 30) {
				// FlashToAllPlayers("Freebuild ends in 30 seconds...",colorGreen,2);
				FlashToAllPlayers("Weapons go Live in 30 seconds...",colorWhite,2,0,False,True);
			}
			if (SecondsToGo <= 10) {
				FlashToAllPlayers(SecondsToGo $ " ...",colorWhite,2,0,False,True);
			}

			SecondsToGo--;
			// SetTimer(1,True);
		}
		CheckBoundaries();
	}
	if (bFastBuildOver) {
		// AddRUGlobally(FastBuildRUPerSecond/4.0); // global trickle in case no core is feeding
		// DONE: This should really be moved to SiegeAnywhere.
		// It's only needed when we can't find an sgBaseCore.
	}
}

function CheckBoundaries() {
	local Pawn p;
	local Vector origin;
	if (!bConfineToBase)
		return;
	origin = (TeamOrigin[0] + TeamOrigin[1]) / 2.0;
	for (p=Level.PawnList; p!=None; p=p.NextPawn) {
		if (p.bIsPlayer && !p.IsA('Spectator')) {
			// if (VSize(p.Location - TeamOrigin[p.PlayerReplicationInfo.Team]) > MaxAllowedRadius) {
			if ((p.Location - origin) Dot (TeamOrigin[p.PlayerReplicationInfo.Team]-origin) < 0) {
				p.ClientMessage("Warning: you may not leave your base during the FastBuild!");
				if (FRand()<0.2) {
					BroadcastMessage(p.getHumanName()$" tried to violate FastBuild boundaries!");
					p.Died(None, '', p.Location);
				} else {
					if (p.IsA('PlayerPawn'))
						FlashMessageToPlayer(PlayerPawn(p),"Warning: you may not leave your base during the FastBuild!",colorYellow,0,2,False,False);
					p.SetLocation(TeamOrigin[p.PlayerReplicationInfo.Team] + Normal(p.Location - TeamOrigin[p.PlayerReplicationInfo.Team])*MaxAllowedRadius);
					p.Velocity = Normal(TeamOrigin[p.PlayerReplicationInfo.Team] - p.Location) * 1024;
					p.Velocity.Z = 1024;
					p.SetPhysics(PHYS_Falling);
				}
			}
		}
	}
}

function AddRUGlobally(float amount) {
	local Actor a;
	foreach AllActors(class'Actor', a) {
		if (sgPRI(a) != None) {
			sgPRI(a).AddRU(amount);
			// We should not affect MaxRU.  FastBuild is meant to run with classic Siege, it does not apply in SiegeCTF or SiegeAssault.
			// Ah but we must at least check MaxRU, otherwise we tend to exceed it, in SiegeCTF at least.
			if (sgPRI(a).RU >= sgPRI(a).MaxRU) {
				sgPRI(a).RU = sgPRI(a).MaxRU;
			}
			/*
			if (sgPRI(a).RU >= sgPRI(a).MaxRU) { // at max
				sgPRI(a).MaxRU += amount / 8.0;
				sgPRI(a).RU = sgPRI(a).MaxRU;
			} else if (sgPRI(a).RU + amount > sgPRI(a).MaxRU) { // reached max
				sgPRI(a).RU = sgPRI(a).MaxRU;
			} else { // below max
				sgPRI(a).AddRU(amount);
				// sgPRI(a).MaxRU += amount / 8.0;
			}
			*/
		}
	}
}

function MutatorTakeDamage( out int ActualDamage, Pawn Victim, Pawn InstigatedBy, out Vector HitLocation, out Vector Momentum, name DamageType) {
	// if (!bGameStarted) { // FALLBACK in case DoGameStart() is not called from FastBuildGI.
		// DoGameStart();
	// }
	if (bGameStarted && !bFastBuildOver) {
		ActualDamage = 0;
		// DamageType = None;
		// Momentum = Momentum * 2.0;
		// Momentum = Momentum * 0.0;
	}
	Super.MutatorTakeDamage(ActualDamage,Victim,InstigatedBy,HitLocation,Momentum,DamageType);
}

function ClearAllProgressMessages() {
	local Pawn p;
	// local int i;
	for (p=Level.PawnList; p!=None; p=p.NextPawn) {
		if (p.IsA('PlayerPawn') && !p.IsA('Spectator')) {
			PlayerPawn(p).ClearProgressMessages();
		}
	}
	// for (i=0;i<8;i++) {
		// FlashToAllPlayers(" ",colorWhite,i);
	// }
}

function FlashMessageToPlayer(PlayerPawn p, string Msg, Color msgColor, optional int linenum, optional int Duration, optional bool bAdditional, optional bool bBeep) {
	Log("FastBuildMut.FlashMessageToPlayer("$p.getHumanName()$"): "$Msg);
	if (Duration == 0)
		Duration = 5;
	if (!bAdditional)
		p.ClearProgressMessages();
	p.SetProgressTime(Duration);
	p.SetProgressColor(msgColor,linenum);
	p.SetProgressMessage(Msg,linenum);
	if (bBeep) // TODO; this beep is broken somehow!  Maybe UnrealShare is not loaded by client at runtime?
		p.PlaySound(sound'Beep', SLOT_Interface, 2.5, False, 32, 32); // we play our own sound
}

function FlashToAllPlayers(String Msg, Color msgColor, optional int linenum, optional int Duration, optional bool bAdditional, optional bool bBeep) {
	local PlayerPawn P;
	foreach AllActors(class'PlayerPawn',P) {
		if (!P.IsA('Spectator')) {
			FlashMessageToPlayer(P,Msg,msgColor,linenum,Duration,bAdditional,bBeep);
		}
	}
}

