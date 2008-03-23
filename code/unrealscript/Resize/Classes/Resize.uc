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

// DONE: Translocator still thrown too fast.

// BUGS TODO:
// Bots are jumping too high again.  All flags are too large, except the red flag when it's on its spot.
// I picked up a weapon with the wrong in-hand drawscale.

// FIXED: CollisionRadius of projectiles needs to be set, but we cannot set default ;_;

// BUGS TODO:
// Translocator still seems to be thrown too fast.
// Flags are still the wrong scale.
// Auto-jump on respawn is too high (wait a bit before respawning to see this).
// Flagslugs fly too far, because we changed the gravity.  We need to change their initial Y velocity.

// var config String AllowedSizes;
var() config float Scale;

// var EDodgeDir LastDodgeState[64];
var String LastDodgeState[64];

// var String WeaponsAlreadyHandled;

var String ProjectilesAlreadyHandled;

var String LastNewActor; // considered using Actor instead of String, but would it become None if Actor is Destroy()ed?

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

/*
replication {
  reliable if (Role == Role_Authority || Role != Role_Authority) ScaleAll, Scale, ScalePawn, ScalePlayerAndInventory, DebugLog;
}
*/

simulated function DebugLog(String msg) {
  Log("<"$Role$"> "$msg);
}

simulated function DebugShout(String msg) {
  DebugLog(msg);
  BroadcastMessage("<"$Role$"> "$msg);
}

simulated function PreBeginPlay() {

  // DEBUGGING info:
  DebugLog("At start of play, ProjectilesAlreadyHandled = "$ProjectilesAlreadyHandled);
  DebugLog("At start of play, default.ProjectilesAlreadyHandled = "$default.ProjectilesAlreadyHandled);
  DebugLog("At start of play, ShockProj properties Speed="$(class'ShockProj'.default.Speed)$" MaxSpeed="$(class'ShockProj'.default.MaxSpeed)$".");

  // TODO: remove this for official release
  // for development, auto-set scale
  if (InStr(""$Self,"HalfSize")>=0) {
    Scale = 0.5;
  }
  if (InStr(""$Self,"DoubleSize")>=0) {
    Scale = 2.0;
  }

  Enable('Tick');
  Level.Game.RegisterDamageMutator(Self);
  ScaleAll();
}

simulated function PostBeginPlay() {
  Enable('Tick');
  ScaleAll();
}

simulated function Tick(float DeltaTime) {
  if (Role != Role_Authority) { DebugShout("[HOORAY NOT SERVER!] Tick()"); }
  ScaleAll(); // a freshly thrown translocator or weapon, or new projectile, should be rescaled ASAP, so we do some shrinking every tick!
  Super.Tick(DeltaTime);
}

function ModifyPlayer(Pawn p) {
  if (p.PlayerReplicationInfo.Deaths == 0 && p.IsA('PlayerPawn')) {
    p.ClientMessage("Your size is "$Int(Scale*100)$"%");
    p.ClientMessage("Map size is "$Int((1.0/Scale)*100)$"%");
  }
  Super.ModifyPlayer(p);
}

simulated function ScaleAll() {
  local Actor a;
  local bool pastLast;
  local Actor LastActorInList;

  // TODO: We could try moving these calls to ScaleActor() into HandleNewActor().
  foreach AllActors(class'Actor',a) {
    ScaleActor(a);
  }

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
  
}

simulated function HandleNewActor(Actor a) {
  // We try to avoid using this method as much as possible.
  // Since sometimes the new Actor has already been replicated before we can get to it (NetTag > 0).
  // In which case, the client will not see the changes we make here until later (or not at all - e.g. projectile speeds).

  // But if the Actor never gets passed through AlwaysKeep(),
  // and setting its default values fails,
  // it seems this is our last resort.

  DebugShout("Handling New Actor: "$a$" [NetTag="$NetTag$"]");

  // TranslocatorTarget does not pass through AlwaysKeep(), and setting it's default DrawScale didn't work.
  if (a.IsA('TranslocatorTarget')) {
    a.DrawScale = a.default.DrawScale * Scale;
  }

  // Trying to detect explosions from scaled weapons, e.g. shock-combo explosion, rockets hitting wall/floor...
  if (a.IsA('Effects')) {
    a.DrawScale = a.default.DrawScale * Scale;
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
    DebugLog("Projectile "$a$" has CC ("$a.CollisionRadius$","$a.CollisionHeight$")");
  }

}

