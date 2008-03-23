//================================================================================
// nBAutoSpectateMMI.
//================================================================================

class nBAutoSpectateOnlineMMI extends UMenuModMenuItem;

var PlayerPawn P;
var nbAutoSpectate AS;

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
    AS = P.Spawn(Class'nbAutoSpectateOnline');
    AS.setpawn(P);
  }
  MenuItem.Owner.Root.EscClose();
}

function bool FindOldAS ()
{
  local nbAutoSpectateOnline OldAS;
  local bool bFound;

  bFound = False;
  foreach P.Level.AllActors(Class'nbAutoSpectateOnline',OldAS)
  {
    OldAS.Destroy();
    bFound = True;
  }
  return bFound;
}

