// Same code but variables may be different, and are stored in a different categorry.

// Not really for Siege, really for non-hardcore games with zeroping and superpros.

class FixWeaponBalanceSiege expands FixWeaponBalance config(FixWeaponBalance);

	/*
	bEnabled=True
	bLogging=False
	bLiveReports=False

	bKeepHeadshotsInAllModes=False
	SniperDamageScale=1.25
	SniperMaxDamage=150
	bScaleSniperByDistance=True
	SniperPowerRange=256
	SniperMaxRange=4096
	SniperMinDamage=12
	bFixCrouchingBug=True
	*/

// We have modified sniper damage mostly because headshots are too easy in zeroping.
// Body shots (usually 45 in Siege) are fairly weak by comparison, so we actually let them go stronger.

defaultproperties {
	// To increase damage to normal hardcore mode (headshot 150), we can multiply FWB's default scale by 1.5
	// But we don't want sniper that strong (because zp has made it easy to use) so we aim for max 125 instead.
	SniperHeadshotDamageScale=1.62
	SniperBodyshotDamageScale=1.62
	SniperHeadshotMaxDamage=125
	SniperBodyshotMaxDamage=56
	RocketDamageScale=1.25
	RocketGrenadeDamageScale=1.25
	BioDamageScale=1.25
	EnforcerDamageScale=1.00
	MinigunDamageScale=0.78
	ShockPrimaryDamageScale=0.90
	ShockBallDamageScale=1.15
	FlakPrimaryDamageScale=1.25
	FlakSecondaryDamageScale=1.25
	PulseDamageScale=1.25
	PulsebeamDamageScale=0.5
	RipperPrimaryDamageScale=1.00
	RipperSecondaryDamageScale=1.25
	RipperHeadshotDamageScale=1.00
	MeleeWeaponDamageScale=1.10
}

	/*
	bAdjustBoost=True
	bOnlyAdjustBoostDownwards=True
	*/

