class ChallengeHUDNew extends ChallengeCTFHUD;

/*
#exec TEXTURE IMPORT NAME=HUDElements1New FILE=TEXTURES\HudElementsNew.PCX GROUP="Icons" MIPS=OFF FLAGS=2
*/


simulated function DrawDigit(Canvas Canvas, int d, int Step, float UpScale, out byte bMinus )
{
	if ( bMinus == 1 )
	{
		Canvas.CurX -= Step;
		// TODO had to remove this because of the texture
		// Canvas.DrawTile(Texture'FriendlyBotAPI.HUDElements1New', UpScale*25, 64*UpScale, 0, 64, 25.0, 64.0);
		bMinus = 0;
	}
	if ( d == 1 )
		Canvas.CurX -= 0.625 * Step;
	else
		Canvas.CurX -= 0.25 * Step;		
	// TODO had to remove this because of the texture
	// Canvas.DrawTile(Texture'FriendlyBotAPI.HUDElements1New', UpScale*25, 64*UpScale, d*25, 0, 25.0, 64.0);
	Canvas.CurX += 7*UpScale;
}


simulated function DrawStatus(Canvas Canvas)
{
	local float StatScale, ChestAmount, ThighAmount, H1, H2, X, Y, DamageTime;
	Local int ArmorAmount,CurAbs,i;
	Local inventory Inv,BestArmor;
	local bool bChestArmor, bShieldbelt, bThighArmor, bJumpBoots, bHasDoll;
	local Bot BotOwner;
	local TournamentPlayer TPOwner;
	local texture Doll, DollBelt;

	ArmorAmount = 0;
	CurAbs = 0;
	i = 0;
	BestArmor=None;
	for( Inv=PawnOwner.Inventory; Inv!=None; Inv=Inv.Inventory )
	{ 
		if (Inv.bIsAnArmor) 
		{
			if ( Inv.IsA('UT_Shieldbelt') )
				bShieldbelt = true;
			else if ( Inv.IsA('Thighpads') )
			{
				ThighAmount += Inv.Charge;
				bThighArmor = true;
			}
			else
			{ 
				bChestArmor = true;
				ChestAmount += Inv.Charge;
			}
			ArmorAmount += Inv.Charge;
		}
		else if ( Inv.IsA('UT_JumpBoots') )
			bJumpBoots = true;
		else
		{
			i++;
			if ( i > 100 )
				break; // can occasionally get temporary loops in netplay
		}
	}

	if ( !bHideStatus )
	{	
		TPOwner = TournamentPlayer(PawnOwner);
		if ( Canvas.ClipX < 400 )
			bHasDoll = false;
		else if ( TPOwner != None)
		{
			Doll = TPOwner.StatusDoll;
			DollBelt = TPOwner.StatusBelt;
			bHasDoll = true;
		}
		else
		{
			BotOwner = Bot(PawnOwner);
			if ( BotOwner != None )
			{
				Doll = BotOwner.StatusDoll;
				DollBelt = BotOwner.StatusBelt;
				bHasDoll = true;
			}
		}
		if ( bHasDoll )
		{ 							
			Canvas.Style = ERenderStyle.STY_Translucent;
			StatScale = Scale * StatusScale;
			X = Canvas.ClipX - 128 * StatScale;
			Canvas.SetPos(X, 0);
			if (PawnOwner.DamageScaling > 2.0)
				Canvas.DrawColor = PurpleColor;
			else
				Canvas.DrawColor = HUDColor;
			Canvas.DrawTile(Doll, 128*StatScale, 256*StatScale, 0, 0, 128.0, 256.0);
			Canvas.DrawColor = HUDColor;
			if ( bShieldBelt )
			{
				Canvas.DrawColor = BaseColor;
				Canvas.DrawColor.B = 0;
				Canvas.SetPos(X, 0);
				Canvas.DrawIcon(DollBelt, StatScale);
			}
			if ( bChestArmor )
			{
				ChestAmount = FMin(0.01 * ChestAmount,1);
				Canvas.DrawColor = HUDColor * ChestAmount;
				Canvas.SetPos(X, 0);
				Canvas.DrawTile(Doll, 128*StatScale, 64*StatScale, 128, 0, 128, 64);
			}
			if ( bThighArmor )
			{
				ThighAmount = FMin(0.02 * ThighAmount,1);
				Canvas.DrawColor = HUDColor * ThighAmount;
				Canvas.SetPos(X, 64*StatScale);
				Canvas.DrawTile(Doll, 128*StatScale, 64*StatScale, 128, 64, 128, 64);
			}
			if ( bJumpBoots )
			{
				Canvas.DrawColor = HUDColor;
				Canvas.SetPos(X, 128*StatScale);
				Canvas.DrawTile(Doll, 128*StatScale, 64*StatScale, 128, 128, 128, 64);
			}
			Canvas.Style = Style;
			if ( (PawnOwner == PlayerOwner) && Level.bHighDetailMode && !Level.bDropDetail )
			{
				for ( i=0; i<4; i++ )
				{
					DamageTime = Level.TimeSeconds - HitTime[i];
					if ( DamageTime < 1 )
					{
						Canvas.SetPos(X + HitPos[i].X * StatScale, HitPos[i].Y * StatScale);
						if ( (HUDColor.G > 100) || (HUDColor.B > 100) )
							Canvas.DrawColor = RedColor;
						else
							Canvas.DrawColor = (WhiteColor - HudColor) * FMin(1, 2 * DamageTime);
						Canvas.DrawColor.R = 255 * FMin(1, 2 * DamageTime);
						// TODO had to remove this because of the texture
						// Canvas.DrawTile(Texture'FriendlyBotAPI.HUDElements1New', StatScale * HitDamage[i] * 25, StatScale * HitDamage[i] * 64, 0, 64, 25.0, 64.0);
					}
				}
			}
		}
	}
	Canvas.DrawColor = HUDColor;
	if ( bHideStatus && bHideAllWeapons )
	{
		X = 0.5 * Canvas.ClipX;
		Y = Canvas.ClipY - 64 * Scale;
	}
	else
	{
		X = Canvas.ClipX - 128 * StatScale - 140 * Scale;
		Y = 64 * Scale;
	}
	Canvas.SetPos(X,Y);
	if ( PawnOwner.Health < 50 )
	{
		H1 = 1.5 * TutIconBlink;
		H2 = 1 - H1;
		Canvas.DrawColor = WhiteColor * H2 + (HUDColor - WhiteColor) * H1;
	}
	else
		Canvas.DrawColor = HUDColor;
	// TODO had to remove this because of the texture
	// Canvas.DrawTile(Texture'FriendlyBotAPI.HUDElements1New', 128*Scale, 64*Scale, 128, 128, 128.0, 64.0);

	if ( PawnOwner.Health < 50 )
	{
		H1 = 1.5 * TutIconBlink;
		H2 = 1 - H1;
		Canvas.DrawColor = Canvas.DrawColor * H2 + (WhiteColor - Canvas.DrawColor) * H1;
	}
	else
		Canvas.DrawColor = WhiteColor;

	DrawBigNum(Canvas, Max(0, PawnOwner.Health), X + 4 * Scale, Y + 16 * Scale, 1);

	Canvas.DrawColor = HUDColor;
	if ( bHideStatus && bHideAllWeapons )
	{
		X = 0.5 * Canvas.ClipX - 128 * Scale;
		Y = Canvas.ClipY - 64 * Scale;
	}
	else
	{
		X = Canvas.ClipX - 128 * StatScale - 140 * Scale;
		Y = 0;
	}
	Canvas.SetPos(X, Y);
	// TODO had to remove this because of the texture
	// Canvas.DrawTile(Texture'FriendlyBotAPI.HUDElements1New', 128*Scale, 64*Scale, 0, 192, 128.0, 64.0);
	if ( bHideStatus && bShieldBelt )
		Canvas.DrawColor = GoldColor;
	else
		Canvas.DrawColor = WhiteColor;
	DrawBigNum(Canvas, Min(150,ArmorAmount), X + 4 * Scale, Y + 16 * Scale, 1);
}


