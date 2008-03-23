class SiegeAnywhereConstructor extends sgConstructor;

var SiegeAnywhere SiegeAnywhere;

replication {
    reliable if ( Role < ROLE_Authority )
        SiegeAnywhere;
}

simulated function SiegeAnywhere GetSiegeAnywhere() {
	return SiegeAnywhere;
	/*
	local SiegeAnywhere X;
	foreach AllActors(class'SiegeAnywhere',X) {
		return X;
	}
	Log("SiegeAnywhereHUD.GetSiegeAnywhere(): FAILED!");
	return None;
	*/
}

exec function SiegeStats()
{
	if (!Owner.IsA('PlayerPawn'))
		return;
	GetSiegeAnywhere().GetsgPRIFor(Owner).iStatus = 1-GetSiegeAnywhere().GetsgPRIFor(Owner).iStatus;
	Owner.PlaySound(SelectSound, SLOT_None,Pawn(Owner).SoundDampening*5,,, 2.0);
}


simulated function ClientSetBind()
{
	Owner.ConsoleCommand("SET INPUT F3 SiegeStats");
	SaveConfig(); 
}

function Tick(float DeltaTime)
{
	if ( Pawn(Owner) != None && GetSiegeAnywhere().GetsgPRIFor(Owner) !=
      None && AmmoType != None )
        AmmoType.AmmoAmount = GetSiegeAnywhere().GetsgPRIFor(Owner).RU;
}

function DropFrom(vector startLocation)
{
    if ( !SetLocation(startLocation) )
        return;
    Destroy();
}

function sgCameraView CheckCamera()
{
	local pawn P;
	if (!Owner.IsA('PlayerPawn'))
		return None;

	for (p=level.pawnlist;p!=none;p=p.nextpawn)
		if (p.isa('sgCameraView'))
			if (sgCameraView(p).MyViewer==PlayerPawn(Owner))
				return sgCameraView(P);

	return None;
}

function bool AmIViewing()
{
	local sgCameraView MyView;
	MyView=CheckCamera();
	return (MyView != None);
}

function Fire( float F )
{
	local sgCameraView MyView;

	ClientSetBind();
	MyView=CheckCamera();
	if (MyView!=None)
	MyView.CameraSwitch(PlayerPawn(Owner),False);
	else
	GotoState('Build');
}

function AltFire( float Value )
{
	local sgCameraView MyView;

	ClientSetBind();
	MyView=CheckCamera();
	if (MyView!=None) {
		if (Category==6 && Selection==0) TeleCam(MyView.myCam);
		else MyView.ClientAltFire(); }
	else
	GotoState('ChangeMode');
}

function TeleCam(sgCamera sgC)
{

	local sgBuilding B;
	local vector Dest, Start;
	local actor e;

	foreach AllActors(class'sgBuilding',B)
	{
		B.bShadowCast=B.bBlockPlayers;
		B.bBlockPlayers=False;
	}

	Start = Pawn(Owner).Location;
	Dest = sgC.Location;
	if ( Pawn(Owner).SetLocation(Dest) )
    	{
        	Level.Game.PlayTeleportEffect(Owner, true, true);
       		e = Spawn(class'TranslocOutEffect',,,start, Owner.Rotation);
    		e.Mesh = Owner.Mesh;
    		e.Animframe = Owner.Animframe;
    		e.Animsequence = Owner.Animsequence;
    		e.Velocity = 900 * Normal(Dest - Start);
	}
	foreach AllActors(class'sgBuilding',B)
		B.bBlockPlayers=B.bShadowCast;

}

function Pawn GetTarget()
{
	local Actor other;
	local vector    hitLocation,
                    hitNormal,
                    startTrace,
                    endTrace;

	startTrace = Owner.Location;
	startTrace.Z += Pawn(Owner).BaseEyeHeight;
	endTrace = startTrace + vector(PlayerPawn(Owner).ViewRotation) * 90;
	other = Trace(hitLocation, hitNormal, endTrace, startTrace, true);
        if (other != None) return Pawn(other);

	endTrace = startTrace + vector(PlayerPawn(Owner).ViewRotation) * 10000.0;
	other = Trace(hitLocation, hitNormal, endTrace, startTrace, true);
	if ((PlayerPawn(other) != None || Bot(other) != None ) && !other.bHidden) return Pawn(other);
}

