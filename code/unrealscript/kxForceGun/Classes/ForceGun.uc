//================================================================================
// ForceGun.
//================================================================================

class ForceGun expands TournamentWeapon Config(kxForceGun);

var config bool bLogging;

var PlayerPawn POwner;
// var Actor Weilder;
// var Pawn Weilder;
var float Grip;
var Actor Target;

replication {
  reliable if (Role==ROLE_Authority)
    bLogging;
}

// TODO: At release time we should comment out all DebugLog() and ClientLog() calls, because we don't want them building those message strings!
simulated function DebugLog(String message) {
  if (bLogging)
    Super.Log(message);
}
simulated function ClientLog(String message) {
  if (bLogging)
    Instigator.ClientMessage(message);
}

/*simulated*/ function PreBeginPlay() {
  Super.PreBeginPlay();

  if ( Level.NetMode != 1 ) {
    foreach AllActors(Class'PlayerPawn',POwner) {
      if ( Viewport(POwner.Player) != None ) {
        break;
      } else {
        POwner = None;
      }
    }
    if ( POwner == None ) {
      Log(Self$".PreBeginPlay() NO LOCAL PLAYERPAWN!");
      // But we actually take no action :P
      return;
    }
  }

  // DebugLog(Self$".PreBeginPlay() Owner = "$Owner$" Instigator="$Instigator$" POwner="$POwner);
  // All=None on server at spawn-time

}

// This doesn't appear to get used, but Instigator is set anyway.
/*
function GiveTo(Pawn o) {
  Instigator = o;
  DebugLog(Self$".GiveTo() Set Instigator="$Instigator$" while Owner="$Owner);
  Super.GiveTo(o);

  // if (Instigator.PlayerReplicationInfo.Deaths == 0 && POwner.Weapon == Self) {
  if (Instigator.PlayerReplicationInfo.Deaths == 0) {
    Instigator.ClientMessage("Welcome to the kx Force Gun "$Self$".  Use primary fire to grab things, secondary fire to throw them!");
  }
}
*/

function AquireTarget() {
  local Vector HitLoction,HitNormal;
  DebugLog(Self$".AquireTarget() Owner = "$Owner$" Instigator="$Instigator$" POwner="$POwner);
  Target = Trace(HitLoction,HitNormal,Instigator.Location+10000*Vector(Instigator.Rotation),Instigator.Location,true);
  if (Target == Instigator || Target == Self || Target==Level) {
    DebugLog(Self$".BeginState() Bad Target = "$Target);
    ClientLog("Aquired bad target: "$Target);
    Target = None;
  } else {
    ClientLog("Aquired target "$Target$" mass "$Target.Mass);
    // InitTarget();
    Target.SetPhysics(PHYS_Flying);
    Target.Velocity = vect(0,0,0);
    if (Target.bStatic) {
      ClientLog("Target is bStatic - may not move.");
    }
    if (Pawn(Target)!=None)
      Pawn(Target).ClientMessage("You have been picked up by "$Instigator.getHumanName());
  }
}

function ReleaseTarget() {
  if (Target != None) {
    Target.SetPhysics(PHYS_Falling);
    Target = None;
  }
}

simulated function ThrowTarget() {
  if (Target != None) {
    Target.Velocity = Vector(Instigator.Rotation) * 1200.0 + Vect(0,0,300);
    Target.Velocity *= 1.0/Difficulty();
    ClientLog("Throwing "$Target$" with velocity "$VSize(Target.Velocity));
    ReleaseTarget();
  }
}

simulated function Destroyed () {
  ReleaseTarget();
  Super.Destroyed();
}

function float Difficulty() { // num from 1.0 to 100.0
  if (Target==None)
    return 1.0;
  if (Target.Mass<1.0)
    return 1.0;
  if (Target.Mass>5000)
    return 100.0;
  return 1 + Mass*100.0/5000.0;
}

function DropFrom (Vector StartLocation) {
  Super.DropFrom(StartLocation);
}

function BringUp () {
  Super.BringUp();
}

function RaiseUp (Weapon OldWeapon) {
  Super.BringUp();
}

function Fire (optional float Value) {
  GotoState('NormalFire');
  if ( Owner.bHidden ) {
    CheckVisibility();
  }
}

function AltFire (float Value) {
  GotoState('AltFiring');
}

