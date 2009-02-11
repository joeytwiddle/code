class sgItemDoubleJumpBoots extends sgaItem;

/*
function Inventory GiveItem(Pawn Target) {
	local Inventory i;
	i = Super.GiveItem(Target);
	if (i == None || Target.FindInventoryType(InventoryClass)==None) {
		BroadcastMessage(Self$" Pickup of "$InventoryClass$" by "$Target$" failed i="$i$" inv="$Target.FindInventoryType(InventoryClass)$"!");
		Log(Self$" Pickup of "$InventoryClass$" by "$Target$" failed i="$i$" inv="$Target.FindInventoryType(InventoryClass)$"!");
	} else {
		BroadcastMessage(Self$" Pickup of "$InventoryClass$" by "$Target$" SUCCEDED :) i="$i$" inv="$Target.FindInventoryType(InventoryClass)$"!");
		Log(Self$" Pickup of "$InventoryClass$" by "$Target$" SUCCEDED :) i="$i$" inv="$Target.FindInventoryType(InventoryClass)$"!");
	}
	return i;
}

simulated event Timer () {
	local Pawn Target;
	Super.Timer();
	// if ( (SCount > 0) || (Role != 4) ) {
		// return;
	// }
	if ( Role != 4 )
		return;
	foreach RadiusActors(Class'Pawn',Target,48.0) {
		Log(Self$" SCount="$SCount$" Role="$Role$" Owner="$Owner$" ~"$Target$"?");
		if ( ((Target == Owner) || (Owner == None) || Owner.bDeleteMe)
			&& Target.bIsPlayer && (Target.Health > 0)
			&& (Target.PlayerReplicationInfo != None)
			&& (Target.PlayerReplicationInfo.Team == Team)
			// && (GiveItem(Target) != None)
			&& (GiveItem(Target) != None)
		) {
			Destroy();
			return;
		}
	}
}
*/

simulated function PostBeginPlay() {
	Super.PostBeginPlay();
	InventoryType = String(class'DoubleJumpBoots');
}

simulated function OnGive(Pawn Target, Inventory Inv) {
	DoubleJumpBoots(Inv).JumpHeight = DoubleJumpBoots(Inv).JumpHeight * (0.6 + 0.45*Grade/5);
}

defaultproperties {
	// InventoryType="kxDoubleJump.DoubleJumpBoots"
	// InventoryClass=class'kxDoubleJump.DoubleJumpBoots'
	BuildingName="DoubleJump Boots"
	BuildCost=600
	UpgradeCost=50
	Model=LodMesh'Botpack.jboot'
}