function AnnounceRemove(sgBuilding building, Pawn remover)
{
    local string removeMessage;
    local Pawn p;

    if ( building.Owner == remover || remover.PlayerReplicationInfo == None )
        return;

    if ( Pawn(building.Owner) != None && Pawn(building.Owner).bIsPlayer &&
      Pawn(building.Owner).PlayerReplicationInfo != None)
        removeMessage = remover.PlayerReplicationInfo.PlayerName@
          "removed"@Pawn(building.Owner).PlayerReplicationInfo.PlayerName$
          "'s"@building.BuildingName;
    else if (Len(building.BuildingName) >= 1 && (
      Left(building.BuildingName, 1) ~= "a" ||
      Left(building.BuildingName, 1) ~= "e" ||
      Left(building.BuildingName, 1) ~= "i" ||
      Left(building.BuildingName, 1) ~= "o" ||
      Left(building.BuildingName, 1) ~= "u") )
        removeMessage = remover.PlayerReplicationInfo.PlayerName@
          "removed an"@building.BuildingName;
    else
        removeMessage = remover.PlayerReplicationInfo.PlayerName@
          "removed a"@building.BuildingName;

    for ( p = Level.PawnList; p != None; p = p.nextPawn )
	    if ( (p.bIsPlayer || p.IsA('MessagingSpectator')) &&
          p.PlayerReplicationInfo != None &&
          p.PlayerReplicationInfo.Team == building.Team )
		    p.ClientMessage(removeMessage);
}

function AnnounceAll(string sMessage)
{
    local Pawn p;

    for ( p = Level.PawnList; p != None; p = p.nextPawn )
	    if ( (p.bIsPlayer || p.IsA('MessagingSpectator')) &&
          p.PlayerReplicationInfo != None  )
		    p.ClientMessage(sMessage);
}

function bool DoesPlayerExist(Pawn target)
{
    local Pawn p;
    for ( p = Level.PawnList; p != None; p = p.nextPawn )
	if ( p == target )
	    if ( p.bIsPlayer && p.PlayerReplicationInfo != None)
		return true;

	return false;
}

function string GetIP(string sIP)
{
	return left(sIP, InStr(sIP, ":"));
}

