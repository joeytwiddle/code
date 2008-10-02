//================================================================================
// kxGrapple.
//================================================================================

// TODO: fires on spawn :f
// TODO: makes amp sounds when used with amp
// TODO: sometimes trying to hook when we are right next to the walls fails - we need an audio indicator of that (sometimes we hear the ThrowSound twice)
// TODO: Wall hit sound could be a bit better / should be clear + distinct from throw sound.
// TODO: Removes Translocator ok, but seems to remove ImpactHammer too!
// TODO: When I switch to this weapon I must wait a moment before I can primary fire, unlike the Translocator.
// DONE: Clicking with both fire buttons should return to previous weapon, as with Translocator.

// class kxGrapple extends XPGrapple Config(kxGrapple);
class kxGrapple extends Projectile Config(kxGrapple);

// #exec AUDIO IMPORT FILE="Sounds\Pull.wav" NAME="Pull" // grindy windy one from ND
#exec AUDIO IMPORT FILE="Sounds\greset.wav" NAME="Slurp" // metallic slurp from ND
// #exec AUDIO IMPORT FILE="Sounds\End.wav" NAME="KrrChink" // kchink when grapple hits target
#exec AUDIO IMPORT FILE="Sounds\hit1g.wav" NAME="hit1g" // From UnrealI.GasBag

var() config bool bDropFlag;
var() config bool bSwingPhysics;
var() config bool bFiddlePhysics1;
var() config bool bFiddlePhysics2;
var() config bool bExtraPower;
var() config float MinRetract;
var() config float ThrowAngle;
var() config sound ThrowSound,HitSound,PullSound,RetractSound;

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
  // PlaySound(sound'UnrealI.hit1g',SLOT_Interface,10.0); // 0x0000002B : 0x0027
  // Master.PlaySound(sound'UnrealI.hit1g',SLOT_Interface,10.0); // 0x00000037 : 0x0036
  // PlaySound(sound'Botpack.Translocator.ReturnTarget',SLOT_Interface,1.0); // 0x0000002B : 0x0027
  // Master.PlaySound(sound'Botpack.Translocator.ReturnTarget',SLOT_Interface,1.0); // 0x00000037 : 0x0036
  PlaySound(RetractSound,SLOT_Interface,2.0,,,240); // 0x0000002B : 0x0027
  Master.PlaySound(RetractSound,SLOT_Interface,2.0,,,240); // 0x00000037 : 0x0036
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
    Log("kxGrapple.ProcessTouch() I've never been here before"); // 0x00000012 : 0x0000
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
    //// I could not hear the ones done with AmbientSound.
    if (grappleSpeed>0) {
      Master.AmbientSound = PullSound;
      AmbientSound = PullSound;
    } else {
      Master.AmbientSound = None;
      AmbientSound = None;
    }
    PlaySound(HitSound,SLOT_None,10.0);
    Master.PlaySound(HitSound,SLOT_None,10.0);
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
    CheckFlagDrop();
  }
  
  simulated function BeginState ()
  {
    local rotator outRot;
    outRot = Rotation;
    outRot.Pitch += ThrowAngle*8192/45;
    Velocity = vector(outRot) * speed; // 0x00000013 : 0x0000
  }
  
}

function OnPull(float DeltaTime) {
  CheckFlagDrop();
  TotalTime += DeltaTime;
  // After 2 seconds on the grappling line, if we have switched weapon away from grapple, then Jump will un-grapple us!
  if (PlayerPawn(Master.Owner)!=None && PlayerPawn(Master.Owner).bPressedJump && TotalTime>=2.0 && kx_GrappleLauncher(PlayerPawn(Master.Owner).Weapon) == None) {
    Destroy();
  }
}

function CheckFlagDrop() {
  if (bDropFlag && Instigator.PlayerReplicationInfo.HasFlag != None) {
    CTFFlag(Instigator.PlayerReplicationInfo.HasFlag).Drop(vect(0,0,0));
  }
}

