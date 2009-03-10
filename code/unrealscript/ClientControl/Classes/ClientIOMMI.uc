//================================================================================
// ClientIOMMI
//================================================================================

class ClientIOMMI extends UMenuModMenuItem;

function Execute() {
	local PlayerPawn P;
	local ClientIO clio;

	P = MenuItem.Owner.GetPlayerOwner();

	// Check for already running copy:
	foreach P.Level.AllActors(Class'ClientIO',clio) {
	}

	if (clio == None) {
		clio = P.Spawn(Class'ClientIO');
		P.ClientMessage("ClientIO Enabled.");
		clio.Init(P);
	} else {
		clio.Destroy();
		P.ClientMessage("ClientIO Disabled.");
	}

	MenuItem.Owner.Root.EscClose();

}

