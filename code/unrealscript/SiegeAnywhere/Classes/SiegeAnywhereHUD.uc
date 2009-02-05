class SiegeAnywhereHUD extends sgHUD config (SiegeAnywhere);

var config bool bDrawClock;

// BUG TODO: sgHUD.PostRender() throws Accessed None

simulated function PostRender( canvas Canvas ) {
	local int X;
	local float clockX,clockY;

	// Super.PostRender( Canvas );
	sgHUDPostRenderFixed(Canvas);

	if (PlayerOwner.bShowMenu || PlayerOwner.bShowScores)
		return;

	Canvas.Style = Style;

	if (PawnOwner == None || PlayerOwner == None || PawnOwner.PlayerReplicationInfo == None || PlayerOwner.PlayerReplicationInfo == None || PlayerOwner.bShowMenu || PlayerOwner.bShowScores || bForceScores || bShowInfo || PlayerOwner.Scoring != None || PawnOwner.PlayerReplicationInfo.bIsSpectator)
		return; // it was one of the last two that finally stopped the clock from appearing before gamestart

	if (bDrawClock && !bHideHUD && !bHideAllWeapons) { // I decided to hide it if weapons are hidden, and therefore there is nothing else in the top-right.
		if (bHideStatus)
			clockX = Canvas.ClipX - (128-4)*Scale;
		else
			clockX = Canvas.ClipX - (256-4)*Scale;
		if (bHideAllWeapons)
			clockY = 32*Scale;
		else
			clockY = 142*Scale;
		DrawTimeAt(Canvas, clockX, clockY);
	}

}

simulated function DrawTimeAt(Canvas Canvas, float X, float Y) { // Modified
	local int Minutes, Seconds, d;
	local float ClockScale;
	local FastBuildMut fbm;

	if ( PlayerOwner.GameReplicationInfo == None )
		return;

	ClockScale = Scale * 0.75;

	Canvas.DrawColor = WhiteColor;
	Canvas.CurX = X;
	Canvas.CurY = Y;
	Canvas.Style = Style;

	if ( PlayerOwner.GameReplicationInfo.RemainingTime > 0 )
	{
		Minutes = PlayerOwner.GameReplicationInfo.RemainingTime/60;
		Seconds = PlayerOwner.GameReplicationInfo.RemainingTime%60;
	}
	else
	{
		Minutes = 0;
		Seconds = 0;
	}

	/*
	// Won't work.  This is clientside code but the mutator is only serverside!
	foreach AllActors(class'FastBuildMut',fbm) {
		if (fbm.bGameStarted && !fbm.bFastBuildOver) {
			Minutes = fbm.SecondsToGo/60;
			Seconds = fbm.SecondsToGo%60;
		}
		break;
	}
	*/

	if ( Minutes > 0 )
	{
		if ( Minutes >= 10 )
		{
			d = Minutes/10;
			Canvas.DrawTile(Texture'BotPack.HudElements1', ClockScale*25, 64*ClockScale, d*25, 0, 25.0, 64.0);
			Canvas.CurX += 7*ClockScale;
			Minutes= Minutes - 10 * d;
		}
		else
		{
			Canvas.DrawTile(Texture'BotPack.HudElements1', ClockScale*25, 64*ClockScale, 0, 0, 25.0, 64.0);
			Canvas.CurX += 7*ClockScale;
		}

		Canvas.DrawTile(Texture'BotPack.HudElements1', ClockScale*25, 64*ClockScale, Minutes*25, 0, 25.0, 64.0);
		Canvas.CurX += 7*ClockScale;
	} else {
		Canvas.DrawTile(Texture'BotPack.HudElements1', ClockScale*25, 64*ClockScale, 0, 0, 25.0, 64.0);
		Canvas.CurX += 7*ClockScale;
	}
	Canvas.CurX -= 4 * ClockScale;
	Canvas.DrawTile(Texture'BotPack.HudElements1', ClockScale*25, 64*ClockScale, 32, 64, 25.0, 64.0);
	// Canvas.CurX += 3 * ClockScale;
	Canvas.CurX += 1 * ClockScale;

	d = Seconds/10;
	Canvas.DrawTile(Texture'BotPack.HudElements1', ClockScale*25, 64*ClockScale, 25*d, 0, 25.0, 64.0);
	Canvas.CurX += 7*ClockScale;

	Seconds = Seconds - 10 * d;
	Canvas.DrawTile(Texture'BotPack.HudElements1', ClockScale*25, 64*ClockScale, 25*Seconds, 0, 25.0, 64.0);
	Canvas.CurX += 7*ClockScale;
}



