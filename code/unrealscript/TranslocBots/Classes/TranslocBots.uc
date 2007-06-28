// TODO: hey look we can create new actors from only their class name! BaseMutator.AddMutator(Spawn(MutatorClass));
//       we can also use .Class to get the class of an actor

// DONE: MyLog is very inefficient, still generating strings when logging is off; replace it or make a C++ macro for it
// TODO CONSIDER: Replace all MyLog with Log, if we don't need the "[TLB]"

class TranslocBots expands Mutator;

var config bool bLogging;
var config bool bForceBotsTransloc; // On a timer, attempts to force bots to translocate somewhere
var config bool bGoForFlag;
var config bool bReplacePathNodes; // Will replace PathNode actors with FreshTranslocStart/Dest/FreshLiftCenter/Exits actors.
var config bool bStopAtEndGame;
var config bool bRenameBots;

var String PropertiesToCheck;

defaultproperties {
  bLogging=False
  bForceBotsTransloc=True
  bGoForFlag=True
  bReplacePathNodes=False
  PropertiesToCheck="From_NavigationPoint:,ownerTeam,taken,upstreamPaths,Paths,PrunedPaths,VisNoReachPaths,visitedWeight,routeCache,bestPathWeight,nextNavigationPoint,nextOrdered,prevOrdered,startPath,previousPath,cost,ExtraCost,bPlayerOnly,bEndPoint,bEndPointOnly,bSpecialCost,bOneWayPath,bNeverUseStrafing,bAutoBuilt,bTwoWay,From_Actor:,bStatic,bHidden,bNoDelete,bAnimFinished,bAnimLoop,bAnimNotify,bAnimByOwner,bDeleteMe,bAssimilated,bTicked,bLightChanged,bDynamicLight,bTimerLoop,bCanTeleport,bOwnerNoSee,bOnlyOwnerSee,bIsMover,bAlwaysRelevant,bAlwaysTick,bHighDetail,bStasis,bForceStasis,bIsPawn,bNetTemporary,bNetOptional,bReplicateInstigator,bTrailerSameRotation,bTrailerPrePivot,bClientAnim,bSimFall,Role,RemoteRole,NetTag,Owner,InitialState,Group,TimerRate,TimerCounter,LifeSpan,AnimSequence,AnimFrame,AnimRate,TweenRate,SkelAnim,LODBias,Level,XLevel,Tag,Event,Target,Instigator,AmbientSound,Inventory,Base,Region,AttachTag,StandingCount,MiscNumber,LatentByte,LatentInt,LatentFloat,LatentActor,Touching[4],Deleted,SpecialTag,Location,Rotation,OldLocation,ColLocation,Velocity,Acceleration,OddsOfAppearing,bHiddenEd,bDirectional,bSelected,bMemorized,bHighlighted,bEdLocked,bEdShouldSnap,bEdSnap,bTempEditor,bDifficulty0,bDifficulty1,bDifficulty2,bDifficulty3,bSinglePlayer,bNet,bNetSpecial,bScriptInitialized,HitActor,EDrawType,ERenderStyle,Sprite,Texture,Skin,Mesh,Brush,DrawScale,PrePivot,ScaleGlow,VisibilityRadius,VisibilityHeight,AmbientGlow,Fatness,SpriteProjForward,bUnlit,bNoSmooth,bParticles,bRandomFrame,bMeshEnviroMap,bMeshCurvy,bFilterByVolume,bShadowCast,bHurtEntry,bGameRelevant,bCarriedItem,bForcePhysicsUpdate,bIsSecretGoal,bIsKillGoal,bIsItemGoal,bCollideWhenPlacing,bTravel,bMovable,MultiSkins[8],SoundRadius,SoundVolume,SoundPitch,TransientSoundVolume,TransientSoundRadius,CollisionRadius,CollisionHeight,bCollideActors,bCollideWorld,bBlockActors,bBlockPlayers,bProjTarget,ELightType,ELightEffect,byte,bSpecialLit,bActorShadows,bCorona,bLensFlare,bBounce,bFixedRotationDir,bRotateToDesired,bInterpolating,bJustTeleported,EDodgeDir,Mass,Buoyancy,RotationRate,DesiredRotation,PhysAlpha,PhysRate,PendingTouch,AnimLast,AnimMinRate,OldAnimRate,SimAnim,NetPriority,NetUpdateFrequency,bNetInitial,bNetOwner,bNetRelevant,bNetSee,bNetHear,bNetFeel,bSimulatedPawn,bDemoRecording,bClientDemoRecording,bClientDemoNetFunc,RenderIteratorClass,RenderInterface"
  bStopAtEndGame=False // TODO: should default to True for sensible servers, but then check UpdateBotName() will be called before AutoTeamBalance updates stats.
  bRenameBots=False // interesting to watch bots change state
}

