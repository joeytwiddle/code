class Resize extends Mutator config(Resize);

// NOTE: I failed to get any code executing on the client in online play, but we are trying to work without it.
// FIXED (mostly): Splash damage needs to be scaled up/down, but HurtRadius() is sometimes called with a FIXED value. :S
// FIXED: Sometimes projectiles move too slow (sometimes they look slow but are actually standing STILL!).
// FIXED: Dodge is too high and long, jump is too high!
// FIXED: When making player large, he bounces on the floor.  =(
// FIXED: The first flakslug thrown looks like it's going on the default trajectory, but explodes at correct point.
// FIXED: Some shock balls seem to travel too fast (or slow): to reproduce, shoot secondary then primary then secondary - the latter will overtake the foremost!  or hold flak secondary fire to see the same problem there.  hold shock secondary for the same
// FIXED: Projectiles (well shock at least) do not come out of what looks like the correct point - shock balls appear higher than primaries, making flat moving combos harder.  mm not sure if i fixed that, but when scaling up, flak slugs do seem to leave the hand a bit low
// FIXED: Grenades don't look like they have changed size (or do they?); grenades seem to explode not where they appear to be!
// FIXED: Shock-balls seem harder to hit (well collision radius has been reduced, along with DrawScale and speed).
// FIXED: No really, some stand-combos can fail!  try aiming down a little to reproduce this.
// Maybe fixed: Shock-combo explosions look a bit funny when scaled, because the 2D sprite is not scaled.
// FIXED: Scaled-down players slide down slopes. :(
// OLD BUGS TO TEST: Tested Deck-16][ single-player: player was always bouncing on the floor; bio primary fine but secondary fires too far; headshots fine; both ripper shots seemed fine; pulse primary fine, secondary is long and broken; minigun bullets look too fast imo.

// FIXED: Some actors don't start at their default DrawScale (e.g. the curtain meshes on Noggins2ndMap), which breaks bNew detection.
//       Ideally we would scale things only ONCE when they are first created, and not relative to their default.DrawScale, but relative to their current scale.
//       Maybe that's why bots appear a little smaller than my eye height suggests they should.
// FIXED: Weapons in other player's hands look the default size; weapons in your hand are over-exaggerated!

// FIXED (I was using bNew on PlayerPawns): JumpZ always ends up at 357.5 - I can't set my own value!  But I can do it with ActorEditor.  :o
// DONE: We changed JumpZ so that the player goes higher (or lower) when scaled up (or down).
//       But now we must change gravity, so they spend the correct amount of time in the air ... and this will change everything else, e.g. flakslugs.

// FIXED: gravity and JumpZ!

// OLD BUG TO TEST: When testing MiniPureAction offline with scale=0.5, bot jumped way too high, hurt himself with his own combo, bounced off my combo too much, and the flag also bounced too far when it was dropped.

// OLD BUG TO TEST: Try scaling to 0.25 - you will see that when throwing TL, dodging or jumping, they start at *normal* height, not from low where they should.
//           But at 0.5 it all seems fine, so maybe we're just getting *too* small, and the engine is refusing to spawn things so close to the ground?

// Since v11:

// FIXED: bots still seem to be jumping too high

// BUG: players can walk up steps which they shouldn't be able to with their new scale

// DONE: ok so we are scaling projectiles fine, but we should also scale the final explosions!

// OLD BUG TO TEST: PlayerPawn.ThrowWeapon() gives the weapon a hard-coded velocity, so we must detect when a weapon is thrown during Tick() and slow it down.  :|

// FIXED: CollisionRadius of projectiles needs to be set, but we cannot set default ;_;
// FIXED: some shock balls come out at the right height; some don't

// TODO: There is at least one case where bots can jump with more velocity than JumpZ (JumpOffPawn()).
//       We may need to detect when a bot starts a jump, and check its velocity.

