//================================================================================
// ClientConsoleLogger
//================================================================================

class ClientConsoleLogger expands Actor;

var PlayerPawn OurPlayer;

var String LastConsoleLine;

function PostBeginPlay() { // In case spawned rather than selected from menu
	Super.PostBeginPlay();
	// This allows us to "spawn ClientConsoleLogger" in a local game:
	if (OurPlayer==None && PlayerPawn(Owner)!=None) {
		Log("ClientConsoleLogger.PostBeginPlay(): setting up for "$ Owner.getHumanName());
		Init(PlayerPawn(Owner));
	}
}

// This is the initialisation function
function Init(PlayerPawn o) {
	OurPlayer = o;
	Log("[ClientConsoleLogger] Started now.");
	Enable('Tick');
}

function bool WeAreOnline() {
	return False;
}

function CheckConsole() {
	local String line;
	if (OurPlayer!=None && OurPlayer.Player!=None) {
		line = OurPlayer.Player.Console.MsgText[OurPlayer.Player.Console.TopLine];
		if (line != LastConsoleLine) {
			// New console line!
			Log("[Console] ("$Int(Level.TimeSeconds)$") "$line);
			LastConsoleLine = line;
		}
	}
}

event Tick (float Delta) {
	CheckConsole();
	Super.Tick(Delta);
}

// TODO: event?!  and start it?!?!
function Timer () {
}

