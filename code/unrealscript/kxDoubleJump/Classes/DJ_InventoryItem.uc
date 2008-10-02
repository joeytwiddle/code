//================================================================================
// DJ_InventoryItem.
//================================================================================

class DJ_InventoryItem extends TournamentPickup;

var int nofJumps;
var int maxJumps;
var float jumpHeight;
var string KeyNames[15];
var int nofJumpKeys;
var int jumpType;

replication
{
  reliable if ( Role == 4 )
    ClientPlayAnim;
  reliable if ( Role < 4 )
    DoubleJump;
}

simulated function PreBeginPlay ()
{
  local PlayerPawn P;

  if ( Level.NetMode == 1 )
  {
    return;
  }
  foreach AllActors(Class'PlayerPawn',P)
  {
    if ( Viewport(P.Player) != None ) {
      break;
    } else {
      P = None;
    }
  }
  if ( P == None )
  {
    Log("DJ: NO LOCAL PLAYERPAWN !!!");
    return;
  }
  InitializeKeys(P);
  if ( nofJumpKeys == 0 )
  {
    Log("DJ: Could NOT find any keys bound to JUMP/DOUBLEJUMP, DoubleJump will NOT work !!!");
  } else {
    Log("DJ: Keys that were changed by DoubleJumpUT can be restored by typing \"restorejumpkeys\" in the UT console when DoubleJumpUT is running.");
  }
}

simulated function InitializeKeys (PlayerPawn P)
{
  local string KeyName;
  local string keyBinding;
  local int i;

  nofJumpKeys = 0;
  for (i=0;i<256;i++) {
    KeyName = P.ConsoleCommand("Keyname" @ string(i));
    if ( (InStr(P.ConsoleCommand("Keybinding" @ KeyName),"Jump") != -1) && (InStr(P.ConsoleCommand("Keybinding" @ KeyName),"DoubleJump") == -1) )
    {
      keyBinding = P.ConsoleCommand("Keybinding" @ KeyName);
      P.ConsoleCommand("SET INPUT" @ KeyName @ "DoubleJump|" $ keyBinding);
      Log("DJ: Changed" @ KeyName @ "from" @ keyBinding @ "to DoubleJump|" $ keyBinding);
      nofJumpKeys++;
    } else {
      if ( InStr(P.ConsoleCommand("Keybinding" @ KeyName),"DoubleJump") != -1 )
      {
        nofJumpKeys++;
      }
    }
  }
}

simulated exec function RestoreJumpKeys ()
{
  local int i;
  local int restoredKeys;
  local int FoundAt;
  local int StripStart;
  local string KeyName;
  local string keyString;
  local string tempKeyString;
  local Actor P;

  Log("DJ: -- RestoreJumpKeys --");
  if ( Owner == None )
  {
    return;
  }
  restoredKeys = 0;
  for (i=0;i<256;i++) {
    KeyName = Owner.ConsoleCommand("Keyname" @ string(i));
    keyString = Owner.ConsoleCommand("Keybinding" @ KeyName);
    if ( InStr(keyString,"DoubleJump|") != -1 )
    {
      FoundAt = InStr(keyString,"DoubleJump|");
      StripStart = FoundAt + Len("DoubleJump|");
      tempKeyString = "";
      if ( FoundAt != 0 )
      {
        tempKeyString = Left(keyString,FoundAt);
      }
      Owner.ConsoleCommand("SET INPUT" @ KeyName @ tempKeyString $ Right(keyString,Len(keyString) - FoundAt + Len("DoubleJump|")));
      Log("DJ: Changed key " $ KeyName $ " from " $ keyString $ " to " $ tempKeyString $ Right(keyString,Len(keyString) - FoundAt + Len("DoubleJump|")));
      restoredKeys++;
    } else {
      if ( InStr(keyString,"DoubleJump |") != -1 )
      {
        FoundAt = InStr(keyString,"DoubleJump |");
        StripStart = FoundAt + Len("DoubleJump |");
        tempKeyString = "";
        if ( FoundAt != 0 )
        {
          tempKeyString = Left(keyString,FoundAt);
        }
        Owner.ConsoleCommand("SET INPUT" @ KeyName @ tempKeyString $ Right(keyString,Len(keyString) - FoundAt + Len("DoubleJump |")));
        Log("DJ: Changed key " $ KeyName $ " from " $ keyString $ " to " $ tempKeyString $ Right(keyString,Len(keyString) - FoundAt + Len("DoubleJump |")));
        restoredKeys++;
      }
    }
  }
  if ( restoredKeys == 0 )
  {
    Log("DJ: No keys could be restored.");
    PlayerPawn(Owner).ClientMessage("DoubleJumpUT: No keys could be restored.");
  } else {
    Log("DJ: Jump keys are restored.");
    PlayerPawn(Owner).ClientMessage("DoubleJumpUT: Jump keys are restored.");
  }
}