// BUGS TODO:
// I picked up a weapon with the wrong in-hand drawscale.
// Thrown weapons fly at wrong speed.
// Fall damage is still a problem.
// Can we change Epic's default splash damage radius?  What about fall damage threshold?  (We could force our own for up-scale, but for down-scale it would be good to make fall damage threshold *more* sensitive.)
// Flakslugs fly too far, because we changed the gravity.  We need to change their initial Y velocity.
// BUG: TranslocTarget *looks* like it is moving too fast, although it isn't.  It also looks too small.
// Hammerjumps on Bleak-DoubleSize were not working.
// This problem with HandleNewActor not getting called on the player when ATB switches them is nasty.
// Although we've moved ScaleCollisionCylinder into Tick, we can't move ScalePawn, and if we don't, then sometimes the player's EyeHeight looks wrong (on Bleak-DoubleSize anyway).  Can we fix NewActor detection so that ATB doesn't break it?  Can we also force ATB to always switch us, to make this easier to test + debug.
// Dodge fix does not appear to be working on Bleak-DoubleSize.  Either because ATB is breaking my Player, or because we get to the Player after he's already lost some Z velocity.  May also be due to my assumed DeltaTime not being as large as the DeltaTime that Tick is logging. ^^

// var config String AllowedSizes;
var() config float Scale;

// var EDodgeDir LastDodgeState[64];
var String LastDodgeState[64];

// var String WeaponsAlreadyHandled;

var String ProjectilesAlreadyHandled;

var String LastNewActor; // I considered using Actor instead of String, but would it become None if Actor is Destroy()ed?

// Store the real default projectile properties:
var config String StoredPropName[64];
var config String StoredPropValue[64];

defaultproperties {
  // AllowedSizes="0.5,0.75,1.25,1.5,2.0"
  // Scale=2.00
  Scale=2.0
  // Scale=0.5
  /*
    // Attempt to get simulation working:
    bAlwaysRelevant=true
    // bNoDelete=True // Stopped mutator from spawning
    // bStasis=True // Stopped mutator from spawning
    bStatic=False
    NetPriority=3.0
    NetUpdateFrequency=10
    RemoteRole=ROLE_SimulatedProxy
  */
}

function DebugLog(String msg) {
  Log("<"$Role$"> "$msg);
}

function DebugShout(String msg) {
  DebugLog(msg);
  BroadcastMessage("<"$Role$"> "$msg);
}

function PreBeginPlay() {

  // DEBUGGING info:
  DebugShout("At start of play, ProjectilesAlreadyHandled = "$ProjectilesAlreadyHandled);
  DebugShout("At start of play, default.ProjectilesAlreadyHandled = "$default.ProjectilesAlreadyHandled);
  DebugShout("At start of play, ShockProj properties Speed="$(class'ShockProj'.default.Speed)$" MaxSpeed="$(class'ShockProj'.default.MaxSpeed)$".");

  // TODO: remove this for official release
  // for development, auto-set scale on my test maps
  // Regardless of whether I put it in PreBeginPlay() or PostBeginPlay(), the new value of Scale became overwritten until I did SaveConfig().
  if (InStr(""$Self,"HalfSize")>=0) {
    Scale = 0.5; SaveConfig();
  }
  if (InStr(""$Self,"DoubleSize")>=0) {
    Scale = 2.0; SaveConfig();
  }
  DebugShout(Self$".PreBeginPlay() Scale="$Scale);

  Enable('Tick');
  Level.Game.RegisterDamageMutator(Self);
  ScaleAll();
}

function PostBeginPlay() {
  Enable('Tick');
  ScaleAll();
}

function Tick(float DeltaTime) {
  ScaleAll(); // a freshly thrown translocator or weapon, or new projectile, should be rescaled ASAP, so we do some shrinking every tick!
  Super.Tick(DeltaTime);
  if (FRand()>0.99) {
    DebugLog("Tick() DeltaTime="$DeltaTime);
  }
}

function ModifyPlayer(Pawn p) {
  if (p.PlayerReplicationInfo.Deaths == 0 && p.IsA('PlayerPawn')) {
    p.ClientMessage("Your size is "$Int(Scale*100)$"%");
    p.ClientMessage("Map size is "$Int((1.0/Scale)*100)$"%");
  }
  p.JumpZ = 325*Scale; // I hope this will stop me jumping too high when I spawn, and prevent the JUMP-BUG reports.
  Super.ModifyPlayer(p);
}

