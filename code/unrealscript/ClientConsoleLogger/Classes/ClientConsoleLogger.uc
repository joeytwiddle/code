//================================================================================
// ClientConsoleLogger
//================================================================================

// TODO: When logger starts up, would be nice to log the date+time and the current map and the server.
// TODO: If we wanna be really nice, we could offer %m=message %n=sender_name %t=time %w=mapname %s=servername %d=date etc., and let the user decide the output format.

class ClientConsoleLogger expands Actor config (ClientConsoleLogger);

var config bool bLogChatOnly;
var config bool bIgnoreBots;

var PlayerPawn OurPlayer;

var String LastConsoleLine;

defaultproperties {
bLogChatOnly=true
bIgnoreBots=true
}

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
	local String pStr;
	local Console cons;
	local PlayerReplicationInfo p;
	if (OurPlayer!=None && OurPlayer.Player!=None) {
		cons = OurPlayer.Player.Console;
		line = cons.MsgText[cons.TopLine];
		if (line != LastConsoleLine) { // TODO: or player!=lastplayer - or even better skip both of them and just check: TopLine!=LastLine
			// New console line!
			p = cons.GetMsgPlayer(cons.TopLine);
			if ( (p==None && bLogChatOnly) || (p!=None && p.bIsABot && bIgnoreBots)  ) {
				// Do not log this line.
			} else {
				pStr = "* ";
				if (p != None) pStr = "<"$ (p.PlayerName) $"> ";
				Log("[Console] ("$Int(Level.TimeSeconds)$") "$ pStr $ line);
			}
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

