// #define DebugLog(X) Log(X)

// Problems:
// At the start, people just run around creating containers, often in stupid places, just because they don't wanna waste RU.  Well I guess they will learn to get a better plan...  We could set the team MAX RU to start at 500, and each player to start at 0, so they don't get worried about their RU reaching capacity too soon.  We can also reduce FastbuildRUPerSecond.

// TODO: make it detect the gamestart!  (esp. for tournament)
// TODO: save and load buildings

class FastBuildMut extends Mutator;

var config Color colorGreen,colorYellow,colorRed;
var config int FastbuildMinutes;
var config int FastbuildRUPerSecond;
var config int FastbuildMaxRU;
// var config int PostbuildMaxRU;

// Config vars but not yet committed to config, because I might rename or remove them, so I don't yet want them written in the .ini file.
var /*config*/ bool bMoreFlashing;

var bool bGameStarted;
var bool bFastbuildOver;
var int SecondsToGo;

defaultproperties {
	colorGreen=(R=0,G=255,B=0,A=0)
	colorYellow=(R=255,G=255,B=0,A=0)
	colorRed=(R=255,G=0,B=0,A=0)
	FastbuildMinutes=3
	// FastbuildRUPerSecond=50
	FastbuildRUPerSecond=30
	FastbuildMaxRU=500 // We could just use SiegeGI.StartingMaxRU, unless we want a *different* setting for Fastbuild + normal Siege.
	// PostbuildMaxRU=1000

	bMoreFlashing=True

	bFastbuildOver=False
	// SecondsToGo=180
}

function PostBeginPlay() {

	Super.PostBeginPlay();

	if (SiegeGI(Level.Game) == None) {
		Log("Gametype " $ Level.Game $ " is not a Siege type!  Destroying FastBuildMut.");
		Self.Destroy();
		return;
	}

	// SiegeGI(Level.Game).GameName = SiegeGI(Level.Game).GameName $ " - AS Mode";
	SiegeGI(Level.Game).GameName = FastbuildMinutes $ " minute FastBuild on " $ SiegeGI(Level.Game).GameName;

	Level.Game.RegisterDamageMutator(Self);

	bGameStarted = False;

	// TODO: remove - not really needed - done in DoGameStart.
	SecondsToGo = 60 * FastbuildMinutes + 1;
	SetTimer(1,True);

}

function ModifyPlayer(Pawn p) {
	// This detects game start, when the first player spawns.
	// But will it work if the tournament mode lets players fight?!  Probably not!
	if (!bGameStarted) {
		DoGameStart();
	}
	Super.ModifyPlayer(p);
}

function DoGameStart() {
	local int i;
	Log("FastBuildMut.DoGameStart()");
	bGameStarted = True;
	bFastbuildOver = False;
	SecondsToGo = 60 * FastbuildMinutes + 1;
	SetStage();
	// TODO: We have not properly detected game start!
	SetTimer(1,True);

		// // SiegeGI(Level.Game).FreeBuild = False;
		// // SiegeGI(Level.Game).RUMax = 1000;
		//// Soddit, start from scratch!
		for (i=0;i<TeamGamePlus(Level.Game).MaxTeams;i++) {
			SiegeGI(Level.Game).MaxRUs[i] = FastbuildMaxRU;
		}
}

