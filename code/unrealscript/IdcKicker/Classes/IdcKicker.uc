// vim: tabstop=2 shiftwidth=2 noexpandtab filetype=uc

// TESTING: We can now leave optional comments/reason in the config files after each player's IDC number.
// TODO: Add mutate ban <adminpass> <player_name> [<reason>]...   which would add the player's IDC.

class IdcKicker expands Mutator config(IdcKicker);

var config bool bEnabled;
var config String IdcToKick[256];
var config String IdcToBan[256];

defaultproperties {
  bEnabled=True
}

function PostBeginPlay() {
	// If we were not added as a mutator, but run in some other way (e.g. as a ServerActor), then we need to register as a mutator:
	// Level.Game.BaseMutator.AddMutator(Self);
	Super.PostBeginPlay();
}

function String FirstArg(String s) {
	local int j;
	j = InStr(s," ");
	if (j>=0) return Left(s,j);
	else return s;
}

function Mutate(String str, PlayerPawn Sender) {
	local string idc,criminal;
	local int i;

	if (str ~= "IDCKICKER DISABLE" && Sender.bAdmin) {
		bEnabled=False;
		Sender.ClientMessage("IdcKicker disabled.");
		SaveConfig();
	}

	if (str ~= "IDCKICKER ENABLE" && Sender.bAdmin) {
		bEnabled=True;
		Sender.ClientMessage("IdcKicker enabled.");
		SaveConfig();
	}

	// Check for banned players when their IDC is broadcast
	if (bEnabled && Left(str,4) ~= "IDC ") {

		idc = Mid(str,4);

		for (i=0;i<256;i++) {
			criminal = FirstArg(IdcToKick[i]);
			if (criminal!="" && criminal~=idc) {
				Log("IdcKicker: "$ Sender.getHumanName() $" has IDC "$ idc $" - KICKING him/her.");
				DoKick(Sender);
			}
			criminal = FirstArg(IdcToBan[i]);
			if (criminal!="" && criminal~=idc) {
				Log("IdcKicker: "$ Sender.getHumanName() $" has IDC "$ idc $" - BANNING him/her.");
				DoBan(Sender);
			}
		}

	}

	Super.Mutate(str, Sender);
}

function DoKick(PlayerPawn Victim) {
	Victim.Destroy();
}

function DoBan(PlayerPawn Victim) {
	local String IP;
	local int j;
	IP = Victim.GetPlayerNetworkAddress();
	if (Level.Game.CheckIPPolicy(IP)) {
		IP = Left(IP, InStr(IP, ":"));

		//// Find an empty record:
		// for (j=0;j<50;j++)
			// if (Level.Game.IPPolicies[j] == "")
				// break;
		// if (j < 50) {
			// Level.Game.IPPolicies[j] = "DENY,"$IP;
			// Log("IdcKicker: Added IP Ban for "$ Victim.getHumanName() $" ("$ IP $") at slot "$ j);
		// } else {
			// Log("IdcKicker: Could not add IP ban for "$ Victim.getHumanName() $" ("$ IP $") - IPPolicies[] is full!");
		// }

		//// Choose a random record:
		// TODO: what if list is not yet full, is it ok to leave earlier IPPolicies empty?
		j = FRand()*50;
		Level.Game.IPPolicies[j] = "DENY,"$IP;

		Level.Game.SaveConfig();
	}
	Victim.Destroy();
}