var bool bWorking; // Whether or not we have succeeded in replacing some pathnodes

var name currentTag;

var int depth; // used to prevent infinite recursion when replacing actors

var Actor lastPathNode;
var Actor RandomFoundNavigationPoint;

// function PreBeginPlay() {
function PostBeginPlay() {
  if (bForceBotsTransloc) SetTimer(2,True);
}

function MyLog(String msg) {
  // if (bLogging) {
    Log("[TLB] " $ msg);
  // }
}

event Timer() {
  local Pawn p;
  local Bot b;
  local Actor dest;
  local int method;
  local vector X,Y,Z;

  if (!bForceBotsTransloc) // admin can turn it on/off mid-game
    return;

  if (Level.Game.bGameEnded && bStopAtEndGame)
    return;

  if (Level.Pauser != "" && bStopAtEndGame)
    return;

  if (bLogging) { MyLog(""); }

  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
  // foreach AllActors(class'Bot', p) {
    if (!p.IsA('Bot')) {
      continue;
    }
    if (Rand(100)>80) {
      continue;
    }

    b = Bot(p);

    if (bRenameBots) {
      UpdateBotName(b);
    }

    // if (bLogging) { MyLog(p.getHumanName() $ " :: " $ p.GetStateName() $ " RoamTarget="$b.RoamTarget$" MoveTarget="$b.MoveTarget$ " Target="$b.Target$""); }
    // if (bLogging) { MyLog(p.getHumanName() $ " :: "$ p.GetStateName()$ " ("$b.Destination$", "$b.RoamTarget$", "$b.MoveTarget$", "$b.Target$")"); }
    // if (bLogging) { MyLog(b.getHumanName() $ "["$Int(b.Skill)$","$b.bNovice$","$b.bThreePlus$","$b.bDumbDown$"] :: "$ p.GetStateName()$ " ("$b.RoamTarget$", "$b.MoveTarget$", "$b.Target$")"); }
    // if (bLogging) { MyLog(p.getHumanName() $ " :: "$ p.GetStateName()$ " ("$b.RoamTarget$", "$b.MoveTarget$", "$b.Target$")"); }
    if (bLogging) { MyLog(p.getHumanName() $ " :: "$ p.GetStateName()$ " ("$b.RoamTarget$", "$b.MoveTarget$", "$(b.Target != None)$")"); }

    if (b.PlayerReplicationInfo.HasFlag != None) {
      if (bLogging) { MyLog(p.getHumanName()$" xx Bot is holding a flag!"); }
      continue;
    }
    if (b.MyTranslocator == None) {
      if (bLogging) { MyLog(p.getHumanName()$" xx Bot has no translocator!"); }
      continue;
    }

    if (b.MyTranslocator.TTarget != None) {
      if (bLogging) { MyLog(p.getHumanName()$" !! Bot already has a TTarget = " $ b.MyTranslocator.TTarget.DesiredTarget /* $ " (" $ b.MyTranslocator.TTarget.RealLocation $ ")" $ "," $ b.MyTranslocator.TTarget.Master */ ); }
      // if (Rand(100)<50 && b.MyTranslocator.TTarget.DesiredTarget != None) {
        // continue;
      // } else {
        b.MyTranslocator.TTarget.Destroy();
        b.MyTranslocator.TTarget = None;
        b.MyTranslocator.bTTargetOut = false;
      // }
    }

    if (Rand(100)<10) {
      DoTranslocateToTarget(b,getRandomVisiblePoint(b.Location),"Random");
      continue;
    }

    // if (b.Destination != None) {
    // if (Rand(100)<100) {
      // // DoTranslocateToTarget(b,b.Destination);
      // TranslocateToVector(b,b.Destination);
    // } else
    if (b.AlternatePath != None) {
      DoTranslocateToTarget(b,b.AlternatePath,"AlternatePath");
    } else
    if (b.RoamTarget != None) {
      DoTranslocateToTarget(b,b.RoamTarget,"RoamTarget");
    } else
    if (b.MoveTarget != None) {
      DoTranslocateToTarget(b,b.MoveTarget,"MoveTarget");
    } else
    if (b.Target != None) {
      DoTranslocateToTarget(b,b.Target,"Target");
    } else {
      // b.RoamTarget = FindEnemyFlag(b);
      // b.GoToState('Roaming');
      // if (bLogging) { MyLog(b.getHumanName()$" ** NEW FLAG TARGET (roam) "$b.RoamTarget); }
    }

    if (Rand(100)<10) {
      // // b.Target = FindEnemyFlag(b);
      // b.MoveTarget = FindEnemyFlag(b);
      // b.GoToState('Attacking');
      // if (bLogging) { MyLog(b.getHumanName()$" ** NEW FLAG TARGET (move) "$b.MoveTarget); }
      b.RoamTarget = FindEnemyFlag(b);
      b.GoToState('Roaming');
      if (bLogging) { MyLog(b.getHumanName()$" ** NEW FLAG TARGET (roam) "$b.RoamTarget); }
    }

    // They occasionally do this ducking or crouching, but only for a brief moment, then they are standing and walking again.
    // If in combat, consider ducking
    if (/*b.Target != None &&*/ Rand(100)>50) {
      // b.bCanDuck = True;
      // b.bDuck = (b.bDuck+1)%2;
      b.bIsCrouching = ! b.bIsCrouching;
      if (b.bIsCrouching) b.bDuck=1; else b.bDuck=0;
      if (b.bIsCrouching) {
        b.PlayDuck();
      }
    } else
    // Consider dodging
    if (/*b.Target != None &&*/ Rand(100)>50) {
      b.TryToDuck(vector(b.Rotation),Rand(2)<1);
      GetAxes(b.Rotation,X,Y,Z);
      b.TryToDuck(Y,Rand(2)<1);
    }

    /*
    if (Rand(100)<5) {
      // b.GoToState('RangedAttack');
    }
    if (Rand(100)<5) {
      // b.GoToState('Roaming');
    }
    if (Rand(100)<10) {
      // if (bLogging) { MyLog(p.getHumanName()$" ** NEW State Attacking"); }
      // b.GoToState('Attacking');
    }
    */

    /*
    // if (b.FindBestPathToward(b.Target,false)) {
      // if (b.LineOfSightTo(b.Destination)) {
        // if (bLogging) { MyLog("TranslocBots.Timer(): " $ b $ ".TranslocateToTarget(" $ b.Destination $ ")"); }
        // b.TranslocateToTarget(b.Destination);
      // }
    // }
    // if (bLogging) { MyLog(p$" Target="$b.Target$", MoveTarget="$b.MoveTarget$""); }
    */

    /*
    dest = None;
    // dest = b.Target;
    // method = Rand(3);
    // method = Rand(2);
    // method = 1;
    if (b.Target==None && b.RoamTarget!=None && Rand(1000)<10) {
      b.Target = b.RoamTarget;
      if (bLogging) { MyLog(p.getHumanName()$" ** NEW ROAM TARGET "$dest); }
    }
    if (b.Target == None || Rand(1000)<10) {
      // if (b.Target == None || (bGoForFlag && Rand(100)<10)) {
        // b.Target = FindEnemyFlag(p);
        // if (bLogging) { MyLog("TranslocBots.Timer(): .. FindEnemyFlag returned " $ "p" $ ".Target = " $ b.Target); }
      // }
      // if (b.Target == None) {
        // if (bLogging) { MyLog("TranslocBots.Timer(): xx p.Target == None"); }
        // continue;
      // }
      // dest = b.Target;
      dest = FindEnemyFlag(p);
      if (dest != None) {
        // if (bLogging) { MyLog("TranslocBots Method 1: FindEnemyFlag = "$dest); }
        if (bLogging) { MyLog(p.getHumanName()$" ** NEW FLAG TARGET "$dest); }
        b.Target = dest;
        // b.MoveTarget = dest;
      }
      dest = None;
    }
    // } else if (method == 1) {
      if (b.Target == None && Rand(1000)<10) {
        b.Target = getRandomNavigationPoint();
        if (bLogging) { MyLog(p.getHumanName()$" ** NEW RANDOM TARGET "$b.Target); }
      }
      // if (bLogging) { MyLog("TranslocBots.Timer(): .. p.Target = "$b.Target); }
      // if (bLogging) { MyLog("TranslocBots.Timer(): .. p.MoveTarget = "$b.MoveTarget); }
      // b.FindBestPathToward(b.Target,false);
      b.FindBestPathToward(b.Target,true);
      // if (bLogging) { MyLog("TranslocBots Method 2: FindBestPathToward = "$b.MoveTarget); }
      // if (bLogging) { MyLog("TranslocBots BESTPATH "$b.MoveTarget); }
      if (b.MoveTarget == None) {
        if (bLogging) { MyLog(p.getHumanName()$" xx FindBestPathToward("$b.Target$") = "$b.MoveTarget); }
      } else {
        // if (bLogging) { MyLog("TranslocBots ** NEW MoveTarget "$b.MoveTarget); }
        // if (bLogging) { MyLog(p.getHumanName()$" -- "$b.MoveTarget$" -> "$b.Target$""); }
        // if (bLogging) { MyLog(p.getHumanName()$" -- "$b.MoveTarget$" -> "$b.Target$""); }
        // // if (bLogging) { MyLog(b $ ".TranslocateToTarget(" $ b.MoveTarget $ ")"); }
        // // if (bLogging) { MyLog("TranslocBots.Timer(): >>> p.MoveTarget = "$b.MoveTarget); }
        // if (bLogging) { MyLog("TranslocBots.Timer(): >> MoveTarget >> TranslocateToTarget(" $ b.MoveTarget $ ")"); }
        // b.TranslocateToTarget(b.MoveTarget);
        dest = b.MoveTarget;
      }
    // } else { // method == 2
    */
    /*
      foreach VisibleActors(class'Actor', dest) {
        if (dest.IsA('NavigationPoint')) {
          // if (bLogging) { MyLog("TranslocBots.Timer(): >> visible >> TranslocateToTarget(" $ dest $ ")"); }
          // b.TranslocateToTarget(dest);
          if (RandomFoundNavigationPoint == None || Rand(1000)<5) {
            RandomFoundNavigationPoint = dest;
            // break;
          }
        }
      }
      dest = RandomFoundNavigationPoint;
      if (bLogging) { MyLog("TranslocBots Method 3: RandomFoundNavigationPoint = "$dest); }
    }
    */
    /*
    // dest = b.FindPathToward(b.Target);
    if (dest == None) {
      // if (bLogging) { MyLog("TranslocBots.Timer(): XX dest = "$dest); }
    } else {
      // if (bLogging) { MyLog("TranslocBots.Timer(): dest = "$dest); }
      // if (bLogging) { MyLog(b $ ".TranslocateToTarget(" $ dest $ ")"); }
      // if (bLogging) { MyLog(p.getHumanName()$" >> TranslocateToTarget(" $ dest $ ")"); }
      if (bLogging) { MyLog(b.getHumanName()$" >> TranslocateToTarget(" $ dest $ ") ("$VSize(dest.Location-b.Location)$","$b.LineOfSightTo(dest)$")"); }
      b.TranslocateToTarget(dest);
      // b.Target = FindEnemyFlag(p);
    }
    */

  }
}

