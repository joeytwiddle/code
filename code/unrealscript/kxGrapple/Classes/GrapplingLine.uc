class GrapplingLine extends Projectile; // TODO: Projectile?  Is that really needed?

/**
 A GrapplingLine is actually only one section of the line.
 *
 * If the line has been bent, and this section is not direcly attached to the
 * hook, then ParentLine is the next section of the line, closer to the hook
 * (further from the player).
 *
 * If this section of line is attached to the hook, then GrapplingHook will be set.
 *
**/

// TODO: try rewriting this class with just from,to (modified externally) and bSuicide.

var config bool bLogging;

var GrapplingLine ParentLine;
var GrapplingHook GrappleParent;
var bool bStopped;
var Vector NearPivot;
var Vector Reached; // OLD BUG: Never gets updated!
var Vector ReachedRender; // When rendering from nearpivot to nextpivot, lines against flat walls disappeared; this keeps the old rendering style and somehow that problem is gone.
var byte Depth;

replication {
  reliable if (Role == ROLE_Authority)
    bLogging;
  reliable if (Role == ROLE_Authority)
    GrappleParent,ParentLine,bStopped,NearPivot,Reached,ReachedRender;
  reliable if (Role == ROLE_Authority)
    DoUpdate;
}

// simulated event PostBeginPlay() {
	// if (bLogging) { Log(Level.TimeSeconds$" "$Self$".PostBeginPlay() New GP="$GrappleParent$" PL="$ParentLine$" bStopped="$bStopped$" NearPivot="$NearPivot); }
	// Super.PostBeginPlay();
// }

// simulated function SetFromTo(Actor f, Actor t) {
	// from = f;
	// to = t;
// }

