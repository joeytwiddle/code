// TODO: hey look we can create new actors from only their class name! BaseMutator.AddMutator(Spawn(MutatorClass));
//       we can also use .Class to get the class of an actor

class TranslocBots expands Mutator;

var config bool bReplacePathNodes; // Will replace PathNode actors with FreshTranslocStart/Dest/FreshLiftCenter/Exits actors.
var config bool bForceBotsTransloc; // On a timer, attempts to force bots to translocate somewhere
var bool bGoForFlag;

var bool bWorking;

var String PropertiesToCheck;

defaultproperties {
  bReplacePathNodes=False
  bForceBotsTransloc=True
  bWorking=False
  bGoForFlag=True
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

event Timer() {
  local Pawn p;
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
    // if (PlayerPawn(p).PlayerReplicationInfo.HasFlag != None) {
      // Log("TranslocBots.Timer(): xx Bot "$p$" is holding a flag!");
      // continue;
    // }
    if (Bot(p).MyTranslocator == None) {
      Log("TranslocBots xx Bot "$p$" has no translocator!");
      continue;
    }
    if (Bot(p).MyTranslocator.TTarget != None) {
      // Log("TranslocBots.Timer(): !! Bot "$p$" already has a TTarget = " $ Bot(p).MyTranslocator.TTarget);
      Log("TranslocBots.Timer(): !! Bot "$p$" already has a TTarget = " $ Bot(p).MyTranslocator.TTarget.DesiredTarget $ "," $ Bot(p).MyTranslocator.TTarget.Master );
      if (RandRange(0,100)<90) {
        continue;
      }
      Bot(p).MyTranslocator.TTarget.Destroy();
      Bot(p).MyTranslocator.TTarget = None;
      Bot(p).MyTranslocator.bTTargetOut = false;
    }

    // if (Bot(p).FindBestPathToward(Bot(p).Target,false)) {
      // if (Bot(p).LineOfSightTo(Bot(p).Destination)) {
        // Log("TranslocBots.Timer(): " $ Bot(p) $ ".TranslocateToTarget(" $ Bot(p).Destination $ ")");
        // Bot(p).TranslocateToTarget(Bot(p).Destination);
      // }
    // }
    // Log("TranslocBots.Timer(): "$p$" Target="$Bot(p).Target$", MoveTarget="$Bot(p).MoveTarget$"");

    dest = None;
    // dest = Bot(p).Target;
    // method = RandRange(0,3);
    // method = RandRange(0,2);
    // method = 1;
    if (Bot(p).Target == None || RandRange(0,100)<25) {
      // if (Bot(p).Target == None || (bGoForFlag && RandRange(1,100)<10)) {
        // Bot(p).Target = FindEnemyFlag(p);
        // Log("TranslocBots.Timer(): .. FindEnemyFlag returned " $ "p" $ ".Target = " $ Bot(p).Target);
      // }
      // if (Bot(p).Target == None) {
        // Log("TranslocBots.Timer(): xx p.Target == None");
        // continue;
      // }
      // dest = Bot(p).Target;
      dest = FindEnemyFlag(p);
      if (dest != None) {
        // Log("TranslocBots Method 1: FindEnemyFlag = "$dest);
        Log("TranslocBots ** NEW TARGET1 "$dest);
        Bot(p).Target = dest;
        // Bot(p).MoveTarget = dest;
      }
      dest = None;
    }
    // } else if (method == 1) {
      if (Bot(p).Target == None || RandRange(0,100)<5) {
        Bot(p).Target = getRandomNavigationPoint();
        Log("TranslocBots ** NEW TARGET2 "$Bot(p).Target);
      }
      // Log("TranslocBots.Timer(): .. p.Target = "$Bot(p).Target);
      // Log("TranslocBots.Timer(): .. p.MoveTarget = "$Bot(p).MoveTarget);
      // Bot(p).FindBestPathToward(Bot(p).Target,false);
      Bot(p).FindBestPathToward(Bot(p).Target,true);
      // Log("TranslocBots Method 2: FindBestPathToward = "$Bot(p).MoveTarget);
      // Log("TranslocBots BESTPATH "$Bot(p).MoveTarget);
      if (Bot(p).MoveTarget == None) {
        Log("TranslocBots xx FindBestPathToward("$Bot(p).Target$") = "$Bot(p).MoveTarget);
      } else {
        // Log("TranslocBots ** NEW MoveTarget "$Bot(p).MoveTarget);
        Log("TranslocBots -- "$Bot(p).MoveTarget$" -> "$Bot(p).Target$"");
        // // Log(Bot(p) $ ".TranslocateToTarget(" $ Bot(p).MoveTarget $ ")");
        // // Log("TranslocBots.Timer(): >>> p.MoveTarget = "$Bot(p).MoveTarget);
        // Log("TranslocBots.Timer(): >> MoveTarget >> TranslocateToTarget(" $ Bot(p).MoveTarget $ ")");
        // Bot(p).TranslocateToTarget(Bot(p).MoveTarget);
        dest = Bot(p).MoveTarget;
      }
    // } else { // method == 2
    /*
      foreach VisibleActors(class'Actor', dest) {
        if (dest.IsA('NavigationPoint')) {
          // Log("TranslocBots.Timer(): >> visible >> TranslocateToTarget(" $ dest $ ")");
          // Bot(p).TranslocateToTarget(dest);
          if (RandomFoundNavigationPoint == None || RandRange(0,1000)<5) {
            RandomFoundNavigationPoint = dest;
            // break;
          }
        }
      }
      dest = RandomFoundNavigationPoint;
      Log("TranslocBots Method 3: RandomFoundNavigationPoint = "$dest);
    }
    */
    // dest = Bot(p).FindPathToward(Bot(p).Target);
    if (dest == None) {
      // Log("TranslocBots.Timer(): XX dest = "$dest);
    } else {
      // Log("TranslocBots.Timer(): dest = "$dest);
      // Log(Bot(p) $ ".TranslocateToTarget(" $ dest $ ")");
      Log("TranslocBots >> TranslocateToTarget(" $ dest $ ")");
      Bot(p).TranslocateToTarget(dest);
    }
  }
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
  team = PlayerPawn(player).PlayerReplicationInfo.Team;
  foreach AllActors(class'Actor', Other) {
    // Log("FindEnemyFlag("$player$"): "$Other);
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
      // Log("FindEnemyFlag(): Other = "$Other$" Other.Team="$FlagBase(Other).Team$" player.Team="$PlayerPawn(player).PlayerReplicationInfo.Team$"");
      // Log("TranslocBots.FindEnemyFlag(): .. Checking "$Other$" Other.Team="$FlagBase(Other).Team$" player.Team="$team);
      if (FlagBase(Other) != None && FlagBase(Other).Team != team)
        return Other;
    } else if (Other != None && Other.IsA('NavigationPoint') && RandRange(0,1000) < 5) {
      // Log("FindEnemyFlag(): Picking up "$Other);
      RandomFoundNavigationPoint = Other;
    }
  }
  // return None;
  return RandomFoundNavigationPoint;
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
      Log( P.name $ "." $ properties[i] $ " = " $ result );
    }
  }

  // Log( "[TYPE]" $ P.class.name $ "[NAME]" $ P.name $ "[VALUE]" $ P $ "[bStatic]" $ P.GetPropertyText( "bStatic" ) );
  // foreach AllActors(class'Actor', P)
    // if(P.Outer == O.Class)
    // if(P.Owner == O.Class)
      // Log( "[TYPE]" @ P.class.name @ "[NAME]" @ P.name @ "[VALUE]" @ O.GetPropertyText( string(P.name) ) );
}