function SetStage() { // Does nothing atm!
	local Actor a;
	local int i;

	if (!bGameStarted)
		return;
	}

		/*
		if (InStr(String(Level.Game),"SiegeGI")>=0) {
			Log("Changing FreeBuild from " $ Level.Game.GetPropertyText("FreeBuild") $ " to True");
			// Level.Game.SetPropertyText("FreeBuild","True");
			Level.Game.SetPropertyText("FreeBuild","False");
			Level.Game.SetPropertyText("RUMax","900");
			Level.Game.SetPropertyText("GameName","Siege Assault - " $ (SecondsToGo/60) $ " minutes to prepare");
		}
		*/

		// foreach AllActors(class'Actor', a) {
			/*
			if (InStr(String(a),"sgBaseCore")>=0) {
				Log("Changing MaxEnergy from " $ a.GetPropertyText("MaxEnergy") $ " to 200");
				a.SetPropertyText("MaxEnergy","200.0");
				Log("Changing Energy from " $ a.GetPropertyText("Energy") $ " to 200");
				a.SetPropertyText("Energy","200.0");
			}
			*/
			// if (sgBaseCore(a) != None) {
				// // sgBaseCore(a).MaxEnergy = 200;
				// // sgBaseCore(a).Energy = 200;
				// // sgBaseCore(a).Grade = 5;
			// }
		// }

	if (bFastbuildOver) {
		if (SiegeGI(Level.Game) != None) {
			// // SiegeGI(Level.Game).FreeBuild = False;
			// // SiegeGI(Level.Game).RUMax = SiegeGI(Level.Game).default.RUMax;
			//// Soddit, start from scratch!
			// SiegeGI(Level.Game).MaxRus[0] = PostbuildMaxRU;
			// SiegeGI(Level.Game).MaxRus[1] = PostbuildMaxRU;
		}
	/*
		if (InStr(String(Level.Game),"SiegeGI")>=0) {
			Level.Game.SetPropertyText("FreeBuild","False");
			Level.Game.SetPropertyText("RUMax",""$FastBuildMaxRU);
		}
	*/
	}

}

event Timer() {
	local Actor a;
	local String str;
	if (Level.Game.bGameEnded) {
		return;
	}
	// if (DeathMatchPlus(Level.Game).Countdown > 0) {
		// FlashToAllPlayers("Weapons will not work during the Fastbuild",colorGreen,2);
		// return;
	// }
	if (!bGameStarted) {
		FlashToAllPlayers("Weapons will not work during the Fastbuild",colorGreen,2);
		return;
	}
	if (!bFastbuildOver) {
		if (SecondsToGo <= 0) {
			Log("FastBuildMut.Timer() Fastbuild over");
			// FlashToAllPlayers("Weapons are Live - Assault the Base!",colorRed,4);
			FlashToAllPlayers("Fastbuild Over",colorGreen,2,0,False,True);
			if (bMoreFlashing) {
				FlashToAllPlayers("Weapons are LIVE!",colorRed,3,0,True,False);
				FlashToAllPlayers("Assault the Base!",colorYellow,4,0,True,False);
			}
			bFastbuildOver = True;
			// SecondsToGo = 330;
			SetStage();
			SetTimer(0,False);
			return;
		} else {
			if ((SecondsToGo % 60) == 0) {
				FlashToAllPlayers("You have " $ (SecondsToGo/60) $ " minutes to build your base!",colorGreen,2,0,False,True);
			}
			if (SecondsToGo == 60) {
				FlashToAllPlayers("Weapons go Live in 1 minute...",colorGreen,2,0,False,True);
			}
			if (SecondsToGo == 30) {
				// FlashToAllPlayers("Freebuild ends in 30 seconds...",colorGreen,2);
				FlashToAllPlayers("Weapons go Live in 30 seconds...",colorGreen,2,0,False,True);
			}
			if (SecondsToGo <= 10) {
				FlashToAllPlayers(SecondsToGo $ " ...",colorYellow,2,0,False,True);
			}
		}
		if (!bFastbuildOver) {
			foreach AllActors(class'Actor', a) {
				if (sgPRI(a) != None) {
					sgPRI(a).AddRU(FastbuildRUPerSecond);
				}
			}
		}
		SecondsToGo--;
		SetTimer(1,True);
	}
}

function MutatorTakeDamage( out int ActualDamage, Pawn Victim, Pawn InstigatedBy, out Vector HitLocation, out Vector Momentum, name DamageType) {
	if (!bFastbuildOver) {
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
	if (bBeep)
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

