class sgItemForceGun extends sgaItem;

simulated function PostBeginPlay() {
	Super.PostBeginPlay();
	InventoryType = String(class'ForceGun');
}

simulated function OnGive(Pawn Target, Inventory Inv) {
	ForceGun(Inv).PullStrength = ForceGun(Inv).PullStrength * (0.5 + 0.5*Grade/5);
}

defaultproperties {
	// InventoryType="kxForceGun.ForceGun"
	// InventoryClass=class'kxForceGun.ForceGun'
	BuildingName="Force Gun"
	BuildCost=1500
	UpgradeCost=100
	Model=LodMesh'Botpack.ImpPick'
}

