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

// BUG: kinda messy, will sometimes add ammo for a weapon which ain't available!
// TODO: is it possible to replace all X with randomY, to keep map's weapons symmetrical?

class ArenaFallback expands Mutator;

var bool bOnlyOnWeaponlessMaps;
var String weaponTypes;
var bool bMultipleWeapons;
var bool bAllowAllPickups;

var int depth; // used to prevent infinite recursion when replacing actors

defaultproperties {
	bOnlyOnWeaponlessMaps=False // TODO: should be True as default
	weaponTypes="Botpack.Translocator,Botpack.ChainSaw,Botpack.ImpactHammer,Botpack.enforcer,Botpack.doubleenforcer,Botpack.ShockRifle,Botpack.ut_biorifle,Botpack.PulseGun,Botpack.SniperRifle,Botpack.ripper,Botpack.minigun2,Botpack.UT_FlakCannon,Botpack.UT_Eightball,Botpack.SuperShockRifle,Botpack.WarheadLauncher"
	bMultipleWeapons=True // TODO should be False as default
	bAllowAllPickups=True // TODO should be False for IG arena
}

var bool bForceArena; // Whether this Mutator will do anything.

function PreBeginPlay() {
	CheckForWeapons();
}

function CheckForWeapons() {
	local Weapon w;

	if (bOnlyOnWeaponlessMaps) {
		foreach AllActors(class'Weapon', w) {
			Log("ArenaFallback: I found a weapon in this map ("$w$"): doing nothing.");
			bForceArena = False;
			// TODO CONSIDER: for neatness, we could try Self.Destroy() here.
			return;
		}
	}

	bForceArena = True;
	// Level.Game.DefaultWeapon = class'Botpack.SuperShockRifle';
	Level.Game.DefaultWeapon = getRandomWeaponClass();
	Log("ArenaFallback: I did not find any weapons in this map: forcing "$Level.Game.DefaultWeapon$" arena.");
}

function class<weapon> getRandomWeaponClass() {
	// return class'Botpack.SuperShockRifle';
	local String weapons[255];
	local int weaponsCount;
	local class<weapon> w;
	weaponsCount = SplitString(weaponTypes,",",weapons);
	// C = class<ChallengeBotInfo>(DynamicLoadObject("Botpack.ChallengeBotInfo", class'Class'));
	// return Class(DynamicLoadObject(weapons[ RandRange(0,weaponsCount) ], class'Class'));
	w = class<weapon>(DynamicLoadObject(weapons[ RandRange(0,weaponsCount) ], class'Class'));
	// Log("ArenaFallback: getRandomWeaponClass() returning: "$w);
	Log("["$depth$"] 2 ! "$w);
	return w;
}

function int SplitString(String str, String divider, out String parts[255]) {
   // local String parts[255];
   // local array<String> parts;
   local int i,nextSplit;
   i=0;
   while (true) {
      nextSplit = InStr(str,divider);
      if (nextSplit >= 0) {
         // parts.insert(i,1);
         parts[i] = Left(str,nextSplit);
         str = Mid(str,nextSplit+1);
         i++;
      } else {
         // parts.insert(i,1);
         parts[i] = str;
         i++;
         break;
      }
   }
   // return parts;
   return i;
}

// Force game to always keep this actor, even if other mutators want to get rid of it.
function bool AlwaysKeep(Actor Other) {

	if (bForceArena) {

		return False; // Why?  What about the other muts?

	} else {

		// Pass to other mutators in the default way:
		return Super.AlwaysKeep(Other);

	}

}

function MyReplaceWith(Actor Other,String str) {
	if (str=="None") {
		Log("["$depth$"] 5 Z "$ Other $ " ! -> ! " $ str);
		return;
	}
	depth++;
	Log("["$depth$"] 3 > "$ Other $ " -> " $ str);
	ReplaceWith(Other,str);
	depth--;
}

// Allow mutators to remove actors.
function bool CheckReplacement(Actor Other, out byte bSuperRelevant) {

	local Actor tmp;

	if (depth>0) {
		Log("("$depth$") 4 x " $ Other $ " ("$bSuperRelevant$")");
		return True;
	}

	// Log("ArenaFallback: CheckReplacement("$Other$","$bSuperRelevant$")");
	Log("["$depth$"] 1 ? " $ Other $ " ("$bSuperRelevant$")");

	if (bForceArena) {

		// Note: as well as weapon pickups, we are also replacing the player's default enforcer and translocator
		// TODO: spawn-with weapons should probably have maxammo
		// TODO: if weapon never runs out of ammo (e.g. ig rifle), ammo pickups should be removed from the map
		if (Other.IsA('Weapon')) {
			// Log("ArenaFallback: Replacing weapon "$Other);
			if (bMultipleWeapons) {
				// MyReplaceWith(Other,String(getRandomWeaponClass().name));
				// MyReplaceWith(Other,String(getRandomWeaponClass()));
				MyReplaceWith(Other,""$getRandomWeaponClass());
			} else {
				MyReplaceWith(Other,String(Level.Game.DefaultWeapon));
			}
		} else

		if (Other.IsA('Ammo')) {
			// Log("ArenaFallback: Replacing ammo "$Other);
			depth++; // to prevent checking the item we spawn
			if (bMultipleWeapons) {
				// TODO CONSIDER: rather than random ammo, make it ammo for the last weapon we replaced?
				// MyReplaceWith(Other,String(Spawn(getRandomWeaponClass()).AmmoName));
				tmp = Spawn(getRandomWeaponClass());
			} else {
				// MyReplaceWith(Other,String(Spawn(Level.Game.DefaultWeapon).AmmoName));
				// MyReplaceWith(Other,String(Spawn(Level.Game.DefaultWeapon).AmmoName));
				tmp = Spawn(Level.Game.DefaultWeapon);
			}
			depth--;
			MyReplaceWith(Other,String(Weapon(tmp).AmmoName));
			tmp.Destroy();
		} else

		// This looks like most pickups, but boots are still allowed.  :)
		if (
			Other.IsA('Weapon') || Other.IsA('Ammo')
			|| Other.IsA('TournamentHealth') || Other.IsA('UT_Shieldbelt')
			|| Other.IsA('Armor2') || Other.IsA('ThighPads')
			|| Other.IsA('UT_Invisibility') || Other.IsA('UDamage')
		) {

			return bAllowAllPickups;

		}

		// It wasn't a weapon or ammo, or one of the pickups irrelevant to IG, so keep it.
		bSuperRelevant = 0;
		return True;

	} else {

		// Pass to other mutators in the default way:
		return Super.CheckReplacement( Other, bSuperRelevant );

	}

}

