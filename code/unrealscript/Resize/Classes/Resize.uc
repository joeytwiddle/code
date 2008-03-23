class Resize extends Mutator config(Resize);

// NOTE: I failed to get any code executing on the client in online play, but we are trying to work without it.
// BUG TODO: Splash damage needs to be scaled up/down, but HurtRadius() is sometimes called with a FIXED value. :S
// FIXED: Sometimes projectiles move too slow (sometimes they look slow but are actually standing STILL!).
// FIXED: Dodge is too high and long, jump is too high!
// FIXED: When making player large, he bounces on the floor.  =(
// BUG TODO: The first flakslug thrown looks like it's going on the default trajectory, but explodes at correct point.
// BUG TODO: Some shock balls seem to travel too fast (or slow): to reproduce, shoot secondary then primary then secondary - the latter will overtake the foremost!  or hold flak secondary fire to see the same problem there.  hold shock secondary for the same
// BUG TODO: Projectiles (well shock at least) do not come out of what looks like the correct point - shock balls appear higher than primaries, making flat moving combos harder.  mm not sure if i fixed that, but when scaling up, flak slugs do seem to leave the hand a bit low
// BUG TODO (fixed?): Grenades don't look like they have changed size (or do they?); grenades seem to explode not where they appear to be!
// FIXED: Shock-balls seem harder to hit (well collision radius has been reduced, along with DrawScale and speed).
// FIXED: No really, some stand-combos can fail!  try aiming down a little to reproduce this.
// BUG TODO: Shock-combo explosions look a bit funny when scaled, because the 2D sprite is not scaled.
// FIXED: Scaled-down players slide down slopes. :(
// BUG TODO: Tested Deck-16][ single-player: player was always bouncing on the floor; bio primary fine but secondary fires too far; headshots fine; both ripper shots seemed fine; pulse primary fine, secondary is long and broken; minigun bullets look too fast imo.

// TODO: Some actors don't start at their default DrawScale (e.g. the curtain meshes on Noggins2ndMap), which breaks bNew detection.
//       Ideally we would scale things only ONCE when they are first created, and not relative to their default.DrawScale, but relative to their current scale.
//       Maybe that's why bots appear a little smaller than my eye height suggests they should.
// TODO: Weapons in other player's hands look the default size; weapons in your hand are over-exaggerated!

// FIXED (I was using bNew on PlayerPawns): JumpZ always ends up at 357.5 - I can't set my own value!  But I can do it with ActorEditor.  :o
// DONE: We changed JumpZ so that the player goes higher (or lower) when scaled up (or down).
//       But now we must change gravity, so they spend the correct amount of time in the air ... and this will change everything else, e.g. flakslugs.

// FIXED: gravity and JumpZ!

// TODO: When testing MiniPureAction offline with scale=0.5, bot jumped way too high, hurt himself with his own combo, bounced off my combo too much, and the flag also bounced too far when it was dropped.

// BUG TODO: Try scaling to 0.25 - you will see that when throwing TL, dodging or jumping, they start at *normal* height, not from low where they should.
//           But at 0.5 it all seems fine, so maybe we're just getting *too* small, and the engine is refusing to spawn things so close to the ground?

// BUG TODO (probably fixed now): bots still seem to be jumping too high

// var config String AllowedSizes;
var() config float Scale;

// var EDodgeDir LastDodgeState[64];
var String LastDodgeState[64];

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
  reliable if (Role == Role_Authority || Role != Role_Authority) ShrinkAll, Shrink, ShrinkPawn, ShrinkPlayerAndInventory, DebugLog;
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
  Enable('Tick');
  Level.Game.RegisterDamageMutator(Self);
  ShrinkAll();
}

simulated function PostBeginPlay() {
  Enable('Tick');
  ShrinkAll();
}

simulated function Tick(float DeltaTime) {
  if (Role != Role_Authority) { DebugShout("[HOORAY NOT SERVER!] Tick()"); }
  ShrinkAll(); // a freshly thrown translocator or weapon, or new projectile, should be rescaled ASAP, so we do some shrinking every tick!
  Super.Tick(DeltaTime);
}

function ModifyPlayer(Pawn p) {
  if (p.PlayerReplicationInfo.Deaths == 0 && p.IsA('PlayerPawn')) {
    p.ClientMessage("Your size is "$Int(Scale*100)$"%");
    p.ClientMessage("Map size is "$Int((1.0/Scale)*100)$"%");
  }
  Super.ModifyPlayer(p);
}

simulated function ShrinkAll() {
  local Actor a;
  foreach AllActors(class'Actor',a) {
    Shrink(a);
  }
}

