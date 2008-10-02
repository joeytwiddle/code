//================================================================================
// kxGrapple.
//================================================================================

class kxGrapple extends XPGrapple Config(kxGrapple);

/*
var Vector pullDest;
var Vector hNormal;
var bool bRight;
var bool bCenter;
var() config int grappleSpeed;
var() config float timeBetweenHit;
var() config float hitdamage;
var bool bPlaySound;
var bool bAttachedToThing;
var() config bool bDoAttachPawn;
var() config bool bLineOfSight;
var Actor thing;
var float TotalTime;
var XP_GrappleLauncher Master;
var bool bThingPawn;
*/

/*
replication
{
  un?reliable if ( Role == 4 )
    pullDest,hNormal,thing,TotalTime,Master,bThingPawn;
}
*/

/*
simulated function Destroyed ()
{
  Master.XPGrapple = None; // 0x00000013 : 0x0000
  AmbientSound = None; // 0x0000001D : 0x0010
  Master.AmbientSound = None; // 0x00000021 : 0x0017
  PlaySound(sound'hit1g',SLOT_Interface,10.0); // 0x0000002B : 0x0027
  Master.PlaySound(sound'hit1g',SLOT_Interface,10.0); // 0x00000037 : 0x0036
  // Master.PlaySound(sound'FlyBuzz', SLOT_Interface, 2.5, False, 32, 16);
  Instigator.SetPhysics(PHYS_Falling); // 0x00000049 : 0x004E
  Super.Destroyed(); // 0x00000054 : 0x005C
}

simulated function SetMaster (XP_GrappleLauncher W)
{
  Master = W; // 0x00000013 : 0x0000
  Instigator = Pawn(W.Owner); // 0x00000018 : 0x000B
}

auto state Flying
{
  simulated function ProcessTouch (Actor Other, Vector HitLocation)
  {
    Log("I'm in Process Touch. I've never been here before"); // 0x00000012 : 0x0000
    if ( Inventory(Other) != None ) // 0x00000047 : 0x0035
    {
      Inventory(Other).GiveTo(Instigator); // 0x00000051 : 0x0045
    }
    Instigator.AmbientSound = None; // 0x0000005E : 0x005E
    AmbientSound = None; // 0x00000068 : 0x006E
    Destroy(); // 0x0000006C : 0x0075
    return; // 0x0000006F : 0x0078
  }
  
  simulated function HitWall (Vector HitNormal, Actor Wall)
  {
    AmbientSound = Sound'hit1g'; // 0x00000016 : 0x0000
    Master.AmbientSound = Sound'ObjectPush'; // 0x0000001B : 0x000B
    pullDest = Location; // 0x00000026 : 0x001F
    hNormal = HitNormal; // 0x0000002B : 0x002A
    SetPhysics(PHYS_None); // 0x00000032 : 0x0035
    if ( Wall.IsA('Pawn') || Wall.IsA('Mover') ) // 0x00000037 : 0x003A
    {
      if (  !bDoAttachPawn &&  !Wall.IsA('Mover') ) // 0x00000055 : 0x0064
      {
        Destroy(); // 0x00000070 : 0x0087
        return; // 0x00000073 : 0x008A
      }
      bAttachedToThing = True; // 0x00000075 : 0x008C
      thing = Wall; // 0x0000007B : 0x0094
      if ( Wall.IsA('Pawn') ) // 0x00000080 : 0x009F
      {
        bThingPawn = True; // 0x0000008E : 0x00B3
      }
      GotoState('PullTowardDynamic'); // 0x00000093 : 0x00BB
    } else { // 0x00000097 : 0x00C2
      GotoState('PullTowardStatic'); // 0x0000009A : 0x00C5
    }
  }
  
  simulated function BeginState ()
  {
    Velocity = vector(Rotation) * speed; // 0x00000013 : 0x0000
  }
  
}

state() PullTowardStatic
{
  simulated function Tick (float DeltaTime)
  {
    local float speed;
    local Vector V;
  
    speed = VSize(Instigator.Velocity); // 0x00000014 : 0x0000
    if ( bLineOfSight &&  !Instigator.LineOfSightTo(self) ) // 0x00000022 : 0x0016
    {
      Destroy(); // 0x0000003A : 0x0033
      return; // 0x0000003D : 0x0036
    }
    if ( VSize(Instigator.Location - pullDest) <= 50 ) // 0x0000003F : 0x0038
    {
      Instigator.SetPhysics(PHYS_None); // 0x00000055 : 0x0057
      AmbientSound = None; // 0x00000060 : 0x0065
      Master.AmbientSound = None; // 0x00000064 : 0x006C
      Instigator.AddVelocity(Instigator.Velocity * -1); // 0x0000006E : 0x007C
      bPlaySound = False; // 0x00000087 : 0x00A1
    } else { // 0x0000008C : 0x00A9
      Instigator.Velocity = Normal(pullDest - Instigator.Location) * grappleSpeed; // 0x0000008F : 0x00AC
    }
  }
  
  simulated function ProcessTouch (Actor Other, Vector HitLocation)
  {
    Instigator.AmbientSound = None; // 0x00000014 : 0x0000
    AmbientSound = None; // 0x0000001E : 0x0010
    Destroy(); // 0x00000022 : 0x0017
  }
  
  simulated function BeginState ()
  {
    Instigator.Velocity = Normal(pullDest - Instigator.Location) * grappleSpeed; // 0x00000013 : 0x0000
    Instigator.SetPhysics(PHYS_Falling); // 0x0000002F : 0x002E
    TotalTime = 0.0; // 0x0000003A : 0x003C
  }
  
}

state() PullTowardDynamic
{
  ignores  Tick;
  
  simulated function ProcessTouch (Actor Other, Vector HitLocation)
  {
    Instigator.AmbientSound = None; // 0x00000014 : 0x0000
    AmbientSound = None; // 0x0000001E : 0x0010
    Destroy(); // 0x00000022 : 0x0017
  }
  
  simulated function BeginState ()
  {
    Instigator.SetPhysics(PHYS_Falling); // 0x00000013 : 0x0000
    TotalTime = 0.0; // 0x0000001E : 0x000E
  }
  
}
*/

defaultproperties
{
    grappleSpeed=600
    timeBetweenHit=0.50
    hitdamage=20.00
    bDoAttachPawn=True
    bLineOfSight=True
    speed=4000.00
    MaxSpeed=4000.00
    MyDamageType=eviscerated
    bNetTemporary=False
    LifeSpan=60.00
    Texture=Texture'UMenu.Icons.Bg41'
    Mesh=LodMesh'UnrealShare.GrenadeM'
    bUnlit=True
    bMeshEnviroMap=True
    bBlockActors=True
    bBlockPlayers=True
}

