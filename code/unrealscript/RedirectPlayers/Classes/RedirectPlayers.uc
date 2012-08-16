// vim: tabstop=2 shiftwidth=2 noexpandtab filetype=uc

// BUG TODO: If a player joins and leaves before SecondsBeforeRedirection, they will not get redirected, and NextPlayerToDo may get stuck on that players PlayerID, and will fail to redirect any more players.
// TODO CONSIDER: If the redirection fails (e.g. because the map on the target server switches before they join) then redirection should ideally be retried.  But we don't want to retry every say 30 seconds, in case the map download takes 40 seconds.  Can we actually tell if the player is in the process of connecting to another server?

class RedirectPlayers expands Mutator config(RedirectPlayers);

var config bool bEnabled;
var config String TargetURL;
var config float SecondsBeforeRedirection;
var config bool bTellPlayers;
var config bool bLog;

// We must not redirect the same player twice, as this will cause his download
// to restart if he is downloading a big file from the target server.
var int NextPlayerToDo;
// However this approach sucks.  If a player joins and leaves fast, it might
// leave a gap that NextPlayerToDo will never cross.

defaultproperties {
	bEnabled=True
	TargetURL="unreal://85.131.247.65:6666/"
	SecondsBeforeRedirection=5
	bTellPlayers=True
	bLog=True
}

function PostBeginPlay() {
	// If we were not added as a mutator, but run in some other way (e.g. as a ServerActor), then we need to register as a mutator:
	// Level.Game.BaseMutator.AddMutator(Self);
	if (bEnabled) {
		Log("[RedirectPlayers] Will send players to: "$ TargetURL);
		// SetTimer(10,True); // BUG DONE: When I had SetTimer(1) my UT crashed.  But there's a 1 in 10 chance of hitting the timer right when you join, so this may still happen to some players.  We should check their time-in-game.
		SetTimer(1,True);
		// BUG DONE: Also this will call repeatedly and if a player has to download packages, it may restart their download every 10 seconds.  =/
		NextPlayerToDo=0;
	}
	Super.PostBeginPlay();
}

function CheckRedirect(Pawn p) {
	// We must check bots too, or NextPlayerToDo will never increase.
	if (bEnabled) {
		// if (UTServerAdminSpectator(p)!=None) // he has id 0 although first player has id 0 also!
		if (InStr(""$p.Class,"UTServerAdminSpectator")>=0 || InStr(""$p.Class,"MvReporterSpectator")>=0) {
			if (bLog)
				Log("[RedirectPlayers] Skipping player "$p$" because class="$p.Class);
			NextPlayerToDo++;
			return;
		}
		if (p.PlayerReplicationInfo!=None && p.PlayerReplicationInfo.PlayerID==NextPlayerToDo && (Level.TimeSeconds - p.PlayerReplicationInfo.StartTime)>SecondsBeforeRedirection) {
			if (PlayerPawn(p) != None) {
				if (!PlayerPawn(p).bAdmin) {
					if (bTellPlayers)
						PlayerPawn(p).ClientMessage("You are being redirected to "$ TargetURL);
					if (bLog)
						Log("[RedirectPlayers] Redirecting "$ p.getHumanName() $" to "$ TargetURL $" class="$p.Class);
					PlayerPawn(p).PreClientTravel();
					PlayerPawn(p).ClientTravel(TargetURL, TRAVEL_Absolute, False);
					// Consider: Just curious, can we also do this with p.ConsoleCommand("open "$ TargetURL); ?
				}
			}
			Log("[RedirectPlayers] Dealt with ["$NextPlayerToDo$"] "$ p.getHumanName());
			NextPlayerToDo++;
		}
	}
}

event Timer() {
	local Pawn p;
	foreach AllActors(class'Pawn', p) {
		CheckRedirect(p);
	}
	Super.Timer();
}

/* Useless
// This doesn't get called until first spawn:
// But maybe that is better.
// 1) It prevents the crashing if Timer is called immediately after they join.
// 2) It won't call repeatedly, each time they die, if they are still downloading from the remote server.
function ModifyPlayer(Pawn p) {
	if (PlayerPawn(p)!=None && p.PlayerReplicationInfo.Deaths > 0) { // If we check for ==0 deaths (first spawn), then players joining a running game spawn immediately, are redirect immediately, and their UT crashes.  With >0, players are redirected the first time they die in play, so only l33t players get to stay on the server! ;)
		CheckRedirect(PlayerPawn(p));
	}
	Log("[RedirectPlayers] "$p.getHumanName()$" is ["$p.PlayerReplicationInfo.PlayerID$"]");
	Super.ModifyPlayer(p);
}
*/

// TODO: mutate help, which should include: Type "admin set RedirectPlayers.RedirectPlayers TargetURL <other_server_address>" to change the redirect location.

