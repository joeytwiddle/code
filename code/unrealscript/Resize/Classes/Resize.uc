class Resize extends Mutator config(Resize);

// TODO: Splash damage needs to be scaled up/down, but HurtRadius() is sometimes called with a FIXED value :S
// FIXED: Sometimes projectiles move too slow (sometimes they look slow but are actually standing STILL!)
// TODO: Dodge is too high!  When I try to fix it, landing goes weird.
// TODO: When making player large, he bounces on the floor.  =(
// TODO: The first flakslug thrown looks like it's going on the default trajectory, but explodes at correct point.
// TODO: some shock balls seem to travel too fast: to reproduce, shoot secondary then primary then secondary - the latter will overtake the foremost!  or hold flak secondary fire to see the same problem there
// TODO: projectiles (well shock at least) do not come out of what looks like the correct point - shock balls appear higher than primaries, making flat moving combos harder.
// TODO: Grenades don't look like they have changed size (or do they?); grenades seem to explode not where they appear to be!
// TODO: shock-balls seem harder to hit (well collision radius has been reduced, along with DrawScale and speed)
// TODO: shock-combo explosions look a bit funny when scaled, because the 2D sprite is not scaled
// TODO: scaled-down players slide down slopes :(

// var config String AllowedSizes;
var config float NewSize;

defaultproperties {
  // AllowedSizes="0.5,0.75,1.25,1.5,2.0"
  // NewSize=2.00
  // NewSize=2.0
  NewSize=0.5
}

replication {
  reliable if (Role == Role_Authority || Role != Role_Authority) ShrinkAll, Shrink, ShrinkPawn, ShrinkPlayerAndInventory;
}

function DebugLog(String msg) {
  Log("<"$Role$"> "$msg);
  BroadcastMessage("<"$Role$"> "$msg);
}

simulated function PreBeginPlay() {
  ShrinkAll(0);
  Enable('Tick');
}

simulated function PostBeginPlay() {
  ShrinkAll(0);
  Enable('Tick');
}

simulated function Tick(float DeltaTime) {
  ShrinkAll(DeltaTime); // a freshly thrown translocator or weapon, or new projectile, should be rescaled ASAP, so we do some shrinking every tick!
  Super.Tick(DeltaTime);
}

function ModifyPlayer(Pawn p) {
  if (p.PlayerReplicationInfo.Deaths == 0) {
    p.ClientMessage("Your size is "$Int(NewSize*100)$"%");
    p.ClientMessage("Map size is "$Int((1.0/NewSize)*100)$"%");
  }
  Super.ModifyPlayer(p);
}

simulated function ShrinkAll(float DeltaTime) {
  local Actor a;
  foreach AllActors(class'Actor',a) {
    Shrink(a,DeltaTime);
  }
}

simulated function Shrink(Actor a, float DeltaTime) {
  local bool bNew;

  if (a.IsA('Brush') && a.IsA('Mover')) {
    return;
  }

  if (a.DrawScale == a.default.DrawScale) {
    bNew = true;
    DebugLog("Scaling New: "$a$" [NetTag="$NetTag$"]");
  }

  // Change DrawScale:
  a.DrawScale = a.default.DrawScale * NewSize;
  // a.SetPropertyText("DrawScale",""$(a.default.DrawScale * NewSize));

  // Change CollisionRadius/Height, and move up/down correspondingly
  // Flags require some special changes
  if (a.IsA('FlagBase') || a.IsA('Flag') || a.IsA('CTFFlag') || a.IsA('CTFFlag0') || a.IsA('CTFFlag1') || a.IsA('RedFlag') || a.IsA('BlueFlag')) {
    if (bNew) { // don't do this more than once!
      DebugLog("Fixing: "$a);
      a.SetCollisionSize(a.default.CollisionRadius * NewSize, a.default.CollisionHeight * NewSize);
      a.SetLocation(a.Location + (-64 + 64*NewSize) * Vect(0,0,1)); // flags are actually 64 units above the floor - but their CollisionHeight is not 128!
    }
  } else {
    a.SetLocation(a.Location - (a.CollisionHeight/2) * Vect(0,0,1));
    a.SetCollisionSize(a.default.CollisionRadius * NewSize, a.default.CollisionHeight * NewSize);
    a.SetLocation(a.Location + (a.CollisionHeight/2) * Vect(0,0,1));
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
    ShrinkPlayerAndInventory(PlayerPawn(a),DeltaTime);
  }

  if (a.IsA('Projectile')) {
    Projectile(a).Speed = Projectile(a).default.Speed * NewSize;
    Projectile(a).MaxSpeed = Projectile(a).default.MaxSpeed * NewSize;
    Projectile(a).MomentumTransfer = Projectile(a).default.MomentumTransfer * NewSize;
    if (Projectile(a).Speed > Projectile(a).MaxSpeed) {
      Projectile(a).Speed = Projectile(a).MaxSpeed;
    }
    if (bNew && Instr(""$a.Name,"Trans")<0) { // don't do this more than once!
      DebugLog("Slowing projectile "$a$" ("$a.NetTag$")");
      Projectile(a).Velocity = Projectile(a).Velocity * NewSize;
    }
  }

  // if (a.IsA('ZoneInfo') || a.IsA('LevelInfo') || a.IsA('LevelProperties') || a.IsA('BananaSoupMonkeyMonster')) {
    // ZoneInfo(a).ZoneGravity = ZoneInfo(a).default.ZoneGravity * NewSize;
  // }

}

simulated function ShrinkPawn(Pawn p) {
  p.BaseEyeHeight = p.default.BaseEyeHeight * NewSize;
  p.EyeHeight = p.default.EyeHeight * NewSize;
  p.GroundSpeed = p.default.GroundSpeed * NewSize;
  p.JumpZ = p.default.JumpZ*NewSize;
}

simulated function ShrinkPlayerAndInventory(PlayerPawn p, float DeltaTime) {
  local Actor inv;
  for( inv=p.Inventory; inv!=None; inv=inv.Inventory ) {
    if (inv.IsA('Weapon')) {
      Weapon(inv).ProjectileSpeed = Weapon(inv).default.ProjectileSpeed * NewSize;
      Weapon(inv).AltProjectileSpeed = Weapon(inv).default.AltProjectileSpeed * NewSize;
    }
    if (inv.IsA('Translocator')) {
      Translocator(inv).TossForce = Translocator(inv).default.TossForce * NewSize;
      Translocator(inv).MaxTossForce = Translocator(inv).default.MaxTossForce * NewSize;
    }
  }
  // Fix the player's dodge height, because it's fixed at 160 in Epic's code!!
  if (p.DodgeDir == DODGE_Active && p.Velocity.Z >= 140 && p.Velocity.Z <= 180) {
    DebugLog("Fixing dodge for "$p$" (acc="$p.Acceleration$")");
    // p.Location.Z = p.Location.Z - p.Velocity.Z * DeltaTime;
    // if (Role == Role_Authority) {
      p.SetLocation(p.Location - Vect(0,0,1) * p.Velocity.Z * DeltaTime); // undo the error caused by the tick before we could fix it
      p.Velocity.Z = p.Velocity.Z * NewSize * NewSize;
    // }
    // p.Velocity = p.Velocity * NewSize;
  }
}

