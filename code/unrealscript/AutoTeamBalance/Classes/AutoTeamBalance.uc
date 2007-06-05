// A mod that makes teams by player strength
// by Daniel Mastersourcerer at Kitana's Castle
//=============================================================================
class XOLTeamBalance expands Mutator config(XOLTeams);

var bool initialized;              // Mutator initialized flag
var bool gameStarted;              // Teams initialized flag

var config int UnknownStrength;    // Default strength for unknown players
var config int BotStrength;        // Default strength for bots
var config int FlagStrength;       // Strength modifier for captured flags
var config bool bClanWar;          // Make teams by clan tag
var config string clanTag;         // Clan tag of red team (all other players to blue)
var config String RedTeam[16];     // Players on red team
var config String BlueTeam[16];    // Players on blue team



// Initialize the system
function PostBeginPlay()
{
  if (initialized) return;

  // register as mutator
  Level.Game.BaseMutator.AddMutator(Self);
  initialized=true;
}



// Do something every tick
event Tick(float DeltaTime)
{
  CheckGameStart();
}



// Checks if the game has begun
function CheckGameStart()
{
  local int c,n,e;

  // only TeamGamePlus has the "countdown" variable and of course teams
  if (gameStarted || !Level.Game.IsA('TeamGamePlus')) return;

  // this mod works on team games only
  if (!Level.Game.GameReplicationInfo.bTeamGame)
  {
    gameStarted=True;
    return;
  }

  // read starting countdown
  c=TeamGamePlus(Level.Game).countdown;
  n=TeamGamePlus(Level.Game).NetWait;
  e=TeamGamePlus(Level.Game).ElapsedTime;
  c=Min(c,n-e);

  // initialize teams 1 second before game is starting
  if (c<2)
  {
    InitTeams();
    gameStarted=True;
  }
}



// Balanced the teams
function InitTeams()
{
  local Pawn p;
  local int st;
  local int pid;
  local int ps[64];
  local int tg[64];
  local Pawn pl[64];
  local int plorder[32];
  local int i;
  local int n;
  local int mx;
  local int teamnr;
  local int teamstr[2];
  local TeamGamePlus g;
  local int oldMaxTeamSize;
  local bool oldbPlayersBalanceTeams, oldbNoTeamChanges;

  // rate all players
  for (p=Level.PawnList; p!=None; p=p.NextPawn)
  {
    // ignore non-player pawns
    if (p.bIsPlayer && !p.IsA('Spectator'))
    {
      st=GetPawnStrength(p);
      pid=p.PlayerReplicationInfo.PlayerID % 64;
      ps[pid]=st;
      tg[pid]=st;
      pl[pid]=p;
    }
  }

  // sort players by strength
  n=0;
  do
  {
    pid=-1;
    mx=0;
    for (i=0; i<64; i++)
    {
      if (tg[i]>mx)
      {
        pid=i;
        mx=tg[i];
      }
    }
    if (pid!=-1)
    {
      plorder[n]=pid;
      tg[pid]=0;
      n++;
    }
  } until (pid==-1);

  // save team changing rules to override them
  g=TeamGamePlus(Level.Game);
  oldMaxTeamSize=g.MaxTeamSize;
  oldbPlayersBalanceTeams=g.bPlayersBalanceTeams;
  oldbNoTeamChanges=g.bNoTeamChanges;

  // deactivate team changing rules
  g.MaxTeamSize=32;
  g.bPlayersBalanceTeams=False;
  g.bNoTeamChanges=False;

  if (bClanWar)
  {

    // rebuild teams by clan tags
    teamstr[0]=0;
    teamstr[1]=0;
    for (i=0; i<n; i++)
    {
      pid=plorder[i];
      teamnr=0;
      if (Instr(Caps(pl[pid].PlayerReplicationInfo.Playername),Caps(clanTag))==-1) teamnr=1;
      Level.Game.ChangeTeam(pl[pid],teamnr);
      teamstr[teamnr]+=ps[pid];
    }

  } else {

    // rebuild teams from strength order 1-2-2-1-1-2-2-1 ...
    teamstr[0]=0;
    teamstr[1]=0;
    for (i=0; i<(n&254); i++)
    {
      pid=plorder[i];
      teamnr=0;
      if ((i&3)==1 || (i&3)==2) teamnr=1;
      Level.Game.ChangeTeam(pl[pid],teamnr);
      teamstr[teamnr]+=ps[pid];
    }

    // if there is an odd number of players put the last player in the weaker team
    if ((n&1)==1)
    {
      pid=plorder[i];
      teamnr=0;
      if (teamstr[0]>teamstr[1]) teamnr=1;
      Level.Game.ChangeTeam(pl[pid],teamnr);
      teamstr[teamnr]+=ps[pid];
    }

  }

  // restore team changing rules
  g.MaxTeamSize=oldMaxTeamSize;
  g.bPlayersBalanceTeams=oldbPlayersBalanceTeams;
  g.bNoTeamChanges=oldbNoTeamChanges;

  // Show team strengths to all players
  BroadcastMessage("Red team strength is " $ teamstr[0] $ " , blue team strength is " $ teamstr[1]);
}