simulated function Shrink(Actor a) {
  local bool bNew;

  if (Role != Role_Authority) { DebugShout("[HOORAY NOT SERVER!] Shrink() START"); }

  if (a.IsA('Brush') && a.IsA('Mover')) {
    return;
  }

  if (a.DrawScale == a.default.DrawScale) {
    bNew = true;
    DebugLog("New: "$a$" [NetTag="$NetTag$"]");
  // } else {
    // return;
  }

  if (!bNew) {
    // Some things we never want to act on twice:
    if (a.IsA('PlayerPawn') || a.IsA('Bot')) {
      // TODO: add JumpZ and other essentials which reset here.
      Pawn(a).JumpZ = 325*Scale;
      // ShrinkPawn(Pawn(a));
      CheckPlayerStateForDodge(Pawn(a));
      // if (a.IsA('PlayerPawn')) {
        // CheckPlayerStateForDodge(PlayerPawn(a));
      // }
      return;
    }
    // If we don't return, the player flickers (kinda bounces up and down on the floor).
    // What causes the flickering?  We could try to track it down.  afaik it's changing a property, not calling Set___().
  }

  // TODO: We should not scale brush-style actors like BlockAll.

  // Change DrawScale:
  // if (!a.IsA('Bot')) {
    a.DrawScale = a.default.DrawScale * Scale;
  // }
  // a.SetPropertyText("DrawScale",""$(a.default.DrawScale * Scale));

  // Change CollisionRadius/Height, and move up/down correspondingly
  // Flags require some special changes
  if (a.IsA('FlagBase') || a.IsA('Flag') || a.IsA('CTFFlag') || a.IsA('CTFFlag0') || a.IsA('CTFFlag1') || a.IsA('RedFlag') || a.IsA('BlueFlag')) {
    if (bNew) { // don't do this more than once!
      DebugShout("Fixing: "$a);
      a.SetCollisionSize(a.default.CollisionRadius * Scale, a.default.CollisionHeight * Scale);
      a.SetLocation(a.Location + (-64 + 64*Scale) * Vect(0,0,1)); // flags are about 64 units above the floor - but their CollisionHeight is not 128!  I tried increasing to 72, but then the flag leaves the spot before the game starts!
    }
  } else {
    if (a.IsA('Pickup')) { a.SetLocation(a.Location - (a.CollisionHeight/2) * Vect(0,0,1)); }
    // if (bNew && a.IsA('PlayerPawn')) {
      // // Attempt to stop the flickering, failed.
    // } else {
      a.SetCollisionSize(a.default.CollisionRadius * Scale, a.default.CollisionHeight * Scale);
    // }
    if (a.IsA('Pickup')) { a.SetLocation(a.Location + (a.CollisionHeight/2) * Vect(0,0,1)); }
  }

  // Other Actor properties worth scaling:
  a.PrePivot = a.default.PrePivot * Scale;
  a.VisibilityRadius = a.default.VisibilityRadius * Scale;
  a.VisibilityHeight = a.default.VisibilityHeight * Scale;
  a.SpriteProjForward = a.default.SpriteProjForward * Scale;
  a.SoundRadius = a.default.SoundRadius * Scale;
  a.SoundVolume = a.default.SoundVolume / Scale; // don't make the sound quieter, just more localised!
  a.TransientSoundRadius = a.default.TransientSoundRadius * Scale;
  a.TransientSoundVolume = a.default.TransientSoundVolume / Scale; // don't make the sound quieter, just more localised!

  if (a.IsA('Pawn')) {
    ShrinkPawn(Pawn(a));
  }

  if (a.IsA('PlayerPawn')) {
    ShrinkPlayerAndInventory(PlayerPawn(a));
    PlayerPawn(a).JumpZ = 325*Scale;
    // PlayerPawn(a).SetPropertyText("JumpZ",""$(325*Scale));
  }

  if (a.IsA('Inventory')) {
      Inventory(a).ThirdPersonScale = Inventory(a).default.ThirdPersonScale * Scale;
      Inventory(a).PickupViewScale = Inventory(a).default.PickupViewScale * Scale;
      // Inventory(a).DrawScale = Inventory(a).default.DrawScale;
      Inventory(a).PlayerViewScale = Inventory(a).default.PlayerViewScale;
  }

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
    if (bNew) { // Don't slow down this projectile more than once!  Only when it's first created.
      DebugShout("Slowing projectile "$a$" [NetTag="$a.NetTag$"]");
      Projectile(a).Velocity = Projectile(a).Velocity * Scale;
    }
  }

  if (a.IsA('ZoneInfo') || a.IsA('LevelInfo') || a.IsA('LevelProperties') || a.IsA('BananaSoupMonkeyMonster')) {
    ZoneInfo(a).ZoneGravity = ZoneInfo(a).default.ZoneGravity * Scale;
  }

  if (Role != Role_Authority) { DebugShout("[HOORAY NOT SERVER!] Shrink() END"); }

}

simulated function ShrinkPawn(Pawn p) {
  p.BaseEyeHeight = p.default.BaseEyeHeight * Scale;
  p.EyeHeight = p.default.EyeHeight * Scale;
  p.GroundSpeed = p.default.GroundSpeed * Scale;
  // p.JumpZ = p.default.JumpZ*Scale;
  // p.JumpZ = 325*Scale;
  // p.SetPropertyText("JumpZ",""$(p.default.JumpZ*Scale));
  // p.SetPropertyText("JumpZ",""$(325*Scale));
  p.JumpZ = 325*Scale;
}

