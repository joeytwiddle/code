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
// TODO: it would be nice to have a second mesh - the line between us and the grapple
// TODO BUG: sometimes doublejump intercepts bJumping and clears it, or at any rate we don't see it.
// TODO BUG: if there is lag (on the server) the physics breaks and the player sometimes gets tugged up in the air unrealistically!  Maybe bLinePhysics can help with this...
// TODO: I have not yet put real physics into PullTowardDynamic.

// class kxGrapple extends XPGrapple Config(kxGrapple);
class kxGrapple extends Projectile Config(kxGrapple);

#exec AUDIO IMPORT FILE="Sounds\Pull.wav" NAME="Pull" // grindy windy one from ND
// #exec AUDIO IMPORT FILE="Sounds\SoftPull.wav" NAME="SoftPull" // softer one from ND (more robotic)
#exec AUDIO IMPORT FILE="Sounds\greset.wav" NAME="Slurp" // metallic slurp from ND
// #exec AUDIO IMPORT FILE="Sounds\End.wav" NAME="KrrChink" // kchink when grapple hits target
#exec AUDIO IMPORT FILE="Sounds\hit1g.wav" NAME="hit1g" // From UnrealI.GasBag

var() config float grappleSpeed;
var() config float hitdamage;
var() config bool bDoAttachPawn;
var() config bool bLineOfSight;
var() config bool bLetHookSlip;
var() config bool bDropFlag;
var() config bool bSwingPhysics;
var() config bool bLinePhysics;
var() config bool bExtraPower;
var() config bool bPrimaryWinch;
var() config bool bCrouchReleases;
var() config float MinRetract;
var() config float ThrowAngle;
var() config sound ThrowSound,HitSound,PullSound,ReleaseSound,RetractSound;
var() config bool bFiddlePhysics1;
var() config bool bFiddlePhysics2;
var() config bool bShowLine;

var Vector pullDest;
var Vector hNormal;
var bool bRight;
var bool bCenter;
var bool bAttachedToThing;
var Actor thing;
var float TotalTime;
var kx_GrappleLauncher Master;
var bool bThingPawn;
var float lineLength;
// var ShockBeam LineSprite;
// var Effects LineSprite;
// var rocketmk2 LineSprite;
var kxLine LineSprite;

replication {
  unreliable if ( Role == ROLE_Authority )
    // pullDest,hNormal,thing,TotalTime,Master,bThingPawn,lineLength,LineSprite;
    pullDest,lineLength,thing,Master;
  reliable if ( Role == ROLE_Authority )
    LineSprite;
}

auto state Flying
{

  simulated function BeginState ()
  {
    local rotator outRot;
    // Set outgoing Velocity of the grapple, adjusting for ThrowAngle:
    outRot = Rotation;
    outRot.Pitch += ThrowAngle*8192/45;
    Velocity = vector(outRot) * speed;
    pullDest = Location;
    InitLineSprite();
    // Let's point the mesh in the opposite direction:
    // outRot.Roll = -Rotation.Roll;
    outRot.Yaw = Rotation.Yaw + 32768;
    outRot.Pitch = -Rotation.Pitch;
    SetRotation(outRot);
  }

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
    pullDest = Location;
    hNormal = HitNormal;
    SetPhysics(PHYS_None);
    lineLength = VSize(Instigator.Location - pullDest);
    if ( Wall.IsA('Pawn') || Wall.IsA('Mover') ) {
      if (  !bDoAttachPawn &&  !Wall.IsA('Mover') ) {
        Destroy();
        return;
      }
      bAttachedToThing = True;
      thing = Wall;
      if ( Wall.IsA('Pawn') ) {
        bThingPawn = True;
      }
      GotoState('PullTowardDynamic');
    } else {
      GotoState('PullTowardStatic');
    }
    CheckFlagDrop();
    PlaySound(HitSound,SLOT_None,10.0);
    Master.PlaySound(HitSound,SLOT_None,10.0);
    //// I could not hear the ones done with AmbientSound.
    if (grappleSpeed>0 && !bPrimaryWinch) {
      Master.AmbientSound = PullSound;
      AmbientSound = PullSound;
    } else {
      Master.AmbientSound = None;
      AmbientSound = None;
    }
  }

  simulated function Tick(float DeltaTime) {
    UpdateLineSprite();
  }

}