simulated event DoUpdate(float DeltaTime) {
	local Vector from,to;
	local Vector X,Y,Z;
	/*
	// Never worked when needed, sometimes worked when unwanted!
	if (GrappleParent == None || GrappleParent.Instigator == None) {
		// if (bLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): Lost my GrappleParent - suiciding."); }
		// Destroy();
		// if (bLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): Lost my GrappleParent - waiting for it ..."); }
		BroadcastMessage(Level.TimeSeconds$" "$Self$".DoUpdate(): Lost my GrappleParent - waiting for it ...");
		// This was NOT cleaning up when we needed it to.
		// I've disabled it because very occasionally on firing my grapple, the line would not appear, and I think here is the problem.
		return;
	}
	// No longer needed since bDestroyed works better, and with folding, we need multiple lines per grapple.
	if (GrappleParent.LineSprite != None && GrappleParent.LineSprite != Self) {
		// if (bLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): My GrappleParent.LineSprite="$GrappleParent.LineSprite$" - not ME "$Self$"!"); }
		if (bLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): My GrappleParent has a new LineSprite="$GrappleParent.LineSprite$" - suiciding."); }
		Destroy();
		return;
	}
	*/
	if (GrappleParent == None) {
		if (bLogging && FRand()<0.01) { Log(Level.TimeSeconds$" "$Self$".DoUpdate() Warning! My GrappleParent == None! I may be a memory leak!"); }
		return;
	}
	// I was unable to destroy this actor by calling its .Destroy() but this mechanism for it to destroy itself works ok:
	if (GrappleParent.bDestroyed) {
		if (bLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): "$GrappleParent$".bDestroyed set (LineSprite="$GrappleParent.LineSprite$") - suiciding."); }
		Destroy();
		return;
	}

	if (GrappleParent.LineSprite != None && GrappleParent.LineSprite != Self && !bStopped) {
		if (bLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate() GrappleParent has a new LineSprite "$GrappleParent.LineSprite$" - setting Self.bStopped."); }
		bStopped = True;
	}

	// if (bLogging && FRand()<0.01) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): Render="$(Role!=ROLE_Authority)$" bStopped="$bStopped$" LineSprite="$GrappleParent.LineSprite$" NearPivot="$NearPivot$" Reached="$Reached); }
	// OK good we are now running on the client with variables replicated.

	// if (Role == ROLE_Authority || Level.NetMode != NM_Client)
	if (Level.NetMode == NM_DedicatedServer)
		return;
	// The rest is *only* done on the client.

	// Update position of line:
	// if (GrappleParent.LineSprite != Self) {
	if (bStopped) {
		// from = Reached;
		from = ReachedRender;
		to = NearPivot;
		// This was supposed to make rendering better when an inbetween line gets stuck against a flat wall.  But it didn't.
		// I wonder now why they didn't always disappear against flat walls.
		if (ParentLine!=None)
			to = ParentLine.Reached;
		// if (bLogging && FRand()<0.01) { Log(Level.TimeSeconds$" "$Self$".DoUpdate() low "$Reached$" <-> high "$NearPivot); }
		// from = GrappleParent.Location;
		// to = GrappleParent.pullDest;
		// Velocity = vect(0,0,0); // was already done when we were stopped
		// TODO: Inefficiency. We keep setting the location etc. of this line, although it is at the moment static
		//       We just need to make sure all the variables have been replicated when we switch off.
		// In fact in *this* case, the server could calculate our Location, and we could do nothing but wait for it to be replicated.
	} else {
		if (GrappleParent.Instigator == None) {
			if (bLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate() Warning! My GrappleParent.Instigator == None so I can't update!  My GrappleParent.InstigatorRep="$GrappleParent.InstigatorRep); }
			return;
		}
		// if (bLogging && FRand()<0.01) { Log(Level.TimeSeconds$" "$Self$".DoUpdate() low "$GrappleParent$" <-> high "$GrappleParent.pullDest$" (NearPivot="$NearPivot$")"); }
		// from = GrappleParent.Instigator.Location + 0.5*GrappleParent.Instigator.BaseEyeHeight*Vect(0,0,1);
		// from = GrappleParent.Instigator.Location + GrappleParent.Instigator.Rotation * Vect(-3.0,+5.0,GrappleParent.Instigator.BaseEyeHeight);
		GetAxes(GrappleParent.Instigator.Rotation,X,Y,Z);
		// from = GrappleParent.Instigator.Location + 50.0*X - 0.0*Y + 0.25*GrappleParent.Instigator.BaseEyeHeight*vect(0,0,1); // Z
		// Translocator has PlayerViewOffset=(X=5.000000,Y=-4.200000,Z=-7.000000)
		from = GrappleParent.Instigator.Location + 1.0*X - 6.0*Y + 0.3*GrappleParent.Instigator.BaseEyeHeight*Z;
		// to = GrappleParent.pullDest;
		to = NearPivot; // better replicated than GrappleParent.pullDest!
		// CONSIDER TODO: could instead use ParentLine.Reached
		// Velocity = GrappleParent.Instigator.Velocity * 0.5 + GrappleParent.Velocity * 0.5; // It could be that either the grapple or the instigator is moving, maybe even both.
		// Velocity = GrappleParent.Instigator.Velocity; // Nicer for firstperson when thrown, maybe not so good for swinging.
		Velocity = GrappleParent.Velocity; // Keeps up with flying hook - best.
		// Velocity = 0.5*GrappleParent.Velocity + 0.5*GrappleParent.Instigator.Velocity; // Nicer for firstperson when thrown, maybe not so good for swinging.
	}
		// if (GrappleParent.LineSprite != Self) {
			// from = ChildLine.NearPivot; // :P
		// } else {
			// from = GrappleParent.Instigator.Location + 0.5*GrappleParent.Instigator.BaseEyeHeight*Vect(0,0,1);
		// }
		// }
		// Velocity = GrappleParent.Instigator.Velocity * 0.5;
		// if (GrappleParent.LineSprite != None && GrappleParent.LineSprite != Self) {
			// if (bLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): My GrappleParent has a new LineSprite="$GrappleParent.LineSprite$" - failing."); }
			// from = GrappleParent.LineSprite.NearPivot;
		// }
		SetLocation((from+to)/2);
		SetRotation(rotator(to-from));
		DrawScale = VSize(from-to) / 64.0;
	// }
	// if (bStopped) {
		// if (bLogging) { Log(Level.TimeSeconds$" "$Self$".DoUpdate(): Disabling my Tick() now that I am bStopped."); }
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
	local GrapplingLine L;
	foreach AllActors(class'GrapplingLine',L) {
		i++;
	}
	// if (bLogging) { Log(Level.TimeSeconds$" "$Self$".Destroyed() Destructing with "$i$" GrapplingLines on the level."); }
	if (bLogging) { Log(Level.TimeSeconds$" "$Self$".Destroyed() Bye! (1/"$i$") GP="$GrappleParent$" PL="$ParentLine$" bStopped="$bStopped$" NearPivot="$NearPivot$" Reached="$Reached$""); }
	Super.Destroyed();
	//// No we don't always want to do this, we might only be moving up a line!
	// if (ParentLine!=None) {
		// if (bLogging) { Log(Level.TimeSeconds$" "$Self$".Destroyed() Now destroying ParentLine "$ParentLine$" ..."); }
		// ParentLine.Destroy();
	// }
}

defaultproperties {
	LifeSpan=120
	// Texture=Texture'UMenu.Icons.Bg41'
	// Texture=Texture'BotPack.ammocount'
	DrawType=DT_Mesh
	RotationRate=(Roll=90000)
	// bUnlit=False
	bUnlit=True
	bNetTemporary=False
	NetPriority=2.6
	RemoteRole=ROLE_SimulatedProxy
	Style=STY_Translucent
	Physics=PHYS_Rotating
	bCollideWorld=False
	bCollideActors=False
	bBlockActors=False
	bBlockPlayers=False
	// bLogging=True // Wasn't getting replicated from server =/
}

