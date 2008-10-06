class kxLine extends Projectile;

// TODO: try rewriting this class with just from,to (modified externally) and bSuicide.

var kxGrapple GrappleParent;
var kxLine ParentLine;
var bool bStopped;
var Vector Pivot;
var Vector Reached; // BUG: Never gets updated!

replication {
  reliable if (Role == ROLE_Authority)
    GrappleParent,ParentLine,bStopped,Pivot,Reached;
  unreliable if (Role == ROLE_Authority)
    DoUpdate;
}

// simulated function SetFromTo(Actor f, Actor t) {
	// from = f;
	// to = t;
// }

simulated event DoUpdate(float DeltaTime) {
	local Vector from,to;
	/*
	// Never worked when needed, sometimes worked when unwanted!
	if (GrappleParent == None || GrappleParent.Instigator == None) {
		// if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): Lost my GrappleParent - suiciding."); }
		// Destroy();
		// if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): Lost my GrappleParent - waiting for it ..."); }
		BroadcastMessage(Level.TimeSeconds$" "$Self$".DoUpdate(): Lost my GrappleParent - waiting for it ...");
		// This was NOT cleaning up when we needed it to.
		// I've disabled it because very occasionally on firing my grapple, the line would not appear, and I think here is the problem.
		return;
	}
	// No longer needed since bDestroyed works better, and with folding, we need multiple lines per grapple.
	if (GrappleParent.LineSprite != None && GrappleParent.LineSprite != Self) {
		// if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): My GrappleParent.LineSprite="$GrappleParent.LineSprite$" - not ME "$Self$"!"); }
		if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): My GrappleParent has a new LineSprite="$GrappleParent.LineSprite$" - suiciding."); }
		Destroy();
		return;
	}
	*/
	if (GrappleParent == None) {
		if (class'kxGrapple'.default.bDebugLogging && FRand()<0.01) { Log(Level.TimeSeconds$" "$Self$".DoUpdate() Warning! My GrappleParent == None! I may be a memory leak!"); }
		return;
	}
	// I was unable to destroy this actor by calling its .Destroy() but this mechanism for it to destroy itself works ok:
	if (GrappleParent.bDestroyed) {
		if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): "$GrappleParent$".bDestroyed set (LineSprite="$GrappleParent.LineSprite$") - suiciding."); }
		Destroy();
		return;
	}
	// OK good this is only running on the client.
	// if (FRand()<0.01)
		// if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate() Called whilst GrappleParent="$GrappleParent); }
	if (GrappleParent.LineSprite != None && GrappleParent.LineSprite != Self && !bStopped) {
		if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate() GrappleParent has a new LineSprite "$GrappleParent.LineSprite$" - setting Self.bStopped."); }
		bStopped = True;
	}
	// if (class'kxGrapple'.default.bDebugLogging && FRand()<0.01) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): Render="$(Role!=ROLE_Authority)$" bStopped="$bStopped$" LineSprite="$GrappleParent.LineSprite$" Pivot="$Pivot$" Reached="$Reached); }
	// OK good we are now running on the client with variables replicated.
	if (Role == ROLE_Authority)
		return;
	// Update position of line:
	// if (GrappleParent.LineSprite != Self) {
	if (bStopped) {
		from = Reached;
		to = Pivot;
		// from = GrappleParent.Location;
		// to = GrappleParent.pullDest;
		Velocity = vect(0,0,0);
	} else {
		from = GrappleParent.Instigator.Location + 0.5*GrappleParent.Instigator.BaseEyeHeight*Vect(0,0,1);
		to = GrappleParent.pullDest;
		Velocity = GrappleParent.Instigator.Velocity * 0.5 + GrappleParent.Velocity * 0.5; // It could be that either the grapple or the instigator is moving, maybe even both.
	}
		// if (GrappleParent.LineSprite != Self) {
			// from = ChildLine.Pivot; // :P
		// } else {
			// from = GrappleParent.Instigator.Location + 0.5*GrappleParent.Instigator.BaseEyeHeight*Vect(0,0,1);
		// }
		// }
		// Velocity = GrappleParent.Instigator.Velocity * 0.5;
		// if (GrappleParent.LineSprite != None && GrappleParent.LineSprite != Self) {
			// if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): My GrappleParent has a new LineSprite="$GrappleParent.LineSprite$" - failing."); }
			// from = GrappleParent.LineSprite.Pivot;
		// }
		SetLocation((from+to)/2);
		SetRotation(rotator(to-from));
		DrawScale = VSize(from-to) / 64.0;
	// }
	// if (bStopped) {
		// if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): Disabling my Tick() now that I am bStopped."); }
		// Disable('Tick');
	// }
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

simulated event Destroyed() {
	local int i;
	local kxLine L;
	foreach AllActors(class'kxLine',L) {
		i++;
	}
	if (class'kxGrapple'.default.bDebugLogging) { Log(Level.TimeSeconds$" "$Self$".Destroyed() Destructing with "$i$" kxLines on the level."); }
	Super.Destroyed();
}

defaultproperties {
	LifeSpan=120
	// Texture=Texture'UMenu.Icons.Bg41'
	// Texture=Texture'BotPack.ammocount'
	DrawType=DT_Mesh
	RotationRate=(Roll=90000)
	bUnlit=False
	bNetTemporary=False
	NetPriority=2.6
	RemoteRole=ROLE_SimulatedProxy
	Style=STY_Translucent
	Physics=PHYS_Rotating
}