function ScaleAll() {
  local Actor a;
  local bool pastLast;
  local Actor LastActorInList;

  // Let's do a test.  Are new Actors always added to the end of the list?
  // Nice, it seems so!  We can use this to fiddle with only newly spawned Actors!
  pastLast = False;
  foreach AllActors(class'Actor',a) {
    if (pastLast) {
      HandleNewActor(a);
    }
    if ((""$a) == LastNewActor) {
      pastLast = True;
    }
    LastActorInList = a;
  }
  LastNewActor = ""$LastActorInList;

  foreach AllActors(class'Actor',a) {
    ScaleActor(a);
  }

}

function ScaleActor(Actor a) {
  // local bool bNew;

  // TODO: We could try moving these calls into HandleNewActor().

  if (a.IsA('Brush') && a.IsA('Mover')) {
    return;
  }

  // if (a.DrawScale == a.default.DrawScale) {
    // bNew = true;
  // }

  if (a.IsA('Pawn')) {
    CheckPlayerStateForDodge(Pawn(a));
    // ScalePawn(Pawn(a)); // Calling this repeatedly is what causes projectiles to leave the weapon at the wrong height.
    ScaleCollisionCylinder(a); // But we must call this, or Player can sometimes end up running around with feet in the floor.  (If HandleNewActor() was not successfully called on that player.)
    if (Pawn(a).JumpZ != 325*Scale) {
      DebugShout("JUMP-BUG: "$a.getHumanName()$".JumpZ != "$325*Scale);
    }
    Pawn(a).JumpZ = 325*Scale; // Best to do it here!  If we do this in HandleNewActor(), it doesn't work.
  }

  // TODO: We should not scale brush-style actors like BlockAll.

  if (a.IsA('Inventory')) {
    // a.DrawScale = a.default.DrawScale * Scale;
    Inventory(a).ThirdPersonScale = Inventory(a).default.ThirdPersonScale * Scale;
    Inventory(a).PickupViewScale = Inventory(a).default.PickupViewScale * Scale;
    Inventory(a).PlayerViewScale = Inventory(a).default.PlayerViewScale / Scale;
  }

}

function HandleNewActor(Actor a) {
  // We try to avoid using this method as much as possible.
  // Since sometimes the new Actor has already been replicated before we can get to it (NetTag > 0).
  // In which case, the client will not see the changes we make here until later (or not at all - e.g. projectile speeds).

  // But if the Actor never gets passed through AlwaysKeep(),
  // and setting its default values fails,
  // it seems this is our last resort.

  if (a.IsA('ShockProj')) { // just to reduce spam while i watch these
  if (a.NetTag>0) {
    DebugShout("LATE! Handling New Actor: "$a$" [NetTag="$a.NetTag$"]");
  } else {
    DebugLog("Handling New Actor: "$a$" [NetTag="$a.NetTag$"]");
  }
  }

  // TranslocatorTarget does not pass through AlwaysKeep(), and setting its default DrawScale didn't work.
  if (a.IsA('TranslocatorTarget')) {
    DebugLog("Scaling TT "$a$" from "$a.DrawScale$"");
    a.DrawScale = a.DrawScale * Scale; // BUG: but it's becoming too small!
  }

  // Trying to detect explosions from scaled weapons, e.g. shock-combo explosion, rockets hitting wall/floor...
  if (a.IsA('Effects')) {
    a.DrawScale = a.DrawScale * Scale;
  }
  // We could try making a list of all such weapon-hit effects, and changing their default scale.
  // This is likely to detect and scale some map-embedded effects, which would not look right.

  if (a.IsA('Projectile')) {
    // I'm not sure whether this is needed or not.
    // The collision cylinder of Projectiles used to be un-scaled,
    // but maybe somehow they are auto-scaled since we changed each Projectile's default.DrawScale.
    // ScaleCollisionCylinder(a);
    // Conclusion: it's definitely not needed, but I don't know why.
    // If we do scale the collision cylinder, shock balls become much harder to hit.
    // DebugLog("Projectile "$a$" has CC ("$a.CollisionRadius$","$a.CollisionHeight$")");
    ScaleCollisionCylinder(a);
    a.DrawScale = a.DrawScale * Scale;
  }

  //// Sometimes this doesn't get called!  So better not to do it at all.
  //// I think it doesn't get called if AutoTeamBalance swaps the player to another team, but I'm not sure.
  // BUT if we don't call it, then we can end up running about with our feet in the floor!
  // BUT still it sometimes doesn't get called, so I'm trying it in Tick()...
  if (a.IsA('Pawn')) {
    // ScalePawn(Pawn(a)); // If we do this in ScaleActor() then projectiles leave the weapon at the wrong height.
    // ScaleCollisionCylinder(a);
  }

  if ((a.IsA('CTFFlag') /*|| a.IsA('FlagBase') <-- never happens */) /*&& a.DrawScale == a.default.DrawScale <-- unreliable! */) {
    // These changes were in FlagBase, but did not affect dropped flags.
    DebugLog("Scaling CTFFlag: "$a$" @"$a.Location$" (DrawScale="$a.DrawScale$" default.DrawScale="$a.default.DrawScale$")");
    // a.SetLocation(a.Location + (-84 + 84*Scale) * Vect(0,0,1));
    ScaleCollisionCylinder(a);
    a.DrawScale = a.DrawScale * Scale; // Otherwise it looks too big when held by player.
  }

}

