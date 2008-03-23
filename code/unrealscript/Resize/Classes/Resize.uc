class Resize extends Mutator config(Resize);

// TODO: Splash damage needs to be scaled up/down, but HurtRadius() is sometimes called with a FIXED value :S
// FIXED: Sometimes projectiles move too slow (sometimes they look slow but are actually standing STILL!)
// TODO: Dodge is too high!
// TODO: When making player large, he bounces on the floor.  =(

// var config String AllowedSizes;
var config float NewSize;

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
}

simulated function Tick(float TimeDelta) {
  super.Tick(TimeDelta);
  ShrinkAll(); // a freshly thrown translocator or weapon, or new projectile, should be rescaled ASAP, so we do some shrinking every tick!
}

function ModifyPlayer(Pawn p) {
  if (p.PlayerReplicationInfo.Deaths == 0) {
    p.ClientMessage("Your size is "$Int(NewSize*100)$"%");
    p.ClientMessage("Map size is "$Int((1.0/NewSize)*100)$"%");
  }
  DebugLog("Resizing ["$Role$"]: "$p.getHumanName());
  Shrink(p);
  Super.ModifyPlayer(p);
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
  }
}

simulated function Shrink(Actor a) {
  local bool bNew;

  // Change CollisionRadius/Height, and move up/down correspondingly
  // Flags require some special changes
  if ((a.IsA('FlagBase') || a.IsA('Flag') || a.IsA('CTFFlag') || a.IsA('CTFFlag0') || a.IsA('CTFFlag1'))) {
    if (a.DrawScale == a.default.DrawScale) { // don't do this more than once!
      DebugLog("Fixing: "$a);
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
    if (bNew && Instr(""$a.Name,"Trans")<0) { // don't do this more than once!
      DebugLog("Slowing projectile "$a$" ("$a.NetTag$")");
      Projectile(a).Velocity = Projectile(a).Velocity * NewSize;
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
  p.BaseEyeHeight = p.default.BaseEyeHeight * NewSize;
  p.EyeHeight = p.default.EyeHeight * NewSize;
  p.GroundSpeed = p.default.GroundSpeed * NewSize;
  p.JumpZ = p.default.JumpZ*NewSize;
}

simulated function ShrinkPlayerAndInventory(PlayerPawn p) {
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
}

