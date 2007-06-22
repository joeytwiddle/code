// TODO: hey look we can create new actors from only their class name! BaseMutator.AddMutator(Spawn(MutatorClass));
//       we can also use .Class to get the class of an actor

class TranslocBots expands Mutator;

var config bool bReplacePathNodes; // Will replace PathNode actors with FreshTranslocStart/Dest/FreshLiftCenter/Exits actors.
var config bool bForceBotsTransloc; // On a timer, attempts to force bots to translocate somewhere
var config bool bGoForFlag;
var config bool bLogging;

var bool bWorking;

var String PropertiesToCheck;

defaultproperties {
  bReplacePathNodes=False
  bForceBotsTransloc=True
  bGoForFlag=True
  bLogging=True
  bWorking=False
  PropertiesToCheck="From_NavigationPoint:,ownerTeam,taken,upstreamPaths,Paths,PrunedPaths,VisNoReachPaths,visitedWeight,routeCache,bestPathWeight,nextNavigationPoint,nextOrdered,prevOrdered,startPath,previousPath,cost,ExtraCost,bPlayerOnly,bEndPoint,bEndPointOnly,bSpecialCost,bOneWayPath,bNeverUseStrafing,bAutoBuilt,bTwoWay,From_Actor:,bStatic,bHidden,bNoDelete,bAnimFinished,bAnimLoop,bAnimNotify,bAnimByOwner,bDeleteMe,bAssimilated,bTicked,bLightChanged,bDynamicLight,bTimerLoop,bCanTeleport,bOwnerNoSee,bOnlyOwnerSee,bIsMover,bAlwaysRelevant,bAlwaysTick,bHighDetail,bStasis,bForceStasis,bIsPawn,bNetTemporary,bNetOptional,bReplicateInstigator,bTrailerSameRotation,bTrailerPrePivot,bClientAnim,bSimFall,Role,RemoteRole,NetTag,Owner,InitialState,Group,TimerRate,TimerCounter,LifeSpan,AnimSequence,AnimFrame,AnimRate,TweenRate,SkelAnim,LODBias,Level,XLevel,Tag,Event,Target,Instigator,AmbientSound,Inventory,Base,Region,AttachTag,StandingCount,MiscNumber,LatentByte,LatentInt,LatentFloat,LatentActor,Touching[4],Deleted,SpecialTag,Location,Rotation,OldLocation,ColLocation,Velocity,Acceleration,OddsOfAppearing,bHiddenEd,bDirectional,bSelected,bMemorized,bHighlighted,bEdLocked,bEdShouldSnap,bEdSnap,bTempEditor,bDifficulty0,bDifficulty1,bDifficulty2,bDifficulty3,bSinglePlayer,bNet,bNetSpecial,bScriptInitialized,HitActor,EDrawType,ERenderStyle,Sprite,Texture,Skin,Mesh,Brush,DrawScale,PrePivot,ScaleGlow,VisibilityRadius,VisibilityHeight,AmbientGlow,Fatness,SpriteProjForward,bUnlit,bNoSmooth,bParticles,bRandomFrame,bMeshEnviroMap,bMeshCurvy,bFilterByVolume,bShadowCast,bHurtEntry,bGameRelevant,bCarriedItem,bForcePhysicsUpdate,bIsSecretGoal,bIsKillGoal,bIsItemGoal,bCollideWhenPlacing,bTravel,bMovable,MultiSkins[8],SoundRadius,SoundVolume,SoundPitch,TransientSoundVolume,TransientSoundRadius,CollisionRadius,CollisionHeight,bCollideActors,bCollideWorld,bBlockActors,bBlockPlayers,bProjTarget,ELightType,ELightEffect,byte,bSpecialLit,bActorShadows,bCorona,bLensFlare,bBounce,bFixedRotationDir,bRotateToDesired,bInterpolating,bJustTeleported,EDodgeDir,Mass,Buoyancy,RotationRate,DesiredRotation,PhysAlpha,PhysRate,PendingTouch,AnimLast,AnimMinRate,OldAnimRate,SimAnim,NetPriority,NetUpdateFrequency,bNetInitial,bNetOwner,bNetRelevant,bNetSee,bNetHear,bNetFeel,bSimulatedPawn,bDemoRecording,bClientDemoRecording,bClientDemoNetFunc,RenderIteratorClass,RenderInterface"
}