function ScalePawn(Pawn p) {
  ScaleCollisionCylinder(p);
  p.DrawScale = p.default.DrawScale * Scale;
  p.BaseEyeHeight = p.default.BaseEyeHeight * Scale;
  p.EyeHeight = p.default.EyeHeight * Scale;
  p.GroundSpeed = p.default.GroundSpeed * Scale;
}

function CloserToFloor(Actor Other) {
  // Other.SetLocation(Other.Location + (-24 + 24*Scale) * Vect(0,0,1));
  Other.SetPhysics(PHYS_Falling);
}

// It seems all Actors go through all 3 of these when they are spawned, except for Projectiles and Effects, and TranslocatorTarget.

function bool AlwaysKeep(Actor Other) {

  // TODO: we don't have to use default any more, since this method should only be called once for each new actor :)

  local bool doneSomething;

  DebugLog("AlwaysKeep("$Other$") "$Other.Location$" [NetTag="$Other.NetTag$"]");

  if (Other.DrawScale != 1.0) {
    DebugShout("Interesting: "$Other$" spawned with DrawScale="$Other.DrawScale);
  }

  // Other Actor properties worth scaling:
  Other.PrePivot = Other.PrePivot * Scale;
  Other.VisibilityRadius = Other.VisibilityRadius * Scale;
  Other.VisibilityHeight = Other.VisibilityHeight * Scale;
  Other.SpriteProjForward = Other.SpriteProjForward * Scale;
  // TODO: sound changes are good for weapons, but should not apply to environment (map) sounds
  Other.SoundRadius = Other.SoundRadius * Scale;
  Other.SoundVolume = Other.SoundVolume / Scale; // don't make the sound quieter, just more localised!
  Other.TransientSoundRadius = Other.TransientSoundRadius * Scale;
  Other.TransientSoundVolume = Other.TransientSoundVolume / Scale; // don't make the sound quieter, just more localised!
  // BUG: I noticed when I picked up a dropped enemy flag, the sirens sound was too quiet,
  // presumably because this is a non-localised sound with no position.

  if (Other.IsA('Pawn')) {
    ScalePawn(Pawn(Other));
    Pawn(Other).JumpZ = 325*Scale;
    doneSomething = True;
  }

  if (Other.IsA('Inventory')) { // || Other.IsA('Pickup') || Other.IsA('Weapon')
    //// These didn't work here, so moved to Tick().
    // Inventory(Other).ThirdPersonScale = Inventory(Other).default.ThirdPersonScale * Scale;
    // Inventory(Other).PickupViewScale = Inventory(Other).default.PickupViewScale * Scale;
    // Other.default.DrawScale = Scale;
    // Other.DrawScale = Scale;
    // Inventory(Other).DrawScale = Inventory(Other).default.DrawScale * Scale;
    // Inventory(Other).PlayerViewScale = Inventory(Other).default.PlayerViewScale;
    ScaleCollisionCylinder(Other);
    CloserToFloor(Other);
    doneSomething = True;
  }

  // Change CollisionRadius/Height, and move up/down correspondingly
  // Flags require some special changes
  // TODO: this isn't scaling the flags, move back into Tick().
  if (Other.IsA('FlagBase') /*|| Other.IsA('CTFFlag')*/) {
    DebugLog("Scaling FlagBase @"$Other.Location$" DrawScale="$Other.DrawScale$" default.DrawScale="$Other.default.DrawScale); // this log showed us that default.DrawScale == Epic's default, not the map's default
    // The default flag has DrawScale 1.3 and centre at about 84 units above the floor, so we move its centre to compensate for its new DrawScale.
    ScaleCollisionCylinder(Other); // We must scale the collision cylinder before we move it!
    Other.SetLocation(Other.Location + (-84 + 84*Scale) * Other.DrawScale/1.3 * Vect(0,0,1)); // makes no difference
    Other.DrawScale = Other.DrawScale * Scale;
    DebugLog("FlagBase now @"$Other.Location$" DrawScale="$Other.DrawScale);
    doneSomething = True;
  }

  if (Other.IsA('ZoneInfo') || Other.IsA('LevelInfo')) {
    ZoneInfo(Other).ZoneGravity = ZoneInfo(Other).ZoneGravity * Scale;
    doneSomething = True;
  }

  if (Other.IsA('PathNode')) {
    CloserToFloor(Other);
    doneSomething = True;
  }

  if (Other.IsA('PlayerStart')) {
    CloserToFloor(Other);
    doneSomething = True;
  }

  if (Other.IsA('InventorySpot')) {
    Other.DrawScale = Other.DrawScale * Scale;
    CloserToFloor(Other);
    doneSomething = True;
  }

  if (Other.IsA('Weapon')) {

    ScaleProjectileClass(Weapon(Other).ProjectileClass);
    ScaleProjectileClass(Weapon(Other).AltProjectileClass);

    // Which setting really affects the speed?  The speed in the weapon or in the projectile?
    Weapon(Other).ProjectileSpeed = Weapon(Other).ProjectileSpeed * Scale;
    Weapon(Other).AltProjectileSpeed = Weapon(Other).AltProjectileSpeed * Scale;

    if (Other.IsA('Translocator')) {
      Translocator(Other).TossForce = Translocator(Other).TossForce * Scale;
      Translocator(Other).MaxTossForce = Translocator(Other).MaxTossForce * Scale;
    }

    doneSomething = True;
  }

  if (Other.IsA('Light') || Other.IsA('locationid')) {
    doneSomething = True; // Just to quieten the reporter
  }

  if (!doneSomething) {
    DebugShout("AlwaysKeep() DID NOTHING FOR "$Other);
  }

  // DebugLog("AlwaysKeep("$Other$") ENDED");

	return Super.AlwaysKeep(Other);
}