function UpdateBotName(Bot b) {
  local String name;
  name = b.PlayerReplicationInfo.PlayerName;
  if (InStr(name,"^") >= 0) {
    name = Left(name,InStr(name,"^"));
  }
  if (!Level.Game.bGameEnded) {
    name = name $ "^" $ b.GetStateName();
  }
  b.PlayerReplicationInfo.PlayerName = name;
}

function bool DoTranslocateToTarget(Bot b, Actor Target, optional String where) {
  local float distance;
  local bool canSee;
  local Actor oldTarget;

  if (where == "") where = "_target_";

  distance = VSize(Target.Location-b.Location);

  if (distance<256) { // TODO: these distances should really be related to gamespeed, and the timer duration too
    // Target is too close; faster to just walk
    if (bLogging) { MyLog(b.getHumanName()$" .. too close to "$where$" "$Target$" ("$distance$")"); }
    return false;
  }

  canSee = b.LineOfSightTo(Target);

  if (bLogging) { MyLog(b.getHumanName()$" >> TranslocateToTarget(" $ Target $ ") "$where$" ("$VSize(Target.Location-b.Location)$","$b.LineOfSightTo(Target)$")"); }

  // If distance to target is too great, or there is no LineOfSightTo, then use FindBestPathToward(Target) to find a closer node
  if (distance>32768 || !canSee) { // target it too far, or not in line-of-sight; try to find a waypoint to it
    oldTarget = b.MoveTarget; // since we are changing the bot's MoveTarget, let's remember it and reset it afterwards
    b.FindBestPathToward(Target,true);
    if (b.MoveTarget != None && VSize(b.MoveTarget.Location-b.Location) >= 32) { // if the waypoint is too close, stick with the further point
      Target = b.MoveTarget;
      if (bLogging) { MyLog(b.getHumanName()$" >> Waypoint(" $ Target $ ") ("$VSize(Target.Location-b.Location)$","$b.LineOfSightTo(Target)$")"); }
    } else {
      if (bLogging) { MyLog(b.getHumanName()$" >> NO Waypoint!"); }
    }
    b.MoveTarget = oldTarget;
  }

  if ( (Target.IsA('PlayerPawn') || Target.IsA('Bot')) && Pawn(Target).PlayerReplicationInfo.Team == b.PlayerReplicationInfo.Team) {
    return false; // try to avoid telefragging your team-mates!
  }

  b.TranslocateToTarget(Target);
}

