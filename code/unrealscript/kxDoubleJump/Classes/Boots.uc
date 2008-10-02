//================================================================================
// Boots.
//================================================================================

class Boots extends DJ_InventoryItem;

simulated function PreBeginPlay ()
{
  maxJumps = Class'DoubleJumpUT'.Default.maxJumps; // 0x00000013 : 0x0000
  jumpType = Class'DoubleJumpUT'.Default.jumpType; // 0x0000001E : 0x0014
  Super.PreBeginPlay(); // 0x00000029 : 0x0028
}

defaultproperties
{
    ExpireMessage="DoubleJumping is over"

    bAutoActivate=True

    bActivatable=True

    bDisplayableInv=True

    PickupMessage="You picked up the DoubleJumpBoots"

    ItemName="DoubleJumpBoots"

    RespawnTime=30.00

    PickupViewMesh=LodMesh'Botpack.jboot'

    MaxDesireability=0.50

    PickupSound=Sound'UnrealShare.Pickups.GenPickSnd'

    ActivateSound=Sound'Botpack.Pickups.BootSnd'

    Icon=Texture'UnrealI.Icons.I_Boots'

    AmbientGlow=64

    CollisionRadius=22.00

    CollisionHeight=14.00

}