// function bool IsRelevant(Actor Other, out byte bSuperRelevant) {
  // DebugLog("IsRelevant("$Other$") [NetTag="$Other.NetTag$"]");
  // return Super.IsRelevant(Other,bSuperRelevant);
// }

// function bool CheckReplacement(Actor Other, out byte bSuperRelevant) {
  // DebugLog("CheckReplacement("$Other$") [NetTag="$Other.NetTag$"]");
  // return Super.CheckReplacement(Other,bSuperRelevant);
// }

function MutatorTakeDamage( out int ActualDamage, Pawn Victim, Pawn InstigatedBy, out Vector HitLocation, out Vector Momentum, name DamageType) {
  local float HitDistance;
  HitDistance = VSize(HitLocation - Victim.Location);
  DebugLog("Damage "$DamageType$" @ "$HitDistance);
  if (DamageType == 'Fell') {
    // Since we change gravity, we must change fall damage also.
    DebugShout("Un-scaling fell damage");
    ActualDamage = ActualDamage / Scale;
  } else {
  // if (DamageType == 'jolted' || DamageType == 'RocketDeath' || DamageType=='FlakDeath' || DamageType=='GrenadeDeath' || DamageType=='RipperAltDeath') {
    // BUG TODO: still haven't properly detected whether damage is splash or not
    // both jolted and RocketDeath count for primary and secondary shots
    // impact *damage* should never be scaled
    // We could check the distance of the HitLocation from the Victim, to see if it is within his collision cylinder or not.  But I'm not sure this is entirely accurate.
    // NOTE:
    // when scaled at 0.5
    // shock-primary had HitDistance ~9
    // shock-secondary had HitDistance ~14
    // bot's collision radius was ~8
    // SO: i check HitDistance < Victim.CollisionRadius * 1.25 which seems to work :P
    if (HitDistance > Victim.CollisionRadius*1.25 /*&& HitDistance > Victim.CollisionHeight*/) {
      // Gets called sometimes even on a direct hit. :(
      // Seems to be outside the Victim, so it's splash damage, so scale it:
      DebugShout("Scaling splash damage "$DamageType$" on "$Victim.getHumanName()$" ("$HitDistance$")");
      ActualDamage = ActualDamage * Scale;
      // Check with scale=2.0 that 1 primary shock deals correct damage (and does not kill)!
    }
    Momentum = Momentum * Scale; // to compensate for scale change
    Momentum.Y = Momentum.Y * Scale; // to compensate for gravity change :: TODO: we should do this for everything which spawns or is thrown, i.e. flag when dropped, weapon when thrown
  // }
  // BUG UNFIXABLE: ofc if the engine already thinks the damage was out of range, we can't scale that damage up. (When scaling up.) =/
  // conversely, when scaling down, if the player runs forward and hits his own combo, the engine says that he *is* in range, and gets hurt by it (even though we scale that damage down, he still gets hurt!)
  }
  Super.MutatorTakeDamage( ActualDamage, Victim, InstigatedBy, HitLocation, Momentum, DamageType );
}

