//================================================================================
// kxGrapple.
//================================================================================

// TODO: fires on spawn :f
// TODO: makes amp sounds when used with amp
// TODO: sometimes trying to hook when we are right next to the walls fails - we need an audio indicator of that (sometimes we hear the ThrowSound twice)
// TODO: Wall hit sound could be a bit better / should be clear + distinct from throw sound.
// TODO: Removes Translocator ok, but seems to remove ImpactHammer too!
// DONE: Clicking with both fire buttons should return to previous weapon, as with Translocator.
// TODO: When I switch to this weapon I must wait a moment before I can primary fire, unlike the Translocator.
// DONE: jump to un-grapple (only when holding another weapon)
// TODO: jump to un-grapple sometimes fails because the tick didn't notice bPressedJump if the player only tapped jump quickly.
// DONE: now that we have lineLength, we should make it a function of grappleSpeed.
// TODO: you can get into very fast swings on the ceiling which never stop, presumably because we are always >MinRetract so the dampening never takes effect.

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
var() config bool bLinePhysics;
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
var() config float hitdamage;
var bool bAttachedToThing;
var() config bool bDoAttachPawn;
var() config bool bLineOfSight;
var Actor thing;
var float TotalTime;
var kx_GrappleLauncher Master;
var bool bThingPawn;
var float lineLength;

replication
{
  unreliable if ( Role == 4 )
    pullDest,hNormal,thing,TotalTime,Master,bThingPawn;
}

simulated function Destroyed ()
{
  Master.kxGrapple = None;
  AmbientSound = None;
  Master.AmbientSound = None;
  // PlaySound(sound'UnrealI.hit1g',SLOT_Interface,10.0);
  // Master.PlaySound(sound'UnrealI.hit1g',SLOT_Interface,10.0);
  // PlaySound(sound'Botpack.Translocator.ReturnTarget',SLOT_Interface,1.0);
  // Master.PlaySound(sound'Botpack.Translocator.ReturnTarget',SLOT_Interface,1.0);
  PlaySound(RetractSound,SLOT_Interface,2.0,,,240);
  Master.PlaySound(RetractSound,SLOT_Interface,2.0,,,240);
  // Master.PlaySound(sound'FlyBuzz', SLOT_Interface, 2.5, False, 32, 16);
  Instigator.SetPhysics(PHYS_Falling);
  Super.Destroyed();
}

simulated function SetMaster (kx_GrappleLauncher W)
{
  Master = W;
  Instigator = Pawn(W.Owner);
}

auto state Flying
{
  simulated function ProcessTouch (Actor Other, Vector HitLocation)
  {
    Log("kxGrapple.ProcessTouch() I've never been here before");
    if ( Inventory(Other) != None )
    {
      Inventory(Other).GiveTo(Instigator);
    }
    Instigator.AmbientSound = None;
    AmbientSound = None;
    Destroy();
    return;
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
    pullDest = Location;
    hNormal = HitNormal;
    SetPhysics(PHYS_None);
    lineLength = VSize( Instigator.Location - Location);
    if ( Wall.IsA('Pawn') || Wall.IsA('Mover') )
    {
      if (  !bDoAttachPawn &&  !Wall.IsA('Mover') )
      {
        Destroy();
        return;
      }
      bAttachedToThing = True;
      thing = Wall;
      if ( Wall.IsA('Pawn') )
      {
        bThingPawn = True;
      }
      GotoState('PullTowardDynamic');
    } else {
      GotoState('PullTowardStatic');
    }
    CheckFlagDrop();
  }
  
  simulated function BeginState ()
  {
    local rotator outRot;
    outRot = Rotation;
    outRot.Pitch += ThrowAngle*8192/45;
    Velocity = vector(outRot) * speed;
  }
  
}

