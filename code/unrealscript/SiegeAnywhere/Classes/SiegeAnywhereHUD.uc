class SiegeAnywhereHUD extends sgHUD;

// TODO: keep a copy of the original HUD we replaced,
//       then maybe we can get that to render too.

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

simulated function PostRender( canvas Canvas )
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

	// if (Owner.IsA('PlayerPawn'))
		// bSiegeStats=(sgPRI(PlayerPawn(Owner).PlayerReplicationInfo).iStatus == 1);
	// else
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

simulated function DrawGameSynopsis(canvas Canvas)
{
    local TournamentGameReplicationInfo
                        GRI;
    local float         XL,XL2,
                        YL,YL2,
                        XOffset,
                        YOffset;
    local int           i,
                        X;
    local string        text;
    local sgConstructor gun;
    local class<sgBuilding>
                        buildType;
    local Inventory     inv;
    local float         fuelPercent;
    local sgPRI		PRI;
    local Jetpack       pack;
    local string 	s;
    local color 	cCol;
    local bool		bSiegeStats;

    local sgBaseCore sgB;

	// PRI = sgPRI(PawnOwner.PlayerReplicationInfo);
	PRI = GetSiegeAnywhere().GetsgPRIFor(PawnOwner);

	if (Owner.IsA('PlayerPawn'))
		bSiegeStats=(PRI.iStatus == 1);
	else
		bSiegeStats=false;
	if (bSiegeStats) return;

	sgB = PRI.Cores[PRI.Team];
	GRI = TournamentGameReplicationInfo(PlayerOwner.GameReplicationInfo);
	if ( GRI != None )
		for ( i = 0; i < 4; i++ )
			DrawTeam(Canvas, GRI.Teams[i]);

	if ( PawnOwner.PlayerReplicationInfo == None ||
      PawnOwner.PlayerReplicationInfo.bIsSpectator ) {
		// Log("SiegeAnywhereHUD.DrawGameSynopsis() - returning because PRI = " $ PawnOwner.PlayerReplicationInfo $ ".");
		return;
	}

	Canvas.Font = MyFonts.GetBigFont(Canvas.ClipX);
	Canvas.DrawColor = WhiteColor;

    Canvas.TextSize("RU:", XL, YL);
    Canvas.TextSize("BaseCore:", XL2, YL2);

    if ( bHideAllWeapons )
	    YOffset = Canvas.ClipY - YL*3;
	else if ( HudScale * WeaponScale * Canvas.ClipX <= Canvas.ClipX - 256 * Scale )
		YOffset = Canvas.ClipY - 64*Scale - YL*3;
	else
		YOffset = Canvas.ClipY - 128*Scale - YL*3;

    Canvas.SetPos(0, YOffset);
    Canvas.Style = ERenderStyle.STY_Masked;
    Canvas.DrawText("RU:", false);

     if (sgB != None)
     {
     	Canvas.SetPos(0, YOffset+YL);
     	Canvas.DrawText("BaseCore:", false);
     }
    if ( PRI != None )
    {

        Canvas.DrawColor = GreyColor;
        Canvas.SetPos(XL, YOffset);
        Canvas.DrawText(""@int(PRI.RU)@"/"@
          int(PRI.MaxRU), false);

	 if (sgB != none)
	 {
        	Canvas.SetPos(XL2, YOffset+YL);

		s = string(sgB.Energy / sgB.MaxEnergy * 100);
		s = left(s, InStr(s, ".")+nHUDDecPlaces+1);
		if (right(s,1)==".")
			s = left(s,len(s)-1);

		fuelPercent = float(s);
		if (fuelPercent > sCore)
			RedColour=256;
		else if (fuelPercent < sCore)
			RedColour = 0;

		if (RedColour<128)
		{
			RedColour+=4;
			cCol.R=128+(128-RedColour);
			cCol.G=RedColour;
			cCol.B=RedColour;
		}
		else if (RedColour>128)
		{
			RedColour-=4;
			cCol.R=RedColour;
			cCol.G=RedColour;
			cCol.B=RedColour;

		}
		else
			cCol=GreyColor;
		
        	Canvas.DrawColor = cCol;
		sCore = fuelPercent;
                Canvas.DrawText(""@s, false);
	 }


        Canvas.SetPos(XL, YOffset+20);
    }



    gun = sgConstructor(PawnOwner.Weapon);
    if ( gun != None )
    {
        text = gun.GetOptionName();

	if ( gun.IsFunction() )
        {
	    Canvas.DrawColor = WhiteColor;
            X = Canvas.ClipX - 160 * Scale;
            Canvas.TextSize(text, XL, YL);
            Canvas.SetPos(X - XL/2, YOffset);
            Canvas.DrawText(text, false);
        }
        else if ( gun.IsCategory() )
        {
            Canvas.DrawColor = GreyColor;
            Canvas.TextSize("Select"@text, XL, YL);
            X = Canvas.ClipX - 160 * Scale - XL/2;
            Canvas.SetPos(X, YOffset);
            Canvas.DrawText("Select", false);

            Canvas.DrawColor = WhiteColor;
            Canvas.TextSize("Select", XL, YL);
            Canvas.SetPos(X + XL, YOffset);
            Canvas.DrawText(""@text, false);

            Canvas.DrawColor = GreyColor;
            X = Canvas.ClipX - 160 * Scale;
            Canvas.TextSize("category", XL, YL);
            Canvas.SetPos(X - XL/2, YOffset + YL);
            Canvas.DrawText("category", false);
        }
        else if ( gun.IsBuilding() )
        {
            Canvas.DrawColor = WhiteColor;
            X = Canvas.ClipX - 160 * Scale;
            Canvas.TextSize(text, XL, YL);
            Canvas.SetPos(X - XL/2, YOffset);
            Canvas.DrawText(text, false);

            buildType = gun.GetBuildType();
            if ( buildType != None )
                text = "Cost:"@buildType.default.BuildCost;
            else
                text = "Cost:";
            Canvas.DrawColor = GreyColor;
            Canvas.TextSize(text, XL, YL);
            Canvas.SetPos(X - XL/2, YOffset + YL);
            Canvas.DrawText(text, false);
        }

    }

    for ( inv = PlayerOwner.Inventory; inv != None; inv = inv.Inventory )
    {
        if ( JetPack(inv) != None )
        {
	    pack = Jetpack(inv);
            Canvas.DrawColor = GreyColor;
            fuelPercent = FClamp(Jetpack(inv).Fuel / Jetpack(inv).MaxFuel,
              0, 1);
            Canvas.SetPos(4, YOffset - YL*2);

	    if ( pack.MaxFuel == 999999999 )
	    {
                 Canvas.DrawTile(texture'SJetStatus', 128, 32, 0, 0, 128, 32);
	    }
	    else
	    {
                 Canvas.DrawTile(texture'JetStatus', 128, 32, 0, 0, 128, 32);
	    }
            Canvas.SetPos(5, YOffset - YL*2);
            Canvas.DrawTile(texture'JetStatusBar', 90 * fuelPercent + 1, 32,
              1, 0, 90 * fuelPercent + 1, 32);
        }
	if ( sgSpeed(inv) != None && sgSpeed(inv).bActivated )
	{
            Canvas.DrawColor = GreyColor;
            Canvas.SetPos(4, YOffset - YL*2 + 26);
            Canvas.DrawTile(texture'SSpeed', 128, 32, 0, 0, 128, 32);
	}
    }
}


simulated function bool DrawIdentifyInfo(canvas Canvas)
{
	local sgTeleporter tel;
	local string s;

	local bool bSiegeStats;

	if (Owner.IsA('PlayerPawn'))
		bSiegeStats=(GetSiegeAnywhere().GetsgPRIFor(PlayerPawn(Owner)).iStatus == 1);
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
	if (sgBuilding(IdentifyPawn) != None && Pawn(sgBuilding(IdentifyPawn).Owner) != None && Pawn(sgBuilding(IdentifyPawn).Owner).PlayerReplicationInfo != None )
	{
		Canvas.Font = MyFonts.GetBigFont(Canvas.ClipX);
		DrawTwoColorID(Canvas, "Built by", Pawn(sgBuilding(IdentifyPawn).Owner).PlayerReplicationInfo.PlayerName, Canvas.ClipY - 136 * Scale);
	}
	return true;
}