var name currentTag;

var int depth; // used to prevent infinite recursion when replacing actors

var Actor lastPathNode;
var Actor RandomFoundNavigationPoint;

// function PreBeginPlay() {
function PostBeginPlay() {
  if (bForceBotsTransloc) SetTimer(2,True);
}

function MyLog(String msg) {
  if (bLogging) {
    Log(msg);
  }
}

event Timer() {
  local Pawn p;
  local Bot b;
  local Actor dest;
  local int method;
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
  // foreach AllActors(class'Bot', p) {
    if (!p.IsA('Bot')) {
      continue;
    }
    if (RandRange(0,100)>50) {
      continue;
    }
    b = Bot(p);
    if (b.PlayerReplicationInfo.HasFlag != None) {
      MyLog("TranslocBots."$p.getHumanName()$" xx Bot is holding a flag!");
      continue;
    }
    if (b.MyTranslocator == None) {
      MyLog("TranslocBots."$p.getHumanName()$" xx Bot has no translocator!");
      continue;
    }

    MyLog("");

    MyLog(p.getHumanName() $ " :: " $ p.GetStateName() $ " Target="$b.Target$" RoamTarget="$b.RoamTarget$" MoveTarget="$b.MoveTarget);

    if (b.MyTranslocator.TTarget != None) {
      MyLog("TranslocBots."$p.getHumanName()$" !! Bot already has a TTarget = " $ b.MyTranslocator.TTarget.DesiredTarget /* $ " (" $ b.MyTranslocator.TTarget.RealLocation $ ")" $ "," $ b.MyTranslocator.TTarget.Master */ );
      if (RandRange(0,100)<50 && b.MyTranslocator.TTarget.DesiredTarget != None) {
        continue;
      } else {
        b.MyTranslocator.TTarget.Destroy();
        b.MyTranslocator.TTarget = None;
        b.MyTranslocator.bTTargetOut = false;
      }
    }

    /*
    if (RandRange(0,100)<5) {
      // b.GoToState('RangedAttack');
    }
    if (RandRange(0,100)<5) {
      // b.GoToState('Roaming');
    }
    if (RandRange(0,100)<10) {
      // MyLog("TranslocBots."$p.getHumanName()$" ** NEW State Attacking");
      // b.GoToState('Attacking');
    }
    */

    /*
    // if (b.FindBestPathToward(b.Target,false)) {
      // if (b.LineOfSightTo(b.Destination)) {
        // MyLog("TranslocBots.Timer(): " $ b $ ".TranslocateToTarget(" $ b.Destination $ ")");
        // b.TranslocateToTarget(b.Destination);
      // }
    // }
    // MyLog("TranslocBots.Timer(): "$p$" Target="$b.Target$", MoveTarget="$b.MoveTarget$"");
    */

    /*
    dest = None;
    // dest = b.Target;
    // method = RandRange(0,3);
    // method = RandRange(0,2);
    // method = 1;
    if (b.Target==None && b.RoamTarget!=None && RandRange(0,1000)<10) {
      b.Target = b.RoamTarget;
      MyLog("TranslocBots."$p.getHumanName()$" ** NEW ROAM TARGET "$dest);
    }
    if (b.Target == None || RandRange(0,1000)<10) {
      // if (b.Target == None || (bGoForFlag && RandRange(1,100)<10)) {
        // b.Target = FindEnemyFlag(p);
        // MyLog("TranslocBots.Timer(): .. FindEnemyFlag returned " $ "p" $ ".Target = " $ b.Target);
      // }
      // if (b.Target == None) {
        // MyLog("TranslocBots.Timer(): xx p.Target == None");
        // continue;
      // }
      // dest = b.Target;
      dest = FindEnemyFlag(p);
      if (dest != None) {
        // MyLog("TranslocBots Method 1: FindEnemyFlag = "$dest);
        MyLog("TranslocBots."$p.getHumanName()$" ** NEW FLAG TARGET "$dest);
        b.Target = dest;
        // b.MoveTarget = dest;
      }
      dest = None;
    }
    // } else if (method == 1) {
      if (b.Target == None && RandRange(0,1000)<10) {
        b.Target = getRandomNavigationPoint();
        MyLog("TranslocBots."$p.getHumanName()$" ** NEW RANDOM TARGET "$b.Target);
      }
      // MyLog("TranslocBots.Timer(): .. p.Target = "$b.Target);
      // MyLog("TranslocBots.Timer(): .. p.MoveTarget = "$b.MoveTarget);
      // b.FindBestPathToward(b.Target,false);
      b.FindBestPathToward(b.Target,true);
      // MyLog("TranslocBots Method 2: FindBestPathToward = "$b.MoveTarget);
      // MyLog("TranslocBots BESTPATH "$b.MoveTarget);
      if (b.MoveTarget == None) {
        MyLog("TranslocBots."$p.getHumanName()$" xx FindBestPathToward("$b.Target$") = "$b.MoveTarget);
      } else {
        // MyLog("TranslocBots ** NEW MoveTarget "$b.MoveTarget);
        // MyLog("TranslocBots."$p.getHumanName()$" -- "$b.MoveTarget$" -> "$b.Target$"");
        // MyLog("TranslocBots."$p.getHumanName()$" -- "$b.MoveTarget$" -> "$b.Target$"");
        // // MyLog(b $ ".TranslocateToTarget(" $ b.MoveTarget $ ")");
        // // MyLog("TranslocBots.Timer(): >>> p.MoveTarget = "$b.MoveTarget);
        // MyLog("TranslocBots.Timer(): >> MoveTarget >> TranslocateToTarget(" $ b.MoveTarget $ ")");
        // b.TranslocateToTarget(b.MoveTarget);
        dest = b.MoveTarget;
      }
    // } else { // method == 2
    */
    /*
      foreach VisibleActors(class'Actor', dest) {
        if (dest.IsA('NavigationPoint')) {
          // MyLog("TranslocBots.Timer(): >> visible >> TranslocateToTarget(" $ dest $ ")");
          // b.TranslocateToTarget(dest);
          if (RandomFoundNavigationPoint == None || RandRange(0,1000)<5) {
            RandomFoundNavigationPoint = dest;
            // break;
          }
        }
      }
      dest = RandomFoundNavigationPoint;
      MyLog("TranslocBots Method 3: RandomFoundNavigationPoint = "$dest);
    }
    */
    /*
    // dest = b.FindPathToward(b.Target);
    if (dest == None) {
      // MyLog("TranslocBots.Timer(): XX dest = "$dest);
    } else {
      // MyLog("TranslocBots.Timer(): dest = "$dest);
      // MyLog(b $ ".TranslocateToTarget(" $ dest $ ")");
      // MyLog("TranslocBots."$p.getHumanName()$" >> TranslocateToTarget(" $ dest $ ")");
      MyLog("TranslocBots."$b.getHumanName()$" >> TranslocateToTarget(" $ dest $ ") ("$VSize(dest.Location-b.Location)$","$b.LineOfSightTo(dest)$")");
      b.TranslocateToTarget(dest);
      // b.Target = FindEnemyFlag(p);
    }
    */

    if (b.MoveTarget != None) {
      DoTranslocateToTarget(b,b.MoveTarget);
    } else
    if (b.RoamTarget != None) {
      DoTranslocateToTarget(b,b.RoamTarget);
    } else
    if (b.Target != None) {
      DoTranslocateToTarget(b,b.Target);
    }
  }
}