// So far, this throws the TL, but the bot doesn't translocate to it.
function TranslocateToVector(Bot b, vector dest) {
  if (bLogging) { MyLog(b.getHumanName()$" >> TranslocateToVector(" $ dest $ ") ("$VSize(dest-b.Location)/*$","$b.LineOfSightTo(dest)*/$")"); }
  b.PendingWeapon = b.MyTranslocator;
  // b.MyTranslocator.DesiredTarget = Destn;
  if ( b.Weapon == None )
    b.ChangedWeapon();
  else if ( b.Weapon != b.PendingWeapon )
    b.Weapon.PutDown();
  else
    b.MyTranslocator.PlayPostSelect();
  // b.MoveTarget = Destn;
  b.DesiredRotation = rotator(dest - b.Location);
  b.SpecialPause = 1.5;

  b.MyTranslocator.TTarget = Spawn(class'TranslocatorTarget',,,dest);
  if (b.MyTranslocator.TTarget != None) {
    b.MyTranslocator.bTTargetOut = True;
    b.MyTranslocator.TTarget.Master = b.MyTranslocator;
    b.MyTranslocator.TTarget.SetCollisionSize(0,0);
    b.MyTranslocator.TTarget.Throw(b,b.MyTranslocator.MaxTossForce,dest);
  }
}

