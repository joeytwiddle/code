//================================================================================
// kxGrapple.
//================================================================================

// class kxGrapple extends XPGrapple Config(kxGrapple);
class kxGrapple extends Projectile Config(kxGrapple);

// #exec AUDIO IMPORT FILE="Sounds\Pull.wav" NAME="Pull"
#exec AUDIO IMPORT FILE="Sounds\greset.wav" NAME="Slurp"

var() config bool bDropFlag;
var() config bool bSwingPhysics;
var() config bool bFiddlePhysics;
var() config bool bExtraPower;
var() config float MinRetract;
var() config float UpRotation;
var() config sound RetractSound;

/*
replication
{
  un?reliable if ( Role == 4 )
    pullDest,hNormal,thing,TotalTime,Master,bThingPawn;
}
*/

var Vector pullDest;
var Vector hNormal;
var bool bRight;
var bool bCenter;
var() config float grappleSpeed;
var() config float timeBetweenHit;
var() config float hitdamage;
var bool bPlaySound;
var bool bAttachedToThing;
var() config bool bDoAttachPawn;
var() config bool bLineOfSight;
var Actor thing;
var float TotalTime;
var kx_GrappleLauncher Master;
var bool bThingPawn;

replication
{
  unreliable if ( Role == 4 )
    pullDest,hNormal,thing,TotalTime,Master,bThingPawn;
}

simulated function Destroyed ()
{
  Master.kxGrapple = None; // 0x00000013 : 0x0000
  AmbientSound = None; // 0x0000001D : 0x0010
  Master.AmbientSound = None; // 0x00000021 : 0x0017
  // PlaySound(sound'hit1g',SLOT_Interface,10.0); // 0x0000002B : 0x0027
  // Master.PlaySound(sound'hit1g',SLOT_Interface,10.0); // 0x00000037 : 0x0036
  // PlaySound(sound'Botpack.Translocator.ReturnTarget',SLOT_Interface,1.0); // 0x0000002B : 0x0027
  // Master.PlaySound(sound'Botpack.Translocator.ReturnTarget',SLOT_Interface,1.0); // 0x00000037 : 0x0036
  PlaySound(RetractSound,SLOT_Interface,1.0); // 0x0000002B : 0x0027
  Master.PlaySound(RetractSound,SLOT_Interface,1.0); // 0x00000037 : 0x0036
  // Master.PlaySound(sound'FlyBuzz', SLOT_Interface, 2.5, False, 32, 16);
  Instigator.SetPhysics(PHYS_Falling); // 0x00000049 : 0x004E
  Super.Destroyed(); // 0x00000054 : 0x005C
}

simulated function SetMaster (kx_GrappleLauncher W)
{
  Master = W; // 0x00000013 : 0x0000
  Instigator = Pawn(W.Owner); // 0x00000018 : 0x000B
}