// Don't show team scores in Assault games:
simulated function DrawTeam(Canvas Canvas, TeamInfo TI) {
	if (InStr(PlayerOwner.GameReplicationInfo.GameClass,"Assault")>=0) {
		return;
	} else {
		Super.DrawTeam(Canvas,TI);
	}
}

/*
// This one is needed for the constructor view and clock!
simulated function DrawGameSynopsis(Canvas Canvas) {
	// if (Level.Game.IsA('Assault')) {
	if (InStr(PlayerOwner.GameReplicationInfo.GameClass,"Assault")>=0) {
		return;
	} else {
		Super.DrawGameSynopsis(Canvas);
	}
}
*/



// A copy of the original with a fix to prevent Accessed None errors with XXL2dRC1, where we check ViewTarget != None.
simulated function sgHUDPostRenderFixed( canvas Canvas )
{
	local float XL, YL, XPos, YPos, FadeValue;
	local string Message;
	local int M, i, j, k, XOverflow;
	local float OldOriginX;

	local bool bWatchingTV;
	local bool bSiegeStats;
	

	HUDSetup(canvas);

	if ( (PawnOwner == None) || (PlayerOwner.PlayerReplicationInfo == None) )
		return;

	if (Owner.IsA('PlayerPawn') && PlayerPawn(Owner).ViewTarget != None)
		bWatchingTV=PlayerPawn(Owner).ViewTarget.IsA('sgCamera');
	else
		bWatchingTV=False;

	if (Owner.IsA('PlayerPawn'))
		bSiegeStats=(sgPRI(PlayerPawn(Owner).PlayerReplicationInfo).iStatus == 1);
	else
		bSiegeStats=false;

	if ( bShowInfo )
	{
		ServerInfo.RenderInfo( Canvas );
		return;
	}

	Canvas.Font = MyFonts.GetSmallFont( Canvas.ClipX );
	OldOriginX = Canvas.OrgX;
	// Master message short queue control loop.
	Canvas.Font = MyFonts.GetSmallFont( Canvas.ClipX );
	Canvas.StrLen("TEST", XL, YL);
	Canvas.SetClip(768*Scale - 10, Canvas.ClipY);
	bDrawFaceArea = false;
	if ( !bHideFaces && !PlayerOwner.bShowScores && !bForceScores && !bHideHUD
			&& !PawnOwner.PlayerReplicationInfo.bIsSpectator && (Scale >= 0.4) )
	{
		DrawSpeechArea(Canvas, XL, YL);
		bDrawFaceArea = (FaceTexture != None) && (FaceTime > Level.TimeSeconds);
		if ( bDrawFaceArea )
		{
			if ( !bHideHUD && ((PawnOwner.PlayerReplicationInfo == None) || !PawnOwner.PlayerReplicationInfo.bIsSpectator) )
				Canvas.SetOrigin( FMax(YL*4 + 8, 70*Scale) + 7*Scale + 6 + FaceAreaOffset, Canvas.OrgY );
		}
	}

	for (i=0; i<4; i++)
	{
		if ( ShortMessageQueue[i].Message != None )
		{
			j++;

			if ( bResChanged || (ShortMessageQueue[i].XL == 0) )
			{
				if ( ShortMessageQueue[i].Message.Default.bComplexString )
					Canvas.StrLen(ShortMessageQueue[i].Message.Static.AssembleString(
											self,
											ShortMessageQueue[i].Switch,
											ShortMessageQueue[i].RelatedPRI,
											ShortMessageQueue[i].StringMessage),
								   ShortMessageQueue[i].XL, ShortMessageQueue[i].YL);
				else
					Canvas.StrLen(ShortMessageQueue[i].StringMessage, ShortMessageQueue[i].XL, ShortMessageQueue[i].YL);
				Canvas.StrLen("TEST", XL, YL);
				ShortMessageQueue[i].numLines = 1;
				if ( ShortMessageQueue[i].YL > YL )
				{
					ShortMessageQueue[i].numLines++;
					for (k=2; k<4-i; k++)
					{
						if (ShortMessageQueue[i].YL > YL*k)
							ShortMessageQueue[i].numLines++;
					}
				}
			}

			// Keep track of the amount of lines a message overflows, to offset the next message with.
			Canvas.SetPos(6, 2 + YL * YPos);
			YPos += ShortMessageQueue[i].numLines;
			if ( YPos > 4 )
				break;

			if ( ShortMessageQueue[i].Message.Default.bComplexString )
			{
				// Use this for string messages with multiple colors.
				ShortMessageQueue[i].Message.Static.RenderComplexMessage(
					Canvas,
					ShortMessageQueue[i].XL,  YL,
					ShortMessageQueue[i].StringMessage,
					ShortMessageQueue[i].Switch,
					ShortMessageQueue[i].RelatedPRI,
					None,
					ShortMessageQueue[i].OptionalObject
					);
			}
			else
			{
				Canvas.DrawColor = ShortMessageQueue[i].Message.Default.DrawColor;
				Canvas.DrawText(ShortMessageQueue[i].StringMessage, False);
			}
		}
	}

	Canvas.DrawColor = WhiteColor;
	Canvas.SetClip(OldClipX, Canvas.ClipY);
	Canvas.SetOrigin(OldOriginX, Canvas.OrgY);

	if ( PlayerOwner.bShowScores || bForceScores )
	{
		if ( (PlayerOwner.Scoring == None) && (PlayerOwner.ScoringType != None) )
			PlayerOwner.Scoring = Spawn(PlayerOwner.ScoringType, PlayerOwner);
		if ( PlayerOwner.Scoring != None )
		{
			PlayerOwner.Scoring.OwnerHUD = self;
			PlayerOwner.Scoring.ShowScores(Canvas);
			if ( PlayerOwner.Player.Console.bTyping )
				DrawTypingPrompt(Canvas, PlayerOwner.Player.Console);
			return;
		}
	}

	YPos = FMax(YL*4 + 8, 70*Scale);
	if ( bDrawFaceArea )
		DrawTalkFace( Canvas,0, YPos );
	if (j > 0)
	{
		bDrawMessageArea = True;
		MessageFadeCount = 2;
	}
	else
		bDrawMessageArea = False;

	if ( !bHideCenterMessages )
	{
		// Master localized message control loop.
		for (i=0; i<10; i++)
		{
			if (LocalMessages[i].Message != None)
			{
				if (LocalMessages[i].Message.Default.bFadeMessage && Level.bHighDetailMode)
				{
					Canvas.Style = ERenderStyle.STY_Translucent;
					FadeValue = (LocalMessages[i].EndOfLife - Level.TimeSeconds);
					if (FadeValue > 0.0)
					{
						if ( bResChanged || (LocalMessages[i].XL == 0) )
						{
							if ( LocalMessages[i].Message.Static.GetFontSize(LocalMessages[i].Switch) == 1 )
								LocalMessages[i].StringFont = MyFonts.GetBigFont( Canvas.ClipX );
							else // ==2
								LocalMessages[i].StringFont = MyFonts.GetHugeFont( Canvas.ClipX );
							Canvas.Font = LocalMessages[i].StringFont;
							Canvas.StrLen(LocalMessages[i].StringMessage, LocalMessages[i].XL, LocalMessages[i].YL);
							LocalMessages[i].YPos = LocalMessages[i].Message.Static.GetOffset(LocalMessages[i].Switch, LocalMessages[i].YL, Canvas.ClipY);
						}
						Canvas.Font = LocalMessages[i].StringFont;
						Canvas.DrawColor = LocalMessages[i].DrawColor * (FadeValue/LocalMessages[i].LifeTime);
						Canvas.SetPos( 0.5 * (Canvas.ClipX - LocalMessages[i].XL), LocalMessages[i].YPos );
						Canvas.DrawText( LocalMessages[i].StringMessage, False );
					}
				}
				else
				{
					if ( bResChanged || (LocalMessages[i].XL == 0) )
					{
						if ( LocalMessages[i].Message.Static.GetFontSize(LocalMessages[i].Switch) == 1 )
							LocalMessages[i].StringFont = MyFonts.GetBigFont( Canvas.ClipX );
						else // == 2
							LocalMessages[i].StringFont = MyFonts.GetHugeFont( Canvas.ClipX );
						Canvas.Font = LocalMessages[i].StringFont;
						Canvas.StrLen(LocalMessages[i].StringMessage, LocalMessages[i].XL, LocalMessages[i].YL);
						LocalMessages[i].YPos = LocalMessages[i].Message.Static.GetOffset(LocalMessages[i].Switch, LocalMessages[i].YL, Canvas.ClipY);
					}
					Canvas.Font = LocalMessages[i].StringFont;
					Canvas.Style = ERenderStyle.STY_Normal;
					Canvas.DrawColor = LocalMessages[i].DrawColor;
					Canvas.SetPos( 0.5 * (Canvas.ClipX - LocalMessages[i].XL), LocalMessages[i].YPos );
					Canvas.DrawText( LocalMessages[i].StringMessage, False );
				}
			}
		}
	}
	Canvas.Style = ERenderStyle.STY_Normal;

    if ( !bWatchingTV && !bSiegeStats) //OVERRIDE
	if ( !PlayerOwner.bBehindView && (PawnOwner.Weapon != None) && (Level.LevelAction == LEVACT_None) )
	{
		Canvas.DrawColor = WhiteColor;
		PawnOwner.Weapon.PostRender(Canvas);
		if ( !PawnOwner.Weapon.bOwnsCrossHair )
			DrawCrossHair(Canvas, 0,0 );
	}

    if ( !bWatchingTV && !bSiegeStats) //OVERRIDE
	if ( (PawnOwner != Owner) && PawnOwner.bIsPlayer )
	{
		Canvas.Font = MyFonts.GetSmallFont( Canvas.ClipX );
		Canvas.bCenter = true;
		Canvas.Style = ERenderStyle.STY_Normal;
		Canvas.DrawColor = CyanColor * TutIconBlink;
		Canvas.SetPos(4, Canvas.ClipY - 96 * Scale);
		Canvas.DrawText( LiveFeed$PawnOwner.PlayerReplicationInfo.PlayerName, true );
		Canvas.bCenter = false;
		Canvas.DrawColor = WhiteColor;
		Canvas.Style = Style;
	}

	if ( bStartUpMessage && (Level.TimeSeconds < 5) )
	{
		bStartUpMessage = false;
		PlayerOwner.SetProgressTime(7);
	}
	if ( (PlayerOwner.ProgressTimeOut > Level.TimeSeconds) && !bHideCenterMessages )
		DisplayProgressMessage(Canvas);

	// Display MOTD
	if ( MOTDFadeOutTime > 0.0 )
		DrawMOTD(Canvas);

	if( !bHideHUD )
	{
		if ( !PawnOwner.PlayerReplicationInfo.bIsSpectator )
		{
			Canvas.Style = Style;

			// Draw Ammo
			if ( !bWatchingTV && !bSiegeStats) //OVERRIDE
			if ( !bHideAmmo )
				DrawAmmo(Canvas);

			// Draw Health/Armor status
			if ( !bWatchingTV  && !bSiegeStats) //OVERRIDE
			DrawStatus(Canvas);

			// Display Weapons
			if ( !bWatchingTV  && !bSiegeStats) //OVERRIDE
			if ( !bHideAllWeapons )
				DrawWeapons(Canvas);
			else if ( Level.bHighDetailMode
					&& (PawnOwner == PlayerOwner) && (PlayerOwner.Handedness == 2) )
			{
				// if weapon bar hidden and weapon hidden, draw weapon name when it changes
				if ( PawnOwner.PendingWeapon != None )
				{
					WeaponNameFade = 1.0;
					Canvas.Font = MyFonts.GetBigFont( Canvas.ClipX );
					Canvas.DrawColor = PawnOwner.PendingWeapon.NameColor;
					Canvas.SetPos(Canvas.ClipX - 360 * Scale, Canvas.ClipY - 64 * Scale);
					Canvas.DrawText(PawnOwner.PendingWeapon.ItemName, False);
				}
				else if ( (Level.NetMode == NM_Client)
						&& PawnOwner.IsA('TournamentPlayer') && (TournamentPlayer(PawnOwner).ClientPending != None) )
				{
					WeaponNameFade = 1.0;
					Canvas.Font = MyFonts.GetBigFont( Canvas.ClipX );
					Canvas.DrawColor = TournamentPlayer(PawnOwner).ClientPending.NameColor;
					Canvas.SetPos(Canvas.ClipX - 360 * Scale, Canvas.ClipY - 64 * Scale);
					Canvas.DrawText(TournamentPlayer(PawnOwner).ClientPending.ItemName, False);
				}
				else if ( (WeaponNameFade > 0) && (PawnOwner.Weapon != None) )
				{
					Canvas.Font = MyFonts.GetBigFont( Canvas.ClipX );
					Canvas.DrawColor = PawnOwner.Weapon.NameColor;
					if ( WeaponNameFade < 1 )
						Canvas.DrawColor = Canvas.DrawColor * WeaponNameFade;
					Canvas.SetPos(Canvas.ClipX - 360 * Scale, Canvas.ClipY - 64 * Scale);
					Canvas.DrawText(PawnOwner.Weapon.ItemName, False);
				}
			}
			// Display Frag count
			if ( !bWatchingTV  && !bSiegeStats) //OVERRIDE
			if ( !bAlwaysHideFrags && !bHideFrags )
				DrawFragCount(Canvas);
		}
		// Team Game Synopsis
		if ( !bWatchingTV  && !bSiegeStats) //OVERRIDE
		if ( !bHideTeamInfo )
			DrawGameSynopsis(Canvas);

		// Display Identification Info
		//if ( !bWatchingTV  && !bSiegeStats) //OVERRIDE
		if ( PawnOwner == PlayerOwner )
			DrawIdentifyInfo(Canvas);

		if ( HUDMutator != None )
			HUDMutator.PostRender(Canvas);

		if ( (PlayerOwner.GameReplicationInfo != None) && (PlayerPawn(Owner).GameReplicationInfo.RemainingTime > 0) )
		{
			if ( TimeMessageClass == None )
				TimeMessageClass = class<CriticalEventPlus>(DynamicLoadObject("Botpack.TimeMessage", class'Class'));

			if ( (PlayerOwner.GameReplicationInfo.RemainingTime <= 300)
			  && (PlayerOwner.GameReplicationInfo.RemainingTime != LastReportedTime) )
			{
				LastReportedTime = PlayerOwner.GameReplicationInfo.RemainingTime;
				if ( PlayerOwner.GameReplicationInfo.RemainingTime <= 30 )
				{
					bTimeValid = ( bTimeValid || (PlayerOwner.GameReplicationInfo.RemainingTime > 0) );
					if ( PlayerOwner.GameReplicationInfo.RemainingTime == 30 )
						TellTime(5);
					else if ( bTimeValid && PlayerOwner.GameReplicationInfo.RemainingTime <= 10 )
						TellTime(16 - PlayerOwner.GameReplicationInfo.RemainingTime);
				}
				else if ( PlayerOwner.GameReplicationInfo.RemainingTime % 60 == 0 )
				{
					M = PlayerOwner.GameReplicationInfo.RemainingTime/60;
					TellTime(5 - M);
				}
			}
		}
	}
	if ( PlayerOwner.Player.Console.bTyping )
		DrawTypingPrompt(Canvas, PlayerOwner.Player.Console);

	if ( PlayerOwner.bBadConnectionAlert && (PlayerOwner.Level.TimeSeconds > 5) )
	{
		Canvas.Style = ERenderStyle.STY_Normal;
		Canvas.DrawColor = WhiteColor;
		Canvas.SetPos(Canvas.ClipX - (64*Scale), Canvas.ClipY / 2);
		Canvas.DrawIcon(texture'DisconnectWarn', Scale);
	}

	if ( bWatchingTV )
		RenderCamera(Canvas,sgCamera(PlayerPawn(Owner).ViewTarget));
	else if (bSiegeStats)
		SiegeStats(Canvas);

}