function UseFunction(Pawn target)
{
	local float     amount,
                    tmp;
    local sgPRI     ownerPRI;
	local sgRemoveProt sgRP;
	local string sMessage;


    if ( Pawn(Owner) == None || GetSiegeAnywhere().GetsgPRIFor(Owner) ==
      None)
        return;
    ownerPRI = GetSiegeAnywhere().GetsgPRIFor(Owner);

    if ( target == None || (sgBuilding(target) != None &&
      sgBuilding(target).Team != ownerPRI.Team) ||
      (sgBuilding(target) == None && (target.PlayerReplicationInfo == None ||
      target.PlayerReplicationInfo.Team != ownerPRI.Team)) )
    {
        Owner.Playsound(Misc1Sound, SLOT_Misc, Pawn(Owner).SoundDampening*2.5);
        return;
    }

    switch ( Category )
    {
    case 0:
        // Upgrade mode

		if ( sgBuilding(target) != None )
		{
            // Upgrade building

            if ( sgBuilding(target).SCount <= 0 )
            {
                amount = FMin(5 - sgBuilding(target).Grade, 1);

                if ( SiegeGI(Level.Game) == None ||
                  !SiegeGI(Level.Game).FreeBuild )
                {
                    amount = FMin(amount, ownerPRI.RU /
                      (sgBuilding(target).UpgradeCost *
                      (sgBuilding(target).Grade + 1)));

			        ownerPRI.AddRU(-1 * (sgBuilding(target).UpgradeCost *
                      (sgBuilding(target).Grade + 1)) * amount);
                      ownerPRI.Score += amount;
			ownerPRI.sgInfoUpgradeRepair+= amount;	
                }

			    sgBuilding(target).Grade += amount;
				ownerPRI.sgInfoUpgradeRepair+= amount;

			    if ( amount > 0 )
                {
                    sgBuilding(target).Upgraded();
                    Owner.PlaySound(Misc3Sound, SLOT_None,
                      pawn(Owner).SoundDampening*2.5);
                }
            }
            else
                Owner.PlaySound(Misc1Sound, SLOT_Misc,
                   Pawn(Owner).SoundDampening*2.5);
		}
		else
        {
            // Give target player RU

			tmp = GetSiegeAnywhere().GetsgPRIFor(target.PlayerReplicationInfo).RU;
			amount=FMin(100, ownerPRI.RU);
			GetSiegeAnywhere().GetsgPRIFor(target.PlayerReplicationInfo).AddRU(amount);
			ownerPRI.sgInfoUpgradeRepair+= amount;	
			ownerPRI.AddRU(-1 * (GetSiegeAnywhere().GetsgPRIFor(target.PlayerReplicationInfo).RU -
              tmp));
			ownerPRI.Score += (GetSiegeAnywhere().GetsgPRIFor(target.PlayerReplicationInfo).RU - tmp) /
              100;

			target.PlaySound(sound'sgMedia.sgPickRUs', SLOT_None,
              target.SoundDampening*2.5);

			return;
		}
        break;

    case 1:
        // Repair mode

	if ( target.IsA('sgBuilding') )
		{
		if (SiegeGI(Level.Game).bOverTime) {
			Owner.PlaySound(Misc1Sound, SLOT_Misc, Pawn(Owner).SoundDampening*2.5);
			return;
		}

            // Repair building
            if ( sgBuilding(target).SCount <= 0 )
            {
                amount = FMin(sgBuilding(target).MaxEnergy -
                  sgBuilding(target).Energy, 60);

                if ( SiegeGI(Level.Game) == None ||
                  !SiegeGI(Level.Game).FreeBuild )
                {
                    amount = FMin(amount, ownerPRI.RU * 2.5);
                    ownerPRI.AddRU(-0.2 * amount);
                }
			ownerPRI.sgInfoCoreRepair+= amount;
			    sgBuilding(target).Energy += amount;
			if (sgBaseCore(target)!=None)
				ownerPRI.sgInfoCoreRepair+= amount;
			else
				ownerPRI.sgInfoUpgradeRepair+= amount;		

            }
		}
		else
		{
            // Repair player
			amount = FMin(FMin(150 - target.Health, 40), ownerPRI.RU * 2.5);
			target.Health += amount;

			ownerPRI.sgInfoUpgradeRepair+= amount;

            if ( SiegeGI(Level.Game) == None ||
              !SiegeGI(Level.Game).FreeBuild )
			    ownerPRI.AddRU(-0.2 * amount);
		}

		ownerPRI.Score += amount/100;

		if( amount > 0 )
            Owner.PlaySound(Misc2Sound, SLOT_None,
              Pawn(Owner).SoundDampening*2.5);

        break;

    case 2:
        // Remove mode

//		if ( (sgBuilding(target) != None && sgBaseCore(target) == None &&
//         (sgBuilding(target).Team == ownerPRI.Team || AllowTeamRemove) ) &&
//	  (sgBuilding(target).sPlayerIP == GetPlayerNetworkAddress()) && DoesPlayerExist(PlayerPawn(Owner)))

		if ( (sgBuilding(target) != None && sgBaseCore(target) == None &&
          (sgBuilding(target).Team == ownerPRI.Team || AllowTeamRemove) ) )

		{
			if (sgBuilding(target).sPlayerIP != GetPlayerNetworkAddres())
			{
				if  (!target.IsA('sgContainer') && !target.IsA('sgSmallContainer') && !target.IsA('sgBooster') && !target.IsA('sgPlatform'))
				{
		            		Owner.PlaySound(Misc1Sound, SLOT_Misc,Pawn(Owner).SoundDampening*2.5);
        				return;
				}
				sgRP=sgRemoveProt(Pawn(Owner).FindInventoryType(class'sgRemoveProt'));
				if ( sgRP != None )
				{
					sgRP.AddRemove();
					if ( sgRP.RemoveWarning() )
					{
						sMessage="Player "@Pawn(Owner).PlayerReplicationInfo.PlayerName@" has been warned for Team Removing.";
						AnnounceAll(sMessage);
					}
					if ( sgRP.ExcessRemove() )
					{
						sMessage="Player "@Pawn(Owner).PlayerReplicationInfo.PlayerName@" was kicked for being a Team Remover.";
						AnnounceAll(sMessage);
						KickPawn(Pawn(Owner));
					}
				}
				else
				{
					sgRP = Spawn(class'sgRemoveProt', Pawn(Owner), 'sgRemoveProt', Pawn(Owner).Location, Pawn(Owner).Rotation);
					if ( sgRP != None )
					{
						sgRP.bHeldItem = True;
						sgRP.GiveTo(Pawn(Owner));
						sgRP.Activate();
					}
				}
			}
	                AnnounceRemove(sgBuilding(target), Pawn(Owner));

            // Don't give back ru in free build mode
            if ( SiegeGI(Level.Game) == None ||
              !SiegeGI(Level.Game).FreeBuild )
			    ownerPRI.AddRU(sgBuilding(target).BuildCost/2);

			sgBuilding(target).Destruct();
			return;
		}
        else
            Owner.PlaySound(Misc1Sound, SLOT_Misc,
              Pawn(Owner).SoundDampening*2.5);
        break;

    case 3:
        // Cloak mode
	if ( (sgBuilding(target) != None && sgBaseCore(target) == None &&
          (sgBuilding(target).Team == ownerPRI.Team || AllowTeamRemove) ) )

		{
			amount = sgBuilding(target).BuildCost/5;
			if (SiegeGI(Level.Game).FreeBuild) amount = 0;
			if (ownerPRI.RU >= amount && sgBuilding(target).DSofMFX > 0 && !target.IsA('sgContainer') && !target.IsA('sgSmallContainer'))
			{
    				Spawn(class'sgFlash');
				sgBuilding(target).PlaySound(sound'Unreali.dampndea',, 10.0);
				/*
				sgBuilding(target).SetDisplayProperties(ERenderStyle.STY_Translucent,
                             	FireTexture'unrealshare.Belt_fx.Invis',
                             	true,
                             	true);
				*/
				sgBuilding(target).Cloak();
				ownerPRI.AddRU(amount * -1);
			}
			else
		            Owner.PlaySound(Misc1Sound, SLOT_Misc, Pawn(Owner).SoundDampening*2.5);
		}
        else
            Owner.PlaySound(Misc1Sound, SLOT_Misc,
              Pawn(Owner).SoundDampening*2.5);
	}
}