function bool DoTranslocateToTarget(Bot b, Actor Target) {
  MyLog("TranslocBots."$b.getHumanName()$" >> TranslocateToTarget(" $ Target $ ") ("$VSize(Target.Location-b.Location)$","$b.LineOfSightTo(Target)$")");
  b.TranslocateToTarget(Target);
}

function Actor getRandomNavigationPoint() {
  local Actor Other;
  foreach AllActors(class'Actor', Other) {
    if (Other.IsA('NavigationPoint') && (RandomFoundNavigationPoint == None || RandRange(0,1000)<5)) {
      RandomFoundNavigationPoint = Other;
    }
  }
  return RandomFoundNavigationPoint;
}

function Actor FindEnemyFlag(Pawn player) {
  local Actor Other;
  local int team;
  local Actor Found;
  team = player.PlayerReplicationInfo.Team;
  // team = RandRange(0,2);
  foreach AllActors(class'Actor', Other) {
    // MyLog("FindEnemyFlag("$player$"): "$Other);
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
    if (Other != None && Other.IsA('FlagBase')) {
      // MyLog("FindEnemyFlag(): Other = "$Other$" Other.Team="$FlagBase(Other).Team$" player.Team="$PlayerPawn(player).PlayerReplicationInfo.Team$"");
      // MyLog("TranslocBots.FindEnemyFlag(): .. Checking "$Other$" Other.Team="$FlagBase(Other).Team$" player.Team="$team);
      if (FlagBase(Other) != None && FlagBase(Other).Team != team) {
        if (Found == None || RandRange(0,10)<5) {
          Found = Other;
        }
      }
    // } else if (Other != None && Other.IsA('NavigationPoint') && RandRange(0,1000) < 5) {
      // // MyLog("FindEnemyFlag(): Picking up "$Other);
      // RandomFoundNavigationPoint = Other;
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
      MyLog( P.name $ "." $ properties[i] $ " = " $ result );
    }
  }

  // MyLog( "[TYPE]" $ P.class.name $ "[NAME]" $ P.name $ "[VALUE]" $ P $ "[bStatic]" $ P.GetPropertyText( "bStatic" ) );
  // foreach AllActors(class'Actor', P)
    // if(P.Outer == O.Class)
    // if(P.Owner == O.Class)
      // MyLog( "[TYPE]" @ P.class.name @ "[NAME]" @ P.name @ "[VALUE]" @ O.GetPropertyText( string(P.name) ) );
}

