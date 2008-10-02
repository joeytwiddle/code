//================================================================================
// kx_GrappleLauncher.
//================================================================================

// class kx_GrappleLauncher expands TournamentWeapon
class kx_GrappleLauncher expands XP_GrappleLauncher Config(kxGrapple);

// var Weapon PreviousWeapon;
// var XPGrapple XPGrapple;
// var bool bManualShot;
// var bool bShooting;
// var() config bool bUseExtra0;
// var() config bool bUseExtra1;

defaultproperties
{
    bCanThrow=False
    FireOffset=(X=115.00,Y=15.00,Z=2.00),
    ProjectileClass=Class'kxGrapple'
    AltProjectileClass=Class'kxGrapple'
    DeathMessage="%k removed %o's skeleton with a rusty hook."
    bRotatingPickup=False
    ItemName="kx Grapple Launcher"
    PlayerViewOffset=(X=5.00,Y=-4.00,Z=-7.00),
    StatusIcon=Texture'Botpack.Icons.UseTrans'
    Mass=10.00
    bUseExtra1=True
}
