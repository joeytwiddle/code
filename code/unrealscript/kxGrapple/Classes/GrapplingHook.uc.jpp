//================================================================================
// GrapplingHook.
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
// DONE: now that we have lineLength, we should make it a function of GrappleSpeed.
// TEST maybe fixed: you can get into very fast swings on the ceiling which never stop, presumably because we are always >MinRetract so the dampening never takes effect.
// DONE: it would be nice to have a second mesh - the line between us and the grapple
// TODO BUG: sometimes doublejump intercepts bJumping and clears it, or at any rate we don't see it.
// TODO BUG: if there is lag (on the server) the physics breaks and the player sometimes gets tugged up in the air unrealistically!  Maybe bLinePhysics can help with this...
// TODO: I have not yet put real physics into PullTowardDynamic.
// TODO: At time of writing, bLinePhysics=False appears to have become a bit broken!  Mmm actually I think that may have just been when default did not match server and I changed value mid-game.
// TODO: shieldbelt behind character (by 1 tick?  can this be fixed with velocity or ordering of processing?)
// FIXED: unwind drops back in steps.  now gravity does the unwinding
// UNDERSTOOD: bCrouchReleases makes a noise when bSwingPhysics=False, but does not act :P  If bPrimaryWinch is disabled, we still only get the noise is we primary fire :P (autowind but don't hear winding in unless i click!)
// TODO: Still the issue of primary fire while swinging but switched to another weapon.  Consider preventing winching but allowing it by some other mechanism.
//       The problem appears to be when the server setting is bPrimaryWinch=False but the class default setting is True (so the client does not think the sound needs to be played?).
// CONSIDER: Instead of bPrimaryWinch, bPrimaryFirePausesWinching or bWalkPausesWinching ?
// DONE: Line wrapping around corners magic.  Now GrapplingLines must be created if bLineFolding=True even if bShowLine=False.
// DONE: We should keep bBehindView preference client side.
// TODO: GrappleGun is always the first weapon when we spawn.  It should be enforcer.
// TODO: If we fire to respawn, sometimes we will immediately fire our grapple.
// CONSIDER: Instead of InstigatorRep, shouldn't we just use Master.Owner?  I wonder if the simulated SetMaster() fn guarantees the replication of the variable.
// DONE: GrappleSpeed is scaled down when used for bSwingPhysics, but this is reasonable since swinging can add a lot of speed on top of the winching.
// TODO: We have described one concept as "folding","wrapping" and "splitting" - pick one term and stick with it.
// TODO: bCrouchReleases is nice now that it just reels out according to gravity.  But for extra realism we could set a max unreel speed.
// TODO: PullTowardDynamic - you can currently grapple onto team-mates :f
// TODO: PullTowardDynamic - quite buggy, the hook stays suspended in the air, while the opponent warps around and the line points somewhere odd
// TODO: line does not appear to come out of weapon in firstperson view.
// TODO: would be nice to have a "whipping" sound when the grapple first flies out
// TODO: Just go ahead and somehow fudge the physics so that players can make large upwards swings from the pivot using air control.
// DONE: Grapple will sometimes refuse to embed into corners.  This was fixed by returning to Physics=PHYS_Projectile.
// TODO: On PullTowardDynamic, sometimes the grapple is just left sitting in space, looks naff.  But is the problem the algorithm or replication?
// TEST: You can telefrag other players (incl. teammates?!) by swinging into them.  I think this was caused by SetLocation(), which I have now replaced with MoveSmooth()/Move().
// BUGS: winching out when on floor there should be no sound (cos lineLength does not increase!)
// BUGS: winching in when stuck, the sounds stops, but lineLength is decreasing!
// LATEST FUN BUGS: throw at a wall on face, then walk thru teleporer and use it - creates a lot of messy lines :P
// LATEST FUN BUGS: odd out the front of face, if i wrap the line around the outer wall, the lengths displayed do not add up!
// DONE: grapple should not hurt teammate
// CONSIDER: If we could do the maths correctly, we could do a trace and work out the ideal trajectory to the target point, forcing the correct ThrowAngle.  Then we could remove the anti-grav hax and have a nice soaring grapple.  :)
// TODO: Damage should be only when grapple is pulled back.
// TODO: Allow maximum range on the grappling hook (aka maximum line length).

class GrapplingHook extends Projectile Config(kxGrapple);

#exec AUDIO IMPORT FILE="Sounds\Pull.wav" NAME="Pull" // grindy windy one from ND
// #exec AUDIO IMPORT FILE="Sounds\SoftPull.wav" NAME="SoftPull" // softer one from ND (more robotic)
#exec AUDIO IMPORT FILE="Sounds\greset.wav" NAME="Slurp" // metallic slurp from ND
// #exec AUDIO IMPORT FILE="Sounds\End.wav" NAME="KrrChink" // kchink when grapple hits target
#exec AUDIO IMPORT FILE="Sounds\hit1g.wav" NAME="hit1g" // From UnrealI.GasBag

var() config float GrappleSpeed;
var() config float HitDamage;
var() config bool bDoAttachPawn;
var() config bool bLineOfSight;
var() config bool bLineFolding;
var() config bool bDropFlag;
var() config bool bSwingPhysics;
var() config bool bLinePhysics;
var() config bool bExtraPower;
var() config bool bPrimaryWinch;
var() config bool bCrouchReleases;
var() config float MinRetract;
var() config float ThrowAngle;
var() config bool bFiddlePhysics0;
var() config bool bFiddlePhysics1;
var() config bool bFiddlePhysics2;
var() config bool bShowLine;
var() config sound HitSound,PullSound,ReleaseSound,RetractSound;
var() config Mesh LineMesh;
var() config Texture LineTexture;
var() config bool bLogging;

