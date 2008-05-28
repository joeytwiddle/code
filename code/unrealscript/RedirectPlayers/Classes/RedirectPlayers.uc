// vim: tabstop=2 shiftwidth=2 noexpandtab filetype=uc

class RedirectPlayers expands Mutator config(RedirectPlayers);

var config bool bEnabled;
var config String TargetURL;
var config bool bTellPlayers;
var config bool bLog;

defaultproperties {
	bEnabled=True
	TargetURL="unreal://85.131.247.65:6666/"
	bTellPlayers=True;
	bLog=True
}

function PostBeginPlay() {
	// If we were not added as a mutator, but run in some other way (e.g. as a ServerActor), then we need to register as a mutator:
	// Level.Game.BaseMutator.AddMutator(Self);
	if (bEnabled) {
		Log("RedirectPlayers will send players to: "$ TargetURL);
		SetTimer(10,True); // TODO BUG: When I had SetTimer(1) my UT crashed.  But there's a 1 in 10 chance of hitting the timer right when you join, so this may still happen to some players.  We should check their time-in-game.
	}
	Super.PostBeginPlay();
}

// This doesn't get called until first spawn:
// function ModifyPlayer(Pawn p) {
// }

function CheckRedirect(PlayerPawn p) {
	if (bEnabled) {
		if (!p.bAdmin) {
			if (bTellPlayers)
				p.ClientMessage("You are being redirected to "$ TargetURL);
			if (bLog)
				Log("Redirecting "$ p.getHumanName() $" to "$ TargetURL);
			p.PreClientTravel();
			p.ClientTravel(TargetURL, TRAVEL_Absolute, False);
			// Consider: Just curious, can we also do this with p.ConsoleCommand("open "$ TargetURL); ?
		}
	}
}

event Timer() {
	local PlayerPawn p;
	foreach AllActors(class'PlayerPawn', p) {
		if (UTServerAdminSpectator(p) != None)
			continue;
		CheckRedirect(p);
	Super.Timer();
}

// TODO: mutate help, which should include: Type "admin set RedirectPlayers.RedirectPlayers TargetURL <other_server_address>" to change the redirect location.