auto state Flying
{
  simulated function ProcessTouch (Actor Other, Vector HitLocation)
  {
    Log("I'm in Process Touch. I've never been here before"); // 0x00000012 : 0x0000
    if ( Inventory(Other) != None ) // 0x00000047 : 0x0035
    {
      Inventory(Other).GiveTo(Instigator); // 0x00000051 : 0x0045
    }
    Instigator.AmbientSound = None; // 0x0000005E : 0x005E
    AmbientSound = None; // 0x00000068 : 0x006E
    Destroy(); // 0x0000006C : 0x0075
    return; // 0x0000006F : 0x0078
  }
  
  simulated function HitWall (Vector HitNormal, Actor Wall)
  {
    PlaySound(sound'hit1g',SLOT_Interface,10.0); // 0x0000002B : 0x0027
    Master.PlaySound(sound'hit1g',SLOT_Interface,10.0); // 0x00000037 : 0x0036
    AmbientSound = Sound'hit1g'; // 0x00000016 : 0x0000
    Master.AmbientSound = Sound'ObjectPush'; // 0x0000001B : 0x000B
    // Master.AmbientSound = Sound'Pull'; // 0x0000001B : 0x000B
    pullDest = Location; // 0x00000026 : 0x001F
    hNormal = HitNormal; // 0x0000002B : 0x002A
    SetPhysics(PHYS_None); // 0x00000032 : 0x0035
    if ( Wall.IsA('Pawn') || Wall.IsA('Mover') ) // 0x00000037 : 0x003A
    {
      if (  !bDoAttachPawn &&  !Wall.IsA('Mover') ) // 0x00000055 : 0x0064
      {
        Destroy(); // 0x00000070 : 0x0087
        return; // 0x00000073 : 0x008A
      }
      bAttachedToThing = True; // 0x00000075 : 0x008C
      thing = Wall; // 0x0000007B : 0x0094
      if ( Wall.IsA('Pawn') ) // 0x00000080 : 0x009F
      {
        bThingPawn = True; // 0x0000008E : 0x00B3
      }
      GotoState('PullTowardDynamic'); // 0x00000093 : 0x00BB
    } else { // 0x00000097 : 0x00C2
      GotoState('PullTowardStatic'); // 0x0000009A : 0x00C5
    }
  }
  
  simulated function BeginState ()
  {
    local rotator outRot;
    outRot = Rotation;
    outRot.Pitch += UpRotation;
    Velocity = vector(outRot) * speed; // 0x00000013 : 0x0000
  }
  
}

state() PullTowardStatic
{
  simulated function Tick (float DeltaTime)
  {
    local float length,outwardPull,power;
    local Vector Inward;

    if (bDropFlag && Instigator.PlayerReplicationInfo.HasFlag != None) {
      CTFFlag(Instigator.PlayerReplicationInfo.HasFlag).Drop(vect(0,0,0));
    }

    if ( bLineOfSight && !Instigator.LineOfSightTo(self) ) // 0x00000022 : 0x0016
    {
      Destroy(); // 0x0000003A : 0x0033
      return; // 0x0000003D : 0x0036
    }
    length = VSize(Instigator.Location - pullDest);
    if ( length <= (MinRetract+8.0) && bSwingPhysics) {
      Instigator.Velocity = Instigator.Velocity*0.99;
    }
    if ( length <= MinRetract ) // 0x0000003F : 0x0038
    {
      if (bSwingPhysics) {
        // Instigator.ClientMessage("Your velocity="$ VSize(Instigator.Velocity));
        Instigator.SetPhysics(PHYS_Falling);
        // Stop the line whirring:
        AmbientSound = None; // 0x00000060 : 0x0065
        Master.AmbientSound = None; // 0x00000064 : 0x006C
        // Dampen:
        // Instigator.AddVelocity( Instigator.Velocity*-0.02 );
      } else {
        // Arrival!
        Instigator.SetPhysics(PHYS_None); // 0x00000055 : 0x0057
        AmbientSound = None; // 0x00000060 : 0x0065
        Master.AmbientSound = None; // 0x00000064 : 0x006C
        Instigator.AddVelocity(Instigator.Velocity * -1); // 0x0000006E : 0x007C
        bPlaySound = False; // 0x00000087 : 0x00A1
      }
    } else { // 0x0000008C : 0x00A9

      if (bSwingPhysics) {

        // if (/*length > 4*MinRetract &&*/ Instigator.Velocity.Z<0) {
          // Instigator.Velocity.Z *= 0.99;
        // }
        outwardPull = Instigator.Velocity Dot Normal(Instigator.Location-pullDest);
        // outwardPull += 3.0*grappleSpeed;
        if (bExtraPower) {
          // power = 2.0 + FMin(3.0, 5.0 * (length - MinRetract) / 1024.0 ); // Too strong sometimes
          // power = 1.0 + FMin(3.0, 5.0 * (length - MinRetract) / 1024.0 );
          // power = 2.0 + FMin(3.0, 5.0 * (length - MinRetract) / 1024.0 );
          power = 3.0 + 4.0 * FMin(1.0, length / 1024.0 );
        } else {
          power = 3.0;
        }
        outwardPull += 0.075*power*grappleSpeed;
        if (outwardPull<0) outwardPull=0;
        // if (outwardPull<1.0) outwardPull=1.0*outwardPull*outwardPull; // Smooth the last inch
        Inward = Normal(pullDest-Instigator.Location) * outwardPull;
        Instigator.AddVelocity( Inward ); // This completely cancels outward momentum - the line length will not increase!
        // Instigator.Velocity = Instigator.Velocity + Inward*DeltaTime; // This makes the line weak and stretchy
        // Dampen gravity (for long pulls)
        if (bFiddlePhysics && Instigator.Velocity.Z<0) {
          // Actually we try to turn it into horizontal motion:
          Instigator.AddVelocity( Normal(Instigator.Velocity) * 0.1*Abs(Instigator.Velocity.Z) );
          Instigator.Velocity.Z *= 0.9;
        }
        // Dampen velocity:
        // Instigator.AddVelocity( Instigator.Velocity*-0.001*DeltaTime );

      } else {

        // Original pull no gravity physics:
        Instigator.Velocity = Normal(pullDest - Instigator.Location) * grappleSpeed; // 0x0000008F : 0x00AC

      }

    }
  }
  
  simulated function ProcessTouch (Actor Other, Vector HitLocation)
  {
    Instigator.AmbientSound = None; // 0x00000014 : 0x0000
    AmbientSound = None; // 0x0000001E : 0x0010
    Destroy(); // 0x00000022 : 0x0017
  }
  
  simulated function BeginState ()
  {
    if (!bSwingPhysics) {
      Instigator.Velocity = Normal(pullDest - Instigator.Location) * grappleSpeed; // 0x00000013 : 0x0000
    }
    Instigator.SetPhysics(PHYS_Falling); // 0x0000002F : 0x002E
    TotalTime = 0.0; // 0x0000003A : 0x003C
  }
  
}