var GrappleGun Master;
var Vector pullDest;
var bool bAttachedToThing;
var Actor thing;
var bool bThingPawn;
var float lineLength;
// var ShockBeam LineSprite;
// var Effects LineSprite;
// var rocketmk2 LineSprite;
var GrapplingLine LineSprite;
var bool bDestroyed;
var Vector hNormal; // Never actually used, just a temporary variable for Trace().
var Pawn InstigatorRep; // Sometimes the Instigator is not replicated, so we use our own variable to propogate it to the client.
var bool isStuck;

replication {
  // I believe all config vars need to be replicated because I have set defaults which the client may see unless we transfer the server's values.  Unfortunately I don't think it's working.  OK if no default is set, then they get replicated just fine.  And this replication statement *is* needed!
  reliable if (Role == ROLE_Authority)
    GrappleSpeed,HitDamage,bDoAttachPawn,bLineOfSight,bLineFolding,bDropFlag,bSwingPhysics,bLinePhysics,bExtraPower,bPrimaryWinch,bCrouchReleases,MinRetract,ThrowAngle,bFiddlePhysics0,bFiddlePhysics1,bFiddlePhysics2,bShowLine,HitSound,PullSound,ReleaseSound,RetractSound,LineMesh,LineTexture,bLogging;
  reliable if (Role == ROLE_Authority)
    pullDest,bDestroyed,lineLength,Thing,Master,InstigatorRep; // ,bThingPawn,hNormal;
  // reliable if (Role == ROLE_Authority)
    // LineSprite; // We don't want the client and server to have different LineSprites, or they will stop themselves!
  // reliable if (Role == ROLE_Authority)
    // InitLineSprite,DoLineOfSightChecks; // This was what was needed to get the variables replicated into the lines.
  // reliable if (Role == ROLE_Authority)
    // UpdateLineSprite; // needed to make it reliable since InitLineSprite was sometimes called from there?
}

auto state Flying {

  simulated function BeginState () {
    local rotator outRot;
    // Set outgoing Velocity of the grapple, adjusting for ThrowAngle:
    outRot = Rotation;
    outRot.Pitch += ThrowAngle*8192/45;
    Velocity = vector(outRot) * Speed;
    if (ThrowAngle != 0) {
      SetPhysics(PHYS_Falling);
    }
    pullDest = Location;
    // Let's point the mesh in the opposite direction:
    outRot.Yaw = Rotation.Yaw + 32768;
    outRot.Pitch = -Rotation.Pitch;
    SetRotation(outRot);
    if (Role == ROLE_Authority) {
      InstigatorRep = Instigator;
    }
    InitLineSprite();
  }

  simulated function Tick(float DeltaTime) {
    local rotator rot;
    // pullDest = Location;
    pullDest = Location + 11.0*DrawScale*Vector(Rotation);
    if (LineSprite != None)
      LineSprite.NearPivot = pullDest;
    UpdateLineSprite();

    // We only need one of the following, depending what Physics type we decide to use.
    // I will probably go with PHYS_Projectile since with PHYS_Falling the hook was refusing to stick in some corners.

    if (Physics == PHYS_Falling) {
      // Orient grapple according to trajectory:
      rot = Rotator(Velocity);
      rot.Yaw += 32768;
      rot.Pitch *= -1;
      SetRotation(rot);

      // Naughty, but I want it to only feel 10% gravity:
      Velocity -= Region.Zone.ZoneGravity*0.9*DeltaTime;
      // If anyone asks, just say that it's equipped with wings which allow it to glide.
      // Alternatively, we could set PHYS_Projectile as before or PHYS_Flying, and do all the gravity ourself.
    }

    if (Physics == PHYS_Projectile) {
      // Falling projectile? ^^
      Velocity += Region.Zone.ZoneGravity*0.2*DeltaTime;
      rot = Rotator(Velocity);
      rot.Yaw += 32768;
      rot.Pitch *= -1;
      SetRotation(rot);
    }
  }

  // Is this pickup handling?
  simulated function ProcessTouch (Actor Other, Vector HitLocation) {
    Log(Level.TimeSeconds$" "$Self$".ProcessTouch() I've never been here before");
    if ( Inventory(Other) != None ) {
      Inventory(Other).GiveTo(Instigator);
    }
    Instigator.AmbientSound = None;
    AmbientSound = None;
    Destroy();
    return;
  }

  simulated function HitWall (Vector HitNormal, Actor Wall) {
    pullDest = Location + 11.0*DrawScale*Vector(Rotation);
    // InitLineSprite();
    if (LineSprite != None)
      LineSprite.NearPivot = pullDest;
    hNormal = HitNormal;
    SetPhysics(PHYS_None);
    lineLength = VSize(Instigator.Location - pullDest);
    if (Wall.IsA('Pawn') || Wall.IsA('Mover')) {
      if (!bDoAttachPawn && !Wall.IsA('Mover')) {
        Destroy();
        return;
      }
      bAttachedToThing = True;
      thing = Wall;
      if (Wall.IsA('Pawn')) {
        bThingPawn = True;
      }
      GotoState('PullTowardDynamic');
    } else {
      Velocity = vect(0,0,0);
      GotoState('PullTowardStatic');
    }
    CheckFlagDrop();
    PlaySound(HitSound,SLOT_None,10.0);
    Master.PlaySound(HitSound,SLOT_None,10.0);
    //// I could not hear the ones done with AmbientSound.
    if (GrappleSpeed>0 && !bPrimaryWinch) {
      Master.AmbientSound = PullSound;
      AmbientSound = PullSound;
    } else {
      Master.AmbientSound = None;
      AmbientSound = None;
    }
  }

}

simulated function SetMaster (GrappleGun W) {
  Master = W;
  Instigator = Pawn(W.Owner);
  // InitLineSprite();
}

