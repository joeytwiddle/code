//================================================================================
// ForceGun.
//================================================================================

class ForceGun expands TournamentWeapon Config(kxForceGun);

var config bool bLogging;
var config bool bTesting;
var config bool bPickupPlayers;
var config bool bSameTeamOnly;

var PlayerPawn POwner;
// var Actor Weilder;
// var Pawn Weilder;
var float Grip;
var Actor Target;
var Actor LastTarget;

var int UpdateCount;

replication {
	reliable if (Role==ROLE_Authority)
		bLogging;
	reliable if (Role==ROLE_Authority)
		bTesting;
	reliable if (Role==ROLE_Authority)
		Target;
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

simulated function PreBeginPlay() {
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

	Enable('Tick');
	DebugLog(Self$".PreBeginPlay() Owner = "$Owner$" Instigator="$Instigator$" POwner="$POwner);
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

function int GetTeam(Actor a) {
	local String s;
	if (Pawn(a)!=None && Pawn(a).PlayerReplicationInfo!=None)
		return Pawn(a).PlayerReplicationInfo.Team;
	s = a.GetPropertyText("Team");
	if (s != "")
		return Int(s);
	return -1;
}

simulated function AcquireTarget() {
	local Vector HitLoction,HitNormal;
	DebugLog(Self$".AcquireTarget() Owner = "$Owner$" Instigator="$Instigator$" POwner="$POwner);
	// Target = Trace(HitLoction,HitNormal,Instigator.Location+10000*Vector(Instigator.Rotation),Instigator.Location,true);
	Target = FindClosestActor(Instigator);
	if (Target==None) return; // No target acquired
	if (!bPickupPlayers && Target.IsA('Pawn') && Pawn(Target).bIsPlayer) {
		ClientLog("You may not pick up "$Target.getHumanName()$"!");
		Target=None;
		return;
	}
	if (bSameTeamOnly && Level.Game.IsA('TeamGamePlus')
			&& GetTeam(Target) != -1
			&& GetTeam(Target) != GetTeam(Instigator)
	) {
		// ClientLog("You cannot move "$Target$", it belongs to the enemy.");
		ClientLog("You cannot move "$Target$", it belongs to the enemy.  ("$GetTeam(Target)$"!="$GetTeam(Instigator)$")");
	}
	if (Target == Instigator || Target == Self || Target==Level) {
		DebugLog(Self$".BeginState() Bad Target = "$Target);
		ClientLog("You can not pick up "$Target);
		Target = None;
	} else {
		ClientLog("Acquired target "$Target$" mass "$Target.Mass$" distance "$Int(VSize(Target.Location-Instigator.Location)));
		InitTarget();
		if (Pawn(Target)!=None)
			Pawn(Target).ClientMessage("You have been picked up by "$Instigator.getHumanName());
		UpdateCount++;
	}
}

simulated function InitTarget() {

	DebugLog(Self$".InitTarget() called while UpdateCount="$UpdateCount$" Target="$Target);
	// Target.SetLocation( Target.Location + vect(0,0,8.0) ); // get things underway?
	// Target.SetPhysics(PHYS_Flying);
	// Target.SetPhysics(PHYS_Swimming);
	// Target.SetPhysics(PHYS_Walking);
	// Target.bSimFall = False;

	ClientLog("Target's Physics was "$Target.Physics);

	Target.SetPhysics(PHYS_Falling);
	// Target.Velocity = vect(0,0,0);
	SetTargetMovement(0.05);

	if (Target.bStatic) {
		ClientLog("Target is bStatic - may not move!");
	}
	if (Projectile(Target)!=None)
		Target.Disable('Tick');
	// Target.default.bStatic = False;
	if (Target.IsA('Inventory'))
		Inventory(Target).bRotatingPickup = False;
	Target.LifeSpan = Target.LifeSpan*2;
	if (Target.LifeSpan<20)
		Target.LifeSpan = 20;
	// DONE: Set some net-relevant properties
	// With ROLE_SimulatedProxy, InventorySpots/Pickups *are* getting moved, but the client does not see it.
	if (bTesting) {
		Target.RemoteRole = ROLE_SimulatedProxy;
		Self.RemoteRole = ROLE_SimulatedProxy;
	} else {
		Target.RemoteRole = ROLE_DumbProxy; // works better when client-server
		Self.RemoteRole = ROLE_DumbProxy;
	}
	// Target.NetPriority = 2.8;
	// Target.bNetTemporary = False;

}

simulated function SetTargetMovement(float DeltaTime) {
	local Vector holdPos;

	// DebugLog("Doing SetTargetMovement("$DeltaTime$" NetMode="$Level.NetMode$" Role="$Role$" with Target="$Target$" and Instigator="$Instigator$" distance="$VSize(holdPos-Target.Location)$" velocity="$Target.Velocity);
	// DebugLog("Doing SetTargetMovement("$DeltaTime$" distance="$VSize(holdPos-Target.Location)$" Target="$Target$" Location="$Target.Location$" Velocity="$Target.Velocity$" Instigator="$Instigator$" Location="$Instigator.Location$" Velocity="$Instigator.Velocity);
	// DebugLog("Doing SetTargetMovement("$DeltaTime$" distance="$VSize(holdPos-Target.Location)$" Target="$Target$" Location="$Target.Location$" Velocity="$Target.Velocity$" Instigator="$Instigator);
	DebugLog("Doing SetTargetMovement("$DeltaTime$" distance="$ Int(VSize(holdPos-Target.Location)) $" tgt.P="$Target.Physics$" ins.P="$Instigator.Physics$" tgt.loc="$Target.Location$" ins.loc="$Instigator.Location);
	holdPos = Instigator.Location + 120.0 * Vector(Instigator.Rotation) + Vect(0,0,50);
	// DebugLog("130 = "$VSize(holdPos-Instigator.Location));

	if (Role==ROLE_Authority || Target.RemoteRole==ROLE_SimulatedProxy) {
		// Should end at distance 130, since 50*50+120*120=130*130.
		// Target.SetLocation( Target.Location*0.99 + holdPos*0.01 );
		// if (VSize(holdPos-Target.Location)<5.0) {
			// Target.Velocity = VRand()*0.1;
		// } else {
			// If I do this on the client, distance and velocity go to inf!
			Target.Velocity = Normal(holdPos - Target.Location) * 480.0 / Difficulty();
		// }
	}
	if (bTesting && Target.RemoteRole==ROLE_SimulatedProxy) {
		Target.SetLocation(Target.Location + DeltaTime*Velocity);
	}
}

/*simulated*/ function ReleaseTarget() {
	if (Target != None) {
		Target.SetPhysics(PHYS_Falling);
		// Target.bSimFall = True;
		Target = None;
		if (Projectile(Target)!=None)
			Target.Enable('Tick');
	}
	DebugLog("ReleaseTarget() running NetMode="$Level.NetMode$" Role="$Role);
}

simulated function ThrowTarget() {
	if (Target != None) {
		Target.Velocity = Vector(Instigator.Rotation) * 1200.0 + Vect(0,0,300);
		Target.Velocity *= 1.0/Difficulty();
		ClientLog("Throwing "$Target$" with velocity "$VSize(Target.Velocity)$" difficulty="$Int(Difficulty()));
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
	else
		return FClamp(1+Mass/50,1,100);
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

simulated function Fire (optional float Value) {
	GotoState('NormalFire');
	if ( Owner.bHidden ) {
		CheckVisibility();
	}
}

function AltFire (float Value) {
	GotoState('AltFiring');
}

simulated function Tick(float DeltaTime) {
	local Vector holdPos;
	local rotator changeRot;

	if (Level.NetMode!=1 && Instigator==None) {
		DebugLog(Self$".Tick() had to set Instigator from POwner.");
		Instigator = POwner;
	}
	if (Target != None) {

		if (Level.NetMode!=1 && Target!=LastTarget) {
			DebugLog(Self$".Tick() - Calling InitTarget()");
			InitTarget();
			LastTarget = Target;
		}

		SetTargetMovement(DeltaTime);

		changeRot = Target.Rotation;
		changeRot.Pitch += 0.05*(FRand()-0.5);
		changeRot.Roll += 0.1*(FRand()-0.5);
		changeRot.Yaw += 0.1*(FRand()-0.5);
		Target.SetRotation(changeRot);
		Target.DesiredRotation = changeRot;
		// Try harder!
		if (Target.IsA('Inventory')) {
			Inventory(Target).bRotatingPickup = False;
			// ...
			// XX Inventory(Target).bRotating = False;
		}

	}
}

state NormalFire {

	simulated function BeginState() {
		if (Target == None)
			AcquireTarget();
	}

	function Fire (float F) {
	}

	function AltFire (float F) {
	}

	function EndState () {
	}

	Begin:
		FinishAnim();
		// AcquireTarget() was here but moved it to BeginState() to ensure it runs simulated.
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

	simulated function BeginState() {
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

function Actor FindClosestActor(Actor from) {
	local Actor A;
	local int distance;
	local int deltaRotation;
	local Actor bestActor;
	local int bestDistance;
	bestActor = None;
	foreach VisibleActors(class'Actor', A, 1024, from.Location) {
	// foreach AllActors(class'Actor', A) { // not using VisibleActors gets us more invisible actors like InventorySpot/Light/...

		// DONE: I want to be able to pick up anything, but there should be a priority!
		if (
			bestActor!=None
			&&
			(
				A.IsA('UTChunk') || A.IsA('Effects') || A.IsA('Light') || A.IsA('Decal') || A.IsA('Carcass')
				||
				A.IsA('UT_ShellCase')
			)
		)
			continue;

		if (A == from) { // don't find self!
			continue;
		}

		distance = VSize(A.Location - from.Location);
		deltaRotation = Abs( Rotator(A.Location - from.Location).Yaw - from.Rotation.Yaw ) % 65536;
		// if (deltaRotation < 8192 || deltaRotation > 8192*7) {
		if (deltaRotation > 8192*4) {
			deltaRotation = 8192*8 - deltaRotation;
		}
		if (deltaRotation < 8192 && deltaRotation > -8192) {
			if (bestActor == None || distance < bestDistance) {
				bestActor = A;
				bestDistance = distance;
				// PlayerPawn(from).ClientMessage("  " $ A $" (" $ deltaRotation $ ") -> " $ distance $ "");
			}
		}

	}
	return bestActor;
}



defaultproperties {
		bCanThrow=False
		FireOffset=(X=115.00,Y=15.00,Z=2.00),
		// ProjectileClass=class'kxGrapple.GrapplingHook'
		// AltProjectileClass=class'kxGrapple.GrapplingHook'
		DeathMessage="%k hurled %o across the room!"
		// bRotatingPickup=True
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

		// TODO: These should be False!
		bLogging=True
		bPickupPlayers=True
		// bTesting=True
}

