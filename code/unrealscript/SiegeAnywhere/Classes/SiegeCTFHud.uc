// BUG TODO: sgHUD.PostRender() throws Accessed None

class SiegeCTFHud extends SiegeAnywhereHUD;

var CTFFlag MyFlag;

function Timer() { // From ChallengeCTFHUD
	Super.Timer();

	if ( (PlayerOwner == None) || (PawnOwner == None) )
		return;
	if ( PawnOwner.PlayerReplicationInfo.HasFlag != None )
		PlayerOwner.ReceiveLocalizedMessage( class'CTFMessage2', 0 );
	if ( (MyFlag != None) && !MyFlag.bHome )
		PlayerOwner.ReceiveLocalizedMessage( class'CTFMessage2', 1 );
}

simulated function PostRender( canvas Canvas ) { // From ChallengeCTFHUD, modified
	local int X, Y, i;
	local CTFFlag Flag;
	local bool bAlt;
	local float clockX,clockY;

	// if (FRand()<0.01)
		// Log("PlayerOwner="$ PlayerOwner $" PawnOwner="$ PawnOwner $" PlayerOwner.GRI="$ PlayerOwner.GameReplicationInfo $" PawnOwner.PRI="$ PawnOwner.PlayerReplicationInfo $" bShowMenu="$ PlayerOwner.bShowMenu $" bShowScores="$ PlayerOwner.bShowScores);

	Super.PostRender( Canvas );

	Canvas.Style = Style;

	if( !bHideHUD && !bHideTeamInfo )
	{
		X = Canvas.ClipX - 70 * Scale;
		Y = Canvas.ClipY - 350 * Scale;
			
		for ( i=0; i<4; i++ )
		{
			// Flag = CTFReplicationInfo(PlayerOwner.GameReplicationInfo).FlagList[i];
			Flag = None;
			foreach AllActors(class'CTFFlag',Flag) {
				if (Flag.Team == i)
					break;
			}
			// if (i<2 && FRand()<0.01)
				// Log(Self$": Got Flag="$Flag$" for "$PawnOwner$"/"$PlayerOwner);
			if ( Flag != None )
			{
				Canvas.DrawColor = TeamColor[Flag.Team];
				Canvas.SetPos(X,Y);

				if (PawnOwner != None && PawnOwner.PlayerReplicationInfo != None
					&& Flag.Team == PawnOwner.PlayerReplicationInfo.Team
				)
					MyFlag = Flag;
				if ( Flag.bHome ) 
					Canvas.DrawIcon(texture'botpack.I_Home', Scale * 2);
				else if ( Flag.bHeld )
					Canvas.DrawIcon(texture'botpack.I_Capt', Scale * 2);
				else
					Canvas.DrawIcon(texture'botpack.I_Down', Scale * 2);
			}
			Y -= 150 * Scale;
		}
	}

	/*
	if (PawnOwner == None || PlayerOwner == None || PawnOwner.PlayerReplicationInfo == None || PlayerOwner.PlayerReplicationInfo == None || PlayerOwner.bShowMenu || PlayerOwner.bShowScores || bForceScores || bShowInfo || PlayerOwner.Scoring != None || PawnOwner.PlayerReplicationInfo.bIsSpectator)
		return; // it was one of the last two that finally stopped the clock for appearing before gamestart

	if (!bHideHUD && !bHideAllWeapons && !bHideTeamInfo) { // I decided to hide it if weapons are hidden, and therefore there is nothing else in the top-right.  Alternatively, we could put the clock somewhere else.
		if (bHideStatus)
			clockX = Canvas.ClipX - 128*Scale;
		else
			clockX = Canvas.ClipX - 256*Scale;
		if (bHideAllWeapons)
			clockY = 32*Scale;
		else
			clockY = 142*Scale;
		DrawTimeAt(Canvas, clockX, clockY);
	}
	*/

	// if (FRand()<0.01)
		// Log(Self$": PawnOwner="$PawnOwner$" PlayerOwner="$PlayerOwner$" Flag="$Flag);

	//// Can we get away with disabling Super?  (Was throwing Accessed None since sgPRI(Owner) == None!
	//// No we need it for RU display, crosshair, etc.
	//// We could copy the method from sgHUD and fix the Accessed Nones.

}

simulated function DrawGameSynopsis(Canvas Canvas) { // From ChallengeTeamHUD
	local TournamentGameReplicationInfo GRI;
	local int i;

	Super.DrawGameSynopsis(Canvas); // sgiHUD probably fails to draw anything anyway, because there are no cores and no SiegeGI.

	GRI = TournamentGameReplicationInfo(PlayerOwner.GameReplicationInfo);
	if ( GRI != None )
		for ( i=0 ;i<4; i++ )
			DrawTeam(Canvas, GRI.Teams[i]);
}

