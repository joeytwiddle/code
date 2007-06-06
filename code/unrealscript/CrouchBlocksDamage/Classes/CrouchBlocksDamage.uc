class CrouchBlocksDamage extends Mutator;

var bool Initialized;

function PostBeginPlay() {
    if (Initialized)
        return;
    Initialized = True;

    Level.Game.RegisterDamageMutator( Self );

    // TODO: really we want this to be displayed in the user's chat area when they join the server (like ZPPure does)
    Log("CrouchBlocksDamage mutator is running.  Crouch when hit to reduce damage by 50%!");
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
    if (Victim.IsA('PlayerPawn') && PlayerPawn(Victim).bIsCrouching) {
        ActualDamage = ActualDamage/2;
        Victim.ClientMessage("Crouching has saved you 50% ("$ ActualDamage $"hp) damage from "$ InstigatedBy.getHumanName() $"'s \""$ DamageType $"\".", 'CriticalEvent', True);
        // Momentum = Momentum/2;
        // Momentum = Momentum*8; // for fun, get an extra boost if you are crouched!
    }
    if ( NextDamageMutator != None )
        NextDamageMutator.MutatorTakeDamage( ActualDamage, Victim, InstigatedBy, HitLocation, Momentum, DamageType );
}