simulated event Destroyed () {
  local int i;
  local GrapplingHook G;
  local GrapplingLine NextLine;
  foreach AllActors(class'GrapplingHook',G) {
    i++;
  }
  if (bLogging) { Log(Level.TimeSeconds$" "$Self$".Destroyed() destructing with "$i$" GrapplingHooks on the level."); }
  Master.GrapplingHook = None;
  AmbientSound = None;
  Master.AmbientSound = None;
  // if (Role==ROLE_Authority && LineSprite!=None) {
  if (bLogging && LineSprite==None) { Log(Level.TimeSeconds$" "$Self$".Destroyed() Cannot destroy LineSprite="$LineSprite$"!"); }
  while (LineSprite!=None) {
    if (bLogging) { Log(Level.TimeSeconds$" "$Self$".Destroyed() destroying LineSprite chain: "$LineSprite); }
    NextLine = LineSprite.ParentLine;
    LineSprite.GrappleParent = None;
    LineSprite.LifeSpan = 1;
    LineSprite.Destroy();
    // LineSprite = None;
    // lol it's still there
    LineSprite = NextLine;
  }
  bDestroyed = True; // At one stage this marker was how we got child lines to self-destruct.
  // PlaySound(sound'UnrealI.hit1g',SLOT_Interface,10.0);
  // Master.PlaySound(sound'UnrealI.hit1g',SLOT_Interface,10.0);
  // PlaySound(sound'Botpack.Translocator.ReturnTarget',SLOT_Interface,1.0);
  // Master.PlaySound(sound'Botpack.Translocator.ReturnTarget',SLOT_Interface,1.0);
  PlaySound(RetractSound,SLOT_Interface,3.0,,,240);
  Master.PlaySound(RetractSound,SLOT_Interface,3.0,,,240);
  // Master.PlaySound(sound'FlyBuzz', SLOT_Interface, 2.5, False, 32, 16);
  Instigator.SetPhysics(PHYS_Falling);
  Super.Destroyed();
}

simulated function InitLineSprite() { // simulated needed?
  local float numPoints;
  if (bShowLine || bLineFolding) {
    // if (bLogging) { Log(Level.TimeSeconds$" "$Self$".InitLineSprite() Running with Role="$Role$" Inst="$Instigator$" InstRep="$InstigatorRep); }
    if (Role != ROLE_Authority) { // spawns it on server only
    // if (Role == ROLE_Authority) { // don't spawn it on server but maybe on client
      if (bLogging) { Log(Level.TimeSeconds$" "$Self$".InitLineSprite() Not spawning LineSprite at this end."); }
      return;
    }
    // if (Level.NetMode ==1)
      // return;
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
    if (InstigatorRep==None) {
      if (bLogging) { Log(Level.TimeSeconds$" "$Self$".InitLineSprite() Warning! InstigatorRep="$InstigatorRep$" so NOT spawning LineSprite now!"); }
      return;
    }
    // Instigator = InstigatorRep; // For client
    if (LineSprite == None) {
      // LineSprite = Spawn(class'GrapplingLine',,,(InstigatorRep.Location+pullDest)/2,rotator(pullDest-InstigatorRep.Location));
      LineSprite = Spawn(class'GrapplingLine',,,,);
      if (bLogging) { Log(Level.TimeSeconds$" "$Self$".InitLineSprite() Spawned "$LineSprite); }
    } else {
      if (bLogging) { Log(Level.TimeSeconds$" "$Self$".InitLineSprite() Re-using old UNCLEANED LineSprite "$LineSprite$"!"); }
      LineSprite.DrawType = DT_Mesh;
      LineSprite.SetLocation((InstigatorRep.Location+pullDest)/2);
      LineSprite.SetRotation(rotator(pullDest-InstigatorRep.Location));
    }
    if (LineSprite == None) {
      if (bLogging) { Log(Level.TimeSeconds$" "$Self$".InitLineSprite() failed to spawn child GrapplingLine!"); }
      return;
    }
    // LineSprite.SetFromTo(InstigatorRep,Self);
    LineSprite.GrappleParent = Self;
    if (bLogging) { Log(Level.TimeSeconds$" "$Self$".InitLineSprite() Setting NearPivot of new LineSprite to "$pullDest); }
    LineSprite.NearPivot = pullDest; // Actually this may be too early, since the grapple itself is moving.  We set it again later.
    if (!bShowLine) {
      LineSprite.bStopped = True;
      LineSprite.DrawType = DT_None;
      // LineSprite.Disable('Tick'); // Ah no it needs to know when to destroy itself.  Anyway bStopped is set, so Tick() is not doing so much work.
    } else {
      // LineSprite.Mesh = 'botpack.shockbm';
      // LineSprite.Mesh = mesh'Botpack.bolt1';
      LineSprite.Mesh = LineMesh;
      LineSprite.Texture = LineTexture;
      //// Made it look a bit of jelly.  Consider trying again but non-translucent.
      // if (Level.bHighDetailMode) {
        // LineSprite.bMeshEnviroMap = True;
      // }
      // LineSprite.SetPhysics(PHYS_Rotating);
      // LineSprite.Style = STY_Translucent;
      // LineSprite.DrawType = DT_Mesh;
      // LineSprite.RemoteRole = ROLE_SimulatedProxy;
      // LineSprite.NetPriority = 2.6;
      // LineSprite.Texture = Texture'UMenu.Icons.Bg41';
      // LineSprite.LifeSpan = 60;
      // LineSprite.RemoteRole = ROLE_None;
      // LineSprite.bParticles = True;
      // LineSprite.SetPhysics(PHYS_Projectile);
      // LineSprite.SetPhysics(PHYS_Flying);
      // LineSprite.bNetTemporary = True;
      // LineSprite.bGameRelevant = True;
      // LineSprite.bReplicateInstigator = True;
    }
  }
}

