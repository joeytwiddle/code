class sgItemDoubleJumpBoots extends sgaItem;

simulated function PostBeginPlay() {
	Super.PostBeginPlay();
	InventoryType = String(class'DoubleJumpBoots');
}

simulated function OnGive(Pawn Target, Inventory Inv) {
	DoubleJumpBoots(Inv).JumpHeight = DoubleJumpBoots(Inv).JumpHeight * (0.6 + 0.45*Grade/5);
	DoubleJumpBoots(Inv).MaxJumps = Int(FClamp(2.0+Grade/3,2.0,3.0));
}

defaultproperties {
	// InventoryType="kxDoubleJump.DoubleJumpBoots"
	// InventoryClass=class'kxDoubleJump.DoubleJumpBoots'
	BuildingName="DoubleJump Boots"
	BuildCost=600
	UpgradeCost=50
	Model=LodMesh'Botpack.jboot'
}

