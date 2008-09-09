//================================================================================
// ClientConsoleLoggerMMI
//================================================================================

class ClientConsoleLoggerMMI extends UMenuModMenuItem;

function Execute() {
	local PlayerPawn P;
	local ClientConsoleLogger logger;

	P = MenuItem.Owner.GetPlayerOwner();

	// Check for already running copy:
	foreach P.Level.AllActors(Class'ClientConsoleLogger',logger) {
	}

	if (logger == None) {
		logger = P.Spawn(Class'ClientConsoleLogger');
		logger.Init(P);
		P.ClientMessage("ClientConsoleLogger turned on");
	} else {
		P.ClientMessage("ClientConsoleLogger turned off");
	}

	MenuItem.Owner.Root.EscClose();

}

