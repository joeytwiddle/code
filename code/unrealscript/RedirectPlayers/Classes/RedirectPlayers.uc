// vim: tabstop=2 shiftwidth=2 noexpandtab filetype=uc

class RedirectPlayers expands Mutator config(RedirectPlayers);

var config bool bEnabled;
var config String TargetURL;

defaultproperties {
  bEnabled=True
  TargetURL=unreal://85.131.247.65:6666/
}

function PostBeginPlay() {
	// If we were not added as a mutator, but run in some other way (e.g. as a ServerActor), then we need to register as a mutator:
	// Level.Game.BaseMutator.AddMutator(Self);
	if (bEnabled)
		Log("RedirectPlayers will send players to: "$ TargetURL);
	Super.PostBeginPlay();
}

function ModifyPlayer(Pawn p) {
	if (bEnabled && PlayerPawn(p) != None) {
		if (!PlayerPawn(p).bAdmin) {
			PlayerPawn(p).PreClientTravel();
			PlayerPawn(p).ClientTravel(TargetURL, TRAVEL_Absolute, False);
		}
	}
	Super.ModifyPlayer(p);
}

// TODO: mutate help, which should include: Type "admin set RedirectPlayers.RedirectPlayers TargetURL <other_server_address>" to change the redirect location.