simulated function SetMaster (kx_GrappleLauncher W)
{
  Master = W;
  Instigator = Pawn(W.Owner);
  // InitLineSprite();
}

simulated function Destroyed ()
{
  Master.kxGrapple = None;
  AmbientSound = None;
  Master.AmbientSound = None;
  if (LineSprite!=None) {
    LineSprite.GrappleParent = None;
    LineSprite.LifeSpan = 0;
    LineSprite.Destroy();
    LineSprite = None;
    // lol it's still there
  }
  // PlaySound(sound'UnrealI.hit1g',SLOT_Interface,10.0);
  // Master.PlaySound(sound'UnrealI.hit1g',SLOT_Interface,10.0);
  // PlaySound(sound'Botpack.Translocator.ReturnTarget',SLOT_Interface,1.0);
  // Master.PlaySound(sound'Botpack.Translocator.ReturnTarget',SLOT_Interface,1.0);
  PlaySound(RetractSound,SLOT_Interface,2.0,,,240);
  Master.PlaySound(RetractSound,SLOT_Interface,2.0,,,240);
  // Master.PlaySound(sound'FlyBuzz', SLOT_Interface, 2.5, False, 32, 16);
  Instigator.SetPhysics(PHYS_Falling);
  // if (Role == ROLE_Authority) {
  // }
  Super.Destroyed();
}

simulated function InitLineSprite() { // simulated needed?
  local float numPoints;
  if (bShowLine) {
    /*
    LineSprite = Spawn(class'ShockBeam',,,Instigator.Location,rotator(pullDest-Instigator.Location));
    // LineSprite.bUnlit = False;
    // LineSprite.Style = STY_Normal;
    numPoints = VSize(pullDest-Instigator.Location)/135.0; if (numPoints<1) numPoints=1;
    numPoints = 16;
    LineSprite.MoveAmount = (pullDest-Instigator.Location)/numPoints;
    // LineSprite.MoveAmount = Vect(0,0,0);
    LineSprite.NumPuffs = numPoints-1;
    LineSprite.LifeSpan = 60;
    // LineSprite.NumPuffs = 0;
    // LineSprite.DrawScale = 0.30;
    */

    // LineSprite = Spawn(class'Effects',,,(Instigator.Location+pullDest)/2,rotator(pullDest-Instigator.Location));
    // LineSprite = Spawn(class'rocketmk2',,,(Instigator.Location+pullDest)/2,rotator(pullDest-Instigator.Location));
    LineSprite = Spawn(class'kxLine',,,(Instigator.Location+pullDest)/2,rotator(pullDest-Instigator.Location));
    // LineSprite.SetFromTo(Instigator,Self);
    LineSprite.GrappleParent = Self;
    // LineSprite.Mesh = 'botpack.shockbm';
    LineSprite.Mesh = mesh'Botpack.bolt1';
    LineSprite.DrawType = DT_Mesh;
    // LineSprite.Style = STY_Translucent;
    LineSprite.RemoteRole = ROLE_SimulatedProxy;
    LineSprite.LifeSpan = 60;
    // LineSprite.RemoteRole = ROLE_None;
    // LineSprite.bParticles = True;
    // LineSprite.SetPhysics(PHYS_Rotating);
    // LineSprite.SetPhysics(PHYS_Projectile);
    LineSprite.SetPhysics(PHYS_Flying);
    // LineSprite.bNetTemporary = True;
    // LineSprite.bGameRelevant = True;
    // LineSprite.bReplicateInstigator = True;
    LineSprite.NetPriority = 2.5;
  }
}

