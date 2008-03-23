// Problems:
// At the start, people just run around creating containers, often in stupid places, just because they don't wanna waste RU.  Well I guess they will learn to get a better plan...  We could set the team MAX RU to start at 500, and each player to start at 0, so they don't get worried about their RU reaching capacity too soon.  We can also reduce FastBuildRUPerSecond.

// DONE: make it detect the gamestart!  (esp. for tournament)
// TODO: Our FALLBACKs require enemies meeting. :F  Can we make a FALLBACK in the timer, that does something like check player inventories / core age / RU>0 ?
// TODO: save and load buildings
// TODO: rename to FastBuildXXL2dMut and FastBuildXXL2dGI.

class FastBuildMut extends Mutator;

var config Color colorGreen,colorYellow,colorRed,colorWhite;
var config float FastBuildMinutes;
var config int FastBuildRUPerSecond;
var config int FastBuildMaxRU;
// var config int PostbuildMaxRU;

// Config vars but not yet committed to config, because I might rename or remove them, so I don't yet want them written in the .ini file.
var /*config*/ bool bMoreFlashing;

var bool bGameStarted;
var bool bFastBuildOver;
var int SecondsToGo;

defaultproperties {
	FastBuildMinutes=3
	// FastBuildRUPerSecond=50
	FastBuildRUPerSecond=15
	FastBuildMaxRU=400 // We could just use SiegeGI.StartingMaxRU, unless we want a *different* setting for FastBuild + normal Siege.
	// PostbuildMaxRU=1000

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

	if (SiegeGI(Level.Game) == None) {
		Log("Gametype " $ Level.Game $ " is not a Siege type!  Destroying FastBuildMut.");
		Self.Destroy();
		return;
	}

	// SiegeGI(Level.Game).GameName = SiegeGI(Level.Game).GameName $ " - AS Mode";
	SiegeGI(Level.Game).GameName = Int(FastBuildMinutes) $ " minute FastBuild on " $ SiegeGI(Level.Game).GameName;

	Level.Game.RegisterDamageMutator(Self);

	bGameStarted = False;

	SetTimer(1,True);

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

// Since ModifyPlayer() is not getting called, and in case DoGameStart() is never called from the FastBuildGI, we do a FALLBACK check on the first kill:
function bool PreventDeath(Pawn Killed, Pawn Killer, name damageType, vector HitLocation) {
	if (!bGameStarted)
		DoGameStart();
	Super.PreventDeath(Killed,Killer,damageType,HitLocation);
	// Actually I put another FALLBACK in MutatorTakeDamage().
}

// Another FALLBACK - possibly the best
function CheckForGameStart() {
	local PlayerPawn p;
	foreach AllActors(class'PlayerPawn', p) {
		if (p.FindInventoryType(class'sgConstructor') != None) {
			Log("FastBuildMut.CheckForGameStart() Detected player " $ p.getHumanName() $ " who is holding a constructor - doing GameStart!");
			DoGameStart();
			return;
		}
	}
}

function DoGameStart() {
	local int i;

	Log("FastBuildMut.DoGameStart() Level.TimeSeconds="$Level.TimeSeconds$" Level.Game.ElapsedTime="$TeamGamePlus(Level.Game).ElapsedTime);
	bGameStarted = True;
	bFastBuildOver = False;
	SecondsToGo = 60 * FastBuildMinutes;
	// SetStage();
	SetTimer(1,True);
	FlashToAllPlayers("FastBuild has begun!",colorWhite,2,0,True,False);

	// // SiegeGI(Level.Game).FreeBuild = False;
	// // SiegeGI(Level.Game).RUMax = 1000;
	//// Soddit, start from scratch!
	for (i=0;i<TeamGamePlus(Level.Game).MaxTeams;i++) {
		SiegeGI(Level.Game).MaxRUs[i] = FastBuildMaxRU;
	} // If this doesn't work, try doing it earlier (PostBeginPlay)
}

event Timer() {
	local Actor a;
	local String str;
	if (Level.Game.bGameEnded) {
		return;
	}
	if (!bGameStarted) {
		if (TeamGamePlus(Level.Game).countdown <= 1) {
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
			SetTimer(0,False);
			return;
		} else {
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
		}

		foreach AllActors(class'Actor', a) {
			if (sgPRI(a) != None) {
				sgPRI(a).AddRU(FastBuildRUPerSecond);
			}
		}

		SecondsToGo--;
		// SetTimer(1,True);
	}
}

function MutatorTakeDamage( out int ActualDamage, Pawn Victim, Pawn InstigatedBy, out Vector HitLocation, out Vector Momentum, name DamageType) {
	if (!bGameStarted) { // FALLBACK in case DoGameStart() is not called from FastBuildGI.
		DoGameStart();
	}
	if (bGameStarted && !bFastBuildOver) {
		ActualDamage = 0;
		// DamageType = None;
		// Momentum = Momentum * 2.0;
		// Momentum = Momentum * 0.0;
	}
	Super.MutatorTakeDamage(ActualDamage,Victim,InstigatedBy,HitLocation,Momentum,DamageType);
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

