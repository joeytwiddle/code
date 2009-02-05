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
  if ( !P.PlayerReplicationInfo.bIsSpectator ) {
    //// I get this message when playing a demo in-eyes.
    // P.ClientMessage("Warning: nBAutoSpectate running in-eyes mode may cause camera trouble!");
    //// TODO: Maybe we should set AS.bInEyes=True;
    // MenuItem.Owner.Root.EscClose();
    // return;
  }
  if ( FindOldAS() == True )
  {
    P.ClientMessage("nBAutoSpectate turned off");
  } else {
    P.ClientMessage("nBAutoSpectate on - by {pIn}Kev + f0x|noggieB");
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

// Does not work
exec function yo() {
	P.ClientMessage("yo");
}

