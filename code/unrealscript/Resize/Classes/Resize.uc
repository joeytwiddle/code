class Resize extends Mutator config(Resize);

// TODO: Splash damage needs to be scaled down, but HurtRadius() is sometimes called with a FIXED value :S
// DONE: Sometimes projectiles move too slow (sometimes they look slow but are actually standing STILL!)
// TODO: Dodge is too high!
// TODO: When making player large, he bounces on the floor.  =(

// var config String AllowedSizes;
var config float NewSize;

// var int lastUpdate;
// var Actor lastProjectileFixed;
// var int lastProjectileFixedNetTag;

defaultproperties {
  // AllowedSizes="0.5,0.75,1.25,1.5,2.0"
  // NewSize=2.00
  // NewSize=2.0
  NewSize=0.5
}

replication {
  reliable if (Role == Role_Authority || Role != Role_Authority) ShrinkAll, Shrink;
}

function DebugLog(String msg) {
  Log(msg);
  BroadcastMessage(msg);
}

simulated function PreBeginPlay() {
  ShrinkAll();
  Enable('Tick');
}

simulated function PostBeginPlay() {
  ShrinkAll();
  // SetTimer(20,True);
}

simulated function Tick(float TimeDelta) {
  super.Tick(TimeDelta);
  // if (lastUpdate == 0 || Level.TimeSeconds > lastUpdate + 10) {
    ShrinkAll();
    // lastUpdate = Level.TimeSeconds + 1;
  // }
}

// event Timer() {
  // ShrinkAll();
// }

function ModifyPlayer(Pawn p) {
  if (p.PlayerReplicationInfo.Deaths == 0) {
    p.ClientMessage("Your size is "$Int(NewSize*100)$"%");
    p.ClientMessage("Map size is "$Int((1.0/NewSize)*100)$"%");
  }
  Super.ModifyPlayer(p);
  RestartPlayer(p);
}

// Not called by Engine since we aren't a GI
function bool RestartPlayer(Pawn p) {
  // if (p.IsA('PlayerPawn') || p.IsA('Bot')) {
    DebugLog("Resizing ["$Role$"]: "$p.getHumanName());
    Shrink(p);
    // ShrinkPawn(p);
    // if (p.IsA('PlayerPawn')) {
      // ShrinkPlayerAndInventory(PlayerPawn(p));
    // }
  // }
  // return Super.RestartPlayer(p);
}

function bool AlwaysKeep(Actor Other) {
  Shrink(Other);
  return super.AlwaysKeep(Other);
}

function bool CheckReplacement(Actor Other, out byte bSuperRelevant) {
  Shrink(Other);
  return super.CheckReplacement(Other,bSuperRelevant);
}

simulated function ShrinkAll() {
  local Actor a;
  foreach AllActors(class'Actor',a) {
    Shrink(a);
    // if (a.IsA('Pawn')) {
      // ShrinkPawn(Pawn(a));
    // }
    // if (a.IsA('PlayerPawn')) {
      // ShrinkPlayerAndInventory(PlayerPawn(a));
    // }
  }
}