// A copy of the original with fixes to prevent Accessed None errors with XXL2dRC1, checking in the very last if.
simulated function bool DrawIdentifyInfo(canvas Canvas)
{
	local sgTeleporter tel;
	local string s;

	local bool bSiegeStats;

	if (Owner.IsA('PlayerPawn'))
		bSiegeStats=(sgPRI(PlayerPawn(Owner).PlayerReplicationInfo).iStatus == 1);
	else
		bSiegeStats=False;
	if (bSiegeStats) return false;

	if ( !TraceIdentify(Canvas) )
		return false;

	if( sgBuilding(IdentifyPawn) != None )
	{

/*
		if (IdentifyPawn.IsA('sgTeleporter')){
			tel=sgTeleporter(IdentifyPawn);
			DrawTwoColorID(Canvas,
          "Properties", tel.URL1, Canvas.ClipY - 300 * Scale);}
*/

        if ( IdentifyPawn.bDeleteMe )
            return false;


	s = string(sgBuilding(IdentifyPawn).Energy /
          sgBuilding(IdentifyPawn).MaxEnergy * 100);

	s = left(s, InStr(s, ".")+nHUDDecPlaces+1);
			if (right(s,1)==".")
				s = left(s,len(s)-1);

		Canvas.Font = MyFonts.GetBigFont(Canvas.ClipX);
		DrawTwoColorID(Canvas,
          sgBuilding(IdentifyPawn).BuildingName, s,
          Canvas.ClipY - 216 * Scale);

		Canvas.Font = MyFonts.GetBigFont(Canvas.ClipX);

			s = string(sgBuilding(IdentifyPawn).Grade);
			s = left(s, InStr(s, ".")+nHUDDecPlaces+1);
			if (right(s,1)==".")
				s = left(s,len(s)-1);

		DrawTwoColorID(Canvas,
          "Level",
          s,
          Canvas.ClipY - 176 * Scale);
	}
	else
	{
        if( IdentifyTarget != None && IdentifyTarget.PlayerName != "" )
		{
			Canvas.Font = MyFonts.GetBigFont(Canvas.ClipX);
			DrawTwoColorID(Canvas,
              IdentifyTarget.PlayerName,
              string(IdentifyPawn.Health),
              Canvas.ClipY - 256 * Scale);
		}
	}
	if (sgBuilding(IdentifyPawn) != None && sgBuilding(IdentifyPawn).Owner != None && Pawn(sgBuilding(IdentifyPawn).Owner).PlayerReplicationInfo != None )
	{
		Canvas.Font = MyFonts.GetBigFont(Canvas.ClipX);
		DrawTwoColorID(Canvas, "Built by", Pawn(sgBuilding(IdentifyPawn).Owner).PlayerReplicationInfo.PlayerName, Canvas.ClipY - 136 * Scale);
	}
	return true;
}

defaultproperties {
	bDrawClock=True
}

