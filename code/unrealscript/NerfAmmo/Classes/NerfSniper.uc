class NerfSniper expands Mutator config(NerfAmmo);

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
	}
	if ( Other.IsA('BulletBox') ) // RifleShell gives 1 already
	{
		Ammo(Other).AmmoAmount = AmmoPerPack;
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

