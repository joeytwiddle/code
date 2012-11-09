class NerfSniper expands Mutator config(NerfAmmo);

// This seems to work fine in normal games (LMS,CTF) but in fSG on nzp it gives you a sniper but no hits register.  O_o

var config int StartingAmmo;
var config int MaxAmmo;
var config int AmmoPerPack;

function PostBeginPlay()
{
	class'BulletBox'.default.MaxAmmo = MaxAmmo;
}

function bool AlwaysKeep(Actor Other)
{
	local bool bTemp;

	if ( Other.IsA('SniperRifle') )
	{
		Weapon(Other).PickupAmmoCount = StartingAmmo;
		return true;
	}
	if ( Other.IsA('BulletBox') ) // RifleShell gives 1 already
	{
		Ammo(Other).AmmoAmount = AmmoPerPack;
		return true;
	}

	if ( NextMutator != None )
		return ( NextMutator.AlwaysKeep(Other) );
	return false;
}

defaultproperties {
	StartingAmmo=6
	MaxAmmo=10
	AmmoPerPack=3
}