simulated function Shrink(Actor a) {
  local bool bNew;
  // if (a.IsA('Pickup') || a.IsA('UT_Pickup') || a.IsA('Weapon') || a.IsA('Pawn') || !a.bHidden || a.IsA('FlagBase')) {
    // if (a.DrawScale != a.default.DrawScale) {
    // if (a.IsA('FlagBase') || a.IsA('Flag') || a.IsA('CTFFlag') || a.IsA('CTFFlag0') || a.IsA('CTFFlag1')) {
      // return;
    // }
  if ((a.IsA('FlagBase') || a.IsA('Flag') || a.IsA('CTFFlag') || a.IsA('CTFFlag0') || a.IsA('CTFFlag1'))) {
    // if (a.Location == a.default.Location) {
    if (a.DrawScale == a.default.DrawScale) {
      DebugLog("Fixing: "$a);
      // a.SetCollisionSize(a.default.CollisionRadius * NewSize, a.default.CollisionHeight);
      // a.SetLocation(a.default.Location - (a.default.CollisionHeight*NewSize) * Vect(0,0,1));
      // a.SetLocation(a.Location - (a.CollisionHeight*2) * Vect(0,0,1));
      a.SetCollisionSize(a.default.CollisionRadius * NewSize, a.default.CollisionHeight * NewSize);
      a.SetLocation(a.Location + (-64 + 64*NewSize) * Vect(0,0,1)); // flags are actually 64 units above the floor - but their CollisionHeight is not 128!
    }
  } else {
    a.SetLocation(a.Location - (a.CollisionHeight/2) * Vect(0,0,1));
    a.SetCollisionSize(a.default.CollisionRadius * NewSize, a.default.CollisionHeight * NewSize);
    a.SetLocation(a.Location + (a.CollisionHeight/2) * Vect(0,0,1));
  }
  if (a.DrawScale == a.default.DrawScale) {
    bNew = true;
  }
  a.DrawScale = a.default.DrawScale * NewSize;
    // a.SetPropertyText("DrawScale",""$(a.default.DrawScale * NewSize));
  // }
  if (a.IsA('Pawn')) {
    ShrinkPawn(Pawn(a));
  }
  if (a.IsA('PlayerPawn')) {
    ShrinkPlayerAndInventory(PlayerPawn(a));
  }
  if (a.IsA('Projectile')) {
    Projectile(a).Speed = Projectile(a).default.Speed * NewSize;
    Projectile(a).MaxSpeed = Projectile(a).default.MaxSpeed * NewSize;
    Projectile(a).MomentumTransfer = Projectile(a).default.MomentumTransfer * NewSize;
    if (Projectile(a).Speed > Projectile(a).MaxSpeed) {
      Projectile(a).Speed = Projectile(a).MaxSpeed;
    }
    // Projectile(a).ProjectileSpeed = Projectile(a).ProjectileSpeed * NewSize;
    // if (Normal(Projectile(a).Velocity) Dot Projectile(a).Velocity >= Projectile(a).MaxSpeed) {
    // if (a != lastProjectileFixed && Instr(""$a.Name,"Trans")<0) {
    // if (a.NetTag > lastProjectileFixedNetTag && Instr(""$a.Name,"Trans")<0) {
    // if (a.DrawScale == a.default.DrawScale && Instr(""$a.Name,"Trans")<0) {
    if (bNew && Instr(""$a.Name,"Trans")<0) {
      DebugLog("Slowing projectile "$a$" ("$a.NetTag$")");
      Projectile(a).Velocity = Projectile(a).Velocity * NewSize;
      // lastProjectileFixed = a;
      // lastProjectileFixedNetTag = a.NetTag;
    }
  }
  if (a.IsA('PlayerPawn')) {
    if (PlayerPawn(a).DodgeDir == DODGE_Active && a.Velocity.Z >= 100 && a.Velocity.Z <= 200) {
      DebugLog("Fixing dodge for "$a);
      a.Velocity.Z = a.Velocity.Z * NewSize;
    }
  }
}

simulated function ShrinkPawn(Pawn p) {
  p.DrawScale = p.default.DrawScale * NewSize;
  // TODO: eye height not working well (too low when scaling down, too low when scaling up) - actually now it seems to work without setting it!
  // p.BaseEyeHeight = p.default.CollisionHeight * NewSize * 2.00;
  // p.EyeHeight = p.default.CollisionHeight * NewSize * 2.00;
  p.BaseEyeHeight = p.default.BaseEyeHeight * NewSize;
  p.EyeHeight = p.default.EyeHeight * NewSize;
  // if (NewSize > 1.0) { p.DrawScale = p.DrawScale * 1.2; }
  p.GroundSpeed = p.default.GroundSpeed * NewSize;
  p.JumpZ = p.default.JumpZ*NewSize;
  // p.SetCollisionSize(p.default.CollisionRadius*NewSize, p.default.CollisionHeight*NewSize);
}

simulated function ShrinkPlayerAndInventory(PlayerPawn p) {
  local Actor inv;
  // Translocator(p.FindInventoryType(class'Translocator')).TossForce = Translocator(p.FindInventoryType(class'Translocator')).default.TossForce * NewSize;
  // Translocator(p.FindInventoryType(class'Translocator')).MaxTossForce = Translocator(p.FindInventoryType(class'Translocator')).default.MaxTossForce * NewSize;
  for( inv=p.Inventory; inv!=None; inv=inv.Inventory ) {
    if (inv.IsA('Weapon')) {
      // inv.DrawScale = inv.default.DrawScale * NewSize;
      // inv.DrawScale = inv.default.DrawScale; // failed attempt to make in-hand weapons look normal size to wielder (actually made it look unscaled in behindview!)
      Weapon(inv).ProjectileSpeed = Weapon(inv).default.ProjectileSpeed * NewSize;
      Weapon(inv).AltProjectileSpeed = Weapon(inv).default.AltProjectileSpeed * NewSize;
    }
    if (inv.IsA('Translocator')) {
      Translocator(inv).TossForce = Translocator(inv).default.TossForce * NewSize;
      Translocator(inv).MaxTossForce = Translocator(inv).default.MaxTossForce * NewSize;
    }
  }
}