simulated function DrawAmmo(Canvas Canvas)
{
	local int X,Y;

	Canvas.Style = Style;
	Canvas.DrawColor = HUDColor;
	if ( bHideAllWeapons || (HudScale * WeaponScale * Canvas.ClipX <= Canvas.ClipX - 256 * Scale) )
		Y = Canvas.ClipY - 63.5 * Scale;
	else
		Y = Canvas.ClipY - 127.5 * Scale;
	if ( bHideAllWeapons )
		X = 0.5 * Canvas.ClipX + 128 * Scale;
	else
		X = Canvas.ClipX - 128 * Scale;
	Canvas.SetPos(X, Y);
	// TODO had to remove this because of the texture
	// Canvas.DrawTile(Texture'FriendlyBotAPI.HUDElements1New', 128*Scale, 64*Scale, 128, 192, 128.0, 64.0);

	if ( (PawnOwner.Weapon == None) || (PawnOwner.Weapon.AmmoType == None) )
		return;

	Canvas.DrawColor = WhiteColor;
	DrawBigNum(Canvas, PawnOwner.Weapon.AmmoType.AmmoAmount, X + 4 * Scale, Y + 16 * Scale);
}


simulated function DrawWeapons(Canvas Canvas)
{
	local Weapon W, WeaponSlot[11];
	local inventory Inv;
	local int i, j, BaseY, BaseX, Pending, WeapX, WeapY;
	local float AmmoScale, WeaponOffset, WeapScale, WeaponX, TexX, TexY;

	BaseX = 0.5 * (Canvas.ClipX - HudScale * WeaponScale * Canvas.ClipX);
	WeapScale = WeaponScale * Scale;
	Canvas.Style = Style;
	BaseY = Canvas.ClipY - 63.5 * WeapScale;
	WeaponOffset = 0.1 * HUDScale * WeaponScale * Canvas.ClipX;

	if ( PawnOwner.Weapon != None )
	{
		W = PawnOwner.Weapon;
		if ( (Opacity > 8) || !Level.bHighDetailMode )
			Canvas.Style = ERenderStyle.STY_Normal;
		WeaponX = BaseX + (W.InventoryGroup - 1) * WeaponOffset;
		Canvas.CurX = WeaponX;
		Canvas.CurY = BaseY;
		Canvas.DrawColor = SolidHUDColor;
		Canvas.DrawIcon(W.StatusIcon, WeapScale);
		Canvas.DrawColor = GoldColor;
		Canvas.CurX = WeaponX + 4 * WeapScale;
		Canvas.CurY = BaseY + 4 * WeapScale;
		Canvas.Style = Style;
		// TODO had to remove this because of the texture
		/*
		if ( W.InventoryGroup == 10 )
			Canvas.DrawTile(Texture'FriendlyBotAPI.HUDElements1New', 0.75 * WeapScale * 25, 0.75 * WeapScale * 64, 0, 0, 25.0, 64.0);
		else
			Canvas.DrawTile(Texture'FriendlyBotAPI.HUDElements1New', 0.75 * WeapScale * 25, 0.75 * WeapScale * 64, 25*W.InventoryGroup, 0, 25.0, 64.0);
		*/

		WeaponSlot[W.InventoryGroup] = W;  
		Canvas.CurX = WeaponX;
		Canvas.CurY = BaseY;
		Canvas.DrawTile(Texture'BotPack.HUDWeapons', 128 * WeapScale, 64 * WeapScale, 128, 64, 128, 64);
	}
	if ( Level.bHighDetailMode && (PawnOwner.PendingWeapon != None) )
	{
		Pending = PawnOwner.PendingWeapon.InventoryGroup;
		Canvas.CurX = BaseX + (Pending - 1) * WeaponOffset - 64 * WeapScale;
		Canvas.CurY = Canvas.ClipY - 96 * WeapScale; 
		Canvas.Style = ERenderStyle.STY_Translucent;
		Canvas.DrawColor = GoldColor;
		Canvas.DrawTile(Texture'BotPack.HUDWeapons', 256 * WeapScale, 128 * WeapScale, 0, 128, 256.0, 128.0);
	}
	else
		Pending = 100;

	Canvas.Style = Style;
	i = 0;
	for ( Inv=PawnOwner.Inventory; Inv!=None; Inv=Inv.Inventory )
	{
		if ( Inv.IsA('Weapon') && (Inv != PawnOwner.Weapon) )
		{
			W = Weapon(Inv);
			if ( WeaponSlot[W.InventoryGroup] == None )
				WeaponSlot[W.InventoryGroup] = W;
			else if ( (WeaponSlot[W.InventoryGroup] != PawnOwner.Weapon)
					&& ((W == PawnOwner.PendingWeapon) || (WeaponSlot[W.InventoryGroup].AutoSwitchPriority < W.AutoSwitchPriority)) )
				WeaponSlot[W.InventoryGroup] = W;
		}
		i++;
		if ( i > 100 )
			break; // can occasionally get temporary loops in netplay
	}
	W = PawnOwner.Weapon;

	// draw weapon list
	TexX = 128 * WeapScale;
	TexY = 64 * WeapScale;
	for ( i=1; i<11; i++ )
	{
		if ( WeaponSlot[i] == None )
		{
			Canvas.Style = Style;
			Canvas.DrawColor =  0.5 * HUDColor;
			Canvas.CurX = BaseX + (i - 1) * WeaponOffset;
			Canvas.CurY = BaseY;
			
			WeapX = ((i-1)%4) * 64;
			WeapY = ((i-1)/4) * 32;
			Canvas.DrawTile(Texture'BotPack.HUDWeapons',TexX,TexY,WeapX,WeapY,64.0,32.0);
		}
		else if ( WeaponSlot[i] != W )
		{
			if ( Pending == i )
			{
				if ( (Opacity > 8) || !Level.bHighDetailMode )
					Canvas.Style = ERenderStyle.STY_Normal;
				Canvas.DrawColor = SolidHUDColor;
			}
			else
			{
				Canvas.Style = Style;
				Canvas.DrawColor = 0.5 * HUDColor;
			}
			Canvas.CurX = BaseX + (i - 1) * WeaponOffset;
			Canvas.CurY = BaseY;
			
			if ( WeaponSlot[i].bSpecialIcon )
				Canvas.DrawIcon(WeaponSlot[i].StatusIcon, WeapScale);
			else
			{
				WeapX = ((i-1)%4) * 64;
				WeapY = ((i-1)/4) * 32;
				Canvas.DrawTile(Texture'BotPack.HUDWeapons',TexX,TexY,WeapX,WeapY,64.0,32.0);
			}
		}
	}

	//draw weapon numbers and ammo
	TexX = 0.75 * WeapScale * 25;
	TexY = 0.75 * WeapScale * 64;
	for ( i=1; i<11; i++ )
	{
		if ( WeaponSlot[i] != None )
		{
			WeaponX = BaseX + (i - 1) * WeaponOffset + 4 * WeapScale;
			if ( WeaponSlot[i] != W )
			{
				Canvas.CurX = WeaponX;
				Canvas.CurY = BaseY + 4 * WeapScale;
				Canvas.DrawColor = GoldColor;
				if ( (Opacity > 8) || !Level.bHighDetailMode )
					Canvas.Style = ERenderStyle.STY_Normal;
				else
					Canvas.Style = Style;
				// TODO had to remove this because of the texture
				/*
				if ( i == 10 )
					Canvas.DrawTile(Texture'FriendlyBotAPI.HUDElements1New', TexX, TexY, 0, 0, 25.0, 64.0);
				else
					Canvas.DrawTile(Texture'FriendlyBotAPI.HUDElements1New', TexX, TexY, 25*i, 0, 25.0, 64.0);
				*/
			}
			if ( WeaponSlot[i].AmmoType != None )
			{
				// Draw Ammo bar
				Canvas.CurX = WeaponX;
				Canvas.CurY = BaseY + 52 * WeapScale;
				Canvas.DrawColor = BaseColor;
				AmmoScale = FClamp(88.0 * WeapScale * WeaponSlot[i].AmmoType.AmmoAmount/WeaponSlot[i].AmmoType.MaxAmmo, 0, 88);
				// TODO had to remove this because of the texture
				// Canvas.DrawTile(Texture'FriendlyBotAPI.HUDElements1New', AmmoScale, 8 * WeapScale,64,64,128.0,8.0);
			}
		}
	}
}


