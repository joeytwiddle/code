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

// var config String AllowedSizes;
var() config float NewSize;

// var EDodgeDir LastDodgeState[64];
var String LastDodgeState[64];

defaultproperties {
  // AllowedSizes="0.5,0.75,1.25,1.5,2.0"
  // NewSize=2.00
  NewSize=2.0
  // NewSize=0.5
    // Attempt to get simulation working:
    bAlwaysRelevant=true
    // bNoDelete=True // Stopped mutator from spawning
    // bStasis=True // Stopped mutator from spawning
    bStatic=False
    NetPriority=3.0
    NetUpdateFrequency=10
    RemoteRole=ROLE_SimulatedProxy
}

replication {
  reliable if (Role == Role_Authority || Role != Role_Authority) ShrinkAll, Shrink, ShrinkPawn, ShrinkPlayerAndInventory, DebugLog;
}

simulated function DebugLog(String msg) {
  Log("<"$Role$"> "$msg);
  // BroadcastMessage("<"$Role$"> "$msg);
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
  if (Role != Role_Authority) { DebugLog("[HOORAY NOT SERVER!] Tick()"); }
  ShrinkAll(); // a freshly thrown translocator or weapon, or new projectile, should be rescaled ASAP, so we do some shrinking every tick!
  Super.Tick(DeltaTime);
}

function ModifyPlayer(Pawn p) {
  if (p.PlayerReplicationInfo.Deaths == 0 && p.IsA('PlayerPawn')) {
    p.ClientMessage("Your size is "$Int(NewSize*100)$"%");
    p.ClientMessage("Map size is "$Int((1.0/NewSize)*100)$"%");
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

  if (Role != Role_Authority) { DebugLog("[HOORAY NOT SERVER!] Shrink() START"); }

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
      PlayerPawn(a).JumpZ = 325*NewSize;
      CheckPlayerStateForDodge(PlayerPawn(a));
      return;
    }
    // If we don't return, the player flickers (kinda bounces up and down on the floor).
    // What causes the flickering?  We could try to track it down.  afaik it's changing a property, not calling Set___().
  }

  // TODO: We should not scale brush-style actors like BlockAll.

  // Change DrawScale:
  // if (!a.IsA('Bot')) {
    a.DrawScale = a.default.DrawScale * NewSize;
  // }
  // a.SetPropertyText("DrawScale",""$(a.default.DrawScale * NewSize));

  // Change CollisionRadius/Height, and move up/down correspondingly
  // Flags require some special changes
  if (a.IsA('FlagBase') || a.IsA('Flag') || a.IsA('CTFFlag') || a.IsA('CTFFlag0') || a.IsA('CTFFlag1') || a.IsA('RedFlag') || a.IsA('BlueFlag')) {
    if (bNew) { // don't do this more than once!
      DebugLog("Fixing: "$a);
      a.SetCollisionSize(a.default.CollisionRadius * NewSize, a.default.CollisionHeight * NewSize);
      a.SetLocation(a.Location + (-64 + 64*NewSize) * Vect(0,0,1)); // flags are about 64 units above the floor - but their CollisionHeight is not 128!  I tried increasing to 72, but then the flag leaves the spot before the game starts!
    }
  } else {
    if (a.IsA('Pickup')) { a.SetLocation(a.Location - (a.CollisionHeight/2) * Vect(0,0,1)); }
    // if (bNew && a.IsA('PlayerPawn')) {
      // // Attempt to stop the flickering, failed.
    // } else {
      a.SetCollisionSize(a.default.CollisionRadius * NewSize, a.default.CollisionHeight * NewSize);
    // }
    if (a.IsA('Pickup')) { a.SetLocation(a.Location + (a.CollisionHeight/2) * Vect(0,0,1)); }
  }

  // Other Actor properties worth scaling:
  a.PrePivot = a.default.PrePivot * NewSize;
  a.VisibilityRadius = a.default.VisibilityRadius * NewSize;
  a.VisibilityHeight = a.default.VisibilityHeight * NewSize;
  a.SpriteProjForward = a.default.SpriteProjForward * NewSize;
  a.SoundRadius = a.default.SoundRadius * NewSize;
  a.SoundVolume = a.default.SoundVolume / NewSize; // don't make the sound quieter, just more localised!
  a.TransientSoundRadius = a.default.TransientSoundRadius * NewSize;
  a.TransientSoundVolume = a.default.TransientSoundVolume / NewSize; // don't make the sound quieter, just more localised!

  if (a.IsA('Pawn')) {
    ShrinkPawn(Pawn(a));
  }

  if (a.IsA('PlayerPawn')) {
    ShrinkPlayerAndInventory(PlayerPawn(a));
    PlayerPawn(a).JumpZ = 325*NewSize;
    // PlayerPawn(a).SetPropertyText("JumpZ",""$(325*NewSize));
  }

  if (a.IsA('Projectile')) {
    //// It's too late now to change the initialisation settings.
    // Projectile(a).Speed = Projectile(a).default.Speed * NewSize;
    // Projectile(a).MaxSpeed = Projectile(a).default.MaxSpeed * NewSize;
    // Projectile(a).MomentumTransfer = Projectile(a).default.MomentumTransfer * NewSize;
    // if (Projectile(a).Speed > Projectile(a).MaxSpeed) {
      // Projectile(a).Speed = Projectile(a).MaxSpeed;
    // }
    // BUG TODO: somehow this fails to detect shock balls when secondary fire is held down (in online play at least)
    if (bNew) { // Don't slow down this projectile more than once!  Only when it's first created.
      DebugLog("Slowing projectile "$a$" [NetTag="$a.NetTag$"]");
      Projectile(a).Velocity = Projectile(a).Velocity * NewSize;
    }
  }

  if (a.IsA('ZoneInfo') || a.IsA('LevelInfo') || a.IsA('LevelProperties') || a.IsA('BananaSoupMonkeyMonster')) {
    ZoneInfo(a).ZoneGravity = ZoneInfo(a).default.ZoneGravity * NewSize;
  }

  if (Role != Role_Authority) { DebugLog("[HOORAY NOT SERVER!] Shrink() END"); }

}