state NormalFire {

  simulated function Tick(float DeltaTime) {
    local Vector holdPos;
    if (Target != None) {
      holdPos = Instigator.Location + 120.0 * Vector(Instigator.Rotation) + Vect(0,0,50);
      // Target.SetLocation( Target.Location*0.99 + holdPos*0.01 );
      Target.Velocity = Normal(holdPos - Target.Location) * 480.0 / Difficulty();
    }
  }

  function Fire (float F) {
  }

  function AltFire (float F) {
  }

  function EndState () {
  }

  Begin:
    FinishAnim();
    if (Target == None)
      AquireTarget();
    Sleep(0.1);
    Finish();

}

state AltFiring {

  function Fire (float F) {
  }

  function AltFire (float F) {
  }

  function EndState () {
  }

  Begin:
    FinishAnim();
    ThrowTarget();
    // Sleep(0.1);
    Sleep(4.0); // wondering if we can delay primary from acting for a while
    Finish();

}

state Idle {

  function BeginState() {
    bPointing = False;
    ReleaseTarget();
    Disable('AnimEnd');
    PlayIdleAnim();
  }

  function AnimEnd () {
    PlayIdleAnim();
  }

  function bool PutDown () {
    GotoState('DownWeapon');
    return True;
  }

}

simulated function Finish () {
  local Pawn PawnOwner;

  if ( bChangeWeapon ) {
    GotoState('DownWeapon');
    return;
  }
  PawnOwner = Pawn(Owner);
  if ( PlayerPawn(Owner) == None ) {
    PawnOwner.StopFiring();
    GotoState('Idle');
    return;
  } else {
    if ( PlayerPawn(Owner).bExtra1 != 0 ) {
      Global.Fire(0.0);
    } else if ( PawnOwner.bFire != 0 ) {
      Global.Fire(0.0);
    } else if ( PawnOwner.bAltFire != 0 ) {
      Global.AltFire(0.0);
    } else {
      GotoState('Idle');
      // We get here when fire button has been released.
    }
  }
}

/*
function SetHand (float hand) {
  Mesh = LodMesh'Transloc'; // Upper hand
  // Mesh = LodMesh'TranslocR'; // Upper hand
  Mesh = Mesh(DynamicLoadObject("Botpack.TranslocR",Class'Mesh'));
  // // Mesh = Mesh(DynamicLoadObject("Botpack.Transloc",Class'Mesh'));
}
*/

function SetHand (float hand) {
  if ( hand != 2 ) {
    if ( hand == 0 ) {
      hand = 1.0;
    } else {
      hand *= -1;
    }
    if ( hand == -1 ) {
      Mesh = Mesh(DynamicLoadObject("Botpack.TranslocR",Class'Mesh'));
      // Mesh = Mesh(DynamicLoadObject("Botpack.Transloc",Class'Mesh'));
    } else {
      Mesh = PlayerViewMesh;
    }
  }
  Super.SetHand(hand);
}

simulated function PlaySelect() {
  // OnSelect();
  Super.PlaySelect(); // Avoids errors thrown by missing meshes.
}

state DownWeapon {
  function BeginState() {
    // OnDeselect(); // Was not being called simulated, so moved it to the GotoState() calls, but still not.  So made it work non-replicated.
    Super.BeginState();
  }
}



defaultproperties {
    bCanThrow=False
    FireOffset=(X=115.00,Y=15.00,Z=2.00),
    // ProjectileClass=class'kxGrapple.GrapplingHook'
    // AltProjectileClass=class'kxGrapple.GrapplingHook'
    DeathMessage="%k hurled %o across the room!"
    // bRotatingPickup=True // Causes it to appear at spawnpoints when given to players
    ItemName="kx Force Gun"
    PlayerViewOffset=(X=5.00,Y=-4.00,Z=-7.00),
    StatusIcon=Texture'Botpack.Icons.UseTrans'
    Mass=25.00
    SelectSound=Sound'UnrealI.flak.load1'
    // NetPriority=2.95 // I was hoping here to make replication of NextCommand as fast as possible.

    AutoSwitchPriority=1
    InventoryGroup=1
    FiringSpeed=1.0
    PickupAmmoCount=1
    PlayerViewMesh=Mesh'Botpack.TranslocR'
    PickupViewMesh=Mesh'Botpack.Trans3loc'
    ThirdPersonMesh=Mesh'Botpack.Trans3loc'
    StatusIcon=Texture'Botpack.Icons.UseTrans'
    Icon=Texture'Botpack.Icons.UseTrans'
    // Mesh=Mesh'Botpack.Trans3loc'

    bLogging=True
}