// We need to check when a player (or other pawn) starts to dodge, so that we can fix (change) their velocity,
// since the dodge height (y velocity) is hard-coded in Epic's code, so we can't set it in advance.
function CheckPlayerStateForDodge(Pawn p) {
  local float DeltaTime;
  // OLD BUG I THINK FIXED: fails in offline play with a lot of AccessedNone, due to PlayerReplicationInfo == None
  // OLD BUG I THINK FIXED: now failing in online play, again due a huge number of Accessed None O_o
  /*
  DebugLog("CheckPlayerStateForDodge [START]");
  DebugLog("CheckPlayerStateForDodge p = "$p.getHumanName());
  DebugLog("CheckPlayerStateForDodge p.DodgeDir = "$p.DodgeDir);
  DebugLog("CheckPlayerStateForDodge p.PRI.PID = "$(p.PlayerReplicationInfo.PlayerID % 64));
  DebugLog("CheckPlayerStateForDodge LastDodgeState = "$LastDodgeState[p.PlayerReplicationInfo.PlayerID % 64]);
  DebugLog("CheckPlayerStateForDodge Checking...");
  */
  if ((""$p.DodgeDir) != LastDodgeState[p.PlayerReplicationInfo.PlayerID % 64]) {
    if (p.DodgeDir == DODGE_Active) {
      DebugShout("Scaling dodge Y for "$p.getHumanName()$" ["$LastDodgeState[p.PlayerReplicationInfo.PlayerID % 64]$" -> "$p.DodgeDir$"]");
      // Try to correct the changes from last tick:
      DeltaTime = 0.05; // BUG: Assumes tickrate 20, but we could arrange for the real DeltaTime to be passed down to us.
      p.SetLocation(p.Location + (-p.Velocity.Z*DeltaTime + p.Velocity.Z*DeltaTime*Scale) * Vect(0,0,1));
      // Set correct upward velocity:
      p.Velocity.Z = p.Velocity.Z * Scale;
    }
  }
  LastDodgeState[p.PlayerReplicationInfo.PlayerID % 64] = ""$p.DodgeDir;
  // CONSIDER: When scaling up, the dodge seems a little low, but distance reached seems ok.
  //           Maybe this is because Velocity.Z has already changed by 1 tick before we get to it.
  // DebugLog("CheckPlayerStateForDodge [END]");
}

