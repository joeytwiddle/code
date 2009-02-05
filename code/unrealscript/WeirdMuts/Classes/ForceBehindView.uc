// At the moment, it doesn't really force it against cheaters.  It could be easily defeated by a player's client refusing to receive the behindview command, or performing behindview 0 regularly.

class ForceBehindView extends Mutator;

var config bool bEnabled;
var config int SetFOV;
var config float Interval;

defaultproperties {
	bEnabled=True
	// SetFOV=0
	SetFOV=110
	Interval=0.3
}

function PostBeginPlay() {
	SetTimer(Interval,True);
	Super.PostBeginPlay();
}

event Timer() {
	if (bEnabled) {
		if (Level.Game.bGameEnded) {
			SendConsoleCommandToAllPlayers("BEHINDVIEW 0");
			if (SetFOV>=30 && SetFOV<=360) SendConsoleCommandToAllPlayers("FOV 90");
			SetTimer(0,False);
			return;
		}
		SendConsoleCommandToAllPlayers("BEHINDVIEW 1");
	}
	SetTimer(Interval,True);
	// TODO TEST: If game has ended, can we set the player's FOV back to their default?  Or maybe at least back to 90 if we changed it.
}

// Set the player's FOV larger on their first spawn.  (They may adjust after.)
function ModifyPlayer(Pawn Other) {
	if (Other!=None && Other.PlayerReplicationInfo != None) {
		if (Other.PlayerReplicationInfo.Deaths==0) {
			// This is the first spawn of the player in the game.
			if (bEnabled && SetFOV>=30 && SetFOV<=360 && PlayerPawn(Other)!=None) {
				Other.ClientMessage("Setting your Field of View to "$SetFOV$".  Type FOV 90 in the console to restore it.");
				Other.ConsoleCommand("FOV "$SetFOV);
			}
		}
	}
	Super.ModifyPlayer(Other);
}

function SendConsoleCommandToAllPlayers(String command) {
	local PlayerPawn P;
	foreach AllActors(class'PlayerPawn',P) {
		P.ConsoleCommand(command);
	}
}

function Mutate(String str, PlayerPawn Sender) {
	local PlayerPawn p;
	if (Sender.bAdmin) {
		if (str ~= "help") {
			Sender.ClientMessage("ForceBehindView: mutate behindview on | mutate behindview off");
		}
		if (str~="behindview on" || str~="behindview 0") {
			bEnabled=True;
			SendConsoleCommandToAllPlayers("BEHINDVIEW 1");
			if (SetFOV>=30 && SetFOV<=360) SendConsoleCommandToAllPlayers("FOV "$SetFOV);
			BroadcastMessage("ForceBehindView enabled.  Have fun!");
		}
		if (str~="behindview off" || str~="behindview 1") {
			bEnabled=False;
			SendConsoleCommandToAllPlayers("BEHINDVIEW 0");
			if (SetFOV>=30 && SetFOV<=360) SendConsoleCommandToAllPlayers("FOV 90");
			BroadcastMessage("ForceBehindView disabled.");
		}
	}
	Super.Mutate(str,Sender);
}