simulated function DrawTeam(Canvas Canvas, TeamInfo TI) { // From ChallengeCTFHUD
	local float XL, YL;

	if ( (TI != None) && (TI.Size > 0) )
	{
		Canvas.DrawColor = TeamColor[TI.TeamIndex];
		DrawBigNum(Canvas, int(TI.Score), Canvas.ClipX - 144 * Scale, Canvas.ClipY - 336 * Scale - (150 * Scale * TI.TeamIndex), 1);
	}
}

/*

// From ChallengeTeamHUD:

simulated function HUDSetup(canvas canvas)
{
	Super.HUDSetup(canvas);
	if ( bUseTeamColor && (PawnOwner.PlayerReplicationInfo != None)
		&& (PawnOwner.PlayerReplicationInfo.Team < 4) )
	{
		HUDColor = TeamColor[PawnOwner.PlayerReplicationInfo.Team];
		SolidHUDColor = HUDColor;
		if ( Level.bHighDetailMode )
			HUDColor = Opacity * 0.0625 * HUDColor;
	}
}

simulated function DrawGameSynopsis(Canvas Canvas)
{
	local TournamentGameReplicationInfo GRI;
	local int i;

	Super.DrawGameSynopsis(Canvas);

	GRI = TournamentGameReplicationInfo(PlayerOwner.GameReplicationInfo);
	if ( GRI != None )
		for ( i=0 ;i<4; i++ )
			DrawTeam(Canvas, GRI.Teams[i]);
}

*/

