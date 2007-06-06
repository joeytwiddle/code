//=============================================================================
// ArenaFallback.
// This will make the game a weapons arena, if there are no weapons
//   found in the map by default.
// Useful if you don't know whether the map will contain weapons or not,
//   and you want to avoid playing a game with just Hammer and Enforcer.
// Most of the time this mutator will do nothing.
// But if a map without weapons is voted, it will force arena mode.
//=============================================================================

// TODO: unless we can actually make it configurable, there's no point trying to make this anything other than an instagib arena

// TODO: compare against the method from /mnt/big/ut/ut_stuff/gone/unrealwiki-offline/changing-the-enforcer-ut.html

// TODO: via config allow different arena weapons than the ig rifle
//       allow an option for a random arena weapon chosen by the mutator at game-start

// TODO: we are no longer an Arena mutator, so other arena mutators may be added.  Check this works properly.

// TODO: we want this to be like other Arena mutators.  So test if InstaGibDM and the other Arena muts let us have boots.

// TODO: check /mnt/big/ut/ut_stuff/www.ut-files.com/Mods/mutteamweaponarena.zip and /mnt/big/stuff/software/games/unreal/server/mods/added_ok/weaponmage_gold.zip to see if they can replace weapons randomly, and how.

class ArenaFallback expands Mutator;

var bool bForceArena; // Whether this Mutator will do anything.

var bool bAllowPickups; // In InstaGibDM (InstaGib Arena) we don't allow any pickups on the map (no health or shields or boots or weapon pickups).
                        // Actually boots should be allowed.  Just health/amp/shield are irrelevant, so they should be removed.
// This is largely irrelevant.  Very few maps will have pickups but not weapon pickups!

var class WeaponType;
// var name WeaponName, AmmoName; // The arena weapon and ammo types we will force.

defaultproperties {
	WeaponType = class'Botpack.SuperShockRifle'
	bAllowPickups = False
	// WeaponName = SuperShockRifle
	// AmmoName = SuperShockCore

	// WeaponType=class'Botpack.PulseGun'
	// bAllowPickups = True
	// WeaponName=PulseGun
	// AmmoName=PAmmo
}

function PreBeginPlay() {
	CheckForWeapons();
}

function CheckForWeapons() {
	local Weapon w;

	foreach AllActors(class'Weapon', w) {
		Log("ArenaFallback: I found a weapon in this map ("$w$"): doing nothing.");
		bForceArena = False;
		return;
	}

	Log("ArenaFallback: I did not find any weapons in this map: forcing Arena game.");
	bForceArena = True;
	Level.Game.DefaultWeapon = WeaponType;
	// Level.Game.bCoopWeaponMode = False;
}

// Force game to always keep this actor, even if other mutators want to get rid of it.
function bool AlwaysKeep(Actor Other) {

	if (bForceArena) {

		if (!bAllowPickups) {
			return False;
		}

		if (Other.IsA('Weapon')) {
			Weapon(Other).PickupAmmoCount = Weapon(Other).AmmoName.Default.MaxAmmo;
			return True;
		}
		if (Other.IsA('Ammo')) {
			Ammo(Other).AmmoAmount = Ammo(Other).MaxAmmo;
			return True;
		}

	} else {

		// Pass to other mutators in the default way:
		return Super.AlwaysKeep(Other);

	}

}

// Allow mutators to remove actors.
function bool CheckReplacement(Actor Other, out byte bSuperRelevant) {

	if (bForceArena) {

		if (!bAllowPickups) {
			return False;
		}

		if (Other.IsA('Weapon')) {
			if (Other.IsA(WeaponName)) {
				return True;
			} else {
				// It's a weapon, but not an arena weapon: replace it.
				ReplaceWith(Other, WeaponString);
				return False;
			}
		}
		if (Other.IsA('Ammo')) {
			if (Other.IsA(AmmoName)) {
				return True;
			} else {
				// It's ammo, but not arena ammo: replace it.
				ReplaceWith(Other, AmmoString);
				return False;
			}
		}

		// It wasn't a weapon or ammo, and pickups are allowed, so keep it.
		bSuperRelevant = 0;
		return True;

		/*
		// TODO: although 
		if ( Other.IsA('TournamentHealth') || Other.IsA('UT_Shieldbelt')
			|| Other.IsA('Armor2') || Other.IsA('ThighPads')
			|| Other.IsA('UT_Invisibility') || Other.IsA('UDamage') )
			return False;
		*/

	} else {

		// Pass to other mutators in the default way:
		return Super.CheckReplacement( Other, bSuperRelevant );

	}

}

