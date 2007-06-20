class CrouchBlocksDamage extends Mutator;

var bool Initialized;

var float SavePercent;
var float SuperBoost;
var bool bInformVictim;

defaultproperties {
SavePercent=50
SuperBoost=2
bInformVictim=True
}

function PostBeginPlay() {
	if (Initialized)
		return;
	Initialized = True;

	Level.Game.RegisterDamageMutator( Self );

	// TODO: really we want this to be displayed in the user's chat area when they join the server (like ZPPure does)
	Log("CrouchBlocksDamage mutator is running.  Crouch when hit to reduce damage by "$SavePercent$"%!");
	BroadcastMessage("CrouchBlocksDamage mutator is running.  Crouch when hit to reduce damage by "$SavePercent$"%!");
	if (SuperBoost != 1.0) {
		Log("SuperBoost at "$SuperBoost$"!");
		BroadcastMessage("SuperBoost at "$SuperBoost$"!");
	}
}

function MutatorTakeDamage( out int ActualDamage, Pawn Victim, Pawn InstigatedBy, out Vector HitLocation, out Vector Momentum, name DamageType) {

	/* Originally from Vampire
	if (InstigatedBy.IsA('Bot') || InstigatedBy.IsA('PlayerPawn'))
	{
		InstigatedBy.Health += ActualDamage;
		if (InstigatedBy.Health > 199)
			InstigatedBy.Health = 199;
	}
	*/

	// SuperBoost:
	Momentum = Momentum * SuperBoost;

	if (Victim.IsA('PlayerPawn') && PlayerPawn(Victim).bIsCrouching) {

		if (bInformVictim) {
			// Victim.ClientMessage("Crouching has saved you "$Int(SavePercent)$"% ("$ Int(ActualDamage*SavePercent) $"hp) damage from "$ InstigatedBy.getHumanName() $"'s \""$ DamageType $"\".", 'CriticalEvent', True);
			// Victim.ClientMessage("Crouching has saved you "$Int(SavePercent)$"% ("$ Int(ActualDamage*SavePercent) $"hp) damage from "$ InstigatedBy.getHumanName() $"'s \""$ DamageType $"\".");
			// Victim.ClientMessage(Int(ActualDamage*SavePercent) $"hp) damage saved by crouching.");
			// Victim.ClientMessage(Int(ActualDamage*SavePercent) $"hp damage saved by crouching from "$DamageType);
			Victim.ClientMessage("Crouching blocked "$Int(ActualDamage*SavePercent) $" hp of "$DamageType$" damage.");
		}
		ActualDamage = ActualDamage*(1.0-SavePercent/100.0);

		// SuperBoost is further increased when crouching:
		Momentum = Momentum * SuperBoost;

		// Crouch blocking is further increased when damaging self:
		if (InstigatedBy == Victim) {
			ActualDamage = ActualDamage*(1.0-SavePercent/100.0);
		}

	}

	if ( NextDamageMutator != None )
		NextDamageMutator.MutatorTakeDamage( ActualDamage, Victim, InstigatedBy, HitLocation, Momentum, DamageType );

}