simulated function UpdateLineSprite() {
  local float numPoints;
  if (bShowLine) {
    /*
    if (FRand()<0.05) {
      LineSprite.Destroy();
      InitLineSprite();
      // return;
    }
    */
    /*
    LineSprite.SetLocation(Instigator.Location);
    LineSprite.SetRotation(rotator(pullDest-Instigator.Location));
    // LineSprite.SetVelocity(new location - last location * smth unknown :P );
    numPoints = VSize(pullDest-Instigator.Location)/135.0; if (numPoints<1) numPoints=1;
    numPoints = 16;
    LineSprite.MoveAmount = (pullDest-Instigator.Location)/numPoints;
    // LineSprite.MoveAmount = Vect(0,0,0);
    LineSprite.NumPuffs = numPoints-1;
    // LineSprite.NumPuffs = 0;
    // LineSprite.DrawScale = 0.04*numPoints;
    */
    /*
    LineSprite.SetLocation((Instigator.Location+pullDest)/2);
    LineSprite.SetRotation(rotator(pullDest-Instigator.Location));
    LineSprite.DrawScale = VSize(Instigator.Location-pullDest) / 64.0;
    LineSprite.Velocity = Instigator.Velocity * 0.5;
    */
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
    local Actor A;

    OnPull(DeltaTime);

    // TODO, for ultra realism, keep a list of points our line has pulled around, and if we swing back to visibility to the previous point, relocate!
    if ( bLineOfSight ) {
      if (bLetHookSlip) {
        A = Trace(Inward,hNormal,pullDest,Instigator.Location,false);
        if (A != None && VSize(Inward-pullDest)>5) {
          Self.DrawType = DT_None;
          // Self.SetLocation(Inward);
          pullDest = Inward;
          if (bLinePhysics) {
            lineLength = VSize(Instigator.Location - pullDest);
          }
          // TODO BUG: for some reason, I am not hearing these sounds!  Now trying SLOT_Interface instead of SLOT_None.
          PlaySound(HitSound,SLOT_Interface,10.0);
          Master.PlaySound(HitSound,SLOT_Interface,10.0);
          // Instigator.ClientMessage("Hook slipped! Hit "$A);
          // return; // Don't return, we gotta render this new style!
        }
      } else {
        if (!Instigator.LineOfSightTo(self)) {
          Destroy();
        }
      }
    }

    length = VSize(Instigator.Location - pullDest);

    if ( length <= MinRetract ) {
        AmbientSound = None;
        Master.AmbientSound = None;
    }

    if (bSwingPhysics) {

      // Dampening when we reach the top:
      // if ( length <= (MinRetract+8.0) /*|| Abs(length-lineLength)<8.0*/ ) {
        Instigator.Velocity = Instigator.Velocity*0.995;
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
      // Cancel the outward velocity - it never really should have happened.
      // Also changes global velocity by pulling it towards the line, not only countering gravity.
      Instigator.AddVelocity( 1.0 * outwardPull * Inward ); // keep the line the same length.  The 0.1 prev.nts slow sinking
      // TODO: I am adding this here, then in a few places below, removing it if I decide it wasn't wanted.
      //       Would be better to pass along a boolean which decides at the end whether or not to apply this.
      //       This might allow us to actually walk and jump normally if our line has gone slack.

      // Deal with grapple pull:
      if (bLinePhysics) {
        if (bCrouchReleases && PlayerPawn(Instigator)!=None && PlayerPawn(Instigator).bDuck!=0) {
          lineLength = lineLength + 0.3 * grappleSpeed*DeltaTime;
          // Force correct length:
          // Instigator.SetLocation( pullDest + lineLength*-Inward );
          // Instigator.SetLocation( pullDest + Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * Vect(0,0,1) );
          // Undo the "keep the same length" from earlier:
          Master.AmbientSound = ReleaseSound;
          AmbientSound = ReleaseSound;
        } else if (lineLength<=MinRetract || (bPrimaryWinch && PlayerPawn(Instigator)!=None && PlayerPawn(Instigator).bFire==0)) { // TODO: right now this applies even if weapon is switched, and we primary fire with that :f
          Master.AmbientSound = None;
          AmbientSound = None;
        } else {
          lineLength = lineLength - 0.3 * grappleSpeed*DeltaTime;
          // 0.3 is my estimate conversion from acceleration to velocity
          Master.AmbientSound = PullSound;
          AmbientSound = PullSound;
        }
        if (lineLength<MinRetract) lineLength=MinRetract;
        if (length > lineLength) {
        /*
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
          if (pullDest.Z > Instigator.Location.Z) {
            Instigator.AddVelocity( (length - lineLength) * Vect(0,0,1) * 1.5 );
            // 2.0 was too strong, allowed u to almost pr.vent swingback entirely!
            // But 1.0 was too weak, you couldn't add enough swing to be useful.
            // Well this does add some extra swinging power, but not really enough.
          }
        */
          // Force correct length:
          Instigator.SetLocation( pullDest + lineLength*-Inward );
        } else if (length < lineLength /*&& length>=MinRetract*/) {
          // Line is shorter than it should be.
          // Undo the inward pull effect:
          Instigator.AddVelocity( - 1.0 * outwardPull * Inward );
          // Alternatively, shorten the line to the new length!
          // lineLength = length;
        }
     } else {
        if (bPrimaryWinch && PlayerPawn(Instigator)!=None && PlayerPawn(Instigator).bFire==0) {
          power = 0.0;
          Master.AmbientSound = None;
          AmbientSound = None;
        } else {
          power = 1.0;
          if (bExtraPower) {
            power += 1.0 + 1.5 * FMin(1.0, length / 1024.0 );
          }
          Master.AmbientSound = PullSound;
          AmbientSound = PullSound;
        }
        if (bCrouchReleases) {
          if (PlayerPawn(Instigator)!=None && PlayerPawn(Instigator).bDuck!=0) {
             power = 0.0;
             // Instigator.AddVelocity( Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * -Inward );
             // Instigator.AddVelocity( Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * Vect(0,0,-1) );
             Instigator.AddVelocity( - 1.0 * outwardPull * Inward );
             Master.AmbientSound = ReleaseSound;
             AmbientSound = ReleaseSound;
           }
        }
        if (length <= MinRetract) {
          power = 0.0;
          Master.AmbientSound = None;
          AmbientSound = None;
        }
        linePull = power*grappleSpeed;

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

    UpdateLineSprite();

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

  // TODO: cause hitdamage!

  simulated function Tick(float DeltaTime) {
    if (Thing==None)
     Self.Destroy();
    pullDest = Thing.Location;
    if (VSize(Thing.Location-Location)>10) {
      Self.SetLocation( Location*0.1 + Thing.Location*0.9);
      Self.Velocity = Thing.Velocity;
    }
    Instigator.Velocity = Normal(Thing.Location - Instigator.Location) * 1.0 * grappleSpeed;
    Master.AmbientSound = PullSound;
    AmbientSound = PullSound;
    OnPull(DeltaTime);
    UpdateLineSprite();
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
    bLetHookSlip=True
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
    DrawScale=1.75
    CollisionRadius=1.0
    CollisionHeight=0.5
    RemoteRole=ROLE_SimulatedProxy
    Physics=PHYS_Projectile
    ThrowAngle=0
    // Physics=PHYS_Falling
    // ThrowAngle=15
    bDropFlag=True
    bSwingPhysics=True
    //// These two are cheats in terms of real physics, but may be useful for large maps without handy ceilings.
    bLinePhysics=True // This attempts to deal with grapple pull in terms of line length, but it still needs some work.
    bExtraPower=False
    ThrowSound=sound'Botpack.Translocator.ThrowTarget'
    // HitSound=sound'UnrealI.GasBag.hit1g'
    // HitSound=sound'KrrChink'
    HitSound=sound'hit1g'
    // PullSound=sound'SoftPull'
    PullSound=sound'Pull'
    ReleaseSound=sound'ObjectPush'
    RetractSound=sound'Slurp'
    bFiddlePhysics1=False // An alternative method for counteracting gravity, which unrealistically helps swing.
    bFiddlePhysics2=False // Try to help swing by turning downward momentum into forward momentum.
    bPrimaryWinch=True // Grapple only winds in while player is holding primary fire.
    bCrouchReleases=True
    bShowLine=False
}

