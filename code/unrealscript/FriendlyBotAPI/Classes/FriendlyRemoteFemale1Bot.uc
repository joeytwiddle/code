class FriendlyRemoteFemale1Bot extends RemoteTMale1Bot;

static function SetMultiSkin( actor SkinActor, string SkinName, string FaceName, byte TeamNum )
{
	super.SetMultiSkin(SkinActor,SkinName,FaceName,TeamNum);
	/*
	if (TeamNum==0)
		SkinActor.Skin = Texture'RobeRedFem';
	else 
	{
		SkinActor.Skin = Texture'RobeBlueFem';
		log("Team is "$string(TeamNum));
	}
	*/
	
}

//ADDED BY AWS 5-24-01
function bool Gibbed( name damageType)
{
	return false; 	
}

function SpawnGibbedCarcass()
{
}

function Carcass SpawnCarcass()
{
	return none;
}
//END ADDED BY AWS 5-24-01