simulated function UpdateLineSprite() {
  local float numPoints;

  // This is here because occasionally the first InitLineSprite() was getting called with Instigator=None.  We try again now, in case that variable has been replicated.
  if (LineSprite==None && Role==ROLE_Authority)
    InitLineSprite();
  // Actually we aren't doing the client spawn anyway, we are just replicating the server's GrapplingLines.

  if (bShowLine) {
    // if (bLogging && FRand()<0.01) { Log(Level.TimeSeconds$" "$Self$".UpdateLineSprite() Running with Role="$Role$" pullDest="$pullDest); }
    // LineSprite.Reached = Instigator.Location;
    if (Role != ROLE_Authority) {
      return;
    }
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
  // If we have switched weapon away from grapple, then Jump will un-grapple us!
  if (PlayerPawn(Master.Owner)!=None && PlayerPawn(Master.Owner).bPressedJump && GrappleGun(PlayerPawn(Master.Owner).Weapon) == None) {
    Destroy();
  }
}

function CheckFlagDrop() {
  if (bDropFlag && Instigator.PlayerReplicationInfo.HasFlag != None) {
    CTFFlag(Instigator.PlayerReplicationInfo.HasFlag).Drop(vect(0,0,0));
  }
}

/*simulated*/ function DoLineOfSightChecks() {
  local Actor A;
  local Vector NewPivot,NewPivotNear,visualPullDest;
  local GrapplingLine LastLine;

  // if (bLogging && LineSprite!=None && VSize(pullDest-LineSprite.NearPivot)>1.0) {
    // Log(Level.TimeSeconds$" "$Self$".DoLineOfSightChecks() Warning! pullDest "$pullDest$" != LS.NearPivot "$LineSprite.NearPivot$" !!");
  // }

  // if (bLogging && FRand()<0.01) { Log(Level.TimeSeconds$" "$Self$".DoLineOfSightChecks() Running with Role="$Role$" LineSprite="$LineSprite); }
  // OK good now got it running on the client too.

  // TESTING: for ultra realism, keep a list of points our line has pulled around, and if we swing back to visibility to the previous point, relocate!
  // BUG: a further harder part, is to have the line slip over the corner where it folds, as the player swings below.
  // BUG: we sometimes re-merge the line although we have slipped under a bridge, so it would not actually be possible due to topology.
  if ( bLineOfSight ) {
    if (bLineFolding) {

      // Check if we have swung back, and the line will become one again:
      if (LineSprite != None) {
        LastLine = LineSprite.ParentLine;
        if (LastLine != None
          // But don't merge if we are almost finished
          // && ( lineLength<-50 || lineLength>MinRetract )
        ) {
          // A = Trace(NewPivot,hNormal,LastLine.NearPivot,Instigator.Location,false);
          A = Trace(NewPivot,hNormal,LastLine.NearPivot,Instigator.Location,false);
          if (A == None || A==LastLine || A==Self) { // A==Self never seems to happen / be needed but w/e
            // We can see the grapple again!
            // pullDest = LastLine.NearPivot; // No this must now become LastLine.LastLine.Reached or grapple Location otherwise!
            if (LastLine.ParentLine == None) {
              pullDest = Location;
            } else {
              pullDest = LastLine.ParentLine.Reached;
            }
            // Neither of these feel great, when you jump because you were stuck then SetLocation worked.
            lineLength = VSize(Instigator.Location - pullDest);
            // lineLength = VSize(pullDest - LastLine.Reached) + lineLength;

            if (bLogging) { Log(Level.TimeSeconds$" "$Self$".DoLineOfSightChecks() Merging "$LineSprite$" into "$LastLine); }

            LineSprite.bStopped = True;
            LineSprite.DrawType = DT_None;
            LineSprite.SetPhysics(PHYS_None);
            // LineSprite.Disable('Tick');
            LineSprite.Destroy(); // Fail as expected :P

            LineSprite = LastLine;

            LastLine.bStopped = False;
            LastLine.SetPhysics(PHYS_Rotating);
            // LastLine.Enable('Tick');
            // LastLine.Reached = vect(0,0,0); // we could use it as a marker like bStopped
            // CONSIDER: If this revival of old GrapplingLine fails, we could just destroy it and spawn a fresh one.
            if (bLogging) Instigator.ClientMessage("Your grappling line has straightened new length "$lineLength);
          }
        }
      }

      // Check if the line has swung around a corner:
      // CONSIDER: problems when moving at high velocity - split is not outside the edge, but on one side or other of the corner.
      if (LineSprite!=None /*&& LineSprite.ParentLine!=None*/) {
        visualPullDest = LineSprite.NearPivot;
      } else {
        visualPullDest = pullDest;
      }

      A = Trace(NewPivotNear,hNormal,visualPullDest,Instigator.Location,false); // TODO: Not so nice for far end.  Far side hitpoint should really be our new pull target but near side should be check-line-of-sight target!  We can do that using pullDest and NewPivot.

      // TODO: swap the next two lines to get old behaviour, when lines against flat walls always show fine!
      A = Trace(NewPivot,hNormal,Instigator.Location,visualPullDest,false); // Nice for far end, causes subsequent near end line splitting to fail tho, cos there is a wall in the way!  (or maybe it splits every tick :f )
      // NewPivot = NewPivotNear;

      if (A != None && VSize(NewPivot-pullDest)>5) {
        // Self.DrawType = DT_None;
        // Self.SetLocation(NewPivot);
        // if (pullDest != LineSprite.NearPivot) {
          // BroadcastMessage("Warning! pullDest "$pullDest$" != NearPivot "$LineSprite.NearPivot$"");
        // }
        // TODO BUG: for some reason, I am not hearing these sounds!  Now trying SLOT_Interface instead of SLOT_None.
        // PlaySound(HitSound,SLOT_Interface,10.0);
        // Master.PlaySound(HitSound,SLOT_Interface,10.0);
        // No joy.
        // Leave the old line part hanging, and create a new part:
        if (LineSprite != None) {
          // LineSprite.NearPivot = pullDest;
          LineSprite.Reached = NewPivot;
          LineSprite.ReachedRender = NewPivotNear;
          LineSprite.bStopped = True;
          LineSprite.SetPhysics(PHYS_None);
          LineSprite.Velocity = vect(0,0,0);
          LastLine = LineSprite;
          LineSprite = None;
          pullDest = NewPivot;
          InitLineSprite();

          if (LineSprite == None) {
            // Oh no! Sometimes we do get "failed to spawn child GrapplingLine!", often when the wall is quite close, and the child line will not spawn because it is inside the wall?
            // This may happen less now that I've set GrapplingLine.bCollideWorld=False and Physics=PHYS_Projectile.
            // Re-enable the line we just failed to split:
            LineSprite = LastLine;
            if (LineSprite.ParentLine!=None)
              pullDest = LineSprite.ParentLine.Reached;
            else
              pullDest = Location;
            LineSprite.bStopped = False;
            LineSprite.SetPhysics(PHYS_Rotating);
            if (bLogging && Role==ROLE_Authority) { Log(Level.TimeSeconds$" "$Self$".DoLineOfSightChecks() FAILED to spawn GrapplingLine to split "$LastLine); }
            if (bLogging) Instigator.ClientMessage("Failed split!");
          } else {
            LineSprite.ParentLine = LastLine;
            LineSprite.NearPivot = NewPivotNear;
            if (bLogging && Role==ROLE_Authority) { Log(Level.TimeSeconds$" "$Self$".DoLineOfSightChecks() Split "$LastLine$" to "$LineSprite); }
            lineLength = VSize(Instigator.Location - pullDest);
            if (bLogging) Instigator.ClientMessage("Your grappling line was caught on a corner new length "$lineLength);
          }

        }
        // return; // Don't return, we gotta render this new style!
      }

    } else {
      if (!Instigator.LineOfSightTo(self)) {
        Destroy();
      }
    }
  }

}

state() PullTowardStatic {

  simulated function BeginState () {
    if (!bSwingPhysics) {
      Instigator.Velocity = Normal(pullDest - Instigator.Location) * GrappleSpeed;
    }
    Instigator.SetPhysics(PHYS_Falling);
  }

  simulated function Tick (float DeltaTime) {

    if (Instigator==None || Master==None) {
      if (Role == ROLE_Authority) {
        if (bLogging && FRand()<0.1) { Log(Level.TimeSeconds$" "$Self$".PullTowardStatic.Tick() Server can't do motion because Instigator="$Instigator$" or Master="$Master$" btw InstigatorRep="$InstigatorRep); }
      } else {
        // We are client side.  Don't make a fuss.
        if (bLogging && FRand()<0.1) { Log(Level.TimeSeconds$" "$Self$".PullTowardStatic.Tick() Client can't do motion because Instigator="$Instigator$" or Master="$Master$" btw InstigatorRep="$InstigatorRep); }
      }
      return; // Proceeding will just throw Accessed Nones.
      // CONSIDER: If this logs that we have InstigatorRep but not Instigator, then we should use former!
    }

    OnPull(DeltaTime);

    DoPhysics(DeltaTime);

    // if (!isStuck)
      DoLineOfSightChecks();

    UpdateLineSprite();

  }

  simulated function DoPhysics(float DeltaTime) { // Returns False if pawn is stuck.
    local float currentLength,outwardPull,linePull,power;
    local Vector Inward;
    local bool doInwardPull,bSingleLine;

    isStuck = False;

    currentLength = VSize(Instigator.Location - pullDest);

    if ( currentLength <= MinRetract ) {
        AmbientSound = None;
        Master.AmbientSound = None;
    }

    // This is used to decide whether to stop at MinRetract.
    // If we are below a fold, we should continue to winch towards pullDest even below MinRetract.
    bSingleLine = !(LineSprite!=None && LineSprite.ParentLine!=None);

    if (!bSwingPhysics) {

      //// Original grapple physics
      if ( currentLength <= MinRetract && bSingleLine ) {
        // When we reach destination, we just stop
        Instigator.SetPhysics(PHYS_None);
        Instigator.AddVelocity(Instigator.Velocity * -1);
      } else {
        // No gravity or swinging
        Instigator.Velocity = Normal(pullDest - Instigator.Location) * GrappleSpeed;
      }

    }

    //// Swing physics

    // Dampening when we reach the top:
    if (currentLength <= 3*MinRetract) {
      Instigator.Velocity = Instigator.Velocity*0.997;
    } else {
      // Instigator.Velocity = Instigator.Velocity*0.999;
    }

    // if (/*currentLength > 4*MinRetract &&*/ Instigator.Velocity.Z<0) {
      // Instigator.Velocity.Z *= 0.99;
    // }

    Inward = Normal(pullDest-Instigator.Location);

    doInwardPull = True;

    //// Testing replication of this option:
    // if (bLogging && FRand()<0.01) { Log(Level.TimeSeconds$" "$Self$".PullTowardStatic.Tick() bLinePhysics="$bLinePhysics); }

    // Deal with grapple pull:
    if (bLinePhysics) {

      //// We know the length of the line!

      if (bCrouchReleases && PlayerPawn(Instigator)!=None && PlayerPawn(Instigator).bDuck!=0) {
        doInwardPull = False;
        lineLength = currentLength;
        // lineLength = lineLength + 2 * 0.3 * GrappleSpeed*DeltaTime;
        // Force correct length:
        // Instigator.SetLocation( pullDest + lineLength*-Inward );
        // Instigator.SetLocation( pullDest + Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * Vect(0,0,1) );
        // Undo the "keep the same length" from earlier:
        Master.AmbientSound = ReleaseSound;
        AmbientSound = ReleaseSound;
        // Don't wind out faster than 1400.
        if (VSize(Instigator.Velocity) > 1400) {
          Instigator.Velocity = 1400 * Normal(Instigator.Velocity);
        }
        // TODO: This should really affect you only in the direction of the line; we could turn lineLength back on!
        //       Also the code is duplicated below, should be refactored.
        // This version, relative to grappling hook, was supposed to be clever, but caused trouble. :P
        // if (VSize(Instigator.Velocity - Velocity) > 1400) {
          // Instigator.Velocity = 1400 * Normal(Instigator.Velocity - Velocity) + Velocity;
        // }
      } else if ((lineLength<=MinRetract && bSingleLine) || (bPrimaryWinch && PlayerPawn(Instigator)!=None && PlayerPawn(Instigator).bFire==0)) { // TODO: right now this applies even if weapon is switched, and we primary fire with that :f
        Master.AmbientSound = None;
        AmbientSound = None;
      } else {
        lineLength = lineLength - 0.3 * GrappleSpeed*DeltaTime;
        // 0.3 is my estimate conversion from acceleration to velocity
        Master.AmbientSound = PullSound;
        AmbientSound = PullSound;
      }
      if (lineLength<MinRetract && bSingleLine) lineLength=MinRetract;
      if (currentLength > lineLength) {
        /// Player is further from pullDest than he should be, as if the line had elongated!  This is not allowed!
        /// We force the player to the correct distance.

        // Instigator.SetLocation( pullDest + lineLength*-Inward );
        //// Can cause random telefrags.  When we get stuck, ev_ntually pulls us through the wall!
        //// This was good, it "guaranteed" getting un-stuck.

        // Instigator.MoveSmooth( (pullDest + lineLength*-Inward) - Instigator.Location );  // 
        //// Can cause crater against wall when stuck!

        // Instigator.Move( (pullDest + lineLength*-Inward) - Instigator.Location );
        //// When stuck we just sit static.

        TryMoveTo(Instigator,pullDest - lineLength*Inward);
        //// Adds random velocity when stuck.

        // If we are stuck, the line keeps getting shorter.
        // Often this is not a problem because as soon as we become unstuck, the line unfolds and lineLength is updated from the new pull dest.
        // But in case that doesn't happen, let's keep lineLength under control:
        // if (lineLength < currentLength - 250)
          // lineLength = currentLength - 250;
        // TODO CONSIDER: Maybe this should be removed if we go back to the SetLocation() method.

      } else if (currentLength < lineLength /*&& currentLength>=MinRetract*/) {
        // Line is shorter than it should be.
        // Undo the inward pull effect:
        doInwardPull = False;
        // Alternatively, shorten the line to the new length!
        // lineLength = currentLength;
      }

   } else {

      //// Try to deal with swing without knowing the line length.
      //// We assume the line length is whatever distance we are from the hook.

      if (bPrimaryWinch && PlayerPawn(Instigator)!=None && PlayerPawn(Instigator).bFire==0) {
        power = 0.0;
        Master.AmbientSound = None;
        AmbientSound = None;
      } else {
        power = 1.0;
        if (bExtraPower) {
          power += 1.0 + 1.5 * FMin(1.0, currentLength / 1024.0 );
        }
        Master.AmbientSound = PullSound;
        AmbientSound = PullSound;
      }
      if (bCrouchReleases) {
        if (PlayerPawn(Instigator)!=None && PlayerPawn(Instigator).bDuck!=0) {
           // We make no pull with the line at all, gravity affects us and we get our new line length.
           power = 0.0;
           // Instigator.AddVelocity( Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * -Inward );
           // Instigator.AddVelocity( Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * Vect(0,0,-1) );
           doInwardPull = False;
           // power = -1.0;
           Master.AmbientSound = ReleaseSound;
           AmbientSound = ReleaseSound;
           // Don't wind out faster than 1400.
           if (VSize(Instigator.Velocity) > 1400) {
             Instigator.Velocity = 1400 * Normal(Instigator.Velocity);
           }
        }
      }
      if (currentLength <= MinRetract && bSingleLine) {
        power = 0.0;
        Master.AmbientSound = None;
        AmbientSound = None;
        doInwardPull = False;
      }
      linePull = power*GrappleSpeed*1.5; // I don't know what changed, but 600 was almost too weak to pull me up!

      // Instigator.Velocity = Instigator.Velocity + linePull*Inward*DeltaTime;
      Instigator.AddVelocity(linePull*Inward*DeltaTime);

    }

    if (doInwardPull) {
      // Deal with any outward velocity (against the line):
      outwardPull = Instigator.Velocity Dot Normal(Instigator.Location-pullDest);
      if (outwardPull<0 || (currentLength <= MinRetract && bSingleLine)) outwardPull=0;
      // if (outwardPull<1.0) outwardPull=1.0*outwardPull*outwardPull; // Smooth the last inch
      // This makes the line weak and stretchy:
      // Instigator.Velocity = Instigator.Velocity + Inward*DeltaTime;
      // This completely cancels outward momentum - the line length should not increase!
      // Instigator.AddVelocity( 2.0 * outwardPull * Inward ); // equilibrium (line stretches very very slowly)
      // Cancel the outward velocity - it never really should have happened.
      // Also changes global velocity by pulling it towards the line, not only countering gravity.
      Instigator.AddVelocity( 1.0 * outwardPull * Inward ); // keep the line the same length.  The 0.1 prev.nts slow sinking
      // Instigator.Velocity = Instigator.Velocity + 1.0*outwardPull*Inward; // keep the line the same length.  The 0.1 prev.nts slow sinking
      // Instigator.Velocity = Instigator.Velocity + 1.0*outwardPull*Normal(Instigator.Velocity); // Turn the lost momentum into forward momentum!  Stupid dangerous non-Newtonian nonsense!
      // DONE: I am adding this here, then in a few places below, removing it if I decide it wasn't wanted.
      //       Would be better to pass along a boolean which decides at the end whether or not to apply this.
      //       This might allow us to actually walk and jump normally if our line has gone slack.
    }

    // Stop slow gentle sinking (due to iterative gravity) by removing half of gravity pre-emptively:
    // I think these were more needed with bLinePhysics=False.
    if (bFiddlePhysics0) {
      // It should only act in Inward direction:
      // Instigator.AddVelocity( - 0.6 * Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * Inward );
      Instigator.AddVelocity( - (Inward Dot Vect(0,0,1)) * 0.6 * Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * Inward );
      // TODO: We should not need this for bLinePhysics but might still need it without it.
    }
    if (bFiddlePhysics1) {
      Instigator.AddVelocity( 0.6 * Instigator.HeadRegion.Zone.ZoneGravity * DeltaTime * Vect(0,0,-1) );
      // This is unrealistic because if they swing outwards, they still get this anti-grav effect.
      // But it but plays well!  It lets you gain and keep height with wide swings.
    }
    // Dampen gravity (useful for swinging outwards on long pulls)
    if (bFiddlePhysics2 && Instigator.Velocity.Z<0) {
      // We want to turn whatever velocity we lost into forward velocity.
      Instigator.AddVelocity( Normal(Instigator.Velocity) * 0.1*Abs(Instigator.Velocity.Z) );
      Instigator.Velocity.Z *= 0.9;
    }
    // Dampen velocity:
    // Instigator.AddVelocity( Instigator.Velocity*-0.001*DeltaTime );

  }

  simulated function ProcessTouch (Actor Other, Vector HitLocation) {
    Instigator.AmbientSound = None;
    AmbientSound = None;
    Destroy();
  }

}

// TESTING:
// What we really need to do is to get the triangle that is Player - NearPivot - NextPivot
// and move NearPivot *out* a bit.
function TryMoveTo(Actor a, Vector targetLoc) { // Removed simulated I don't know what it means when there is a Rand() involved.
  local float offness;
  local Vector NextPivot;
  // if (a.MoveSmooth(targetLoc - a.Location)) // Sucks, warpish and ugly when stuck!  Also danger of leaving a crater.  :P
  if (a.Move(targetLoc - a.Location))
    return;

  // Player is stuck!

  // Attempted solutions to getting un-stuck:

  // Best solution so far (better than staying stuck?):
  // if (lineLength<MinRetract) // We are close to winding up to the next line part.  Assumption (bad?): we only get stuck at these corners.  Yes bad assumption - sometimes the line merges through line-of-sight but we are still stuck.
  // if (FRand()<0.2) // don't always do it (mid-air collisions won't guarantee telefrag :P )  Mmm not good sometimes caused large jumps due to waiting.
    a.SetLocation(targetLoc);
  // TODO: If we are still getting mid-air telefrags, then do a trace on this movement (starting from targetLoc?), and only force it if there is a wall in the way, not a person.
  // TODO: Play sound!

  // Neither of the following attempts to un-stick the player made things better.
  /*
  offness = VSize(targetLoc - a.Location);

  if (LineSprite!=None && LineSprite.ParentLine!=None
    // Will not attempt push if angles are close
    && (Normal(LineSprite.ParentLine.NearPivot - pullDest) Dot Normal(pullDest - a.Location) < 0.8)
  ) {
    // Push out the pivot a bit:
    // BUG: sometimes this is pushing it IN and that is bad!
    //       That happens when it merges and then re-breaks at <MinRetract.
    NextPivot = LineSprite.ParentLine.NearPivot;
    // pullDest += Normal( (pullDest-a.Location) + (pullDest - NextPivot) ) * 5.0;
    pullDest += ( Normal(pullDest-a.Location) + Normal(pullDest-NextPivot) ) * 5.0; // more square
    LineSprite.NearPivot = pullDest; // May update sprite!
    PlayerPawn(a).ClientMessage("Moved pivot by "$ VSize(LineSprite.ParentLine.Reached-pullDest) $" lineLength="$lineLength);
    LineSprite.ParentLine.Reached = pullDest; // May update sprite!
  } else if (offness > 10.0 && FRand()<0.25) {
    // Bounce the player around a bit:
    // Cap the velocity we will add
    if (offness > 200)
      offness = 200;
    offness *= 2;
    a.Velocity += 1.0 * offness * Normal(VRand());
    PlayerPawn(a).ClientMessage("You are stuck "$ offness $" ("$ VSize(targetLoc - a.Location) $") lineLength="$lineLength);
    // TODO: play sound
  }
  */

  isStuck = True;
}

state() PullTowardDynamic {
  // ignores  Tick;

  // CONSIDER TODO: I did not bother implementing bPrimaryWinch and bCrouchReleases for PullTowardDynamic.

  simulated function BeginState () {
    SetPhysics(PHYS_Flying);
    Velocity = vect(0,0,0);
    Instigator.SetPhysics(PHYS_Flying);
  }

  simulated function Tick(float DeltaTime) {
    if (Thing==None)
      Self.Destroy();

    pullDest = Thing.Location;
    //// TODO: This is the wrong thing to do - only the top line moves with the Thing, but we may have a LineSprite below that one.
    ////       What we should really do is keep a copy of the tail LineSprite as well as the head.  If Thing moves, it will pull or release the lines, moving the player.
    if (LineSprite != None)
      LineSprite.NearPivot = Thing.Location; // TODO: This ain't gonna work - folding linesprite on Dynamic target is gonna be messy!

    if (Instigator==None || Master==None) {
      if (Role == ROLE_Authority) {
        if (bLogging) { Log(Level.TimeSeconds$" "$Self$".PullTowardDynamic.Tick() Server can't do motion because Instigator="$Instigator$" or Master="$Master$" btw InstigatorRep="$InstigatorRep); }
      } else {
        // We are client side.  Don't make a fuss.
        if (bLogging && FRand()<0.1) { Log(Level.TimeSeconds$" "$Self$".PullTowardDynamic.Tick() Client can't do motion because Instigator="$Instigator$" or Master="$Master$" btw InstigatorRep="$InstigatorRep); }
      }
      return; // Proceeding will just throw Accessed Nones.
      // CONSIDER: If this logs that we have InstigatorRep but not Instigator, then we should use former!
    }

    // Cause HitDamage, if not friendly:
    if ( FRand()<DeltaTime*2 // Avg twice a second
      && !(Level.Game.IsA('TeamGamePlus')
           && bThingPawn
           && Pawn(Thing).PlayerReplicationInfo!=None && Instigator.PlayerReplicationInfo!=None
           && Pawn(Thing).PlayerReplicationInfo.Team == Instigator.PlayerReplicationInfo.Team)
    ) {

      //               Avg half HitDamage
      Thing.TakeDamage(0.25*HitDamage+0.5*FRand()*HitDamage,Instigator,(Thing.Location+Location)/2,vect(0,0,0),'');

      // Allow victim to shake the hook off:
      if (VSize(Thing.Velocity) > 1.2*VSize(Instigator.Velocity) && FRand()<0.5) {
        // Thing managed to shake the hook off (special move = moving faster than opponent when check comes around)
        Self.Destroy();
        // TODO: play sound
        // Grappler's Velocity was previously either Thing.Velocity or GrappleSpeed, but now it's given more freedom.
        return;
      }

    }

    // Check for problems.
    if (VSize(Thing.Location-Location)>200.0) {
      // Somehow the Thing got far from our grapple.
      // Maybe it took a teleporter, or respawned.  We should ungrapple.
      // (Our previous pullDest may have be pointing right at the teleporter Thing took.)
      if (bLogging) Instigator.ClientMessage("Your target "$Thing.getHumanName()$" got too far away - maybe took a teleporter.");
      Destroy();
    }
    // Update Grapple's movement:
    if (VSize(Thing.Location-Location)>10.0) {
      // Self.SetLocation( Location*0.5 + Thing.Location*0.5);
      Self.Move( (Location*0.5 + Thing.Location*0.5) - Location );
      // TryMoveTo(Self, Location*0.5 + Thing.Location*0.5 );
      Self.Velocity = Thing.Velocity;
    } // TODO: gonna be jerky - better to use FChop

    // We could dampen Thing's movement.  He is carrying the mass of Instigator now!

    if (bCrouchReleases && PlayerPawn(Instigator)!=None && PlayerPawn(Instigator).bDuck!=0) {
      // Player is pressing release.  Do not affect his velocity with the line.
      Master.AmbientSound = ReleaseSound;
      AmbientSound = ReleaseSound;
    } else {

       // Update Grappler's velocity:
       if (VSize(Self.Location - Instigator.Location) > MinRetract) {
         Instigator.Velocity = Normal(Self.Location - Instigator.Location) * 0.3 * GrappleSpeed;
         Master.AmbientSound = PullSound;
         AmbientSound = PullSound;
       } else {
         // Instigator.Velocity = Thing.Velocity;  // Let our player free for a moment!
         //// Maybe better not to have sound flickering on and off
         Master.AmbientSound = None;
         AmbientSound = None;
       }

   }

    OnPull(DeltaTime);
    UpdateLineSprite();

  }

  simulated function ProcessTouch (Actor Other, Vector HitLocation) {
    Instigator.AmbientSound = None;
    AmbientSound = None;
    Destroy();
  }

}

defaultproperties {
    MyDamageType=eviscerated
    LifeSpan=60.00
    // bUnlit=True
    bUnlit=False
    bBlockActors=True
    bBlockPlayers=True
    DrawScale=1.25
    CollisionRadius=0.1
    CollisionHeight=0.2
    bNetTemporary=False
    NetPriority=2.6
    RemoteRole=ROLE_SimulatedProxy
    bMeshEnviroMap=True
    Texture=Texture'UMenu.Icons.Bg41'
    Mesh=Mesh'UnrealShare.GrenadeM'
    Physics=PHYS_Projectile // Makes it land properly in corners, hit walls well
    ThrowAngle=0
    // Physics=PHYS_Falling
    // ThrowAngle=15
    // ThrowAngle=1
    //// With PHYS_Falling I had trouble getting the hook to attach to some of the corners in Bleak

    Speed=4000.00
    MaxSpeed=4000.00
    GrappleSpeed=900 // 600 is quite fast for old Expert100, but quite sober for GrapplingHook.
    // GrappleSpeed=0
    HitDamage=20.00 // Not very strong, but you can switch weapon and hit them with something else too!
    bDoAttachPawn=True
    bSwingPhysics=True
    bLinePhysics=True // This deals with grapple pull in terms of line length.
    bLineOfSight=True
    bLineFolding=True
    bShowLine=True
    // MinRetract=50
    MinRetract=125
    // MinRetract=200
    // MinRetract=250
    bPrimaryWinch=True // Grapple only winds in while player is holding primary fire.
    bCrouchReleases=True // Grapple line unwinds while player is crouching
    bDropFlag=True
    bExtraPower=False
    bFiddlePhysics0=False // May be needed if bSwingPhysics=True but bLinePhysics=False
    bFiddlePhysics1=False // An alternative method for counteracting gravity, which unrealistically helps swing.
    bFiddlePhysics2=False // Try to help swing by turning downward momentum into forward momentum.
    // HitSound=sound'UnrealI.GasBag.hit1g'
    // HitSound=sound'KrrChink'
    HitSound=sound'hit1g'
    // PullSound=sound'SoftPull'
    PullSound=sound'Pull'
    ReleaseSound=sound'ObjectPush'
    RetractSound=sound'Slurp'
    // LineMesh=mesh'botpack.shockbm'
    // LineMesh=mesh'botpack.plasmaM'
    LineMesh=Mesh'Botpack.bolt1'
    // LineMesh=mesh'Botpack.TracerM'
    LineTexture=Texture'UMenu.Icons.Bg41'
    // LineTexture=Texture'Botpack.ammocount'
    // bLogging=False // FIXED: Since GrapplingLine accesses this as a default, sometimes this setting is more relevant than the server setting.  OK now we have one config var per class.
    // Hey would we have got the config default if we hadn't set a default here? :o
}

// It's true what he said.  If the defaultproperties has bLinePhysics=True, even if the server sets it to False and tries to replicate it, the client will see bLinePhysics=True.
