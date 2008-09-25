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
		P.ClientMessage("ClientConsoleLogger Enabled.");
		logger.Init(P);
	} else {
		logger.Destroy();
		P.ClientMessage("ClientConsoleLogger Disabled.");
	}

	MenuItem.Owner.Root.EscClose();

}

