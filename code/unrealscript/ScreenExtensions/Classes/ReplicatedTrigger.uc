class ReplicatedTrigger extends Triggers;

defaultproperties {
    // One of these 4 booleans is essential!
    bAlwaysRelevant=true
    bNoDelete=True
    bStasis=True
    bStatic=False
    NetPriority=3.0
    NetUpdateFrequency=10
    RemoteRole=ROLE_SimulatedProxy
}

replication {
    reliable if ( Role == ROLE_Authority )
        CurrentUpdate;
}

var int CurrentUpdate;

var int LastUpdate;

function Trigger(Actor Other, Pawn EventInstigator) {

	// Inform the client that we were triggered:
	Log(Self$" ["$Role$"]: Triggering Update #"$CurrentUpdate$"");
	CurrentUpdate++;

}

simulated function PostBeginPlay() {
	Enable('Tick');
}

// If there was an update, get the client to call it's client-side code.
simulated function Tick(float TimeDelta) {
	Super.Tick( TimeDelta );
	if (CurrentUpdate > LastUpdate) {
		// Call the trigger code client-side:
		ReplicatedTrigger();
		LastUpdate = CurrentUpdate;
		// The ReplicatedTrigger() function should decide whether or not to act when Role < or Role == ROLE_Authority.
		// For my issues with ScreenSlide, I wanted it to only act when < (client-side), if it acted on both sides, then the eventually replicated SlideSwitch would confuse the client+server, and the slides would get out of sync
		// But by doing it when only <, the same code would not work in single-player mode.
	}
}

simulated function ReplicatedTrigger() {
	Log(Self$".ReplicatedTrigger(): You should have overwritten this method; I'm doing nothing.");
}