function OnPull(float DeltaTime) {
  CheckFlagDrop();
  TotalTime += DeltaTime;
  // After 2 seconds on the grappling line, if we have switched weapon away from grapple, then Jump will un-grapple us!
  if (PlayerPawn(Master.Owner)!=None && PlayerPawn(Master.Owner).bPressedJump /*&& TotalTime>=2.0*/ && kx_GrappleLauncher(PlayerPawn(Master.Owner).Weapon) == None) {
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
    if ( bLineOfSight && !Instigator.LineOfSightTo(self) ) {
      // TODO: For more realism, we could reposition the grapple at the new point the line must now swing around.
      Destroy();
      return;
    }

    length = VSize(Instigator.Location - pullDest);

    if ( length <= MinRetract ) {
        AmbientSound = None;
        Master.AmbientSound = None;
    }

    if (bSwingPhysics) {

      // Dampening when we reach the top:
      // if ( length <= (MinRetract+8.0) /*|| Abs(length-lineLength)<8.0*/ ) {
        Instigator.Velocity = Instigator.Velocity*0.99;
      // }
      // Now always dampening.

      // if (/*length > 4*MinRetract &&*/ Instigator.Velocity.Z<0) {
        // Instigator.Velocity.Z *= 0.99;
      // }

      // Deal with any outward velocity (against the line):
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

      // Deal with grapple pull:
      if (bLinePhysics) {
        lineLength = lineLength - 0.3 * grappleSpeed*DeltaTime;
        if (lineLength<MinRetract) lineLength=MinRetract;
        if (length > lineLength) {
          // Line has elongated!  This is not allowed!
          // Instigator.ClientMessage("Elongation = "$ (length - lineLength) $" Velocity = "$ VSize(Instigator.Velocity));
          // Elasticity if the line was lengthened by the player swinging.
          // linePull += (length - lineLength)*100.0; // Provide velocity to cancel the change in 0.01 seconds.
          Instigator.AddVelocity( (length - lineLength) * Inward * 1.0 );
          // Instigator.AddVelocity( grappleSpeed * Inward * DeltaTime );
          // TODO BUG: because it is not pre-emptive, we get pulled up in jerks.
          // TODO BUG: this method is always jerky unless we also do the below.
          //           but the below can cause unrealistic swinging around on the ceiling!
          // Instigator.AddVelocity( (length - lineLength) * Inward * 5.0 );
          // TODO: This new approach could replace all the gravity tweaks we made.
          // TODO: But it just doesn't work; it doesn't allow us to swing.
          // Respond with upward pull:  (This must be *as well as* and *less than* inward pull, otherwise we can just float up and up, and elongation increases!)
          if (Location.Z > Instigator.Location.Z) {
            Instigator.AddVelocity( (length - lineLength) * Vect(0,0,1) * 1.5 );
            // 2.0 was too strong, allowed u to almost pr.vent swingback entirely!
            // But 1.0 was too weak, you couldn't add enough swing to be useful.
            // Well this does add some extra swinging power, but not really enough.
          }
        } else if (length < lineLength && length>=MinRetract) {
          // Line has shortened.
          // lineLength = length;
        }
     } else {
        power = 1.0;
        if (bExtraPower) {
          power += 1.0 + 1.5 * FMin(1.0, length / 1024.0 );
        }
        linePull = power*grappleSpeed;
        if (length <= MinRetract) {
          linePull = 0;
        }
        Instigator.AddVelocity( linePull * Inward * DeltaTime ); // This completely cancels outward momentum - the line length will not increase!
      }

      // Stop slow gentle sinking (due to iterative gravity) by removing half of gravity pre-emptively:
      if (bFiddlePhysics1) {
        Instigator.AddVelocity( 0.6 * Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * Vect(0,0,-1) );
        // This is unrealistic because if they swing outwards, they still get this anti-grav effect.
        // But it but plays well!  It lets you gain and keep height with wide swings.
      } else {
        // It should only act in Inward direction:
        // Instigator.AddVelocity( - 0.6 * Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * Inward );
        Instigator.AddVelocity( - (Inward Dot Vect(0,0,1)) * 0.6 * Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * Inward );
      }

      // Dampen gravity (useful for swinging outwards on long pulls)
      if (bFiddlePhysics2 && Instigator.Velocity.Z<0) {
        // We want to turn whatever velocity we lost into forward velocity.
        Instigator.AddVelocity( Normal(Instigator.Velocity) * 0.1*Abs(Instigator.Velocity.Z) );
        Instigator.Velocity.Z *= 0.9;
      }
      // Dampen velocity:
      // Instigator.AddVelocity( Instigator.Velocity*-0.001*DeltaTime );

    } else {

      // Original grapple
      if ( length <= MinRetract ) {
        // When we reach destination, we just stop
        Instigator.SetPhysics(PHYS_None);
        Instigator.AddVelocity(Instigator.Velocity * -1);
      } else {
        // No gravity or swinging
        Instigator.Velocity = Normal(pullDest - Instigator.Location) * grappleSpeed;
      }

    }

  }
  
  simulated function ProcessTouch (Actor Other, Vector HitLocation)
  {
    Instigator.AmbientSound = None;
    AmbientSound = None;
    Destroy();
  }
  
  simulated function BeginState ()
  {
    if (!bSwingPhysics) {
      Instigator.Velocity = Normal(pullDest - Instigator.Location) * grappleSpeed;
    }
    Instigator.SetPhysics(PHYS_Falling);
    TotalTime = 0.0;
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
    Instigator.AmbientSound = None;
    AmbientSound = None;
    Destroy();
  }
  
  simulated function BeginState ()
  {
    Instigator.SetPhysics(PHYS_Flying);
    TotalTime = 0.0;
  }
  
}

defaultproperties
{
    grappleSpeed=600 // 600 was good for old Expert100, and is adjusted to work with new code also.
    // grappleSpeed=0
    // grappleSpeed=100 // Almost nothing, slight pull upwards
    // grappleSpeed=800
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
    // bUnlit=True
    bUnlit=False
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
    bFiddlePhysics1=False // An alternative method for counteracting gravity, which unrealistically helps swing.
    bFiddlePhysics2=False // Try to help swing by turning downward momentum into forward momentum.
    bLinePhysics=False // This attempts to deal with grapple pull in terms of line length, but it still needs some work.
    bExtraPower=False
    ThrowSound=sound'Botpack.Translocator.ThrowTarget'
    // HitSound=sound'UnrealI.GasBag.hit1g'
    // HitSound=sound'KrrChink'
    HitSound=sound'hit1g'
    PullSound=sound'ObjectPush'
    // PullSound=sound'Pull'
    RetractSound=sound'Slurp'
}

