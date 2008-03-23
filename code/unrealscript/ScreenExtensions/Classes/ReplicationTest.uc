// ========================================================================

//
// Coded by JonAzz

// fuxxored by nogginBasher

// ========================================================================

class ReplicationTest extends Actor;

var int intA;

// replication Block
replication
{
    // Sends these values from the server to the clients
    reliable if(Role == ROLE_Authority)
        ClientFunction, intA;

}

simulated event PostNetReceive()
{
    if ( IntA == 1 )
    {
	// Log("PostNetReceive (Role="$Role$")");
        ClientFunctionB();
    }
}

function ServerParseFunction(int A)
{
    if(A == 1)
    {
        ClientFunction(); // Other server required changes
//        A = 0;

    if(Role == Role_Authority)
        // log("A The server has done something");
	Log("[Role=Auth] ServerParseFunction(A="$A$") IntA="$IntA);
    else if(Role < Role_Authority)
	Log("[Role=____] ServerParseFunction(A="$A$") IntA="$IntA);

    }
}

simulated function ClientFunction()   // THIS DOES NOT WORK!!
{
//    if(Level.NetMode == NM_DedicatedServer)
//        return;

    // if(Role == Role_Authority)
        // log("B The server has done something");
	// Log("[Role=Auth] ServerParseFunction(A="$A$") IntA="$IntA);
    // else if(Role < Role_Authority)
        // log("B The client has done something");
	// Log("[Role=Auth] ServerParseFunction(A="$A$") IntA="$IntA);

}

simulated function ClientFunctionB()    // THIS WORKS!
{
//    if(Level.NetMode == NM_DedicatedServer)
//        return;

    if(Role == Role_Authority)
	Log("[Role=Auth] ClientFunctionB-PostNetReceive(IntA="$IntA);
    else if(Role < Role_Authority)
	Log("[Role=____] ClientFunctionB-PostNetReceive(IntA="$IntA);

}

function Trigger(Actor Other, Pawn EventInstigator)
{
    Log("Trigger("$Other$","$EventInstigator$")");
    ServerParseFunction( 1 );
    intA = 1;
}

defaultproperties
{
    bEdShouldSnap=true
    DrawType=DT_StaticMesh
    // StaticMesh=StaticMesh'AS_Decos.ExplodingBarrel'
    DrawScale=1

    // bStaticLighting=True
    bCollideActors=True
    bBlockActors=False
    // bBlockKarma=True
    CollisionHeight=+000001.000000
    CollisionRadius=+000001.000000

    Physics=PHYS_None

    bAlwaysRelevant=true
//  bNoDelete=false
    bStasis=True
    bStatic=False
    // bSkipActorPropertyReplication=false
    RemoteRole=ROLE_SimulatedProxy
    // bReplicateMovement=false
    NetPriority=3.0
    NetUpdateFrequency=10
    // bNetNotify=true
}