state() PullTowardStatic
{
  simulated function Tick (float DeltaTime)
  {
    local float length,outwardPull,linePull,power;
    local Vector Inward;

    OnPull(DeltaTime);
    if ( bLineOfSight && !Instigator.LineOfSightTo(self) ) // 0x00000022 : 0x0016
    {
      // TODO: For more realism, we could reposition the grapple at the new point the line swung around.
      Destroy(); // 0x0000003A : 0x0033
      return; // 0x0000003D : 0x0036
    }
    length = VSize(Instigator.Location - pullDest);
    if ( length <= (MinRetract+8.0) && bSwingPhysics) {
      Instigator.Velocity = Instigator.Velocity*0.99;
    }
    if ( length <= MinRetract && !bSwingPhysics ) // 0x0000003F : 0x0038
    {
      // if (bSwingPhysics) {
        // // Instigator.ClientMessage("Your velocity="$ VSize(Instigator.Velocity));
        // // Instigator.SetPhysics(PHYS_Falling);
        // // Stop the line whirring:
        // AmbientSound = None; // 0x00000060 : 0x0065
        // Master.AmbientSound = None; // 0x00000064 : 0x006C
        // // Dampen:
        // // Instigator.AddVelocity( Instigator.Velocity*-0.02 );
      // } else {
        // Stick on arrival.
        Instigator.SetPhysics(PHYS_None); // 0x00000055 : 0x0057
        AmbientSound = None; // 0x00000060 : 0x0065
        Master.AmbientSound = None; // 0x00000064 : 0x006C
        Instigator.AddVelocity(Instigator.Velocity * -1); // 0x0000006E : 0x007C
        bPlaySound = False; // 0x00000087 : 0x00A1
      // }
    } else { // 0x0000008C : 0x00A9

      if (bSwingPhysics) {

        // if (/*length > 4*MinRetract &&*/ Instigator.Velocity.Z<0) {
          // Instigator.Velocity.Z *= 0.99;
        // }

        outwardPull = Instigator.Velocity Dot Normal(Instigator.Location-pullDest);

        if (outwardPull<0 || length <= MinRetract) outwardPull=0;
        // if (outwardPull<1.0) outwardPull=1.0*outwardPull*outwardPull; // Smooth the last inch
        Inward = Normal(pullDest-Instigator.Location);

        // This makes the line weak and stretchy:
        // Instigator.Velocity = Instigator.Velocity + Inward*DeltaTime;

        // This completely cancels outward momentum - the line length should not increase!
        // Instigator.AddVelocity( 2.0 * outwardPull * Inward ); // equilibrium (line stretches very very slowly)
        // Instigator.AddVelocity( 1.0 * outwardPull * Inward ); // keep the line the same length.  The 0.1 prev.nts slow sinking
        Instigator.AddVelocity( 1.0 * outwardPull * Inward ); // keep the line the same length.  The 0.1 prev.nts slow sinking

        // Stop slow gentle sinking by removing half of gravity pre-emptively:
        if (bFiddlePhysics1) {
          Instigator.AddVelocity( 0.6 * Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * Vect(0,0,-1) );
          // This is unrealistic because if they swing outwards, they still get this anti-grav effect.
          // But it but plays well!  It lets you gain and keep height with wide swings.
        } else {
          // It should only act in Inward direction:
          // Instigator.AddVelocity( - 0.6 * Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * Inward );
          Instigator.AddVelocity( - (Inward Dot Vect(0,0,1)) * 0.6 * Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * Inward );
        }

        power = 1.0;
        if (bExtraPower) {
          power += 1.0 + 1.5 * FMin(1.0, length / 1024.0 );
        }
        linePull = power*grappleSpeed;
        if (length <= MinRetract) {
          linePull = 0;
        }
        Instigator.AddVelocity( linePull * Inward * DeltaTime ); // This completely cancels outward momentum - the line length will not increase!

        // Dampen gravity (for long pulls)
        if (bFiddlePhysics2 && Instigator.Velocity.Z<0) {
          // We want to turn whatever velocity we lost into forward velocity.
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
  // ignores  Tick;

  simulated function Tick(float DeltaTime) {
    OnPull(DeltaTime);
  }

  simulated function ProcessTouch (Actor Other, Vector HitLocation)
  {
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
    grappleSpeed=600 // 600 was good for old Expert100, but is quite weak with new code
    // grappleSpeed=0
    // grappleSpeed=100 // Almost nothing, slight pull upwards
    // grappleSpeed=800
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
    ThrowAngle=0
    // Physics=PHYS_Falling
    // ThrowAngle=15
    bDropFlag=True
    bSwingPhysics=True
    //// These two are cheats in terms of real physics, but may be useful for large maps without handy ceilings.
    bFiddlePhysics1=False
    bFiddlePhysics2=False
    bExtraPower=False
    ThrowSound=sound'Botpack.Translocator.ThrowTarget'
    // HitSound=sound'UnrealI.GasBag.hit1g'
    // HitSound=sound'KrrChink'
    HitSound=sound'hit1g'
    PullSound=sound'ObjectPush'
    // PullSound=sound'Pull'
    RetractSound=sound'Slurp'
}