simulated function ScaleActor(Actor a) {
  local bool bNew;

  if (Role != Role_Authority) { DebugShout("[HOORAY NOT SERVER!] ScaleActor() START"); }

  if (a.IsA('Brush') && a.IsA('Mover')) {
    return;
  }

  if (a.DrawScale == a.default.DrawScale) {
    bNew = true;
    // DebugLog("New: "$a$" [NetTag="$a.NetTag$"]");
    // No longer applies, since we are no longer changing DrawScale of all actors.
  // } else {
    // return;
  }

  if (a.IsA('Pawn')) {
    CheckPlayerStateForDodge(Pawn(a));
  }

  /*
  if (!bNew) {
    // Some things we never want to act on twice:
    if (a.IsA('PlayerPawn') || a.IsA('Bot')) {
      // TODO: add JumpZ and other essentials which reset here.
      Pawn(a).JumpZ = 325*Scale;
      // ScalePawn(Pawn(a));
      // CheckPlayerStateForDodge(Pawn(a));
      // if (a.IsA('PlayerPawn')) {
        // CheckPlayerStateForDodge(PlayerPawn(a));
      // }
      return;
    }
    // If we don't return, the player flickers (kinda bounces up and down on the floor).
    // What causes the flickering?  We could try to track it down.  afaik it's changing a property, not calling Set___().
  }
  */

  // TODO: We should not scale brush-style actors like BlockAll.

  // Change DrawScale:
  // if (!a.IsA('Bot')) {
    // a.DrawScale = a.default.DrawScale * Scale;
  // }
  // a.SetPropertyText("DrawScale",""$(a.default.DrawScale * Scale));

  if (a.IsA('Pawn')) {
    ScalePawn(Pawn(a));
    ScalePlayerAndInventory(Pawn(a));
  }

  if (a.IsA('Inventory')) {
    // a.DrawScale = a.default.DrawScale * Scale;
    Inventory(a).ThirdPersonScale = Inventory(a).default.ThirdPersonScale * Scale;
    Inventory(a).PickupViewScale = Inventory(a).default.PickupViewScale * Scale;
    Inventory(a).PlayerViewScale = Inventory(a).default.PlayerViewScale / Scale;
  }

  /*
  if (a.IsA('FlagBase') || a.IsA('Flag') || a.IsA('CTFFlag') || a.IsA('CTFFlag0') || a.IsA('CTFFlag1') || a.IsA('RedFlag') || a.IsA('BlueFlag')) {
    a.SetCollisionSize(a.default.CollisionRadius * Scale, a.default.CollisionHeight * Scale);
    a.SetLocation(a.default.Location + (-64 + 64*Scale) * Vect(0,0,1)); // flags are about 64 units above the floor - but their CollisionHeight is not 128!  I tried increasing to 72, but then the flag leaves the spot before the game starts!
  }
  */

  // if (a.IsA('PlayerPawn')) {
    // PlayerPawn(a).SetPropertyText("JumpZ",""$(325*Scale));
  // }

  if (a.IsA('Projectile')) {
    //// It's too late now to change the initialisation settings.
    // Projectile(a).Speed = Projectile(a).default.Speed * Scale;
    // Projectile(a).MaxSpeed = Projectile(a).default.MaxSpeed * Scale;
    // Projectile(a).MomentumTransfer = Projectile(a).default.MomentumTransfer * Scale;
    // if (Projectile(a).Speed > Projectile(a).MaxSpeed) {
      // Projectile(a).Speed = Projectile(a).MaxSpeed;
    // }
    // BUG TODO: somehow this fails to detect shock balls when secondary fire is held down (in online play at least)
    //           When it works, NetTag=0, when it fails, NetTag>0, so I guess the projectile has already been replicated, before we could slow it, which is why the client sees it moving too fast/slow.
    // OK so now we set each weapon's projectile's default speeds, so we no longer need to detect and slow new projectiles.
    // We could however check here that the projectiles aren't moving too fast.  (Some custom weapons might have their initial speed hard-coded.)
    /*
    if (bNew) { // Don't slow down this projectile more than once!  Only when it's first created.
      DebugShout("Slowing new projectile "$a$" [NetTag="$a.NetTag$"]");
      Projectile(a).Velocity = Projectile(a).Velocity * Scale;
      a.DrawScale = a.DrawScale * Scale;
      a.SetCollisionSize(a.default.CollisionRadius * Scale, a.default.CollisionHeight * Scale);
    }
    */
  }

  if (Role != Role_Authority) { DebugShout("[HOORAY NOT SERVER!] ScaleActor() END"); }

}

