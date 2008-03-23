// Modifies the default sgConstructor, so that it will refuse to build anywhere nearby an important Actor.
// This should prevent people from blocking objectives in Assault, or building things right next to flagspots in CTF.

class SiegeAnywhereConstructor extends sgConstructor;

function PlaceIt()
{
    local sgPRI     ownerPRI;
    local class<sgBuilding>
                    type;
	local sgBuilding
                    test;
	local Vector TargetLocation;
	local sgCameraView
					stest;
	local int i;
	local float amount;


    if ( Pawn(Owner) == None || sgPRI(Pawn(Owner).PlayerReplicationInfo) ==
      None)
        return;
    ownerPRI = sgPRI(Pawn(Owner).PlayerReplicationInfo);

    type = GetBuildType();

    if ( type == None )
        return;

/*
	if (type==class'sgFactory')
		if (!CanIMakeMoreFactoriesPlease())
		{
			Owner.PlaySound(Misc1Sound, SLOT_Misc, Pawn(Owner).SoundDampening*2.5);
			Pawn(Owner).ClientMessage("Build Message: one Factory allowed per team once a Super Supplier is built.");
			return;
		}
*/
	if (type==class'sgTeleporter')
		if (!CanIMakeMoreTeleportersPlease())
		{
			Owner.PlaySound(Misc1Sound, SLOT_Misc, Pawn(Owner).SoundDampening*2.5);
			Pawn(Owner).ClientMessage("Build Message: only one pair of Teleporters allowed per player.");
			return;
		}
	if (type==class'sgPlatform')
		if (!CanIMakeMorePlatformsPlease())
		{
			Owner.PlaySound(Misc1Sound, SLOT_Misc, Pawn(Owner).SoundDampening*2.5);
			Pawn(Owner).ClientMessage("Build Message: only one Platform allowed per player.");
			return;
		}

	if (type==class'sgHomingBeacon')
		if (!CanIMakeMoreBeaconsPlease())
		{
			Owner.PlaySound(Misc1Sound, SLOT_Misc, Pawn(Owner).SoundDampening*2.5);
			Pawn(Owner).ClientMessage("Build Message: only one Homing Beacon allowed per player.");
			return;
		}

	if (!(SiegeGI(Level.Game) != None && SiegeGI(Level.Game).FreeBuild)
      && ownerPRI.RU < type.default.BuildCost )
	{
		Owner.PlaySound(Misc1Sound, SLOT_Misc, Pawn(Owner).SoundDampening*2.5);
		Pawn(Owner).ClientMessage("Build Message: You do not have sufficient RU to build this.");
		return;
	}

	if (type==class'sgCameraView')
	{
		CreateScreen(OwnerPRI);
		return;
	}

	TargetLocation = Owner.Location - vect(0,0,8) +
		vector(Pawn(Owner).ViewRotation) * 45;

	if (ShouldNotBuild(type,TargetLocation,Pawn(Owner))) {
		test = None;
	} else {
		test = Spawn(type, Owner,, TargetLocation, Pawn(Owner).ViewRotation);
	}

	if ( test != None)
	{

		for (i=0;i<NumBuildings;i++)
		{
			if (test.BuildingName == right(sgBuildings[i],len(sgBuildings[i])-1))
			{
				sgGameReplicationInfo(SiegeGI(Level.Game).GameReplicationInfo).sgTotalBuilt[i]++;
				ownerPRI.sgBuilt[i]++;
			}
		}


		Owner.PlaySound(FireSound, SLOT_None, Pawn(Owner).SoundDampening*2.5);
        	if ( SiegeGI(Level.Game) == None || !SiegeGI(Level.Game).FreeBuild )
			ownerPRI.RU -= type.default.BuildCost;

		ownerPRI.Score += type.default.BuildCost / 100;
		ownerPRI.sgInfoBuildingMaker++;
	}
	else
        Owner.PlaySound(Misc1Sound, SLOT_Misc, Pawn(Owner).SoundDampening*2.5);
}

function bool ShouldNotBuild(class type, Vector Location, optional Pawn Owner) {
	local Actor p;
	local String OwnerName;
	foreach AllActors(class'Actor',p) {
		if (
				!
				( // these actors can never block:
					p.IsA('PlayerPawn')
					|| p.IsA('Bot')
					|| p.IsA('PathNode')
					|| p.IsA('SoundEvent')
					|| p.IsA('Light')
					|| p.IsA('locationid')
					|| p.IsA('sgBuilding') // presumably dealt with elsewhere
					|| (p.IsA('Weapon') && (Owner==p.Owner || Weapon(p).bHeldItem)) // my weapon, or a held weapon
					|| p.IsA('Projectile') // my weapon, or a held weapon
				)
			&&
				( // these actors block building when near:
					p.IsA('Trigger')
					|| p.IsA('FlagBase')
					|| p.IsA('CTFFlag')
					|| p.IsA('PlayerStart')
					|| (p.IsA('Weapon') && Owner!=p.Owner && !Weapon(p).bHeldItem) // pickup point, and not my weapon!
					|| (p.IsA('Inventory') && Owner!=p.Owner && !Inventory(p).bHeldItem) // pickup point, and not my weapon!
					|| (p.IsA('Pickup') && Owner!=p.Owner && !Pickup(p).bHeldItem) // pickup point, and not my weapon!
					|| true // whitelist atm - see above
				)
			&&
				VSize(p.Location - Location) < 96 // 128
		) {
			// if (p.IsA('Weapon') && (p.Owner == Owner) || Weapon(p).bHeld)
				// continue; // Somehow this was not caught above.
			OwnerName = p.Owner.getHumanName();
			if (OwnerName == "")
				OwnerName = String(p.Owner);
			if (OwnerName == "")
				OwnerName = "The";
			else
				OwnerName = OwnerName $ "'s";
			// Owner.ClientMessage("The " $ p.class $ " blocks you from placing a " $ type $ " here.");
			Owner.ClientMessage(OwnerName $ " " $ p.class.name $ " blocks you from placing a " $ type $ " here.");
			// p.bvisible=true; p.drawscale=1.0+0.5*(FRand());
			return true;
		}
	}
	return false;
}

