
class FirstBloodMessageNew extends FirstBloodMessage;

static simulated function ClientReceive( 
	PlayerPawn P,
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2,
	optional Object OptionalObject
	)
{
	class'CriticalEventPlus'.static.ClientReceive(P, Switch, RelatedPRI_1, RelatedPRI_2, OptionalObject);

	if (RelatedPRI_1 != P.PlayerReplicationInfo)
		return;

	/* PLAY SOUND HERE
	
	P.ClientPlaySound(sound'Announcer.FirstBlood',, true);
	*/
}


