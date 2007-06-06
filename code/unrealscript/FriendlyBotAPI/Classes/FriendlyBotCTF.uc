class FriendlyBotCTF extends BotCTF
	config(FriendlyBotAPI);

event playerpawn Login (string Portal, string Options, out string Error, class<playerpawn> SpawnClass) {
	local playerpawn newPlayer;

	if (SpawnClass!=class'FriendlyMale1' && SpawnClass!=class'FriendlyFemale1')
		newPlayer = Super.Login(Portal,Options,Error,class'FriendlyMale1');
	else
		newPlayer = Super.Login(Portal,Options,Error,SpawnClass);

	//// Tried removing this; surely super does it ok.
	newPlayer.PlayerReplicationInfo.VoiceType = class<VoicePack>(DynamicLoadObject(newPlayer.VoiceType, class'Class'));
	return newPlayer;
}

function PostBeginPlay() {
	Super.PostBeginPlay();
	bLowGore=true;
     	bVeryLowGore=true;
    if(!bServerLoaded)
    {
    	theVizServer = spawn(class'VizServer',self);
        theVizServer.gameClass = "FriendlyBotCTF";
        
    	theBotServer = spawn(class'BotServer',self);
        theBotServer.gameClass = "FriendlyBotCTF";
        
        bServerLoaded = true;
    }
	RemoteBotConfig = spawn(class'FriendlyRemoteBotInfo',self);
    RemoteBotConfig.Difficulty = BotConfig.Difficulty;
}


event InitGame( string Options, out string Error ) {
	local Mutator M;
	local class<Mutator> MutatorClass;

	Super.InitGame(Options, Error);

	//// DON'T add the weird weapons mutator!

	/*
	// BaseMutator.addmutator(spawn(class'FriendlyBotAPI.FriendlyWeaponMUT'));
	MutatorClass = class<Mutator>(DynamicLoadObject("FriendlyBotAPI.FriendlyWeaponMUT", class'Class'));

	for (M = BaseMutator; M != None; M = M.NextMutator)
		if (M.class == MutatorClass)
			break;
	if (M == None)
		BaseMutator.AddMutator(Spawn(MutatorClass));
	*/
}


// Monitor killed messages for fraglimit
function Killed(pawn killer, pawn Other, name damageType)
{
	local int NextTaunt, i;
	local bool bAutoTaunt, bEndOverTime;
	local Pawn P, Best;
	
	if ( !bFirstBlood )
		if ( Killer.bIsPlayer && (Killer != Other) )
			if (!Self.IsA('TrainingDM'))
			{
				bFirstBlood = True;
				BroadcastLocalizedMessage( class'FirstBloodMessageNew', 0, Killer.PlayerReplicationInfo );
			}

	Super.Killed(killer, Other, damageType);
	if ( Other.bIsPlayer )
				Other.HidePlayer();
			else
				Other.Destroy();
}

function NotifySpree(Pawn Other, int num)
{
	local Pawn P;

	if ( num == 5 )
		num = 0;
	else if ( num == 10 )
		num = 1;
	else if ( num == 15 )
		num = 2;
	else if ( num == 20 )
		num = 3;
	else if ( num == 25 )
		num = 4;
	else
		return;

	for ( P=Level.PawnList; P!=None; P=P.NextPawn )
		if ( P.IsA('TournamentPlayer') )
			P.ReceiveLocalizedMessage( class'KillingSpreeMessageNew', Num, Other.PlayerReplicationInfo );
}

