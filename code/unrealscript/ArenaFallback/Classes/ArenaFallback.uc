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

// TODO: we are failing to remove the player's default weapons (translocator and enforcer), although we do try to replace them.
//       impact-hammers are not appearing, but i don't see them being replaced either

// TODO: compare against the method from /mnt/big/ut/ut_stuff/gone/unrealwiki-offline/changing-the-enforcer-ut.html

// TODO: we are no longer an Arena mutator, so other arena mutators may be added.  Check this works properly.  Maybe we need to detect other Arena muts by overriding AddMutator().

// TODO: we want this to be like other Arena mutators.  So test if InstaGibDM and the other Arena muts let us have boots.

// TODO: hey look we can create new actors from only their class name! BaseMutator.AddMutator(Spawn(MutatorClass));
//       we can also use .Class to get the class of an actor

// BUG: kinda messy, will sometimes add ammo for a weapon which ain't available!
// TODO: is it possible to replace all X with randomY, to keep map's weapons symmetrical?

// Current problems:
//   if we add just random weapons and ammo, we sometimes find dropped weapon which we can pickup, but cannot use (maybe because they had ammo 0 when we picked them up)
//   some of the default weapons (e.g. enforcer) report their ammotype as minigun2 :S  we need to build a table of real ammotypes for them, and hope that non-default weapons will have their set correctly
//   

// TODO: if the only weapons you find on the map are Redeemers, then maybe switch to SLV Arena

class ArenaFallback expands Mutator;

var config bool bOnlyOnWeaponlessMaps;
var config String weaponTypes;
var config bool bMultipleWeapons;
var config bool bAllowAllPickups;
var config bool bLogging;

defaultproperties {
	bOnlyOnWeaponlessMaps=True // False will cause ArenaFallback to Always replace weapons.  For testing; should be True as default.
	// weaponTypes="UnrealShare.NaliRabbit,Botpack.ShockRifle,Botpack.Rabit_FlakCannon"
	// NaliRabbit,Cow,
	// ,Botpack.ut_biorifle,Botpack.PulseGun,Botpack.SniperRifle,Botpack.ripper,Botpack.minigun2,Botpack.UT_FlakCannon,Botpack.UT_Eightball,Botpack.SuperShockRifle,Botpack.WarheadLauncher"
	          // Botpack.Translocator,Botpack.enforcer,Botpack.ChainSaw,Botpack.ImpactHammer,Botpack.doubleenforcer,
	weaponTypes="Botpack.ShockRifle,Botpack.ut_biorifle,Botpack.PulseGun,Botpack.SniperRifle,Botpack.ripper,Botpack.minigun2,Botpack.UT_FlakCannon,Botpack.UT_Eightball,Botpack.SuperShockRifle,Botpack.WarheadLauncher"
	bMultipleWeapons=False // TODO should be False as default
	bAllowAllPickups=True // TODO should be (forced to?) False for instagib arena; maybe we can check damageStrength of weapon, but some weapons like whart have large damage strength but there is large health to aquire ^^
	bLogging=True
}

var bool bForceArena; // Whether this Mutator will do anything.

var int depth; // used to prevent infinite recursion when replacing actors

// var String WeaponType;
var String replaceWeaponType;
var String replaceAmmoType;

function PreBeginPlay() {
	CheckForWeapons();
	Super.PreBeginPlay();
}

function CheckForWeapons() {
	local Weapon w;
	local String ammoType;

	if (bOnlyOnWeaponlessMaps) {
		foreach AllActors(class'Weapon', w) {
			if (bLogging) { Log("ArenaFallback: I found a weapon in this map ("$w$"): doing nothing."); }
			bForceArena = False;
			// TODO CONSIDER: for neatness, we could try Self.Destroy() here.
			return;
		}
	}

	bForceArena = True;
	// Level.Game.DefaultWeapon = class'Botpack.SuperShockRifle';
	while (true) {
		Level.Game.DefaultWeapon = getRandomWeaponClass();
		replaceWeaponType = ""$getRandomWeaponClass();
		replaceAmmoType = ""$getAmmoClassForWeaponClass(Level.Game.DefaultWeapon);
		// maybe None is good; maybe it means the weapon doesn't need ammo
		// if (replaceAmmoType != "None" && replaceAmmoType != "minigun2")
		if (replaceAmmoType != "minigun2")
			break;
	}
	if (bLogging) { Log("ArenaFallback: I did not find any weapons in this map: forcing "$Level.Game.DefaultWeapon$" arena with replaceWeaponType="$replaceWeaponType$" and replaceAmmoType="$replaceAmmoType$"."); }
}