function CopyProperties( Object P, Object N ) {
  local String properties[255];
  local int numProperties,i;
  local String result;

  numProperties = SplitString(PropertiesToCheck, ",", properties);
  Log("TranslocBots: Copying "$numProperties$" properties from "$P$" to "$N);

  for (i=0;i<numProperties;i++) {
    result = P.GetPropertyText(properties[i]);
    if ( (""$result) != "None" ) {
      N.SetPropertyText(properties[i],result);
      // Log( N.Name $ "." $ properties[i] $ " = " $ P.Name $ "." $ properties[i] $ " (" $ result $ ")" );
      if (N.GetPropertyText(properties[i]) == result) {
        // Log("  succeeded.");
      }
    }
  }

  /*
  NavigationPoint(N).nextNavigationPoint = NavigationPoint(P).nextNavigationPoint;
  Log(N $ ".nextNavigationPoint = " $ NavigationPoint(P).nextNavigationPoint);
  NavigationPoint(N).nextOrdered = NavigationPoint(P).nextOrdered;
  Log(N $ ".nextOrdered = " $ NavigationPoint(P).nextOrdered);
  NavigationPoint(N).prevOrdered = NavigationPoint(P).prevOrdered;
  Log(N $ ".prevOrdered = " $ NavigationPoint(P).prevOrdered);
  NavigationPoint(N).startPath = NavigationPoint(P).startPath;
  Log(N $ ".startPath = " $ NavigationPoint(P).startPath);
  NavigationPoint(N).previousPath = NavigationPoint(P).previousPath;
  Log(N $ ".previousPath = " $ NavigationPoint(P).previousPath);
  */
  /*
  Log("CopyProperties("$P$","$N$")");
  if ( NavigationPoint(N).nextNavigationPoint != NavigationPoint(P).nextNavigationPoint ) {
    Log("!!! nextNavigationPoint !!! "$NavigationPoint(N).nextNavigationPoint$"/"$NavigationPoint(P).nextNavigationPoint$"");
  }
  if ( NavigationPoint(N).nextOrdered != NavigationPoint(P).nextOrdered ) {
    Log("!!! nextOrdered !!! "$NavigationPoint(N).nextOrdered$"/"$NavigationPoint(P).nextOrdered$"");
  }
  if ( NavigationPoint(N).prevOrdered != NavigationPoint(P).prevOrdered ) {
    Log("!!! prevOrdered !!! "$NavigationPoint(N).prevOrdered$"/"$NavigationPoint(P).prevOrdered$"");
  }
  if ( NavigationPoint(N).startPath != NavigationPoint(P).startPath ) {
    Log("!!! startPath !!! "$NavigationPoint(N).startPath$"/"$NavigationPoint(P).startPath$"");
  }
  if ( NavigationPoint(N).previousPath != NavigationPoint(P).previousPath ) {
    Log("!!! previousPath !!! "$NavigationPoint(N).previousPath$"/"$NavigationPoint(P).previousPath$"");
  }
  */

}