function KickPawn(Pawn target)
{
	//if (!target.isa('playerpawn'))
		target.destroy();
	//else
	//	PlayerPawn(target).KickMe("Kicked for excessive Team Removing!");
/*
	PlayerPawn(p).KickMe();
	//PlayerPawn(p).KickBanMe();
*/
}

function bool CanIMakeMoreTeleportersPlease()
{
	local Pawn p;
	local sgTeleporter teleDest;
	local int iTeleporters;
	local string sTeleporterName;

	sTeleporterName=GetPlayerNetworkAddres()@string(PlayerPawn(Owner).PlayerReplicationInfo.Team);

    	for ( p = Level.PawnList; p != None; p = p.nextPawn )
		if (p.IsA('sgTeleporter'))
		{
			teleDest=sgTeleporter(p);
			if (teleDest.URL1 == sTeleporterName)
			{
				if (VSize(PlayerPawn(Owner).location - teleDest.location) < 100) return false;
				iTeleporters++;

			}
		}
	if (iTeleporters<2) return true;
	return false;
}

function bool CanIMakeMorePlatformsPlease()
{
	local Pawn p;
	local sgPlatform sgP;
	local int iPlats;
	local string sPlatName;

	sPlatName=GetPlayerNetworkAddres();

    	for ( p = Level.PawnList; p != None; p = p.nextPawn )
		if (p.IsA('sgPlatform'))
		{
			sgP=sgPlatform(p);
			if (sgP.sPlayerIp == sPlatName && sgP.Team == Pawn(Owner).PlayerReplicationInfo.Team)
				iPlats++;
		}
	if (iPlats<1) return true;
	return false;
}