// Returns the strength of a player or a bot
function int GetPawnStrength(Pawn p)
{
  local int st;

  if (p.IsA('PlayerPawn') && p.bIsHuman)
  {
    // a human player - get his strength
    st=GetPlayerStrength(PlayerPawn(p));
  } else {
    // a bot - use default strength
    st=BotStrength;
  }

  return st;
}



// Returns the strength of a player
function int GetPlayerStrength(PlayerPawn p)
{
  // unknown player or player is too weak for list
  return UnknownStrength;
}



// Send new players to the correct team
function ModifyLogin(out class<playerpawn> SpawnClass, out string Portal, out string Options)
{
  local int selectedTeam;
  local int teamSize[2];
  local int teamSizeWithBots[2];
  local int teamStr[2];
  local int teamnr;
  local String plname;
  local Pawn p;
  local TournamentGameReplicationInfo GRI;

  if (NextMutator!= None) NextMutator.ModifyLogin(SpawnClass, Portal, Options);

  // check if this is a team game and if InitTeams has been passed
  if (!gameStarted || !Level.Game.IsA('TeamGamePlus')) return;

  // read this player's selected team
  selectedTeam=Level.Game.GetIntOption(Options,"Team",255);

  // get team scores
  GRI=TournamentGameReplicationInfo(Level.Game.GameReplicationInfo);
  teamStr[0]=GRI.Teams[0].Score*FlagStrength;
  teamStr[1]=GRI.Teams[1].Score*FlagStrength;
  teamSize[0]=0;
  teamSize[1]=0;
  teamSizeWithBots[0]=0;
  teamSizeWithBots[1]=0;

  // check team balance
  for (p=Level.PawnList; p!=None; p=p.NextPawn)
  {
    // ignore non-player pawns
    if (p.bIsPlayer && !p.IsA('Spectator'))
    {
      teamnr=p.PlayerReplicationInfo.Team;
      if (teamnr<2)
      {
        if (!p.IsA('Bot')) teamSize[teamnr]+=1;
        teamSizeWithBots[teamnr]+=1;
        teamStr[teamnr]+=GetPawnStrength(p);
      }
    }
  }

  if (bClanWar)
  {

    // send player to his clan's team
    teamnr=0;
    plname=Level.Game.ParseOption(Options,"Name");
    if (Instr(Caps(plname),Caps(clanTag))==-1) teamnr=1;

  } else {

    // if both teams have the same number of players send the new player to the weaker team
    if (teamSize[0]==teamSize[1])
    {
      teamnr=0;
      if (teamStr[0]>teamStr[1]) teamnr=1;
    } else {
      // send player to the team with fewer players
      teamnr=0;
      if (teamSize[0]>teamSize[1]) teamnr=1;
    }  

  }

  // if selected team does not equal forced team then modify login
  if (teamnr!=selectedTeam) Options="?Team=" $ teamnr $ Options;

  // fix teamsize bug in Botpack.TeamGamePlus
  if (GRI.Teams[0].Size!=teamSizeWithBots[0] || GRI.Teams[1].Size!=teamSizeWithBots[1])
  {
    Log("Fixing team size (" $ GRI.Teams[0].Size $ "," $ GRI.Teams[1].Size $ ") should be (" $ teamSizeWithBots[0] $ "," $ teamSizeWithBots[1] $ ")");
    GRI.Teams[0].Size=teamSizeWithBots[0];
    GRI.Teams[1].Size=teamSizeWithBots[1];
  }
}



defaultproperties
{
  bHidden=True
  bClanWar=False
  UnknownStrength=40
  BotStrength=20
  FlagStrength=50
}