function CopyProperties( Object P, Object N ) {
  local String properties[255];
  local int numProperties,i;
  local String result;

  numProperties = SplitString(PropertiesToCheck, ",", properties);
  MyLog("TranslocBots: Copying "$numProperties$" properties from "$P$" to "$N);

  for (i=0;i<numProperties;i++) {
    result = P.GetPropertyText(properties[i]);
    if ( (""$result) != "None" ) {
      N.SetPropertyText(properties[i],result);
      // MyLog( N.Name $ "." $ properties[i] $ " = " $ P.Name $ "." $ properties[i] $ " (" $ result $ ")" );
      if (N.GetPropertyText(properties[i]) == result) {
        // MyLog("  succeeded.");
      }
    }
  }

  /*
  NavigationPoint(N).nextNavigationPoint = NavigationPoint(P).nextNavigationPoint;
  MyLog(N $ ".nextNavigationPoint = " $ NavigationPoint(P).nextNavigationPoint);
  NavigationPoint(N).nextOrdered = NavigationPoint(P).nextOrdered;
  MyLog(N $ ".nextOrdered = " $ NavigationPoint(P).nextOrdered);
  NavigationPoint(N).prevOrdered = NavigationPoint(P).prevOrdered;
  MyLog(N $ ".prevOrdered = " $ NavigationPoint(P).prevOrdered);
  NavigationPoint(N).startPath = NavigationPoint(P).startPath;
  MyLog(N $ ".startPath = " $ NavigationPoint(P).startPath);
  NavigationPoint(N).previousPath = NavigationPoint(P).previousPath;
  MyLog(N $ ".previousPath = " $ NavigationPoint(P).previousPath);
  */
  /*
  MyLog("CopyProperties("$P$","$N$")");
  if ( NavigationPoint(N).nextNavigationPoint != NavigationPoint(P).nextNavigationPoint ) {
    MyLog("!!! nextNavigationPoint !!! "$NavigationPoint(N).nextNavigationPoint$"/"$NavigationPoint(P).nextNavigationPoint$"");
  }
  if ( NavigationPoint(N).nextOrdered != NavigationPoint(P).nextOrdered ) {
    MyLog("!!! nextOrdered !!! "$NavigationPoint(N).nextOrdered$"/"$NavigationPoint(P).nextOrdered$"");
  }
  if ( NavigationPoint(N).prevOrdered != NavigationPoint(P).prevOrdered ) {
    MyLog("!!! prevOrdered !!! "$NavigationPoint(N).prevOrdered$"/"$NavigationPoint(P).prevOrdered$"");
  }
  if ( NavigationPoint(N).startPath != NavigationPoint(P).startPath ) {
    MyLog("!!! startPath !!! "$NavigationPoint(N).startPath$"/"$NavigationPoint(P).startPath$"");
  }
  if ( NavigationPoint(N).previousPath != NavigationPoint(P).previousPath ) {
    MyLog("!!! previousPath !!! "$NavigationPoint(N).previousPath$"/"$NavigationPoint(P).previousPath$"");
  }
  */

}


