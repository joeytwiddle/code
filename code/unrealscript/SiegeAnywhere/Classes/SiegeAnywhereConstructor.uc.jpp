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
	foreach AllActors(class'Actor',p) {
		if (
				!
				( // these actors do not block:
					p.IsA('PlayerPawn')
					|| p.IsA('Bot')
					|| p.IsA('PathNode')
					|| p.IsA('SoundEvent')
				)
			&&
				( // these actors block building when near:
					p.isa('Trigger')
					|| p.isa('PlayerStart')
					|| p.isa('FlagBase')
					|| (p.isa('Weapon') && Owner!=p.Owner) // pickup point, and not my weapon!
					|| (p.isa('Inventory') && Owner!=p.Owner) // pickup point, and not my weapon!
					|| (p.isa('Pickup') && Owner!=p.Owner) // pickup point, and not my weapon!
					|| true // blacklist atm - see above
				)
			&&
				VSize(p.Location - Location) < 32
		) {
			Owner.ClientMessage("The " $ p.class $ " blocks you from placing a " $ type $ " here.");
			// p.bvisible=true; p.drawscale=1.0+0.5*(FRand());
			return false;
		}
	}
}

