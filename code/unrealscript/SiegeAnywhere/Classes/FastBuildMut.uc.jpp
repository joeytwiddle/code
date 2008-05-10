// For convenience, FastBuildMut can be invoked by selecting the FastBuildGI gametype.  But adding the mutator to a classic Siege game has the same effect.

// TODO: FastBuild starts even if the server is empty!

// TODO: colours are shite
// TODO: damage protection should be optional - if you shoot you lose it
// TODO: Problem with: sgTDM and sgDOM - sometimes spawnpoints are outside the safe area!  Ideally they wouldn't have one.

// DONE: no selfharm during the fastbuild is ok, but fall harm and zone harm should be ON!  ok now only harm from other players is blocked

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

// DONE: Do not allow players into enemy base during the FastBuild.
// TODO: They can get in for a moment (long enough to take a pickup!) using their translocator.  Translocators should not be allowed into the enemy volume either.
// Consider: kill players who are too far inside enemy base (more than just walking or dodging into the wall).

class FastBuildMut extends Mutator;

var config Color colorGreen,colorYellow,colorRed,colorWhite;
var config float FastBuildMinutes;
var config int FastBuildRUPerSecond;
var config int FastBuildStartingMaxRU;
// var config int PostbuildMaxRU;
var config bool bNoDamage;
var config bool bConfineToBase;

// Config vars but not yet committed to config, because I might rename or remove them, so I don't yet want them written in the .ini file.
var /*config*/ bool bMoreFlashing;

var FlagBase FlagSpots[2];
var Vector TeamOrigin[2];
var float MaxAllowedRadius;

var bool bGameStarted;
var bool bFastBuildOver;
var int SecondsToGo;

defaultproperties {
	FastBuildMinutes=3
	// FastBuildRUPerSecond=50
	FastBuildRUPerSecond=15 // For non SiegeGI, Max RU in 26 seconds, then remaining 154 seconds give half to max => 400 + 1155 = 1555 (actually came out at 1422).  But if you build and upgrade conts, you never exceed max, so you get total 60 * 3 * 15 = 2700!
	FastBuildStartingMaxRU=400 // We could just use SiegeGI.StartingMaxRU, unless we want a *different* setting for FastBuild + normal Siege.
	// PostbuildMaxRU=1000
	bNoDamage=True
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

	FindBoundary();

	SetTimer(1,True);
	Enable('Tick');

}

function FindBoundary() {

	//// We don't need to scan, if we aren't gonna confine them.  But we scan anyway, in case an admin turns it on during play.
	// if (!bConfineToBase)
		// return;

	MaxAllowedRadius = 0;

	FindBoundaryFromFlags();

	// if (MaxAllowedRadius < 2048) { // failed on Panels, PureAction and ShockBlock, but worked on 2on2Crates.
	if (MaxAllowedRadius < 1024) { // TODO: The REAL check we want to make, is whether all PlayerStarts are within the boundary.
		// Log("FastBuildMut.FindBoundary(): Could not find both FlagBases, or they were too close.  Now averaging PlayerStarts...");
		FindBoundaryFromSpawnPoints();
		//// TODO: this is desirable for assault, but maybe not TDM and *definitely* not DOM!
		// bConfineToBase = False;
		// return;
	}

	// MaxAllowedRadius is now deprecated, dotproduct is used instead.
	// Log("FastBuildMut.FindBoundary(): MaxAllowedRadius="$MaxAllowedRadius);
	if (MaxAllowedRadius < 256) { // DM-Richocet was 409, bleak was 3117.
		// MaxAllowedRadius = 1024 * 256;
		Log("FastBuildMut.FindBoundary(): TeamOrigins are too close together to build a wall!");
		if (bConfineToBase) {
			Log("FastBuildMut.FindBoundary(): Disabling bConfineToBase");
		}
		bConfineToBase = False;
	}

	if ( String(Level.Game.Class)~="Botpack.TeamGamePlusDeathMatchPlus" || String(Level.Game.Class)~="Botpack.DeathMatchPlus") {
		Log("FastBuildMut.FindBoundary(): Disabling because DM/TDM map");
		bConfineToBase = False;
	}

	// Some maps, e.g. CTF-Burning, have the flags quite close to each other, but the map expands away from them.
	// Good sanity check.  We could try to detect this, e.g. by looking at PlayerStarts, PathNodes, InventorySpots...
	// DONE: Or maybe we shouldn't use radius at all - just look for players crossing the plane which divides the to bases.
	// TODO: Even then, on some maps, you might cross that boundary without actually leaving your base.  E.g. Joust maps, where every PlayerStart will be *beyond* the boundary!

}