// Force game to always keep this actor, even if other mutators want to get rid of it.
function bool AlwaysKeep(Actor Other) {

  // MyLog("TranslocBots.AlwaysKeep("$Other$") called.");
  // MyLog("TranslocBots.AlwaysKeep("$Other$") called; currentTag="$currentTag$"");
  // MyLog("TranslocBots.AlwaysKeep("$Other$") called; currentTag="$currentTag$" Location="$Other.Location$"");
  // MyLog("TranslocBots.AlwaysKeep("$Other$") at "$Other.Location$"");
  // if ( (""$Other.Class)=="TranslocStart" || (""$Other.Class)=="TranslocDest" || (""$Other.Class)=="LiftCenter" ) {
  // }
  if (Other.IsA('LiftExit')) {
    // MyLog("TranslocBots.AlwaysKeep("$Other$"): LiftExit Tag = "$LiftExit(Other).LiftTag$"");
    if ((""$LiftExit(Other).LiftTag) == "None") {
      refreshCurrentTag();
      MyLog("= Setting "$Other$""$Other.Location$".LiftTag = "$currentTag);
      LiftExit(Other).LiftTag = currentTag;
      if (bReplacePathNodes) { CopyProperties(lastPathNode,Other); }
    } else {
      currentTag = LiftExit(Other).LiftTag;
      // MyLog(". Leaving "$Other$"("$currentTag$")");
    }
  }
  if (Other.IsA('LiftCenter')) {
    // MyLog("TranslocBots.AlwaysKeep("$Other$"): LiftCenter Tag = "$LiftCenter(Other).LiftTag$"");
    if ((""$LiftCenter(Other).LiftTag) == "None") {
      refreshCurrentTag();
      MyLog("= Setting "$Other$""$Other.Location$".LiftTag = "$currentTag);
      LiftCenter(Other).LiftTag = currentTag;
      if (bReplacePathNodes) { CopyProperties(lastPathNode,Other); }
    } else {
      currentTag = LiftCenter(Other).LiftTag;
      // MyLog(". Leaving "$Other$"("$currentTag$")");
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
    // MyLog("TranslocBots.refreshCurrentTag() NEW! SetPropertyText(\"currentTag\",\"" $ currentTag $ "\")");
  }
}

function MyReplaceWith(Actor Other,String str) {
  if (str=="None") {
    MyLog("["$depth$"] x skipping "$ Other $ " -> " $ str);
    return;
  }
  // MyLog("["$depth$"] > replacing "$ Other $ "" $ Other.Location $ " -> " $ str);
  // MyLog("["$depth$"] > replacing "$ Other $ "" $ Other.Location $ " -> " $ str);
  if ((""$Other.Class) != "PathNode") {
    MyLog("["$depth$"] > replacing "$ Other.Class $ "(" $ Other.Location $ ") -> " $ str);
  }
  depth++;
    // DumpProperties(Other);
    lastPathNode = Other;
    ReplaceWith(Other,str);
  depth--;
  if (!bWorking) {
    bWorking=True;
    MyLog("TranslocBots is attempting to create new transloc paths for bots on this map ("$Level$")");
  }
}

// Allow mutators to remove actors (by returning false), or replace them.
function bool CheckReplacement(Actor Other, out byte bSuperRelevant) {
  local int i;

  if (depth>0) {
    // MyLog("("$depth$") < checkreplacement ignoring " $ Other $ " ("$bSuperRelevant$")");
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
      MyLog("TranslocBots.CheckReplacement(): RandRange(1,4) returned "$i);
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
      str = Mid(str,nextSplit+1);
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
