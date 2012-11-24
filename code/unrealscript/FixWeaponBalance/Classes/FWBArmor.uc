class FWBArmor extends TournamentPickup;

var FixWeaponBalance Mut;

function int ArmorAbsorbDamage(int Damage, name DamageType, vector HitLocation) {
	// We should always be given a mutator.
	// If we are not, do nothing (absorb nothing, don't start our weird call chain).
	// This might happen if the mutator gets destroyed during a game.
	// We also stop doing stuff if the mutator is disabled mid-game.
	if (Mut == None || !Mut.bEnabled) {
		return Damage;
	}

	if (!Mut.bUseMagicMethod) {
		return Mut.AdjustDamageAndMomentum(Damage,Pawn(Owner),None,HitLocation,vect(0,0,0),DamageType);
	}

	if (Mut.Stage == 0) {
		Mut.Stage = 1;
		Mut.InDamage = Damage;
		Mut.InDamageType = DamageType;
		Mut.InHitLocation = HitLocation;
		return 0;
	} else if (Mut.Stage == 2) {
		Mut.Stage = 3;
		// Yay, we now have the LastInstigator and LastMomentum stored by the mutator.
		//// It is often the case that Damage now has a different value from the one we received before.
		//// This happens in hardcore mode, we see 225 for a headshot, and 100 for a body hit.
		//// The reason for this is that when the mutator calls Victim.TakeDamage, the damage we have gets scaled up for a *second time* by DeathMatchPlus.ReduceDamage(), because of hardcore mode.
		//// This is no direct problem for us.  We use the Mut.InDamage we stored last time round.
		// if (Damage != Mut.InDamage) {
		// 	Mut.Report(Mut.LastInstigator,"FWBArmor.ArmorAbsorbDamage() Damage!=InDamage ("$ Damage $" vs "$ Mut.InDamage $")");
		// }
		//// So we use all old values.
		return Mut.AdjustDamageAndMomentum(Mut.InDamage,Pawn(Owner),Mut.LastInstigator,Mut.InHitLocation,Mut.LastMomentum,Mut.InDamageType);
	} else {
		// This is a critical bug which I want to hear about if it occurs.  If admins still want to use the mod even if its occurring, they can suppress it.
		Log("BUG! Mut.Stage="$ Mut.Stage $" in FWBArmor.ArmorAbsorbDamage()!");
		return Damage;
	}
}

defaultproperties {
	bIsAnArmor=True
	ArmorAbsorption=100
	AbsorptionPriority=-999
}