function FindBoundaryFromFlags() {
	local FlagBase Flag;
	// Look for flagspots in map
	foreach AllActors(class'FlagBase', Flag) {
		FlagSpots[Flag.Team] = Flag;
		TeamOrigin[Flag.Team] = Flag.Location;
	}
	if (FlagSpots[0] != None && FlagSpots[1] != None) {
		MaxAllowedRadius = VSize(TeamOrigin[0] - TeamOrigin[1]) / 2;
		Log("FastBuildMut.FindBoundaryFromFlags(): Found two flags, MaxAllowedRadius = "$MaxAllowedRadius);
	} else {
		Log("FastBuildMut.FindBoundaryFromFlags(): Could not find two flags.");
	}
}

function FindBoundaryFromSpawnPoints() {
	local PlayerStart ps;
	local int TeamCount[4];
	TeamCount[0] = 0;
	TeamCount[1] = 0;
	TeamOrigin[0] = vect(0,0,0);
	TeamOrigin[1] = vect(0,0,0);
	foreach AllActors(class'PlayerStart', ps) {
		TeamOrigin[ps.TeamNumber] = ( TeamOrigin[ps.TeamNumber] * TeamCount[ps.TeamNumber] + ps.Location * 1 ) / (TeamCount[ps.TeamNumber]+1);
		TeamCount[ps.TeamNumber]++;
	}
	MaxAllowedRadius = VSize(TeamOrigin[0] - TeamOrigin[1]) / 2.0;
	Log("FastBuildMut.FindBoundaryFromSpawnPoints(): Found "$TeamCount[0]$" PlayerStarts for Red, and "$TeamCount[1]$" for blue.");
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
		// TODO: This is no good in SiegeCTF in tournament mode (at least when Pure is present)!
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
	SecondsToGo = 60 * FastBuildMinutes + 3; // The +3 is for the initial message below - then the 3 minute warning will begin :)
	// SetStage();
	SetTimer(1,True);
	FlashToAllPlayers("FastBuild has begun!",colorWhite,2,0,False,False);
	FlashToAllPlayers("",colorWhite,2,0,False,False); // Just clear the earlier messages.

	// // SiegeGI(Level.Game).FreeBuild = False;
	// // SiegeGI(Level.Game).RUMax = 1000;
	//// Soddit, start from scratch!
	if (SiegeGI(Level.Game) != None) {
		for (i=0;i<SiegeGI(Level.Game).MaxTeams;i++) {
			SiegeGI(Level.Game).MaxRUs[i] = FastBuildStartingMaxRU;
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
			Log("FastBuild.Timer(): Doing game start at "$Level.TimeSeconds$".");
			DoGameStart();
		} else {
			if (bConfineToBase) {
				FlashToAllPlayers("You may not leave your base during the FastBuild.",colorWhite,3,0,True,False);
			} else {
				FlashToAllPlayers("Weapons will not work during the FastBuild.",colorWhite,3,0,True,False);
			}
			FlashToAllPlayers("Resource rate = "$FastBuildRUPerSecond$" RU/sec",colorWhite,4,0,True,False);
			CheckForGameStart();
			return;
		}
	}
	if (!bFastBuildOver) {
		if (SecondsToGo <= 0) {
			bFastBuildOver = True;
			Log("FastBuildMut.Timer() FastBuild ended at "$Level.TimeSeconds);
			// FlashToAllPlayers("Weapons are Live - Assault the Base!",colorRed,4);
			FlashToAllPlayers("FastBuild is over",colorGreen,2,0,False,True);
			if (bMoreFlashing) {
				FlashToAllPlayers("Weapons are LIVE!",colorRed,3,0,True,False);
				FlashToAllPlayers("Assault the Base!",colorYellow,4,0,True,False);
			}
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
				FlashToAllPlayers(SecondsToGo $ " ...",colorWhite,2,0,False, (SecondsToGo==10) );
			}

			SecondsToGo--;
			// SetTimer(1,True);
		}
	}
	if (bFastBuildOver) {
		// AddRUGlobally(FastBuildRUPerSecond/4.0); // global trickle in case no core is feeding
		// DONE: This should really be moved to SiegeAnywhere.
		// It's only needed when we can't find an sgBaseCore.
	}
}

