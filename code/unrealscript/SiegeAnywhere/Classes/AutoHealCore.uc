// TODO: Sometimes when this mutator is present the whole mutator chain breaks down.  I wonder if it is the self-destruct in PostBeginPlay().

class AutoHealCore extends Mutator config (SiegeAnywhere);

var config float EnergyPerSec;
var config float EnergyPerSecPerBuilding;

defaultproperties {
	EnergyPerSec=5
	EnergyPerSecPerBuilding=0.2
}

function PostBeginPlay() {
	local int i;

	Super.PostBeginPlay();

	if (SiegeGI(Level.Game) == None) {
		Log("[AutoHealCore] Gametype " $ Level.Game $ " is not a Siege type!  Destroying Self.");
		// Self.Destroy();
		// Calling Destroy() from here appears to break the mutator chain.  Better just to idle around but not start the Timer.
		// return;
	}

	if (Instr(Level.Game.GameName," with ")>=0)
		Level.Game.GameName = Level.Game.GameName $ " and Auto-Healing Core";
	else
		Level.Game.GameName = Level.Game.GameName $ " with Auto-Healing Core";

	SetTimer(1,True);

}

event Timer() {
	HealCores();
}

function HealCores() {
	local sgBaseCore c;
	local sgBuilding b;
	local int countTeamBuildings;
	local float healAmount;
	// TODO: It would be more efficient to get the base cores from the SiegeGI.
	foreach AllActors(class'sgBaseCore', c) {
		countTeamBuildings = 0;
		foreach AllActors(class'sgBuilding', b) {
			if (b.Team == c.Team)
				countTeamBuildings++;
		}
		healAmount = EnergyPerSec + EnergyPerSecPerBuilding*countTeamBuildings;
		if (c.Energy < c.MaxEnergy) {
			c.Energy += healAmount;
			if (c.Energy > c.MaxEnergy)
				c.Energy = c.MaxEnergy;
		}
	}
}