simulated function ScalePawn(Pawn p) {
  p.DrawScale = p.default.DrawScale * Scale;
  p.BaseEyeHeight = p.default.BaseEyeHeight * Scale;
  p.EyeHeight = p.default.EyeHeight * Scale;
  p.GroundSpeed = p.default.GroundSpeed * Scale;
  p.JumpZ = 325*Scale;
  ScaleCollisionCylinder(p);
}

simulated function ScalePlayerAndInventory(Pawn p) {
  local Inventory inv;
  // All disabled; we now scale these actors when they are created (in AlwaysKeep()).
  for( inv=p.Inventory; inv!=None; inv=inv.Inventory ) {
    // if (p.IsA('Bot')) {
      // inv.DrawScale = inv.default.DrawScale * Scale;
    // }
    /*
    if (inv.bHeldItem) { <-- has been picked up
      inv.ThirdPersonScale = inv.default.ThirdPersonScale * Scale;
      inv.PickupViewScale = inv.default.PickupViewScale * Scale;
      inv.DrawScale = inv.default.DrawScale;
      inv.PlayerViewScale = inv.default.PlayerViewScale;
    }
    */
    //// Didn't seem to have any affect, so now we slow down projectiles when they are first created.
    // if (inv.IsA('Weapon')) {
      // Weapon(inv).ProjectileSpeed = Weapon(inv).default.ProjectileSpeed * Scale;
      // Weapon(inv).AltProjectileSpeed = Weapon(inv).default.AltProjectileSpeed * Scale;
    // }
    // if (inv.IsA('Translocator')) {
      // Translocator(inv).TossForce = Translocator(inv).default.TossForce * Scale;
      // Translocator(inv).MaxTossForce = Translocator(inv).default.MaxTossForce * Scale;
    // }
  // }
  // Fix the player's dodge height, because it's fixed at 160 in Epic's code!!
  // if (p.DodgeDir == DODGE_Active && p.Velocity.Z >= 140 && p.Velocity.Z <= 180) {
    // DebugShout("Fixing dodge for "$p$" (acc="$p.Acceleration$")");
    // // p.Location.Z = p.Location.Z - p.Velocity.Z * DeltaTime;
    // // if (Role == Role_Authority) {
      // p.SetLocation(p.Location - Vect(0,0,1) * p.Velocity.Z * DeltaTime); // undo the error caused by the tick before we could fix it
      // p.Velocity.Z = p.Velocity.Z * Scale * Scale;
    // // }
    // // p.Velocity = p.Velocity * Scale;
  }
}

function CloserToFloor(Actor Other) {
  // Other.SetLocation(Other.Location + (-24 + 24*Scale) * Vect(0,0,1));
  Other.SetPhysics(PHYS_Falling);
}

// It seems all Actors go through all 3 of these when they are spawned, except for Projectiles and Effects, and TranslocatorTarget.