event Tick(float DeltaTime) {
	if (bGameStarted && !bFastBuildOver)
		CheckBoundaries();
}

function CheckBoundaries() {
	local Actor a;
	local Pawn p;
	local Vector origin;
	local float depth, intoWall, withWall;

	if (!bConfineToBase)
		return;

	origin = (TeamOrigin[0] + TeamOrigin[1]) / 2.0; // The position of the wall

	// for (p=Level.PawnList; p!=None; p=p.NextPawn) {
		// if (p.bIsPlayer && !p.IsA('Spectator')) {

	foreach AllActors(class'Actor',a) {

		/*
		if (TranslocatorTarget(a) != None) {
			if (Pawn(a.Owner) == None) {
				Log("FastBuildMut.CheckBoundaries(): Could not check "$a); // happens alot!
				continue;
			}
			depth = (a.Location - origin) Dot Normal(TeamOrigin[Pawn(a.Owner).PlayerReplicationInfo.Team]-origin);
			if (depth < 0) {
				// TODO CONSIDER: If this does not work, we could loop Translocators, and empty their TTarget if neccessary.
				TranslocatorTarget(a).Destroy();
			}
		}
		*/

		if (Pawn(a) != None && Pawn(a).bIsPlayer) {
			p = Pawn(a);
			depth = (p.Location - origin) Dot Normal(TeamOrigin[p.PlayerReplicationInfo.Team]-origin);
			if (depth < 0) {
				// Player has passed the invisible boundary wall
				/*
				if (FRand()<0.2) {
					BroadcastMessage(p.getHumanName()$" tried to violate FastBuild boundaries!");
					p.Died(None, '', p.Location);
				} else {
					*/
				// if (FRand()<0.25) {
				if (true) {
				}
				// if (FRand()<0.1) {
				if (true) {
					// p.TakeDamage(10*FRand(),p,p.Location,vect(0,0,0),'Fell'); // self-damage
					p.TakeDamage(5,p,p.Location,vect(0,0,0),'Fell'); // self-damage
					// p.ClientMessage("Warning: you may not leave your base during the FastBuild!");
					// p.ClientMessage("Warning: "$depth);
					if (p.IsA('PlayerPawn'))
						FlashMessageToPlayer(PlayerPawn(p),"Warning: you may not leave your base during the FastBuild!",colorYellow,0,2,False,False);
					// DONE: this result is according to the spheres and not the wall - but we are detecting using the wall!
					// p.SetLocation(TeamOrigin[p.PlayerReplicationInfo.Team] + Normal(p.Location - TeamOrigin[p.PlayerReplicationInfo.Team])*MaxAllowedRadius);
					// p.Velocity = Normal(TeamOrigin[p.PlayerReplicationInfo.Team] - p.Location) * 128;
					// if (VSize(p.Velocity) < 128)
						// p.Velocity = vect(128,0,0);
					// p.Velocity = VSize(p.Velocity) * Normal(TeamOrigin[p.PlayerReplicationInfo.Team] - p.Location);
					// withWall / intoWall
					// Calculate component of velocity which is banging into wall (as opposed to scraping alongside).
					intoWall = (p.Velocity) Dot Normal(TeamOrigin[p.PlayerReplicationInfo.Team] - origin);
					if (intoWall<256) // In case their velocity was 0 or small, e.g. they just translocated inside the zone.
						intoWall = 256;
					// withWall = VSize(p.Velocity + Abs(intoWall)*Normal(TeamOrigin[p.PlayerReplicationInfo.Team] - origin));
					// Bounce player backwards off the wall:
					p.Velocity = p.Velocity + 2.0*Abs(intoWall)*Normal(TeamOrigin[p.PlayerReplicationInfo.Team] - origin);
					// Force the player to the wall edge, since right now they are slightly inside it.
					p.SetLocation(p.Location + Abs(depth+0.05) * Normal(TeamOrigin[p.PlayerReplicationInfo.Team] - origin));
					// p.Velocity.Z = 12;
					// p.Velocity.Z += 6;
					p.SetPhysics(PHYS_Falling);
					// p.PlaySound(class'Shield'.default.EffectSound1);
					// p.PlaySound(sound'bmbreak2',SLOT_Misc,2.0);
					// p.PlaySound(sound'Hit1',SLOT_Misc,2.0);
					// p.PlaySound(sound'BNewGib',SLOT_Misc,2.0);
					p.PlaySound(sound'TDisrupt',SLOT_Misc,2.0);
				}
			}

		}
	}
	// DONE: We should also check for any TranslocTargets outside the boundary, or indeed TODO grappling hooks or anything else they might use.
}