// Inefficient, but fairly distributed:
function Actor getRandomVisiblePoint(vector Loc) {
  local Actor Other;
  local int count;
  local int target,num;
  count=0;
  foreach VisibleActors(class'Actor', Other, 65536, Loc) {
    if (Other.IsA('NavigationPoint')) {
      count++;
    }
  }
  if (count == 0)
    return getRandomNavigationPoint();
  target = Rand(count);
  num=0;
  foreach VisibleActors(class'Actor', Other, 65536, Loc) {
    if (Other.IsA('NavigationPoint')) {
      if (num == target)
        RandomFoundNavigationPoint = Other;
      num++;
    }
  }
  return RandomFoundNavigationPoint;
}

function Actor getRandomNavigationPoint() {
  local NavigationPoint N;
  // foreach AllActors(class'Navigati', Other) {
  for ( N=Level.NavigationPointList; N!=None; N=N.NextNavigationPoint ) {
    // Other.IsA('NavigationPoint') && 
    if ((RandomFoundNavigationPoint == None || Rand(1000)<5)) {
      RandomFoundNavigationPoint = N;
    }
  }
  return RandomFoundNavigationPoint;
}

function Actor FindEnemyFlag(Pawn player) {
  local Actor Other;
  local int team;
  local Actor Found;
  team = player.PlayerReplicationInfo.Team;
  // team = Rand(2);
  foreach AllActors(class'Actor', Other) {
    // if (bLogging) { MyLog("FindEnemyFlag("$player$"): "$Other); }
    // if (Other.IsA('Flag1')) {
      // if (Flag1(Other).Team != PlayerPawn(player).PlayerReplicationInfo.Team) return Other;
    // }
    /*
    if (Other.IsA('Flag2')) {
      if (Flag2(Other).Team != PlayerPawn(player).PlayerReplicationInfo.Team) return Other;
    }
    if (Other.IsA('Flag3')) {
      if (Flag3(Other).Team != PlayerPawn(player).PlayerReplicationInfo.Team) return Other;
    }
    if (Other.IsA('Flagb')) {
      if (Flagb(Other).Team != PlayerPawn(player).PlayerReplicationInfo.Team) return Other;
    }
    */
    if (Other.IsA('FlagBase')) {
      // if (bLogging) { MyLog("FindEnemyFlag(): Other = "$Other$" Other.Team="$FlagBase(Other).Team$" player.Team="$PlayerPawn(player).PlayerReplicationInfo.Team$""); }
      // if (bLogging) { MyLog("TranslocBots.FindEnemyFlag(): .. Checking "$Other$" Other.Team="$FlagBase(Other).Team$" player.Team="$team); }
      if (FlagBase(Other).Team != team) {
        if (Found == None || Rand(10)<5) {
          Found = Other;
        }
      }
    // } else if (Other != None && Other.IsA('NavigationPoint') && Rand(1000) < 5) {
      // // if (bLogging) { MyLog("FindEnemyFlag(): Picking up "$Other); }
      // RandomFoundNavigationPoint = Other;
    }
    if (Other.IsA('CTFFlag')) {
      if (CTFFlag(Other).Team != team) {
        if (Found == None || Rand(10)<5) {
          Found = Other;
        }
      }
    }
  }
  return Found;
}

