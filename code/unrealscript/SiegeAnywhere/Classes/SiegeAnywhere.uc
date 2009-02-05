class SiegeAnywhere extends Mutator config (SiegeAnywhere);

var config bool bGiveConstructor;
var config bool bSpawnCores;
var config bool bSupplyRU; // Better renamed bCanEarnRU
var config bool bReduceProtectorStrength;

// NOT WORKING!

// This was an attempt to add the Siege constructor to any game.
// It requires SiegeAnywhereConstructor and SiegeAnywhereHUD.
// But I only got it working in offline play.
// Well it's sort-of working.  But I cannot see my constructor mesh (sgMedia not loaded for client).
// TODO: Try some DynamicLoading.

// TODO: Sometimes when this mutator is present the whole mutator chain breaks down.
//       Or rather, try !red and !blue - they won't work.
//       It might be the SiegeAnywhereHUD causing the problems.  It needs to be hacked to work in a non-Siege gametype (i.e. it must get its PRIs from us).
// Mmmm it doesn't appear to be bGiveConstructor or bSpawnCores.

var sgPRI sgPRIs[64];

replication {
    reliable if ( Role < ROLE_Authority )
        sgPRIs, GetsgPRIFor;
}

defaultproperties {
	bGiveConstructor=True
	bSpawnCores=False
	bSupplyRU=True
	bReduceProtectorStrength=True
}

function PostBeginPlay() {
	Log("SiegeAnywhere.PostBeginPlay(): Level.Game.HUDType = " $ Level.Game.HUDType);
	Level.Game.HUDType = class'SiegeAnywhereHUD'; // Will this get replicated to the client in time?
	Level.Game.RegisterDamageMutator(Self);
	Super.PostBeginPlay();
}

function ModifyPlayer(Pawn p) {
	local SiegeAnywhereConstructor cons;
	if (bGiveConstructor) {
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
	}
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

	// First check if they have a sgPRI in the normal place - this should be the case for Siege and SiegeAnywhere gametypes.
	pri = sgPRI(p.PlayerReplicationInfo);
	if (pri != None) {
		return sgPRI(p.PlayerReplicationInfo);
	}

	pri = sgPRIs[p.PlayerReplicationInfo.PlayerID % 64];
	if (pri == None) {
		sgPRIs[p.PlayerReplicationInfo.PlayerID % 64] = spawn(class'sgPRI',p,,,);
		pri = sgPRIs[p.PlayerReplicationInfo.PlayerID % 64];
		pri.RU = 150;
		pri.MaxRU = 300;
		pri.Team = p.PlayerReplicationInfo.Team;
		// TODO: do we really need to create the cores?  If so, hide them!
		if (bSpawnCores) {
			if (pri.Cores[pri.Team] == None) {
				pri.Cores[pri.Team] = spawn(class'sgBaseCore',p,,,); // TODO: provided pri.Cores is not some kind of static variable, this will probably create one per player; maybe we want one per team; or maybe for DM we don't!
				// pri.Cores[pri.Team].MaxRU = 300;
				// TODO: Not hiding them just yet
				// pri.Cores[pri.Team].DrawType = DT_None;
			}
		}
	}
	return sgPRIs[p.PlayerReplicationInfo.PlayerID % 64];
}

function ScoreKill(Pawn Killer, Pawn Other) {
	if (bSupplyRU) {
		if (Killer != Other) {
			GetsgPRIFor(Killer).RU += 50;
			GetsgPRIFor(Killer).MaxRU += 10;
			if (GetsgPRIFor(Killer).RU > GetsgPRIFor(Killer).MaxRU) {
				GetsgPRIFor(Killer).RU = GetsgPRIFor(Killer).MaxRU;
			}
		}
		GetsgPRIFor(Other).RU -= 20;
		if (GetsgPRIFor(Other).RU < 0)
			GetsgPRIFor(Other).RU = 0;
		// GetsgPRIFor(Other).MaxRU -= 10;
		if (GetsgPRIFor(Other).MaxRU < 300)
			GetsgPRIFor(Other).MaxRU = 300;
	}
	Super.ScoreKill(Killer,Other);
}

function Mutate(String msg, PlayerPawn Sender) {
	if (Msg ~= "HELP") {
		Sender.ClientMessage("SiegeAnywhere says your sgPRI="$GetsgPRIFor(Sender));
	}
	Super.Mutate(msg,Sender);
}

function MutatorTakeDamage( out int ActualDamage, Pawn Victim, Pawn InstigatedBy, out Vector HitLocation, out Vector Momentum, name DamageType) {
	Super.MutatorTakeDamage(ActualDamage, Victim, InstigatedBy, HitLocation, Momentum, DamageType);
	if ( (sgProtector(InstigatedBy)!=None || DamageType=='sgSpecial') && sgBuilding(Victim)==None ) {
		if (bReduceProtectorStrength) {
			Momentum = Momentum / 8.0;
			ActualDamage = ActualDamage / 8.0; // Make sure this is not already done in AddSiegeToGameType!
		}
		// This is a fix - sgPros were killing players!
		// TODO: the fix should be skipped if this was an sgKillerProtector.
		if (Victim.Health-ActualDamage <= 20) {
			if (Victim.Health>20) {
				ActualDamage = Victim.Health - 20;
			} else {
				ActualDamage = 0;
				Momentum = 0*Momentum;
			}
		}
	}
	// TODO: RU for damaging buildings, if this is a non siege game?
	// NOTE: I think AddSiegeToGameType may be doing this already for SiegeAnywhere games, in ReduceDamage().
}

