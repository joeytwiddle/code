//=============================================================================
// ArenaFallback.
// This will make the game an instagib arena, if there are no weapons
//   found in the map by default.
// Useful if you don't know whether the map will contain weapons or not,
//   and you want to avoid playing a game with just Hammer and Enforcer.
// Most of the time this mutator will do nothing.
// But if a map without weapons is voted, it will force instagib arena mode.
//=============================================================================

// DONE: the ig rifle is not appearing!!  Ok we make sure in CheckReplacement() that it isn't removed
// DONE: but still getting a lot of Accessed None in logs.  Ok that was fixed by checking SuperShockCore ammo is not removed.

// DONE: probably not our fault, but we were getting: ChatLogger CTF-Control.ChatLogger0 (Function Engine.PlayerPawn.ClientVoiceMessage:002C) Accessed None
//       idd it happens when bForceArena=False

// TODO: compare against the method from /mnt/big/ut/ut_stuff/gone/unrealwiki-offline/changing-the-enforcer-ut.html

// TODO: we are no longer an Arena mutator, so other arena mutators may be added.  Check this works properly.  Maybe we need to detect other Arena muts by overriding AddMutator().

// TODO: we want this to be like other Arena mutators.  So test if InstaGibDM and the other Arena muts let us have boots.

// TODO: hey look we can create new actors from only their class name! BaseMutator.AddMutator(Spawn(MutatorClass));
//       we can also use .Class to get the class of an actor

class ArenaFallback expands Mutator;

var bool bForceArena; // Whether this Mutator will do anything.

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

	Log("ArenaFallback: I did not find any weapons in this map: forcing Instagib Arena.");
	bForceArena = True;
	Level.Game.DefaultWeapon = class'Botpack.SuperShockRifle';
}

// Force game to always keep this actor, even if other mutators want to get rid of it.
function bool AlwaysKeep(Actor Other) {

	if (bForceArena) {

		return False;

	} else {

		// Pass to other mutators in the default way:
		return Super.AlwaysKeep(Other);

	}

}

// Allow mutators to remove actors.
function bool CheckReplacement(Actor Other, out byte bSuperRelevant) {

	if (bForceArena) {

		if (Other.IsA('SuperShockRifle') || Other.IsA('SuperShockCore')) {
			return True;
		}

		// This looks like most pickups, but boots are still allowed.  :)
		if (
			Other.IsA('Weapon') || Other.IsA('Ammo')
			|| Other.IsA('TournamentHealth') || Other.IsA('UT_Shieldbelt')
			|| Other.IsA('Armor2') || Other.IsA('ThighPads')
			|| Other.IsA('UT_Invisibility') || Other.IsA('UDamage')
		) {

			return False;

		}

		// It wasn't a weapon or ammo, or one of the pickups irrelevant to IG, so keep it.
		bSuperRelevant = 0;
		return True;

	} else {

		// Pass to other mutators in the default way:
		return Super.CheckReplacement( Other, bSuperRelevant );

	}

}