function DumpProperties( Object O ) {
  local Actor P;
  local String properties[255];
  local int numProperties,i;
  local String result;

  P=Actor(O);

  numProperties = SplitString(PropertiesToCheck, ",", properties);

  for (i=0;i<numProperties;i++) {
    result = P.GetPropertyText(properties[i]);
    if ( (""$result) != "None" ) {
      if (bLogging) { MyLog( P.name $ "." $ properties[i] $ " = " $ result ); }
    }
  }

  // if (bLogging) { MyLog( "[TYPE]" $ P.class.name $ "[NAME]" $ P.name $ "[VALUE]" $ P $ "[bStatic]" $ P.GetPropertyText( "bStatic" ) ); }
  // foreach AllActors(class'Actor', P)
    // if(P.Outer == O.Class)
    // if(P.Owner == O.Class)
      // if (bLogging) { MyLog( "[TYPE]" @ P.class.name @ "[NAME]" @ P.name @ "[VALUE]" @ O.GetPropertyText( string(P.name) ) ); }
}

function CopyProperties( Object P, Object N ) {
  local String properties[255];
  local int numProperties,i;
  local String result;

  numProperties = SplitString(PropertiesToCheck, ",", properties);
  if (bLogging) { MyLog("TranslocBots: Copying "$numProperties$" properties from "$P$" to "$N); }

  for (i=0;i<numProperties;i++) {
    result = P.GetPropertyText(properties[i]);
    if ( (""$result) != "None" ) {
      N.SetPropertyText(properties[i],result);
      // if (bLogging) { MyLog( N.Name $ "." $ properties[i] $ " = " $ P.Name $ "." $ properties[i] $ " (" $ result $ ")" ); }
      if (N.GetPropertyText(properties[i]) == result) {
        // if (bLogging) { MyLog("  succeeded."); }
      }
    }
  }

  /*
  NavigationPoint(N).nextNavigationPoint = NavigationPoint(P).nextNavigationPoint;
  if (bLogging) { MyLog(N $ ".nextNavigationPoint = " $ NavigationPoint(P).nextNavigationPoint); }
  NavigationPoint(N).nextOrdered = NavigationPoint(P).nextOrdered;
  if (bLogging) { MyLog(N $ ".nextOrdered = " $ NavigationPoint(P).nextOrdered); }
  NavigationPoint(N).prevOrdered = NavigationPoint(P).prevOrdered;
  if (bLogging) { MyLog(N $ ".prevOrdered = " $ NavigationPoint(P).prevOrdered); }
  NavigationPoint(N).startPath = NavigationPoint(P).startPath;
  if (bLogging) { MyLog(N $ ".startPath = " $ NavigationPoint(P).startPath); }
  NavigationPoint(N).previousPath = NavigationPoint(P).previousPath;
  if (bLogging) { MyLog(N $ ".previousPath = " $ NavigationPoint(P).previousPath); }
  */
  /*
  if (bLogging) { MyLog("CopyProperties("$P$","$N$")"); }
  if ( NavigationPoint(N).nextNavigationPoint != NavigationPoint(P).nextNavigationPoint ) {
    if (bLogging) { MyLog("!!! nextNavigationPoint !!! "$NavigationPoint(N).nextNavigationPoint$"/"$NavigationPoint(P).nextNavigationPoint$""); }
  }
  if ( NavigationPoint(N).nextOrdered != NavigationPoint(P).nextOrdered ) {
    if (bLogging) { MyLog("!!! nextOrdered !!! "$NavigationPoint(N).nextOrdered$"/"$NavigationPoint(P).nextOrdered$""); }
  }
  if ( NavigationPoint(N).prevOrdered != NavigationPoint(P).prevOrdered ) {
    if (bLogging) { MyLog("!!! prevOrdered !!! "$NavigationPoint(N).prevOrdered$"/"$NavigationPoint(P).prevOrdered$""); }
  }
  if ( NavigationPoint(N).startPath != NavigationPoint(P).startPath ) {
    if (bLogging) { MyLog("!!! startPath !!! "$NavigationPoint(N).startPath$"/"$NavigationPoint(P).startPath$""); }
  }
  if ( NavigationPoint(N).previousPath != NavigationPoint(P).previousPath ) {
    if (bLogging) { MyLog("!!! previousPath !!! "$NavigationPoint(N).previousPath$"/"$NavigationPoint(P).previousPath$""); }
  }
  */

}


