




class DeathMessageNew extends DeathMessagePlus;


static function string GetString(
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2,
	optional Object OptionalObject 
	)
{
	switch (Switch)
	{
		case 0:
			if (RelatedPRI_1 == None)
				return "";
			if (RelatedPRI_1.PlayerName == "")
				return "";
			if (RelatedPRI_2 == None)
				return "";
			if (RelatedPRI_2.PlayerName == "")
				return "";
			if (Class<Weapon>(OptionalObject) == None)
			{
				return "";
			}
			return class'GameInfo'.Static.ParseKillMessage(
				RelatedPRI_1.PlayerName, 
				RelatedPRI_2.PlayerName,
				Class<Weapon>(OptionalObject).Default.ItemName,
				Class<Weapon>(OptionalObject).Default.DeathMessage
			);
			break;
		case 1: // Suicided
			if (RelatedPRI_1 == None)
				return "";
			if (RelatedPRI_1.bIsFemale)
				return RelatedPRI_1.PlayerName$class'FriendlyBotCTF'.Default.FemaleSuicideMessage;
			else
				return RelatedPRI_1.PlayerName$class'FriendlyBotCTF'.Default.MaleSuicideMessage;
			break;
		case 2: // Fell
			if (RelatedPRI_1 == None)
				return "";
			return RelatedPRI_1.PlayerName$class'FriendlyBotCTF'.Default.FallMessage;
			break;
		case 3: // Eradicated (Used for runes, but not in UT)
			if (RelatedPRI_1 == None)
				return "";
			return RelatedPRI_1.PlayerName$class'FriendlyBotCTF'.Default.ExplodeMessage;
			break;
		case 4:	// Drowned
			if (RelatedPRI_1 == None)
				return "";
			return RelatedPRI_1.PlayerName$class'FriendlyBotCTF'.Default.DrownedMessage;
			break;
		case 5: // Burned
			if (RelatedPRI_1 == None)
				return "";
			return RelatedPRI_1.PlayerName$class'FriendlyBotCTF'.Default.BurnedMessage;
			break;
		case 6: // Corroded
			if (RelatedPRI_1 == None)
				return "";
			return RelatedPRI_1.PlayerName$class'FriendlyBotCTF'.Default.CorrodedMessage;
			break;
		case 7: // Mortared
			if (RelatedPRI_1 == None)
				return "";
			return RelatedPRI_1.PlayerName$class'FriendlyBotCTF'.Default.MortarMessage;
			break;
		case 8: // Telefrag
			if (RelatedPRI_1 == None)
				return "";
			if (RelatedPRI_2 == None)
				return "";
			return class'GameInfo'.Static.ParseKillMessage(
				RelatedPRI_1.PlayerName,
				RelatedPRI_2.PlayerName,
				class'Translocator'.Default.ItemName,
				class'Translocator'.Default.DeathMessage
			);
			break;
	}
}

static function ClientReceive( 
	PlayerPawn P,
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2,
	optional Object OptionalObject
	)
{
	if (RelatedPRI_1 == P.PlayerReplicationInfo)
	{
		// Interdict and send the child message instead.
		if ( TournamentPlayer(P).myHUD != None )
		{
			TournamentPlayer(P).myHUD.LocalizedMessage( Default.ChildMessage, Switch, RelatedPRI_1, RelatedPRI_2, OptionalObject );
			TournamentPlayer(P).myHUD.LocalizedMessage( Default.Class, Switch, RelatedPRI_1, RelatedPRI_2, OptionalObject );
		}

		if ( Default.bIsConsoleMessage )
		{
			TournamentPlayer(P).Player.Console.AddString(Static.GetString( Switch, RelatedPRI_1, RelatedPRI_2, OptionalObject ));
		}

		if (( RelatedPRI_1 != RelatedPRI_2 ) && ( RelatedPRI_2 != None ))
		{
			if ( (TournamentPlayer(P).Level.TimeSeconds - TournamentPlayer(P).LastKillTime < 3) && (Switch != 1) )
			{
				TournamentPlayer(P).MultiLevel++;
				TournamentPlayer(P).ReceiveLocalizedMessage( class'MultiKillMessageNew', TournamentPlayer(P).MultiLevel );
			} 
			else
				TournamentPlayer(P).MultiLevel = 0;
			TournamentPlayer(P).LastKillTime = TournamentPlayer(P).Level.TimeSeconds;
		}
		else
			TournamentPlayer(P).MultiLevel = 0;
		if ( ChallengeHUD(P.MyHUD) != None )
			ChallengeHUD(P.MyHUD).ScoreTime = TournamentPlayer(P).Level.TimeSeconds;
	} 
	else if (RelatedPRI_2 == P.PlayerReplicationInfo) 
	{
		TournamentPlayer(P).ReceiveLocalizedMessage( class'VictimMessageNew', 0, RelatedPRI_1 );
		class'LocalMessage'.static.ClientReceive(P, Switch, RelatedPRI_1, RelatedPRI_2, OptionalObject);
	} 
	else
		Super.ClientReceive(P, Switch, RelatedPRI_1, RelatedPRI_2, OptionalObject);
}