/*

simulated function DrawTeam(Canvas Canvas, TeamInfo TI)
{
	local float XL, YL;

	if ( (TI != None) && (TI.Size > 0) )
	{
		Canvas.Font = MyFonts.GetHugeFont( Canvas.ClipX );
		Canvas.DrawColor = TeamColor[TI.TeamIndex];
		Canvas.SetPos(Canvas.ClipX - 64 * Scale, Canvas.ClipY - (336 + 128 * TI.TeamIndex) * Scale);
		Canvas.DrawIcon(TeamIcon[TI.TeamIndex], Scale);
		Canvas.StrLen(int(TI.Score), XL, YL);
		Canvas.SetPos(Canvas.ClipX - XL - 66 * Scale, Canvas.ClipY - (336 + 128 * TI.TeamIndex) * Scale + ((64 * Scale) - YL)/2 );
		Canvas.DrawText(int(TI.Score), false);
	}
}

// Entry point for string messages.
simulated function Message( PlayerReplicationInfo PRI, coerce string Msg, name MsgType )
{
	local int i;
	local Class<LocalMessage> MessageClass;

	switch (MsgType)
	{
		case 'Say':
			MessageClass = class'SayMessagePlus';
			break;
		case 'TeamSay':
			MessageClass = class'TeamSayMessagePlus';
			break;
		case 'CriticalEvent':
			MessageClass = class'CriticalStringPlus';
			LocalizedMessage( MessageClass, 0, None, None, None, Msg );
			return;
		default:
			MessageClass= class'StringMessagePlus';
			break;
	}
	if ( ClassIsChildOf(MessageClass, class'SayMessagePlus') || 
				     ClassIsChildOf(MessageClass, class'TeamSayMessagePlus') )
	{
		FaceTexture = PRI.TalkTexture;
		FaceTeam = TeamColor[PRI.Team];
		if ( FaceTexture != None )
			FaceTime = Level.TimeSeconds + 3;
		if ( Msg == "" )
			return;
	} 

	for (i=0; i<4; i++)
	{
		if ( ShortMessageQueue[i].Message == None )
		{
			// Add the message here.
			ShortMessageQueue[i].Message = MessageClass;
			ShortMessageQueue[i].Switch = 0;
			ShortMessageQueue[i].RelatedPRI = PRI;
			ShortMessageQueue[i].OptionalObject = None;
			ShortMessageQueue[i].EndOfLife = MessageClass.Default.Lifetime + Level.TimeSeconds;
			if ( MessageClass.Default.bComplexString )
				ShortMessageQueue[i].StringMessage = Msg;
			else
				ShortMessageQueue[i].StringMessage = MessageClass.Static.AssembleString(self,0,PRI,Msg);
			return;
		}
	}

	// No empty slots.  Force a message out.
	for (i=0; i<3; i++)
		CopyMessage(ShortMessageQueue[i],ShortMessageQueue[i+1]);

	ShortMessageQueue[3].Message = MessageClass;
	ShortMessageQueue[3].Switch = 0;
	ShortMessageQueue[3].RelatedPRI = PRI;
	ShortMessageQueue[3].OptionalObject = None;
	ShortMessageQueue[3].EndOfLife = MessageClass.Default.Lifetime + Level.TimeSeconds;
	if ( MessageClass.Default.bComplexString )
		ShortMessageQueue[3].StringMessage = Msg;
	else
		ShortMessageQueue[3].StringMessage = MessageClass.Static.AssembleString(self,0,PRI,Msg);
}

simulated function SetIDColor( Canvas Canvas, int type )
{
	if ( type == 0 )
		Canvas.DrawColor = AltTeamColor[IdentifyTarget.Team] * 0.333 * IdentifyFadeTime;
	else
		Canvas.DrawColor = TeamColor[IdentifyTarget.Team] * 0.333 * IdentifyFadeTime;

}

simulated function bool DrawIdentifyInfo(canvas Canvas)
{
	local float XL, YL, XOffset, X1;
	local Pawn P;

	if ( !Super.DrawIdentifyInfo(Canvas) )
		return false;

	Canvas.StrLen("TEST", XL, YL);
	if( PawnOwner.PlayerReplicationInfo.Team == IdentifyTarget.Team )
	{
		P = Pawn(IdentifyTarget.Owner);
		Canvas.Font = MyFonts.GetSmallFont(Canvas.ClipX);
		if ( P != None )
			DrawTwoColorID(Canvas,IdentifyHealth,string(P.Health), (Canvas.ClipY - 256 * Scale) + 1.5 * YL);
	}
	return true;
}

function DrawTalkFace(Canvas Canvas, int i, float YPos)
{
	if ( !bHideHUD && (PawnOwner.PlayerReplicationInfo != None) && !PawnOwner.PlayerReplicationInfo.bIsSpectator )
	{
		Canvas.Style = ERenderStyle.STY_Normal;
		Canvas.SetPos(FaceAreaOffset, 0);
		Canvas.DrawColor = FaceTeam;
		Canvas.DrawTile(texture'LadrStatic.Static_a00', YPos + 7*Scale, YPos + 7*Scale, 0, 0, texture'FacePanel1'.USize, texture'FacePanel1'.VSize);
		Canvas.DrawColor = WhiteColor;
		Canvas.Style = ERenderStyle.STY_Normal;
		Canvas.SetPos(FaceAreaOffset + 4*Scale, 4*Scale);
		Canvas.DrawTile(FaceTexture, YPos - 1*Scale, YPos - 1*Scale, 0, 0, FaceTexture.USize, FaceTexture.VSize);
		Canvas.Style = ERenderStyle.STY_Translucent;
		Canvas.DrawColor = FaceColor;
		Canvas.SetPos(FaceAreaOffset, 0);
		Canvas.DrawTile(texture'LadrStatic.Static_a00', YPos + 7*Scale, YPos + 7*Scale, 0, 0, texture'LadrStatic.Static_a00'.USize, texture'LadrStatic.Static_a00'.VSize);
		Canvas.DrawColor = WhiteColor;
	}
}

defaultproperties
{
     TeamIcon(0)=Texture'Botpack.Icons.I_TeamR'
     TeamIcon(1)=Texture'Botpack.Icons.I_TeamB'
     TeamIcon(2)=Texture'Botpack.Icons.I_TeamG'
     TeamIcon(3)=Texture'Botpack.Icons.I_TeamY'
     TeamColor(0)=(R=255)
     TeamColor(1)=(G=128,B=255)
     TeamColor(2)=(G=255)
     TeamColor(3)=(R=255,G=255)
     AltTeamColor(0)=(R=200)
     AltTeamColor(1)=(G=94,B=187)
     AltTeamColor(2)=(G=128)
     AltTeamColor(3)=(R=255,G=255,B=128)
     OrderNames(0)=Defend
     OrderNames(1)=Hold
     OrderNames(2)=Attack
     OrderNames(3)=Follow
     OrderNames(4)=Freelance
     OrderNames(5)=Point
     OrderNames(10)=Attack
     OrderNames(11)=Freelance
     NumOrders=5
     ServerInfoClass=Class'Botpack.ServerInfoTeam'
}
*/

defaultproperties {
     ServerInfoClass=Class'Botpack.ServerInfoCTF'
}

