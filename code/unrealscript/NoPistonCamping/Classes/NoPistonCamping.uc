// A mod to prevent players from camping near teleporters with the impact hammer
// Original by Daniel Mastersourcerer at Kitana's Castle
// Adaptations by nogginBasher

class NoPistonCamping expands Mutator;

var config bool bDebugLogging;
var config int MaxSecondsAbusingPiston;
var config bool bOnlyByTeleporter;
var config float TeleporterAbuseDistance;
var config float PistonDamage;
var config float PistonBoost;
var config String BackfireMessage;

var float SecondsAbusingPiston[64];

function PostBeginPlay() {
  Super.PostBeginPlay();
  SetTimer(1, True);
}

event Timer() {
  CheckPlayers();
}

// Check players with impact hammer
function CheckPlayers() {
  local Pawn p;
  local ImpactHammer ih;
  local vector X, Y, Z;
  local Teleporter teleporterActor;
  local float dist;
  local bool bChargingPiston;
  local bool bCloseToTeleporter;
  local bool bAbusingPiston;
  local int arrayId;

  for (p = Level.PawnList; p != None; p = p.NextPawn) {
    if (p.IsA('PlayerPawn') && p.bIsPlayer && p.bIsHuman) {
      arrayId = p.PlayerReplicationInfo.PlayerID % 64;
      bChargingPiston = False;
      bCloseToTeleporter = False;

      // check if that player is holding an impact hammer
      if (p.Weapon != None && p.Weapon.IsA('ImpactHammer')) {
        ih = ImpactHammer(p.Weapon);
        if (bDebugLogging) {
          BroadcastMessage(p.getHumanName() $ " is holding piston with charge " $ ih.ChargeSize);
        }

        if (ih.ChargeSize > 4) {
          bChargingPiston = True;
        }

        // Just an optimization, we don't need to check for teleporters if they aren't charging their piston
        if (bChargingPiston) {
          foreach RadiusActors(class'Teleporter', teleporterActor, 500.0, p.location) {
            dist = VSize(teleporterActor.Location - p.location);
            if (bDebugLogging) {
              BroadcastMessage("Distance from teleporter: " $ dist);
            }
            if (dist < TeleporterAbuseDistance) {
              bCloseToTeleporter = True;
            }
          }
        }
      }

      bAbusingPiston = bChargingPiston && (bCloseToTeleporter || !bOnlyByTeleporter);
      if (bAbusingPiston) {
        SecondsAbusingPiston[arrayId] += 1.0;
      } else {
        // If we reset to 0 immediately, they could easily fool the system by moving out of the teleporter range for just one second.
        // So we cool down their abuse counter slowly.
        if (SecondsAbusingPiston[arrayId] > 0) {
          SecondsAbusingPiston[arrayId] -= 0.25;
        }
      }

      if (SecondsAbusingPiston[arrayId] >= MaxSecondsAbusingPiston) {
        GetAxes(p.ViewRotation, X, Y, Z);
        // Is this discharge likely to kill the player?
        if (p.Health <= PistonDamage && BackfireMessage != "") {
          BroadcastMessage(p.getHumanName() $ "'s " $ BackfireMessage);
        }
        p.TakeDamage(PistonDamage, p, vect(0,0,0), -PistonBoost * X, ih.MyDamageType);
        ih.ChargeSize = 0;
        // We could reset to 0, but why not give them trouble sooner?
        SecondsAbusingPiston[arrayId] = MaxSecondsAbusingPiston / 2;
      }
    }
  }
}

defaultproperties {
  bDebugLogging=False
  MaxSecondsAbusingPiston=4
  bOnlyByTeleporter=True
  TeleporterAbuseDistance=120
  //PistonDamage=36.0
  PistonDamage=56.0
  PistonBoost=120000.0
  BackfireMessage="piston discharged prematurely."
}