function bool AlwaysKeep(Actor Other) {

  // TODO: we don't have to use default any more, since this method should only be called once for each new actor :)

  local bool doneSomething;

  DebugShout("AlwaysKeep("$Other$") [NetTag="$Other.NetTag$"]");

  if (Other.DrawScale != 1.0) {
    DebugShout("Interesting: "$Other$" spawned with DrawScale="$Other.DrawScale);
  }

  // Other Actor properties worth scaling:
  Other.PrePivot = Other.default.PrePivot * Scale;
  Other.VisibilityRadius = Other.default.VisibilityRadius * Scale;
  Other.VisibilityHeight = Other.default.VisibilityHeight * Scale;
  Other.SpriteProjForward = Other.default.SpriteProjForward * Scale;
  // TODO: sound changes are good for weapons, but should not apply to environment (map) sounds
  Other.SoundRadius = Other.default.SoundRadius * Scale;
  Other.SoundVolume = Other.default.SoundVolume / Scale; // don't make the sound quieter, just more localised!
  Other.TransientSoundRadius = Other.default.TransientSoundRadius * Scale;
  Other.TransientSoundVolume = Other.default.TransientSoundVolume / Scale; // don't make the sound quieter, just more localised!
  // BUG: I noticed when I picked up a dropped enemy flag, the sirens sound was too quiet,
  // presumably because this is a non-localised sound with no position.

  if (Other.IsA('Pawn')) {
    //// These didn't work here, so moved to Tick().
    ScalePawn(Pawn(Other));
    ScalePlayerAndInventory(Pawn(Other));
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
  if (Other.IsA('FlagBase')) { // || Other.IsA('CTFFlag')) {
    Other.SetLocation(Other.Location + (-64 + 64*Scale) * Vect(0,0,1)); // flags are about 64 units above the floor - but their CollisionHeight is not 128!  I tried increasing to 72, but then the flag leaves the spot before the game starts!
    doneSomething = True;
  }
  if (Other.IsA('CTFFlag')) {
    // These changes were in FlagBase, but did not affect dropped flags.
    DebugLog("Scaling flag: "$Other);
    ScaleCollisionCylinder(Other);
    Other.DrawScale = Other.default.DrawScale * Scale;
    doneSomething = True;
  }

  if (Other.IsA('ZoneInfo') || Other.IsA('LevelInfo')) {
    ZoneInfo(Other).ZoneGravity = ZoneInfo(Other).default.ZoneGravity * Scale;
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

    // DONE: Safer to use ProjectilesAlreadyHandled, in case two weapons have the same projectiles.
    // if (InStr(WeaponsAlreadyHandled," "$Other.class$" ") == -1) {
      DebugLog("Scaling weapon type: "$Other.class);
      ScaleProjectileClass(Weapon(Other).ProjectileClass);
      ScaleProjectileClass(Weapon(Other).AltProjectileClass);
    // }
    // WeaponsAlreadyHandled = WeaponsAlreadyHandled $ " " $ Other.class $ " ";

    // Which setting really affects the speed?  The speed in the weapon or in the projectile?
    Weapon(Other).ProjectileSpeed = Weapon(Other).default.ProjectileSpeed * Scale;
    Weapon(Other).AltProjectileSpeed = Weapon(Other).default.AltProjectileSpeed * Scale;

    if (Other.IsA('Translocator')) {
      // Translocator(Other).default.TossForce = Translocator(Other).default.TossForce * Scale;
      // Translocator(Other).default.MaxTossForce = Translocator(Other).default.MaxTossForce * Scale;
      Translocator(Other).TossForce = Translocator(Other).default.TossForce * Scale;
      Translocator(Other).MaxTossForce = Translocator(Other).default.MaxTossForce * Scale;
      // // class'TranslocatorTarget'.default.DrawScale = class'TranslocatorTarget'.default.DrawScale * Scale;
      //// This did not work: DrawScale of new TranslocatorTargets was changed, but didn't appear any different
      //// until I changed it to something different and then back, using ActorEditor.  :o
      // class'TranslocatorTarget'.default.DrawScale = Scale;
      // if (InStr(WeaponsAlreadyHandled," TranslocatorTarget ") == -1) {
        // class'TranslocatorTarget'.default.DrawScale = class'TranslocatorTarget'.default.DrawScale * Scale;
        // WeaponsAlreadyHandled = WeaponsAlreadyHandled $ " TranslocatorTarget ";
      // }
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
  // DebugShout("Damage "$DamageType$" @ "$HitDistance);
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
  if (InStr(default.ProjectilesAlreadyHandled," "$ProjectileClass$" ") != -1) {
    return; // We already scaled these default properties earlier.
  }
  DebugLog("Scaling projectile type: "$ProjectileClass);
  // BUG TODO: UTChunk and flakslug report 2 x "Accessed null" here.
  ProjectileClass.default.DrawScale = ProjectileClass.default.DrawScale * Scale;
  ProjectileClass.default.Speed = ProjectileClass.default.Speed * Scale;
  ProjectileClass.default.MaxSpeed = ProjectileClass.default.MaxSpeed * Scale;
  ProjectileClass.default.MomentumTransfer = ProjectileClass.default.MomentumTransfer * Scale;
  ProjectileClass.default.ExplosionDecal.default.DrawScale = ProjectileClass.default.ExplosionDecal.default.DrawScale * Scale;
  DebugLog("New properties for "$ProjectileClass$": Speed="$ProjectileClass.default.Speed$" MaxSpeed="$ProjectileClass.default.MaxSpeed$".");
  // We cannot scale default CollisionSize here, so we do it later.  Oh well strangely we didn't need to!
  // Add this Projectile type to the list, so that we don't scale its defaults again later:
  default.ProjectilesAlreadyHandled = default.ProjectilesAlreadyHandled $ " " $ ProjectileClass $ " ";
}

function ScaleCollisionCylinder(Actor Other) {
  Other.SetCollisionSize(Other.default.CollisionRadius * Scale, Other.default.CollisionHeight * Scale);
}