simulated function ShrinkPlayerAndInventory(PlayerPawn p) {
  local Inventory inv;
  local float DeltaTime;
  DeltaTime = 0.05;
  //// Didn't seem to have any affect, so now we slow down projectiles when they are first created.
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

// It seems all Actors go through all 3 of these when they are spawned, except for Projectiles and Effects.

function bool AlwaysKeep(Actor Other) {
  DebugLog("AlwaysKeep("$Other$") [NetTag="$NetTag$"]");
  if (Other.DrawScale != 1.0) {
    DebugShout("Interesting: "$Other$" spawned with DrawScale="$Other.DrawScale);
  }
	return Super.AlwaysKeep(Other);
}

function bool IsRelevant(Actor Other, out byte bSuperRelevant) {
  DebugLog("IsRelevant("$Other$") [NetTag="$NetTag$"]");
	return Super.IsRelevant(Other,bSuperRelevant);
}

function bool CheckReplacement(Actor Other, out byte bSuperRelevant) {
  DebugLog("CheckReplacement("$Other$") [NetTag="$NetTag$"]");
	return Super.CheckReplacement(Other,bSuperRelevant);
}

function MutatorTakeDamage( out int ActualDamage, Pawn Victim, Pawn InstigatedBy, out Vector HitLocation, out Vector Momentum, name DamageType) {
  local float HitDistance;
  HitDistance = VSize(HitLocation - Victim.Location);
  // DebugShout("Damage "$DamageType$" @ "$HitDistance);
  // BUG: both jolted and RocketDeath count for primary and secondary shots
  //      We could check the distance of the HitLocation from the Victim, to see if it is within his collision cylinder or not.
  // if (DamageType == 'jolted' || DamageType == 'RocketDeath' || DamageType=='FlakDeath' || DamageType=='GrenadeDeath' || DamageType=='RipperAltDeath') {
    // BUG TODO: still haven't detected whether damage is splash or not
    // Well HitDistance is ALWAYS (when scaled up) 56 (or 55.99) :f
    if (HitDistance > Victim.CollisionRadius /*&& HitDistance > Victim.CollisionHeight*/) {
      // Gets called sometimes even on a direct hit. :(
      // Seems to be outside the Victim, so it's splash damage, so scale it:
      DebugShout("Scaling splash damage "$DamageType$" on "$Victim.getHumanName()$" ("$HitDistance$")");
      ActualDamage = ActualDamage * Scale;
      // Check with scale=2.0 that 1 primary shock deals correct damage (and does not kill)!
    }
    Momentum = Momentum * Scale; // to compensate for scale change
    Momentum.Y = Momentum.Y * Scale; // to compensate for gravity change :: TODO: we should do this for everything which spawns, i.e. flag when dropped, weapon when thrown
  // }
  // BUG: ofc if the engine already thinks the damage was out of range, we can't scale that damage up. (When scaling up.) =/
  Super.MutatorTakeDamage( ActualDamage, Victim, InstigatedBy, HitLocation, Momentum, DamageType );
}
// NOTE TODO:
// when scaled at 0.5
// shock-primary had HitDistance ~9
// shock-secondary had HitDistance ~13
// bot's collision radius was ~8

function CheckPlayerStateForDodge(Pawn p) {
  local float DeltaTime;
  // BUG TODO: fails in offline play with a lot of AccessedNone, due to PlayerReplicationInfo == None
  // BUG TODO: now failing in online play, again due a huge number of Accessed None O_o
  DebugLog("CheckPlayerStateForDodge [START]");
  DebugLog("CheckPlayerStateForDodge p = "$p.getHumanName());
  DebugLog("CheckPlayerStateForDodge p.DodgeDir = "$p.DodgeDir);
  DebugLog("CheckPlayerStateForDodge p.PRI.PID = "$(p.PlayerReplicationInfo.PlayerID % 64));
  DebugLog("CheckPlayerStateForDodge LastDodgeState = "$LastDodgeState[p.PlayerReplicationInfo.PlayerID % 64]);
  DebugLog("CheckPlayerStateForDodge Checking...");
  if ((""$p.DodgeDir) != LastDodgeState[p.PlayerReplicationInfo.PlayerID % 64]) {
    if (p.DodgeDir == DODGE_Active) {
      DebugShout("Scaling dodge Y for "$p.getHumanName()$" ["$LastDodgeState[p.PlayerReplicationInfo.PlayerID % 64]$" -> "$p.DodgeDir$"]");
      // Try to correct the changes from last tick:
      DeltaTime = 0.05;
      p.SetLocation(p.Location + (-p.Velocity.Z*DeltaTime + p.Velocity.Z*DeltaTime*Scale) * Vect(0,0,1));
      // Set correct upward velocity:
      p.Velocity.Z = p.Velocity.Z * Scale;
    }
  }
  LastDodgeState[p.PlayerReplicationInfo.PlayerID % 64] = ""$p.DodgeDir;
  // CONSIDER: When scaling up, the dodge seems a little low, but distance reached seems ok.
  //           Maybe this is because Velocity.Z has already changed by 1 tick before we get to it.
  DebugLog("CheckPlayerStateForDodge [END]");
}

