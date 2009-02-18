// This mutator reduces the RU gained from leeching sgBuildings by up to 90%, for buildings situated a long way away from their core.
// In other words, leeching is best near the enemy core.  Destroying enemy buildings which are built far from their core might be a good idea, but it won't make you rich.

// TEST: To compensate for enemy buildings near your base giving less RU, make them easier to destroy.
//       But currently haven't we made them harder to destroy?!  I think they should be as easy to destroy as normal, but give less RU!
//       Oh we have done that.  We add back the "lost" damage but without an InstigatedBy to receive RU.

class DiscourageLeeching extends Mutator config (SiegeAnywhere);

var Actor TeamOrigin[4];
var float TeamOriginDistance;
var bool bEnabled;

function PostBeginPlay() {
	Super.PostBeginPlay();
	FindTeamOrigins();
}

function FindTeamOrigins() {
	local FlagBase f;
	local sgBaseCore c;
	foreach AllActors(class'FlagBase',f) {
		TeamOrigin[ f.Team ] = f;
	}
	foreach AllActors(class'sgBaseCore',c) {
		TeamOrigin[ c.Team ] = c;
	}
	if (TeamOrigin[0] == None || TeamOrigin[1] == None) {
		Log("DiscourageLeeching.FindTeamOrigins(): FAILED to find two TeamOrigins - disabling self.");
		bEnabled = False;
	} else {
		bEnabled = True;
		TeamOriginDistance = VSize(TeamOrigin[0].Location - TeamOrigin[1].Location);
	}
}

function MutatorTakeDamage( out int ActualDamage, Pawn Victim, Pawn InstigatedBy, out Vector HitLocation, out Vector Momentum, name DamageType) {
	local float distance,percentageLoss;
	if (bEnabled) {
		if (InstigatedBy!=None && sgBuilding(Victim)!=None && sgBaseCore(Victim)==None) {
			distance = VSize(Victim.Location - (TeamOrigin[sgBuilding(Victim).Team]).Location);
			percentageLoss = 0.9 * distance / TeamOriginDistance;
			if (percentageLoss>0.9) percentageLoss=0.9;
			if (percentageLoss<0.0) percentageLoss=0.0;
			// We still want to cause the same amount of damage
			// So we cause the "lost" damage to happen with instigator None.
			// TODO CONSIDER: This might cause other mods to throw Accessed None.
			Victim.TakeDamage(ActualDamage*percentageLoss,None,instigatedBy.Location,HitLocation, DamageType);
			// And we cause the remaining damage to happen as it was.
			ActualDamage = ActualDamage * (1.0 - percentageLoss);
		}
	}
	Super.MutatorTakeDamage(ActualDamage,Victim,InstigatedBy,HitLocation,Momentum,DamageType);
}

