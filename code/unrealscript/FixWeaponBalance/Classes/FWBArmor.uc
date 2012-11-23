class FWBArmor extends TournamentPickup;

var FixWeaponBalance Mut;

function int ArmorAbsorbDamage(int Damage, name DamageType, vector HitLocation) {
	if (Mut.Stage == 0) {
		Mut.Stage = 1;
		Mut.InDamage = Damage;
		Mut.InDamageType = DamageType;
		Mut.InHitLocation = HitLocation;
		return 0;
	} else if (Mut.Stage == 2) {
		Mut.Stage = 3;
		//// It is often the case that Damage now has a different value from the one we received before, InDamage.
		//// Commonly 225 for a headshot, 100 for a body hit, but occasionally *less* than InDamage!
		//// The reason for this is that when the mut calls Victim.TakeDamage with the value we have, it gets scaled up by DeathMatchPlus.ReduceDamage() for a second time, because of hardcore mode.
		// if (Damage != Mut.InDamage) {
		// 	Mut.Report(Mut.LastInstigator,"FWBArmor.ArmorAbsorbDamage() Damage!=InDamage ("$ Damage $" vs "$ Mut.InDamage $")");
		// }
		//// So we use all old values.  There is also some change to momentum done there.
		return Mut.ModifyDamage(Mut.InDamage,Pawn(Owner),Mut.LastInstigator,Mut.InHitLocation,Mut.LastMomentum,Mut.InDamageType);
	} else {
		Log("BUG! Mut.Stage="$ Mut.Stage $" in FWBArmor.ArmorAbsorbDamage()!");
		return Damage;
	}
}

defaultproperties {
	bIsAnArmor=True
	ArmorAbsorption=100
	AbsorptionPriority=-999
}