simulated function ShrinkPawn(Pawn p) {
  p.BaseEyeHeight = p.default.BaseEyeHeight * NewSize;
  p.EyeHeight = p.default.EyeHeight * NewSize;
  p.GroundSpeed = p.default.GroundSpeed * NewSize;
  // p.JumpZ = p.default.JumpZ*NewSize;
  // p.JumpZ = 325*NewSize;
  // p.SetPropertyText("JumpZ",""$(p.default.JumpZ*NewSize));
  // p.SetPropertyText("JumpZ",""$(325*NewSize));
  p.JumpZ = 325*NewSize;
}

simulated function ShrinkPlayerAndInventory(PlayerPawn p) {
  local Actor inv;
  local float DeltaTime;
  DeltaTime = 0.05;
  //// Didn't seem to have any affect, so now we slow down projectiles when they are first created.
  for( inv=p.Inventory; inv!=None; inv=inv.Inventory ) {
    if (p.IsA('Bot')) {
      inv.DrawScale = inv.default.DrawScale * NewSize;
    }
    // if (inv.IsA('Weapon')) {
      // Weapon(inv).ProjectileSpeed = Weapon(inv).default.ProjectileSpeed * NewSize;
      // Weapon(inv).AltProjectileSpeed = Weapon(inv).default.AltProjectileSpeed * NewSize;
    // }
    // if (inv.IsA('Translocator')) {
      // Translocator(inv).TossForce = Translocator(inv).default.TossForce * NewSize;
      // Translocator(inv).MaxTossForce = Translocator(inv).default.MaxTossForce * NewSize;
    // }
  // }
  // Fix the player's dodge height, because it's fixed at 160 in Epic's code!!
  // if (p.DodgeDir == DODGE_Active && p.Velocity.Z >= 140 && p.Velocity.Z <= 180) {
    // DebugLog("Fixing dodge for "$p$" (acc="$p.Acceleration$")");
    // // p.Location.Z = p.Location.Z - p.Velocity.Z * DeltaTime;
    // // if (Role == Role_Authority) {
      // p.SetLocation(p.Location - Vect(0,0,1) * p.Velocity.Z * DeltaTime); // undo the error caused by the tick before we could fix it
      // p.Velocity.Z = p.Velocity.Z * NewSize * NewSize;
    // // }
    // // p.Velocity = p.Velocity * NewSize;
  }
}

// It seems all Actors go through all 3 of these when they are spawned, except for Projectiles and Effects.

function bool AlwaysKeep(Actor Other) {
  DebugLog("AlwaysKeep("$Other$") [NetTag="$NetTag$"]");
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
  // BroadcastMessage("Damage "$DamageType$" @ "$HitDistance);
  // BUG: both jolted and RocketDeath count for primary and secondary shots
  //      We could check the distance of the HitLocation from the Victim, to see if it is within his collision cylinder or not.
  // if (DamageType == 'jolted' || DamageType == 'RocketDeath' || DamageType=='FlakDeath' || DamageType=='GrenadeDeath' || DamageType=='RipperAltDeath') {
    // BUG TODO: still haven't detected whether damage is splash or not
    // Well HitDistance is ALWAYS (when scaled up) 56 (or 55.99) :f
    if (HitDistance > Victim.CollisionRadius /*&& HitDistance > Victim.CollisionHeight*/) {
      // Gets called sometimes even on a direct hit. :(
      // Seems to be outside the Victim, so it's splash damage, so scale it:
      BroadcastMessage("Scaling splash damage "$DamageType$" on "$Victim.getHumanName()$" ("$HitDistance$")");
      ActualDamage = ActualDamage / NewSize;
      Momentum = Momentum / NewSize;
      // Check with scale=2.0 that 1 primary shock deals correct damage (and does not kill)!
    }
  // }
  // BUG: ofc if the engine already thinks the damage was out of range, we can't scale that damage up. (When scaling up.) =/
  Super.MutatorTakeDamage( ActualDamage, Victim, InstigatedBy, HitLocation, Momentum, DamageType );
}

function CheckPlayerStateForDodge(PlayerPawn p) {
  // BUG TODO: fails in offline play with a lot of AccessedNone, due to PlayerReplicationInfo == None
  if ((""$p.DodgeDir) != LastDodgeState[p.PlayerReplicationInfo.PlayerID % 64]) {
    if (p.DodgeDir == DODGE_Active) {
      BroadcastMessage("Scaling dodge Y for "$p.getHumanName()$" ["$LastDodgeState[p.PlayerReplicationInfo.PlayerID % 64]$" -> "$p.DodgeDir$"]");
      p.Velocity.Z = p.Velocity.Z * NewSize;
    }
  }
  LastDodgeState[p.PlayerReplicationInfo.PlayerID % 64] = ""$p.DodgeDir;
  // CONSIDER: When scaling up, the dodge seems a little low, but distance reached seems ok.
  //           Maybe this is because Velocity.Z has already changed by 1 tick before we get to it.
}

