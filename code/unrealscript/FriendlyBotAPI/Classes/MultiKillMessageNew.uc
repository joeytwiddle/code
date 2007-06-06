class MultiKillMessageNew extends MultiKillMessage;


static simulated function ClientReceive( 
	PlayerPawn P,
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2,
	optional Object OptionalObject
	)
{
	Super.ClientReceive(P, Switch, RelatedPRI_1, RelatedPRI_2, OptionalObject);

	switch (Switch)
	{
		case 1:
			//P.ClientPlaySound(sound'Announcer.DoubleKill',, true);
			break;
		case 2:
			//P.ClientPlaySound(sound'Announcer.MultiKill',, true);
			break;
		case 3:
			//P.ClientPlaySound(sound'Announcer.UltraKill',, true);
			break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			//P.ClientPlaySound(sound'Announcer.MonsterKill',, true);
			break;
	}
}