// Force game to always keep this actor, even if other mutators want to get rid of it.
function bool AlwaysKeep(Actor Other) {

  // Log("TranslocBots.AlwaysKeep("$Other$") called.");
  // Log("TranslocBots.AlwaysKeep("$Other$") called; currentTag="$currentTag$"");
  // Log("TranslocBots.AlwaysKeep("$Other$") called; currentTag="$currentTag$" Location="$Other.Location$"");
  // Log("TranslocBots.AlwaysKeep("$Other$") at "$Other.Location$"");
  // if ( (""$Other.Class)=="TranslocStart" || (""$Other.Class)=="TranslocDest" || (""$Other.Class)=="LiftCenter" ) {
  // }
  if (Other.IsA('LiftExit')) {
    // Log("TranslocBots.AlwaysKeep("$Other$"): LiftExit Tag = "$LiftExit(Other).LiftTag$"");
    if ((""$LiftExit(Other).LiftTag) == "None") {
      refreshCurrentTag();
      Log("= Setting "$Other$""$Other.Location$".LiftTag = "$currentTag);
      LiftExit(Other).LiftTag = currentTag;
      if (bReplacePathNodes) { CopyProperties(lastPathNode,Other); }
    } else {
      currentTag = LiftExit(Other).LiftTag;
      // Log(". Leaving "$Other$"("$currentTag$")");
    }
  }
  if (Other.IsA('LiftCenter')) {
    // Log("TranslocBots.AlwaysKeep("$Other$"): LiftCenter Tag = "$LiftCenter(Other).LiftTag$"");
    if ((""$LiftCenter(Other).LiftTag) == "None") {
      refreshCurrentTag();
      Log("= Setting "$Other$""$Other.Location$".LiftTag = "$currentTag);
      LiftCenter(Other).LiftTag = currentTag;
      if (bReplacePathNodes) { CopyProperties(lastPathNode,Other); }
    } else {
      currentTag = LiftCenter(Other).LiftTag;
      // Log(". Leaving "$Other$"("$currentTag$")");
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
    // Log("TranslocBots.refreshCurrentTag() NEW! SetPropertyText(\"currentTag\",\"" $ currentTag $ "\")");
  }
}

function MyReplaceWith(Actor Other,String str) {
  if (str=="None") {
    Log("["$depth$"] x skipping "$ Other $ " -> " $ str);
    return;
  }
  // Log("["$depth$"] > replacing "$ Other $ "" $ Other.Location $ " -> " $ str);
  // Log("["$depth$"] > replacing "$ Other $ "" $ Other.Location $ " -> " $ str);
  if ((""$Other.Class) != "PathNode") {
    Log("["$depth$"] > replacing "$ Other.Class $ "(" $ Other.Location $ ") -> " $ str);
  }
  depth++;
    // DumpProperties(Other);
    lastPathNode = Other;
    ReplaceWith(Other,str);
  depth--;
  if (!bWorking) {
    bWorking=True;
    Log("TranslocBots is attempting to create new transloc paths for bots on this map ("$Level$")");
  }
}

// Allow mutators to remove actors (by returning false), or replace them.
function bool CheckReplacement(Actor Other, out byte bSuperRelevant) {
  local int i;

  if (depth>0) {
    // Log("("$depth$") < checkreplacement ignoring " $ Other $ " ("$bSuperRelevant$")");
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
      Log("TranslocBots.CheckReplacement(): RandRange(1,4) returned "$i);
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