exec function DoubleJump ()
{
  local PlayerPawn P;
  local float forwardness,rightness;
  local Rotator right;
  local name newMesh;

  if ( PlayerPawn(Owner) == None )
  {
    return;
  }
  P = PlayerPawn(Owner);
  if ( P.Physics == 2 )
  {
    if ( nofJumps < maxJumps )
    {
      switch (jumpType)
      {
        case 0:
        if (  !(P.Physics == 2) && (Abs(P.Velocity.Z) < 100.0) )
        {
          return;
        }
        break;
        case 1:
        if (  !(P.Physics == 2) && (P.Velocity.Z > -100.0) )
        {
          return;
        }
        break;
        default:
      }
      P.Velocity.Z = P.Velocity.Z + 0.5 * P.JumpZ * jumpHeight;
      if (P.Velocity.Z < P.JumpZ * jumpHeight) {
        P.Velocity.Z = P.JumpZ * jumpHeight;
      }
      nofJumps++;
      P.PlaySound(P.JumpSound,SLOT_Interface,1.5,True,1200.0,1.0);
      if ( (Level.Game != None) && (Level.Game.Difficulty > 0) ) {
        P.MakeNoise(0.1 * Level.Game.Difficulty);
      }
      if ( nofJumps == maxJumps ) {
        newMesh = '';
        forwardness = Normal(P.Velocity) Dot Normal(Vector(P.Rotation));
        right = P.Rotation;
        right.Yaw += 8192;
        rightness = Normal(P.Velocity) Dot Normal(Vector(right));
        if ( VSize(P.Velocity)<50.0 && P.HasAnim('Flip')) {
          newMesh = 'Flip';
        } else if ( rightness>Abs(forwardness) && P.HasAnim('ROLLRIGHT') ) {
          newMesh = 'ROLLRIGHT';
        } else if ( rightness>Abs(forwardness) && P.HasAnim('DodgeL') ) {
          newMesh = 'DodgeL';
        } else if ( rightness < (-Abs(forwardness)) && P.HasAnim('ROLLLEFT') ) {
          newMesh = 'ROLLLEFT';
        } else if ( rightness < (-Abs(forwardness)) && P.HasAnim('DodgeR') ) {
          newMesh = 'DodgeR';
        } else if ( forwardness < (-Abs(rightness)) && P.HasAnim('DodgeB') ) {
          newMesh = 'DodgeB';
        } else if ( P.HasAnim('Flip') ) { /*forwardness>Abs(rightness) &&*/
          newMesh = 'Flip';
        }
        if (newMesh != '' && P.HasAnim(newMesh)) {
          P.PlayAnim(newMesh,1.352 * FMax(0.34999999,Region.Zone.ZoneGravity.Z / Region.Zone.Default.ZoneGravity.Z),0.06);
          if ( Level.NetMode != 0 ) {
            ClientPlayAnim(newMesh,1.352 * FMax(0.34999999,Region.Zone.ZoneGravity.Z / Region.Zone.Default.ZoneGravity.Z),0.06);
          }
        }
      }
    }
  } else {
    P.DoJump();
    nofJumps = 1;
  }
}

simulated function ClientPlayAnim (name Sequence, optional float Rate, optional float TweenTime) {
  Owner.PlayAnim(Sequence,Rate,TweenTime);
}

defaultproperties {
    nofJumps=1
    ExpireMessage="ExpireMessage"
    PickupMessage="PickupMessage"
    ItemName="DoubleJumpUT"
    Mesh=LodMesh'Botpack.jboot'
    AmbientGlow=0
}