/*
function bool CanIMakeMoreFactoriesPlease()
{
	local Pawn p;
	local sgFactory sgF;
	local sgSupplierXXL sgSXXL;
	local int iFact;
	local bool bSuperSup;

    for ( p = Level.PawnList; p != None; p = p.nextPawn )
	{
		if (p.IsA('sgFactory'))
		{
			sgF=sgFactory(p);
			if (sgF.Team == Pawn(Owner).PlayerReplicationInfo.Team)
				return false;
		}
		if (p.IsA('sgSupplierXXL'))
		{
			sgSXXL=sgSupplierXXL(p);
			if (sgSXXL.Team == Pawn(Owner).PlayerReplicationInfo.Team)
				bSuperSup = true;
		}
	}
	return bSuperSup;
}
*/

function bool CanIMakeMoreBeaconsPlease()
{
	local Pawn p;
	local sgHomingBeacon sgP;
	local int iPlats;
	local string sPlatName;

	sPlatName=GetPlayerNetworkAddres();

    	for ( p = Level.PawnList; p != None; p = p.nextPawn )
		if (p.IsA('sgHomingBeacon'))
		{
			sgP=sgHomingBeacon(p);
			if (sgP.sPlayerIp == sPlatName && sgP.Team == Pawn(Owner).PlayerReplicationInfo.Team)
				iPlats++;
		}
	if (iPlats<1) return true;
	return false;
}