// PROBLEM TODO: When we change the default values of projectiles, they stay changed next map!!  And hence get scaled again, becoming over-scaled (too slow/fast).
// Maybe we can get around this by caching things already been scaled
// Well it seems that default.ProjectilesAlreadyHandled does not last between maps, the same way that Projectiles default properties do.  =(
// Well I guess one yukky solution, would be to save the first values we ever find, in our config file, and use those stored values as the defaults we scale from in future.
function ScaleProjectileClass(class<Projectile> ProjectileClass) {
  if (ProjectileClass == None) {
    return; // Presumably the parent weapon had no projectile for primary/secondary fire.
  }
  if (InStr(ProjectilesAlreadyHandled," "$ProjectileClass$" ") != -1) {
    return; // We already scaled these default properties earlier.
  }
  DebugShout("Scaling projectile type: "$ProjectileClass);
  // BUG TODO: UTChunk and flakslug report 2 x "Accessed null" here.
  // ProjectileClass.default.DrawScale = ProjectileClass.default.DrawScale * Scale; // This does not seem to work, so doing it in HandleNewActor() instead.
  ProjectileClass.default.Speed = ScaleOnceOnly(ProjectileClass$".default.Speed", ProjectileClass.default.Speed);
  ProjectileClass.default.MaxSpeed = ScaleOnceOnly(ProjectileClass$".default.MaxSpeed", ProjectileClass.default.MaxSpeed);
  ProjectileClass.default.MomentumTransfer = ScaleOnceOnly(ProjectileClass$".default.MomentumTransfer", ProjectileClass.default.MomentumTransfer);
  ProjectileClass.default.ExplosionDecal.default.DrawScale = ScaleOnceOnly(ProjectileClass$".default.ExplosionDecal.default.DrawScale", ProjectileClass.default.ExplosionDecal.default.DrawScale);
  DebugShout("New properties for "$ProjectileClass$": Speed="$ProjectileClass.default.Speed$" MaxSpeed="$ProjectileClass.default.MaxSpeed$".");
  // We cannot scale default CollisionSize here, so we do it later.  Oh well strangely we didn't need to!
  // Add this Projectile type to the list, so that we don't scale its defaults again later:
  ProjectilesAlreadyHandled = ProjectilesAlreadyHandled $ " " $ ProjectileClass $ " ";
}

function float ScaleOnceOnly(String prop, float currentValue) {
  local String storedValue;
  storedValue = GetStoredProp(prop);
  if (storedValue == "") {
    PutStoredProp(prop,""$currentValue);
  } else {
    currentValue = Float(storedValue);
  }
  return currentValue * Scale;
}

function String GetStoredProp(String prop) {
  local int i;
  for (i=0;i<64;i++) {
    if (StoredPropName[i] == prop) {
      return StoredPropValue[i];
    }
  }
  return "";
}

function PutStoredProp(String prop, String value) {
  local int i;
  for (i=0;i<64;i++) {
    if (StoredPropName[i] == "") {
      DebugShout("Storing property "$(i+1)$"/64: "$prop$" = "$value);
      StoredPropName[i] = prop;
      StoredPropValue[i] = value;
      SaveConfig();
      return;
    }
  }
  DebugShout("FAILED to store property "$prop$" = "$value);
  return;
}

function ScaleCollisionCylinder(Actor Other) {
  Other.SetCollisionSize(Other.default.CollisionRadius * Scale, Other.default.CollisionHeight * Scale);
}

