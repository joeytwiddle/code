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

var config bool bAutoDrop;

function Fire(float Value) {
  Super.AltFire(Value);
}

function AltFire(float Value) {
  /*
  if (!bAutoDrop) { // If you fire when it is already out, retract it:
    if ( XPGrapple != None ) // 0x0000009C : 0x00C2
    {
      AmbientSound = None; // 0x000000A4 : 0x00CD
      XPGrapple.Destroy(); // 0x000000A8 : 0x00D4
      XPGrapple = None; // 0x000000B1 : 0x00E0
      return;
    }
  }
  */
  Super.Fire(Value);
}


function Finish ()
{
  local Pawn PawnOwner;

  if ( bChangeWeapon ) // 0x00000014 : 0x0000
  {
    GotoState('DownWeapon'); // 0x0000001B : 0x0009
    return; // 0x0000001F : 0x0010
  }
  PawnOwner = Pawn(Owner); // 0x00000021 : 0x0012
  if ( PlayerPawn(Owner) == None ) // 0x00000028 : 0x0022
  {
    PawnOwner.StopFiring(); // 0x00000032 : 0x0032
    GotoState('Idle'); // 0x0000003C : 0x0041
    return; // 0x00000040 : 0x0048
  } else { // 0x00000042 : 0x004A
    if ( PlayerPawn(Owner).bExtra1 != 0 ) // 0x00000045 : 0x004D
    {
      Global.Fire(0.0); // 0x00000057 : 0x0067
    } else { // 0x0000005F : 0x0072
      if ( PawnOwner.bFire != 0 ) // 0x00000062 : 0x0075
      {
        Global.Fire(0.0); // 0x00000072 : 0x008A
      } else { // 0x0000007A : 0x0095
        if ( PawnOwner.bAltFire != 0 ) // 0x0000007D : 0x0098
        {
          Global.AltFire(0.0); // 0x0000008D : 0x00AD
        } else { // 0x00000095 : 0x00B8
          GotoState('Idle'); // 0x00000098 : 0x00BB
          if (bAutoDrop) { // If you release primary fire, your grapple retracts
            if ( XPGrapple != None ) // 0x0000009C : 0x00C2
            {
              AmbientSound = None; // 0x000000A4 : 0x00CD
              XPGrapple.Destroy(); // 0x000000A8 : 0x00D4
              XPGrapple = None; // 0x000000B1 : 0x00E0
            }
          } else {
            // Hook continues to fly until you force it to release and retract.
          }
        }
      }
    }
  }
}


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
    bAutoDrop=False
}