// Force game to always keep this actor, even if other mutators want to get rid of it.
function bool AlwaysKeep(Actor Other) {

  // if (bLogging) { MyLog("TranslocBots.AlwaysKeep("$Other$") called."); }
  // if (bLogging) { MyLog("TranslocBots.AlwaysKeep("$Other$") called; currentTag="$currentTag$""); }
  // if (bLogging) { MyLog("TranslocBots.AlwaysKeep("$Other$") called; currentTag="$currentTag$" Location="$Other.Location$""); }
  // if (bLogging) { MyLog("TranslocBots.AlwaysKeep("$Other$") at "$Other.Location$""); }
  // if ( (""$Other.Class)=="TranslocStart" || (""$Other.Class)=="TranslocDest" || (""$Other.Class)=="LiftCenter" ) {
  // }
  if (Other.IsA('LiftExit')) {
    // if (bLogging) { MyLog("TranslocBots.AlwaysKeep("$Other$"): LiftExit Tag = "$LiftExit(Other).LiftTag$""); }
    if ((""$LiftExit(Other).LiftTag) == "None") {
      refreshCurrentTag();
      if (bLogging) { MyLog("= Setting "$Other$""$Other.Location$".LiftTag = "$currentTag); }
      LiftExit(Other).LiftTag = currentTag;
      if (bReplacePathNodes) { CopyProperties(lastPathNode,Other); }
    } else {
      currentTag = LiftExit(Other).LiftTag;
      // if (bLogging) { MyLog(". Leaving "$Other$"("$currentTag$")"); }
    }
  }
  if (Other.IsA('LiftCenter')) {
    // if (bLogging) { MyLog("TranslocBots.AlwaysKeep("$Other$"): LiftCenter Tag = "$LiftCenter(Other).LiftTag$""); }
    if ((""$LiftCenter(Other).LiftTag) == "None") {
      refreshCurrentTag();
      if (bLogging) { MyLog("= Setting "$Other$""$Other.Location$".LiftTag = "$currentTag); }
      LiftCenter(Other).LiftTag = currentTag;
      if (bReplacePathNodes) { CopyProperties(lastPathNode,Other); }
    } else {
      currentTag = LiftCenter(Other).LiftTag;
      // if (bLogging) { MyLog(". Leaving "$Other$"("$currentTag$")"); }
    }
  }

  // Pass to other mutators in the default way:
  return Super.AlwaysKeep(Other);

}

