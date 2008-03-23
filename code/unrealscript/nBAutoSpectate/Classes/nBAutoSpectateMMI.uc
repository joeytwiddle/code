//================================================================================
// nBAutoSpectateMMI.
//================================================================================

class nBAutoSpectateMMI extends UMenuModMenuItem;

var PlayerPawn P;
var nBAutoSpectate AS;

function Execute ()
{
  local string IP;

  P = MenuItem.Owner.GetPlayerOwner();
  // if (  !P.PlayerReplicationInfo.bIsSpectator )
  // {
    // P.ClientMessage("nBAuto spectate can only be run in spectate mode");
    // MenuItem.Owner.Root.EscClose();
    // return;
  // }
  if ( FindOldAS() == True )
  {
    P.ClientMessage("nBAutoSpectate turned off");
  } else {
    P.ClientMessage("nBAutoSpectate by {pIn}Kev on");
    AS = P.Spawn(Class'nBAutoSpectate');
    AS.setpawn(P);
  }
  MenuItem.Owner.Root.EscClose();
}

function bool FindOldAS ()
{
  local nBAutoSpectate OldAS;
  local bool bFound;

  bFound = False;
  foreach P.Level.AllActors(Class'nBAutoSpectate',OldAS)
  {
    OldAS.Destroy();
    bFound = True;
  }
  return bFound;
}

