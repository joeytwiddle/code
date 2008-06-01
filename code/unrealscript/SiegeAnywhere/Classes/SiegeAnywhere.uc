class SiegeAnywhere extends Mutator;

// NOT WORKING!

// This was an attempt to add the Siege constructor to any game.
// It requires SiegeAnywhereConstructor and SiegeAnywhereHUD.
// But I only got it working in offline play.
// Well it's sort-of working.  But I cannot see my constructor mesh (sgMedia not loaded for client).

var sgPRI sgPRIs[64];

replication {
    reliable if ( Role < ROLE_Authority )
        sgPRIs, GetsgPRIFor;
}

function PostBeginPlay() {
	Log("SiegeAnywhere.PostBeginPlay(): Level.Game.HUDType = " $ Level.Game.HUDType);
	Level.Game.HUDType = class'SiegeAnywhereHUD';
	Super.PostBeginPlay();
}

function ModifyPlayer(Pawn p) {
	local SiegeAnywhereConstructor cons;
	// cons = sgConstructor(p.FindInventoryType(class'sgConstructor'));
	cons = SiegeAnywhereConstructor(p.FindInventoryType(class'SiegeAnywhereConstructor'));
	// Log("SiegeAnywhere.ModifyPlayer(" $ p.getHumanName() $ ") cons = " $ cons);
	if (cons == None) {
		cons = Spawn(class'SiegeAnywhereConstructor',p,,,rot(0,0,0));
		// cons.SiegeAnywhere = Self;
		Log("SiegeAnywhere.ModifyPlayer(" $ p.getHumanName() $ ") Adding cons = " $ cons);
		cons.GiveTo(p);
		// p.AddInventory(cons);
		if (PlayerPawn(p) != None) {
			if (SiegeAnywhereHUD(PlayerPawn(p).myHUD) != None) {
				// SiegeAnywhereHUD(PlayerPawn(p).myHUD).SiegeAnywhere = Self;
			}
		}
	}
	// if (sgPRI(p.PlayerReplicationInfo) == None) {
		// Log("SiegeAnywhere.ModifyPlayer(" $ p.getHumanName() $ ") Had to change PRI from " $ p.PlayerReplicationInfo.class $ " to " $ class'sgPRI');
		// p.PlayerReplicationInfo = spawn(class'sgPRI',p,,,);
	// }
	Super.ModifyPlayer(p);
}

/*
function ModifyLogin(out class<playerpawn> SpawnClass, out string Portal, out string Options) {
	local class<PlayerReplicationInfo> priClass;
	// Ugly hack to spawn the correct type of PRI
	priClass = spawnClass.default.PlayerReplicationInfoClass;
	spawnClass.default.PlayerReplicationInfoClass = class'SiegeXXL2dRC1.sgPRI';
	Super.ModifyLogin(SpawnClass, Portal, Options);
	spawnClass.default.PlayerReplicationInfoClass = priClass;
}
*/

simulated function sgPRI GetsgPRIFor(Actor a) {
	local Pawn p;
	local sgPRI pri;
	if (Pawn(a) == None) {
		return None;
	}
	p = Pawn(a);
	if (p.PlayerReplicationInfo == None) {
		if (p.IsA('PlayerPawn')) {
			Log("SiegeAnywhere.GetsgPRIFor("$p.getHumanName()$") - has no PRI!");
		}
		return None;
	}
	pri = sgPRIs[p.PlayerReplicationInfo.PlayerID % 64];
	if (pri == None) {
		sgPRIs[p.PlayerReplicationInfo.PlayerID % 64] = spawn(class'sgPRI',p,,,);
		pri = sgPRIs[p.PlayerReplicationInfo.PlayerID % 64];
		pri.RU = 150;
		pri.MaxRU = 300;
		pri.Team = p.PlayerReplicationInfo.Team;
		// TODO: do we really need to create the cores?  If so, hide them!
		if (pri.Cores[pri.Team] == None) {
			pri.Cores[pri.Team] = spawn(class'sgBaseCore',p,,,); // TODO: provided pri.Cores is not some kind of static variable, this will probably create one per player; maybe we want one per team; or maybe for DM we don't!
			// pri.Cores[pri.Team].MaxRU = 300;
		}
	}
	return sgPRIs[p.PlayerReplicationInfo.PlayerID % 64];
}

function ScoreKill(Pawn Killer, Pawn Other) {
	if (Killer != Other) {
		GetsgPRIFor(Killer).RU += 50;
		GetsgPRIFor(Killer).MaxRU += 10;
		if (GetsgPRIFor(Killer).RU > GetsgPRIFor(Killer).MaxRU) {
			GetsgPRIFor(Killer).RU = GetsgPRIFor(Killer).MaxRU;
		}
	}
	GetsgPRIFor(Other).RU -= 20;
	GetsgPRIFor(Other).MaxRU -= 10;
	if (GetsgPRIFor(Other).RU < 0)
		GetsgPRIFor(Other).RU = 0;
	if (GetsgPRIFor(Other).MaxRU < 300)
		GetsgPRIFor(Other).MaxRU = 300;
	Super.ScoreKill(Killer,Other);
}