function class<weapon> getRandomWeaponClass() {
	// return class'Botpack.SuperShockRifle';
	local String weapons[255];
	local int weaponsCount;
	local class<weapon> w;
	weaponsCount = SplitString(weaponTypes,",",weapons);
	// C = class<ChallengeBotInfo>(DynamicLoadObject("Botpack.ChallengeBotInfo", class'Class'));
	// return Class(DynamicLoadObject(weapons[ Rand(weaponsCount) ], class'Class'));
	w = class<weapon>(DynamicLoadObject(weapons[ Rand(weaponsCount) ], class'Class'));
	// if (bLogging) { Log("ArenaFallback: getRandomWeaponClass() returning: "$w); }
	// if (bLogging) { Log("["$depth$"] 2 ! choosing "$w); }
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
         str = Mid(str,nextSplit+Len(divider));
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

		return False; // From Arena.uc, but why?  What about the other muts?  They get queried separately?  Or do we really want to keep all actors?

	} else {

		// Pass to other mutators in the default way:
		return Super.AlwaysKeep(Other);

	}

}

function MyReplaceWith(Actor Other,String str) {
	if (str=="None") {
		if (bLogging) { Log("["$depth$"] 5 x skipping "$ Other $ " -> " $ str); }
		return;
	}
	depth++;
	if (bLogging) { Log("["$depth$"] 3 > replacing "$ Other $ " -> " $ str); }
	ReplaceWith(Other,str);
	depth--;
}

// Allow mutators to remove actors (by returning false), or replace them.
function bool CheckReplacement(Actor Other, out byte bSuperRelevant) {

	if (depth>0) {
		if (bLogging) { Log("("$depth$") 4 < todeep " $ Other $ " ("$bSuperRelevant$")"); }
		return True;
	}

	// if (bLogging) { Log("ArenaFallback: CheckReplacement("$Other$","$bSuperRelevant$")"); }
	// if (bLogging) { Log("["$depth$"] 1 ? checking " $ Other $ " ("$bSuperRelevant$")"); }

	if (bForceArena) {

		// Note: as well as weapon pickups, we are also replacing the player's default enforcer and translocator
		// TODO: spawn-with weapons should probably have maxammo
		// TODO: if weapon never runs out of ammo (e.g. ig rifle), ammo pickups should be removed from the map
		if (Other.IsA('Weapon')) {
			// if (bLogging) { Log("ArenaFallback: Replacing weapon "$Other); }
			if (bMultipleWeapons) {
				// MyReplaceWith(Other,String(getRandomWeaponClass().name));
				// MyReplaceWith(Other,String(getRandomWeaponClass()));
				MyReplaceWith(Other,""$getRandomWeaponClass());
			} else {
				MyReplaceWith(Other,String(Level.Game.DefaultWeapon));
			}
		} else

		if (Other.IsA('Ammo')) {
			// if (bLogging) { Log("ArenaFallback: Replacing ammo "$Other); }
			depth++; // to prevent checking the item we spawn
			if (bMultipleWeapons) {
				// TODO CONSIDER: rather than random ammo, make it ammo for the last weapon we replaced?
				// MyReplaceWith(Other,String(Spawn(getRandomWeaponClass()).AmmoName));
				// tmp = Spawn(getRandomWeaponClass());
				MyReplaceWith(Other,""$getAmmoClassForWeaponClass(getRandomWeaponClass())); // could only replace ammo from a generated list of ammo for those weapons we spawned so far
			} else {
				// MyReplaceWith(Other,String(Spawn(Level.Game.DefaultWeapon).AmmoName));
				// MyReplaceWith(Other,String(Spawn(Level.Game.DefaultWeapon).AmmoName));
				// tmp = Spawn(Level.Game.DefaultWeapon);
				MyReplaceWith(Other,replaceAmmoType);
			}
			depth--;
			// MyReplaceWith(Other,String(Weapon(tmp).AmmoName));
			// tmp.Destroy();
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

function class<ammo> getAmmoClassForWeaponClass(class<weapon> weaponClass) {
	local Actor instance;
	local class<ammo> ammoType;
	instance = Spawn(weaponClass);
	ammoType = Weapon(instance).AmmoName;
	instance.Destroy();
	return ammoType;
}