function refreshCurrentTag() {
  if ((""$currentTag) == "None" || RandRange(1,7)<2) {
    // currentTag = "POST_GEN_TAG_" $ RandRange(1,100);
    // currentTag = 'POST_GEN_TAG_';
    // currentTag = 'POST_GEN_TAG_' $ RandRange(1,100);
    SetPropertyText("currentTag","Fresh_Tag" $ RandRange(1,100)); // workaround to build a new name variable from a string
    // if (bLogging) { MyLog("TranslocBots.refreshCurrentTag() NEW! SetPropertyText(\"currentTag\",\"" $ currentTag $ "\")"); }
  }
}

function MyReplaceWith(Actor Other,String str) {
  if (str=="None") {
    if (bLogging) { MyLog("["$depth$"] x skipping "$ Other $ " -> " $ str); }
    return;
  }
  // if (bLogging) { MyLog("["$depth$"] > replacing "$ Other $ "" $ Other.Location $ " -> " $ str); }
  // if (bLogging) { MyLog("["$depth$"] > replacing "$ Other $ "" $ Other.Location $ " -> " $ str); }
  if ((""$Other.Class) != "PathNode") {
    if (bLogging) { MyLog("["$depth$"] > replacing "$ Other.Class $ "(" $ Other.Location $ ") -> " $ str); }
  }
  depth++;
    // DumpProperties(Other);
    lastPathNode = Other;
    ReplaceWith(Other,str);
  depth--;
  if (!bWorking) {
    bWorking=True;
    if (bLogging) { MyLog("TranslocBots is attempting to create new transloc paths for bots on this map ("$Level$")"); }
  }
}

// Allow mutators to remove actors (by returning false), or replace them.
function bool CheckReplacement(Actor Other, out byte bSuperRelevant) {
  local int i;

  if (depth>0) {
    // if (bLogging) { MyLog("("$depth$") < checkreplacement ignoring " $ Other $ " ("$bSuperRelevant$")"); }
    return True;
  }

  if (bReplacePathNodes && Other.IsA('PathNode')) {
    i = RandRange(1,5);
    if (i == 1) {
      MyReplaceWith(Other,"TranslocBots.FreshTranslocStart");
    } else if (i == 2) {
      MyReplaceWith(Other,"TranslocBots.FreshTranslocDest");
    } else if (i == 3) {
      MyReplaceWith(Other,"TranslocBots.FreshLiftCenter");
    } else if (i == 4) {
      MyReplaceWith(Other,"TranslocBots.FreshLiftExit");
    } else {
      if (bLogging) { MyLog("TranslocBots.CheckReplacement(): RandRange(1,4) returned "$i); }
    }
    return true;
  }

  return Super.CheckReplacement( Other, bSuperRelevant );

}


/*
//// CONSIDER: we could add support for squeezing multiple delimiters into 1
// function array<String> SplitString(String str, String divider) {
// function int SplitString(String str, String divider, out array<String> parts) {
*/
function int SplitString(String str, String divider, out String parts[255]) {
  // local String parts[255];
  // local array<String> parts;
  local int i,nextSplit;
  i=0;
  while (true) {
    nextSplit = InStr(str,divider);
    if (nextSplit >= 0) {
      // parts.insert(i,1);
      parts[i] = Left(str,nextSplit);
      str = Mid(str,nextSplit+Len(divider));
      i++;
    } else {
      // parts.insert(i,1);
      parts[i] = str;
      i++;
      break;
    }
  }
  // return parts;
  return i;
}
