class kxLine extends Projectile;

var kxGrapple GrappleParent;

replication {
  unreliable if ( Role == ROLE_Authority )
    // from,to;
    GrappleParent;
}

// simulated function SetFromTo(Actor f, Actor t) {
	// from = f;
	// to = t;
// }

simulated event DoUpdate(float DeltaTime) {
	local Vector from,to;
	if (GrappleParent == None || GrappleParent.Instigator == None) {
		// if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): Lost my GrappleParent - suiciding."); }
		// Destroy();
		// if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): Lost my GrappleParent - waiting for it ..."); }
		BroadcastMessage(Level.TimeSeconds$" "$Self$".DoUpdate(): Lost my GrappleParent - waiting for it ...");
		// This was NOT cleaning up when we needed it to.
		// I've disabled it because very occasionally on firing my grapple, the line would not appear, and I think here is the problem.
		return;
	}
	if (GrappleParent.LineSprite != None && GrappleParent.LineSprite != Self) {
		// if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): My GrappleParent.LineSprite="$GrappleParent.LineSprite$" - not ME "$Self$"!"); }
		if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): My GrappleParent has a new LineSprite="$GrappleParent.LineSprite$" - suiciding."); }
		Destroy();
		return;
	}
	if (GrappleParent.bDestroyed) {
		if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): My GrappleParent "$GrappleParent$" is marked as bDestroyed - suiciding."); }
		Destroy();
		return;
	}
	if (Role == ROLE_Authority)
		return;
	// OK good this is only running on the client.
	// if (FRand()<0.01)
		// if (class'kxGrapple'.default.bDestroyed) { Log(Level.TimeSeconds$" "$Self$".DoUpdate() Called whilst GrappleParent="$GrappleParent); }
	// Update position of line:
	from = GrappleParent.Instigator.Location + 0.5*GrappleParent.Instigator.BaseEyeHeight*Vect(0,0,1);
	to = GrappleParent.pullDest;
	Velocity = GrappleParent.Instigator.Velocity * 0.5;
	SetLocation((from+to)/2);
	SetRotation(rotator(to-from));
	DrawScale = VSize(from-to) / 64.0;
}

simulated event Tick(float DeltaTime) {
	DoUpdate(DeltaTime);
}

// simulated function PostBeginPlay() {
	// Super.PostBeginPlay();
	// SetTimer(0.1,True);
// }
// simulated function Timer() {
	// Tick(0.1);
// }

simulated function Destroyed() {
	local int i;
	local kxLine L;
	foreach AllActors(class'kxLine',L) {
		i++;
	}
	if (class'kxGrapple'.default.bDestroyed) { Log(Level.TimeSeconds$" "$Self$".Destroyed() Destructing with "$i$" kxLines on the level."); }
	Super.Destroyed();
}

defaultproperties {
	LifeSpan=120
	// Texture=Texture'UMenu.Icons.Bg41'
	// Texture=Texture'BotPack.ammocount'
	DrawType=DT_Mesh
	RotationRate=(Roll=90000)
	bUnlit=False
}

