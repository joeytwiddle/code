class SiegeAnywhereHUD extends sgHUD;

// BUG TODO: shows TeamScore in Assault, where none is neccessary.

simulated function PostRender( canvas Canvas ) {
	local int X;
	local float clockX,clockY;

	Super.PostRender( Canvas );

	if (PlayerOwner.bShowMenu || PlayerOwner.bShowScores)
		return;

	Canvas.Style = Style;

	if (!bHideHUD && !bHideAllWeapons) { // I decided to hide it if weapons are hidden, and therefore there is nothing else in the top-right.
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

}

simulated function DrawTimeAt(Canvas Canvas, float X, float Y) {
	local int Minutes, Seconds, d;
	local float ClockScale;

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
		Seconds = PlayerOwner.GameReplicationInfo.RemainingTime % 60;
	}
	else
	{
		Minutes = 0;
		Seconds = 0;
	}
	
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
	Canvas.CurX += 3 * ClockScale;

	d = Seconds/10;
	Canvas.DrawTile(Texture'BotPack.HudElements1', ClockScale*25, 64*ClockScale, 25*d, 0, 25.0, 64.0);
	Canvas.CurX += 7*ClockScale;

	Seconds = Seconds - 10 * d;
	Canvas.DrawTile(Texture'BotPack.HudElements1', ClockScale*25, 64*ClockScale, 25*Seconds, 0, 25.0, 64.0);
	Canvas.CurX += 7*ClockScale;
}