function AddRUGlobally(float amount) {
	local Actor a;
	foreach AllActors(class'Actor', a) {
		if (sgPRI(a) != None) {

			// if (sgPRI(a).MaxRU < FastBuildStartingMaxRU)
				// sgPRI(a).MaxRU = FastBuildStartingMaxRU;

			sgPRI(a).AddRU(amount);

			// FastBuild is meant to run with classic Siege, where MaxRU is dealt with at game start and then by the cores.
			// Although it wasn't designed for SiegeAnywhere, it can work in *some* of those types.
			if (sgPRI(a).RU >= sgPRI(a).MaxRU) {
				// If SiegeCTF starts with MaxRU 100, we should expand it as they score.
				if (SiegeGI(Level.Game) == None /*&& sgPRI(a).MaxRU < FastBuildMaxRU*/) {
					// If we're not playing siege, then the only way to increase Max is by fragging.
					// Since fragging is impossible during FastBuild, we let AddRUGlobally push up their max.
					// You get twice as much RU if you spend, but you can save if you need to.
					sgPRI(a).MaxRU = (sgPRI(a).MaxRU + sgPRI(a).RU) / 2; // grow max at half the speed
					/*
					if (sgPRI(a).MaxRU > FastBuildMaxRU) {
						sgPRI(a).MaxRU = FastBuildMaxRU;
					}
					*/
					sgPRI(a).RU = sgPRI(a).MaxRU;
				} else {
					sgPRI(a).RU = sgPRI(a).MaxRU;
				}
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
	if (bGameStarted && !bFastBuildOver && bNoDamage) {
		// Only block if damage was caused by another player or an enemy building
		if (/*PlayerPawn(InstigatedBy) != None*/ InstigatedBy != None && DamageType != 'Fell' && DamageType != 'Suicided') {
			ActualDamage=0;
			// DamageType = None;
			// Momentum = Momentum * 2.0;
			if (InstigatedBy.PlayerReplicationInfo.Team != Victim.PlayerReplicationInfo.Team)
				Momentum = Momentum * 0.0;
		}
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
	// Log("FastBuildMut.FlashMessageToPlayer("$p.getHumanName()$"): "$Msg);
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

