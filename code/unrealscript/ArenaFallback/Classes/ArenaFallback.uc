//=============================================================================
// ArenaFallback.
// This will make the game a weapons arena, if there are no weapons
//   in the map by default.
// Useful if you don't know whether the map will contain weapons or not,
//   and you want to avoid playing a game with just Hammer and Enforcer.
// This is basically a modified version of the default InstaGibDM arena mutator.
//=============================================================================

// Alternatively to expanding Arena,we could use the method from /mnt/big/ut/ut_stuff/gone/unrealwiki-offline/changing-the-enforcer-ut.html

// TODO: via config allow different arena weapons than the ig rifle
//       allow an option for a random arena weapon chosen by the mutator at game-start

// LOL FFS it replaces my enforcer with the DefaultWeapon below when there are weapons on the map
// and when there aren't weapons on the map, it replaces my enforcer with nothing!
// Ahh that was my fault, had a bool the wrong way round.  :P

class ArenaFallback expands Arena;

var bool DoneCheck; // Thankfully UT defaults this to False.
var bool FoundWeapon;

defaultproperties {
	WeaponName=SuperShockRifle
	DefaultWeapon=class'Botpack.SuperShockRifle'
	AmmoName=SuperShockCore
	// WeaponName=PulseGun
	// DefaultWeapon=class'Botpack.PulseGun'
	// AmmoName=PAmmo
	// DoneCheck = False;
}

function PreBeginPlay() {
	CheckForWeapons();
}

function CheckForWeapons() {
	local Weapon w;

	if (DoneCheck)
		return;
	DoneCheck=True;

	FoundWeapon = False;
	foreach AllActors( class 'Weapon', w ) {
		FoundWeapon = True;
		Log("ArenaFallback: I found a weapon in this map: "$w);
		break;
	}
	if (FoundWeapon) {
		// I don't understand how Self.DefaultWeapon was being accessed, but it was, so:
		DefaultWeapon = Level.Game.DefaultWeapon;
	}
	if (!FoundWeapon) {
		Log("ArenaFallback: I did not find any weapons in this map: forcing Arena game.");
	}
}

function bool AlwaysKeep(Actor Other) {
	CheckForWeapons(); // in case PreBeginPlay() was not yet called
	if (FoundWeapon)
		return True;
	else
		return Super.AlwaysKeep(Other);
}

function bool CheckReplacement(Actor Other, out byte bSuperRelevant) {
	CheckForWeapons(); // in case PreBeginPlay() was not yet called
	if (FoundWeapon)
		return True;

	// TODO: only remove these powerups if it's an instagib arena; keep them for other weapons
	if ( Other.IsA('TournamentHealth') || Other.IsA('UT_Shieldbelt')
		|| Other.IsA('Armor2') || Other.IsA('ThighPads')
		|| Other.IsA('UT_Invisibility') || Other.IsA('UDamage') )
		return false;

	return Super.CheckReplacement( Other, bSuperRelevant );
}