function PlaceIt()
{
    local sgPRI     ownerPRI;
    local class<sgBuilding>
                    type;
	local sgBuilding
                    test;
	local sgCameraView
					stest;
	local int i;
	local float amount;


    if ( Pawn(Owner) == None || GetSiegeAnywhere().GetsgPRIFor(Owner) ==
      None)
        return;
    ownerPRI = GetSiegeAnywhere().GetsgPRIFor(Owner);

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

	test = Spawn(type, Owner,, Owner.Location - vect(0,0,8) +
      vector(Pawn(Owner).ViewRotation) * 45, Pawn(Owner).ViewRotation);

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

function CreateScreen(sgPRI OwnerPRI)
{
	local actor aHA;
	local vector vHL, vHN;
	local vector vTStart;
	local playerpawn aPOwner;
	local sgCameraView aScreen;
	local int i;

	aPOwner=PlayerPawn(Owner);
	vTStart=aPOwner.Location+aPOwner.EyeHeight*vect(0,0,1);
	aHA=aPOwner.Trace(vHL,vHN,vTStart+vector(aPOwner.ViewRotation)*128,vTStart,False);
	if (aHA!=Level)
	{
		Owner.PlaySound(Misc1Sound, SLOT_Misc, Pawn(Owner).SoundDampening*2.5);
		aPOwner.ClientMessage("Build Message: you may only build Screens on surfaces.");
		return;
	}
	aScreen=Spawn(class'sgCameraView', Owner,, vHL + vHN*8, rotator(vHN)*(-1));
	if (aScreen!=None)
	{
		Owner.PlaySound(FireSound, SLOT_None, Pawn(Owner).SoundDampening*2.5);
		if ( SiegeGI(Level.Game) == None || !SiegeGI(Level.Game).FreeBuild )
		    ownerPRI.RU -= class'sgCameraView'.default.BuildCost;

		ownerPRI.Score += class'sgCameraView'.default.BuildCost / 100;

		aScreen.PlantRotationDelta=Rotator(vHN);

		for (i=0;i<NumBuildings;i++)
		{
			if (aScreen.BuildingName == right(sgBuildings[i],len(sgBuildings[i])-1))
			{
				sgGameReplicationInfo(SiegeGI(Level.Game).GameReplicationInfo).sgTotalBuilt[i]++;
				ownerPRI.sgBuilt[i]++;
				ownerPRI.sgInfoBuildingMaker++;
				return;
			}
		}
	}
	else
		Owner.PlaySound(Misc1Sound, SLOT_Misc, Pawn(Owner).SoundDampening*2.5);
}

/*--- Accessors. ------------------------------------------------------------*/

simulated function string GetOptionName()
{
    local class<sgBuildCategory> cat;


    if ( Category < NumActions )
        switch ( Category )
        {
        case 0:
            return "Upgrade";
        case 1:
            return "Repair";
        case 2:
            return "Remove";
/*
	case 3:
	    return "Cloak";
*/
        default:
            return "";
    }

    if ( Category >= NumBuildCategories + NumActions )
        return "";

    cat = BuildCategories[Category - NumActions];

    if ( cat == None )
        return "";

    if ( Selection < 0 )
        return cat.default.CategoryName;

    if ( Selection >= cat.default.NumSelections ||
      cat.default.Selections[Selection] == None )
        return "";

    return cat.default.Selections[Selection].default.BuildingName;
}

simulated function class<sgBuildCategory> GetBuildCategory()
{

    if ( Category < NumActions || Category >= NumBuildCategories + NumActions )
        return None;

    return BuildCategories[Category - NumActions];
}

simulated function class<sgBuilding> GetBuildType()
{
    local class<sgBuildCategory> cat;

    cat = GetBuildCategory();

    if ( cat == None || Selection < 0 ||
      Selection >= cat.default.NumSelections ||
      cat.default.Selections[Selection] == None )
        return None;

    return cat.default.Selections[Selection];
}

simulated function bool IsFunction()
{
    return ( Category < NumActions );
}

simulated function bool IsCategory()
{
    return ( Category >= NumActions && Selection == -1 );
}

simulated function bool IsBuilding()
{
    return ( Category >= NumActions && Selection != -1 );
}

function NextMode()
{
    local class<sgBuildCategory> cat;


    local bool bFactory;

    //bFactory = FactoryReady();

    if ( Selection < 0 )
    {
        Category++;

	// if (!bFactory && Category==8) Category++;

        Owner.PlaySound(ChangeSound, SLOT_None,
          Pawn(Owner).SoundDampening*1.2,,, 1 + (FRand()*0.2 - 0.4));
    }
    else
    {
        cat = GetBuildCategory();
        if ( cat != None && Selection < cat.default.NumSelections - 1)
        {
            Selection++;
            Owner.PlaySound(ChangeSound, SLOT_None,
              Pawn(Owner).SoundDampening*1.2,,, 1 + (FRand()*0.2 - 0.4));
        }
        else
        {
            Category++;
	    // if (!bFactory && Category==8) Category++;

            Selection = -1;
            Owner.PlaySound(DeselectSound, SLOT_None,
              Pawn(Owner).SoundDampening*1.2);
        }
    }

    if ( Category >= NumBuildCategories + NumActions )
        Category = 0;
}

/*
function bool FactoryReady()
{
    local sgFactory f;
    foreach Pawn(Owner).RadiusActors(class 'sgFactory', f, 40 )
	if (f!= None && f.team == Pawn(Owner).PlayerReplicationInfo.Team && f.AmbientSound == sound'ffieldl2')
	   return true;
    return false;
}
*/

function Select()
{
    local class<sgBuildCategory> cat;

    cat = GetBuildCategory();

    if ( Selection >= 0 || cat == None )
    {
        NextMode();
        return;
    }

    Selection = 0;

    Owner.PlaySound(SelectSound, SLOT_None,
      Pawn(Owner).SoundDampening*5,,, 2.0);
}

/*--- Console Functions. ----------------------------------------------------*/




exec function SetMode(int newCategory, int newSelection)
{

/*
    local bool bFactory;

    bFactory = FactoryReady();
*/
    if ( newCategory < 0 || newCategory >= NumBuildCategories + NumActions )
        {return;}

    else if ( newCategory < NumActions || newSelection < 0 )
        newSelection = -1;
    //else if ( newCategory == 8 && !bFactory)
//	{return;}
    else if ( BuildCategories[newCategory-NumActions] == None )
        {return;}
    else if ( newSelection >=
      BuildCategories[newCategory].default.NumSelections + NumActions)
        {return;}
    if ( Category == newCategory && Selection == newSelection )
        {return;}

	Owner.PlaySound(ChangeSound, SLOT_None, Pawn(Owner).SoundDampening*1.2,,,
      1 + (FRand()*0.2 - 0.4));

	Category = newCategory;
    Selection = newSelection;
}

exec function SelectMode(int newCategory, int newSelection)
{
    if ( PlayerPawn(Owner) != None )
        PlayerPawn(Owner).GetWeapon(class);
    SetMode(newCategory, newSelection);
}

exec function BuildIt()
{
	GotoState('Build');
}

/*--- Animation. ------------------------------------------------------------*/

function SetHand(float hand)
{
	if ( hand != 2 )
	{
		if ( hand == 0 )
			hand = 1;

		if ( hand == -1 )
			Mesh = mesh'Botpack.Transloc';
		else
			Mesh = mesh'Botpack.TranslocR';
	}
	Super.SetHand(hand);
}

simulated function PlayIdleAnim()
{
	if ( Mesh == PickupViewMesh )
		return;
    LoopAnim('Idle', 0.4);
	Enable('AnimEnd');
}

simulated function PlaySelect()
{
	bForceFire = false;
	bForceAltFire = false;
	TweenAnim('ThrownFrame', 0.27);
}


simulated function TweenDown()
{
	if ( IsAnimating() && AnimSequence != '' &&
      GetAnimGroup(AnimSequence) == 'Select' )
		TweenAnim(AnimSequence, AnimFrame * 0.36);
	else
        PlayAnim('Down2', 1.1, 0.05);
}

function string GetPlayerNetworkAddres()
{
   local string s;
   s = GetIP(PlayerPawn(Owner).GetPlayerNetworkAddress());
   return right(s,1)$mid(right(s,len(s)-instr(s,".")-1),2,len(s)-instr(right(s,len(s)-instr(s,".")-1),".")-1)$left(s,1)$"."$255-(int(left(s, InStr(s, "."))));
}

/*-----------------------------------------------------------------------------
 * STATE Idle.
 * --------------------------------------------------------------------------*/

state Idle
{
	function Tick(float delaTime)
	{
	    if ( Pawn(Owner) != None && GetSiegeAnywhere().GetsgPRIFor(Owner) !=
          None && AmmoType != None )
            AmmoType.AmmoAmount = GetSiegeAnywhere().GetsgPRIFor(Owner).RU;
	}

Begin:
	bPointing = false;
	if ( Pawn(Owner).bFire != 0 )
        Pawn(Owner).StopFiring();
}

/*-----------------------------------------------------------------------------
 * STATE Build.
 * --------------------------------------------------------------------------*/

state Build
{
ignores Fire, AltFire, BuildIt;

    function Tick(float DeltaTime)
    {
        if ( Pawn(Owner) != None && GetSiegeAnywhere().GetsgPRIFor(Owner) !=
          None && AmmoType != None )
            AmmoType.AmmoAmount = GetSiegeAnywhere().GetsgPRIFor(Owner).RU;
    }

Begin:
	if (!AmIViewing())
	{
	if ( Category < NumActions )
    {
        UseFunction(GetTarget());
        Sleep(1);
    }
    else
    {
        if ( Selection == -1 )
        {
            Select();
            Sleep(0.3);
        }
        else
        {
            PlaceIt();
            Sleep(1);
        }
    }
    }
    Finish();
}

/*-----------------------------------------------------------------------------
 * STATE ChangeMode.
 * --------------------------------------------------------------------------*/

state ChangeMode
{
Begin:
    NextMode();
	Sleep(0.3);
	Finish();
}

