// CrouchBlocksDamage
// Reduces damage by SavePercent if the player is crouching when hit.
// Will also multiply the boost by BoostMultiplier if they are crouching.

class CrouchBlocksDamage extends Mutator;

var config float SavePercent;
var config float BoostMultiplier;
var config bool bInformVictim;

var bool Initialized;

defaultproperties {
  SavePercent=50 // How much damage is saved by crouching when hit
  BoostMultiplier=0.5 // The boost from hits received when crouching can be reduced
  bInformVictim=False
}

function PostBeginPlay() {
	if (Initialized)
		return;
	Initialized = True;

	Level.Game.RegisterDamageMutator( Self );

	// DONE: really we want this to be displayed in the user's chat area when they join the server (like ZPPure does)
	Log("CrouchBlocksDamage mutator is running.  Crouch when hit to reduce damage by "$SavePercent$"%!");
	BroadcastMessage("CrouchBlocksDamage mutator is running.  Crouch when hit to reduce damage by "$SavePercent$"%!");
	Super.PostBeginPlay();
}

function ModifyPlayer(Pawn Other) {
	if (Other.IsA('PlayerPawn') && Other.PlayerReplicationInfo.Deaths==0) {
		PlayerPawn(Other).ClientMessage("Crouching will block damage by "$SavePercent$"%");
	}
	// Super.ModifyPlayer(Other);
	if (NextMutator != None) {
		NextMutator.ModifyPlayer(Other);
	}
}

function MutatorTakeDamage( out int ActualDamage, Pawn Victim, Pawn InstigatedBy, out Vector HitLocation, out Vector Momentum, name DamageType) {

	// SuperBoost:
	// Momentum = Momentum * SuperBoost;

	if (Victim.IsA('PlayerPawn') && PlayerPawn(Victim).bIsCrouching) {

		if (bInformVictim && Victim.bIsHuman && !Victim.IsA('Bot')) {
			// Victim.ClientMessage("Crouching has saved you "$Int(SavePercent)$"% ("$ Int(ActualDamage*SavePercent/100) $"hp) damage from "$ InstigatedBy.getHumanName() $"'s \""$ DamageType $"\".", 'CriticalEvent', True);
			// Victim.ClientMessage("Crouching has saved you "$Int(SavePercent)$"% ("$ Int(ActualDamage*SavePercent/100) $"hp) damage from "$ InstigatedBy.getHumanName() $"'s \""$ DamageType $"\".");
			// Victim.ClientMessage(Int(ActualDamage*SavePercent/100) $"hp) damage saved by crouching.");
			// Victim.ClientMessage(Int(ActualDamage*SavePercent/100) $"hp damage saved by crouching from "$DamageType);
			Victim.ClientMessage("Crouching blocked "$Int(ActualDamage*SavePercent/100) $" hp of "$DamageType$" damage.");
			// NOTE: watching this I found at that the Titan gun's secondary fire has 'Burned' type damage.
			// I think Titan gun is overpowered, so I might sometime reduce that damage.
		}
		ActualDamage = ActualDamage*(1.0-SavePercent/100.0);

		Momentum = Momentum * BoostMultiplier;

	}

	if (NextDamageMutator != None)
		NextDamageMutator.MutatorTakeDamage( ActualDamage, Victim, InstigatedBy, HitLocation, Momentum, DamageType );

}

