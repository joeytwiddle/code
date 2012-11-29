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
		//// We should never usually get called with Damage=0, unless a custom implementation of ReduceDamage is being used.
		if (Damage == 0 || Mut.InDamage == 0) {
			Mut.Report(Mut.LastInstigator,"FWBArmor.ArmorAbsorbDamage() Unexpected Damage="$Damage$" InDamage="$Mut.InDamage$" InDamageType="$Mut.InDamageType);
		}
		//// So we use all old values.
		return Mut.AdjustDamageAndMomentum(Mut.InDamage,Pawn(Owner),Mut.LastInstigator,Mut.InHitLocation,Mut.LastMomentum,Mut.InDamageType);
	} else {
		Mut.EmitWarning("BUG! Mut.Stage="$ Mut.Stage $" in FWBArmor.ArmorAbsorbDamage()!  DamageType="$DamageType$" Damage="$Damage$" Owner="$Mut.GHN(Owner));
		return Damage;
	}
}

function int ArmorPriority(name DamageType) {
	// Return high priority regardless of DamageType ('Drowned' usually returns 0 for all armors!).
	return AbsorptionPriority;
}

// Clone of the default which does not check bIsAnArmor.
function inventory PrioritizeArmor( int Damage, name DamageType, vector HitLocation )
{
	local Inventory FirstArmor, InsertAfter;

	if ( Inventory != None )
		FirstArmor = Inventory.PrioritizeArmor(Damage, DamageType, HitLocation);
	else
		FirstArmor = None;

	if ( /*bIsAnArmor*/ true)
	{
		if ( FirstArmor == None )
		{
			nextArmor = None;
			return self;
		}

		// insert this armor into the prioritized armor list
		if ( FirstArmor.ArmorPriority(DamageType) < ArmorPriority(DamageType) )
		{
			nextArmor = FirstArmor;
			return self;
		}
		InsertAfter = FirstArmor;
		while ( (InsertAfter.nextArmor != None) 
			&& (InsertAfter.nextArmor.ArmorPriority(DamageType) > ArmorPriority(DamageType)) )
			InsertAfter = InsertAfter.nextArmor;

		nextArmor = InsertAfter.nextArmor;
		InsertAfter.nextArmor = self;
	}
	return FirstArmor;
}

defaultproperties {
	//// We cannot set this because UT_Shieldbelt.HandlePickupQuery() will destroy any other armors in inventory!
	//// Instead we will override PrioritizeArmor to put us in the list despite missing this flag.
	// bIsAnArmor=True
	// ArmorAbsorption=100
	AbsorptionPriority=1000001
}