simulated function DrawFragCount(Canvas Canvas)
{
	local float Whiten;
	local int X,Y;

	if ( PawnOwner.PlayerReplicationInfo == None )
		return;

	Canvas.Style = Style;
	if ( bHideAllWeapons || (HudScale * WeaponScale * Canvas.ClipX <= Canvas.ClipX - 256 * Scale) )
		Y = Canvas.ClipY - 63.5 * Scale;
	else
		Y = Canvas.ClipY - 127.5 * Scale;
	if ( bHideAllWeapons )
		X = 0.5 * Canvas.ClipX - 256 * Scale;
	Canvas.CurX = X;
	Canvas.CurY = Y;
	Canvas.DrawColor = HUDColor; 
	Whiten = Level.TimeSeconds - ScoreTime;
	if ( Whiten < 3.0 )
	{
		if ( HudColor == GoldColor )
			Canvas.DrawColor = WhiteColor;
		else
			Canvas.DrawColor = GoldColor;
		if ( Level.bHighDetailMode )
		{
			Canvas.CurX = X - 64 * Scale;
			Canvas.CurY = Y - 32 * Scale;
			Canvas.Style = ERenderStyle.STY_Translucent;
			Canvas.DrawTile(Texture'BotPack.HUDWeapons', 256 * Scale, 128 * Scale, 0, 128, 256.0, 128.0);
		}
		Canvas.CurX = X;
		Canvas.CurY = Y;
		Whiten = 4 * Whiten - int(4 * Whiten);
		Canvas.DrawColor = Canvas.DrawColor + (HUDColor - Canvas.DrawColor) * Whiten;
	}

	// TODO had to remove this because of the texture
	// Canvas.DrawTile(Texture'FriendlyBotAPI.HUDElements1New', 128*Scale, 64*Scale, 0, 128, 128.0, 64.0);
	Canvas.DrawColor = WhiteColor;
	DrawBigNum(Canvas, PawnOwner.PlayerReplicationInfo.Score, X + 40 * Scale, Y + 16 * Scale);
}


