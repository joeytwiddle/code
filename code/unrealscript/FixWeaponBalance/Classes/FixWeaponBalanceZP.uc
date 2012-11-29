// Same code but variables may be different, and are stored in a different categorry.

// Not really for zeroping, really for when UTPure is missing (which zeroping forces).

class FixWeaponBalanceZP expands FixWeaponBalance config(FixWeaponBalance);

defaultproperties {
	bFixCrouchingBug=True
	//// No we should leave it high, to allow for amp
	// SniperHeadshotMaxDamage=150
	//// Reduced power because zeroping
	SniperPowerRange=256
	MinigunDamageScale=0.78
	PulsebeamDamageScale=0.78
}

