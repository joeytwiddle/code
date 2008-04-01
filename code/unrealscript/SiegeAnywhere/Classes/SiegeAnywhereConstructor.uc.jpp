// Modifies the default sgConstructor, so that it will refuse to build anywhere nearby an important Actor.
// This should prevent people from blocking objectives in Assault, or building things right next to flagspots in CTF.

class SiegeAnywhereConstructor extends sgConstructor;

var config bool bBlockBuildingOnKeypoints;
var config float HealthBasedRUPerSec;

defaultproperties {
	bBlockBuildingOnKeypoints=True
	HealthBasedRUPerSec=1.0 // For non Siege games, how much RU to give a player each second (if they have 100 health).
}

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

	if (bBlockBuildingOnKeypoints && ShouldNotBuild(type,TargetLocation,Pawn(Owner))) {
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
					|| p.IsA('Projectile')
					|| p.IsA('Mover') // TODO CONSIDER: the jury is still out on this one
					|| p.IsA('Decal') // TODO CONSIDER: the jury is still out on this one
					|| p.IsA('Decoration') // TODO CONSIDER: the jury is still out on this one
					|| p.IsA('Effect') // TODO CONSIDER: the jury is still out on this one
				)
			&&
				( // these actors always block building when near:
					true // So we can collect the names of relevant actors.  Add ones we wish to whitelist above.
					|| p.IsA('Trigger')
					|| p.IsA('FlagBase')
					// || p.IsA('CTFFlag')
					|| p.IsA('FortStandard')
					|| p.IsA('PlayerStart')
					|| p.IsA('InventorySpot')
					|| (p.IsA('Weapon') && Owner!=p.Owner && !Weapon(p).bHeldItem) // pickup point, and not my weapon, or held by another
					|| (p.IsA('Inventory') && Owner!=p.Owner && !Inventory(p).bHeldItem) // pickup point, and not my weapon, or held by another
					|| (p.IsA('Pickup') && Owner!=p.Owner && !Pickup(p).bHeldItem) // pickup point, and not my weapon, or held by another
				)
			&&
				VSize(p.Location - Location) < 96
				// 128
				// 96 seemed a little too close to me; mines could still reach a player grabbing the flag.
				// I wouldn't mind expanding it even more.  Although it could end up blocking too much of the map.
		) {
			// if (p.IsA('Weapon') && (p.Owner == Owner) || Weapon(p).bHeld)
				// continue; // Somehow this was not caught above.
			OwnerName = p.Owner.getHumanName();
			if (OwnerName == "")
				OwnerName = String(p.Owner);
			if (OwnerName == "" || OwnerName == "None")
				OwnerName = "The";
			else
				OwnerName = OwnerName $ "'s";
			// Owner.ClientMessage("The " $ p.class $ " blocks you from placing a " $ type $ " here.");
			// Owner.ClientMessage(OwnerName $ " " $ p.class.name $ " blocks you from placing a " $ type $ " here.");
			Owner.ClientMessage(OwnerName $ " " $ p.class.name $ " blocks you from building here.");
			// p.bvisible=true; p.drawscale=1.0+0.5*(FRand());
			return true;
		}
	}
	return false;
}



function PostBeginPlay() {
	SetTimer(1,True);
}

event Timer() {
	local PlayerPawn p;
	local sgPRI pri;
	p = PlayerPawn(Owner);
	if (p == None)
		return;
	pri = sgPRI(p.PlayerReplicationInfo);
	if (pri == None)
		return;
	if (HealthBasedRUPerSec > 0 && SiegeGI(Level.Game)==None) {
		pri.AddRU( HealthBasedRUPerSec * p.Health / 100.0 );
		if (pri.RU > pri.MaxRU) {
			pri.MaxRU = (pri.MaxRU + pri.RU) / 2; // grow max at half the speed
			pri.RU = pri.MaxRU;
		}
	}
}