state() PullTowardDynamic
{
  ignores  Tick;
  
  simulated function ProcessTouch (Actor Other, Vector HitLocation)
  {
    if (bDropFlag && Instigator.PlayerReplicationInfo.HasFlag != None) {
      CTFFlag(Instigator.PlayerReplicationInfo.HasFlag).Drop(vect(0,0,0));
    }
    Instigator.AmbientSound = None; // 0x00000014 : 0x0000
    AmbientSound = None; // 0x0000001E : 0x0010
    Destroy(); // 0x00000022 : 0x0017
  }
  
  simulated function BeginState ()
  {
    Instigator.SetPhysics(PHYS_Flying); // 0x00000013 : 0x0000
    TotalTime = 0.0; // 0x0000001E : 0x000E
  }
  
}

defaultproperties
{
    grappleSpeed=600
    timeBetweenHit=0.50
    hitdamage=20.00
    bDoAttachPawn=True
    bLineOfSight=False
    speed=4000.00
    MaxSpeed=4000.00
    MyDamageType=eviscerated
    bNetTemporary=False
    LifeSpan=60.00
    Texture=Texture'UMenu.Icons.Bg41'
    Mesh=LodMesh'UnrealShare.GrenadeM'
    bUnlit=True
    bMeshEnviroMap=True
    bBlockActors=True
    bBlockPlayers=True
    // MinRetract=50
    MinRetract=150
    // MinRetract=200
    // MinRetract=250
    DrawScale=2.0
    RemoteRole=ROLE_SimulatedProxy
    Physics=PHYS_Projectile
    UpRotation=0
    // Physics=PHYS_Falling
    // UpRotation=1536
    bDropFlag=True
    bSwingPhysics=True
    //// These two are cheats in terms of real physics, but may be useful for large maps without handy ceilings.
    bFiddlePhysics=False
    bExtraPower=False
    RetractSound=sound'Slurp'
}

