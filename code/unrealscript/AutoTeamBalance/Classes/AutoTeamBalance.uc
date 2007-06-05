//== AutoTeamBalance ==========================================================

// A mod that makes teams by player strength
// by Daniel Mastersourcerer at Kitana's Castle
// modified by nogginBasher May 2007

// vim: tabstop=2 shiftwidth=2 expandtab

//=============================================================================

class AutoTeamBalance expands Mutator config(AutoTeamBalance);

var bool initialized;              // Mutator initialized flag
var bool gameStarted;              // Teams initialized flag

var config int UnknownStrength;    // Default strength for unknown players
var config int BotStrength;        // Default strength for bots
var config int FlagStrength;       // Strength modifier for captured flags
var config bool bClanWar;          // Make teams by clan tag
var config string clanTag;         // Clan tag of red team (all other players to blue)
// var config String RedTeam[16];     // Players on red team (unreferenced)
// var config String BlueTeam[16];    // Players on blue team (unreferenced)

var int MaxPlayerData; // The value 4096 is used in the declaration and the defaultproperties, but throughout the rest of the code, MaxPlayerData can be used to save duplication lol

var config String playerData[4096]; // String-format of the player data stored in the config (ini-file), including ip/nick/avg_score/time_played data

var String ip[4096];
var String nick[4096];
var float avg_score[4096];
var int games_played[4096];
var float hours_played[4096];
// TODO: date_last_played

defaultproperties {
  bHidden=True
  bClanWar=False
  UnknownStrength=40
  BotStrength=20
  FlagStrength=50
  MaxPlayerData=4096
}

// At the moment we are doing mostly 

// The field delimeter for playerData in the config files is a space " " since that can't appear in UT nicks (always _)



// ==== Daniel's HOOKS / OVERRIDES ==== //

// Initialize the system
function PostBeginPlay() {
  if (initialized) return;

  Super.PostBeginPlay();

  // register as mutator
  // I was getting an infinite recursion error, so I removed this.
  // Maybe because I had already added the mutator
  // Maybe this was designed for ServerActor
  // Before uncommenting, consider moving the initialized=true; to the line before.
  // Level.Game.BaseMutator.AddMutator(Self);
  initialized=true;
  Log("AutoTeamBalance.PostBeginPlay(): Done.");  
}

// Do something every tick
event Tick(float DeltaTime) {
  CheckGameStart();
}

// Send new players to the correct team
function ModifyLogin(out class<playerpawn> SpawnClass, out string Portal, out string Options) {
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

  Log("AutoTeamBalance.ModifyLogin()");  

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
    Log("AutoTeamBalance.ModifyLogin(): Fixing team size (" $ GRI.Teams[0].Size $ "," $ GRI.Teams[1].Size $ ") should be (" $ teamSizeWithBots[0] $ "," $ teamSizeWithBots[1] $ ")");
    GRI.Teams[0].Size=teamSizeWithBots[0];
    GRI.Teams[1].Size=teamSizeWithBots[1];
  }
}



// ==== Daniel's other functions: ==== //

// Checks if the game has begun (CONSIDER/TEST: optimise this and move/inline the check right into Tick() above.  ATM it is called every tick, although exits fast.)
function CheckGameStart() {
  local int c,n,e;

  // only TeamGamePlus has the "countdown" variable and of course teams
  if (gameStarted || !Level.Game.IsA('TeamGamePlus')) return;

  // this mod works on team games only
  if (!Level.Game.GameReplicationInfo.bTeamGame) {
    gameStarted=True;
    return;
  }

  // read starting countdown
  c = TeamGamePlus(Level.Game).countdown;
  n = TeamGamePlus(Level.Game).NetWait;
  e = TeamGamePlus(Level.Game).ElapsedTime;
  c = Min(c,n-e);

  // initialize teams 1 second before game is starting
  if (c<2) {
    BroadcastMessage("AutoTeamBalance.alpha");
    InitTeams();
    gameStarted=True;
  }
  // Log("AutoTeamBalance.CheckGameStart(): Done.");  // Too noisy; gets called many times before the balancing+start
}

// Balance the teams
function InitTeams() {
  local Pawn p;
  local int st;
  local int pid;
  local Pawn pl[64]; // list of pawns, with i = a hash of PlayerID
  local int ps[64]; // their strengths
  local int tg[64]; // their ...
  local int plorder[32];
  local int i;
  local int n;
  local int mx;
  local int teamnr;
  local int teamstr[2];
  local TeamGamePlus g; // BUG TODO: ATM uncommenting this make ucc make crash without an error
  local int oldMaxTeamSize;
  local bool oldbPlayersBalanceTeams, oldbNoTeamChanges;

  CopyConfigIntoArrays();

  Log("AutoTeamBalance.InitTeams(): Running...");  

  // rate all players
  for (p=Level.PawnList; p!=None; p=p.NextPawn)
  {
    // ignore non-player pawns
    if (p.bIsPlayer && !p.IsA('Spectator'))
    {
      st=GetPawnStrength(p);
      pid=p.PlayerReplicationInfo.PlayerID % 64;
      // Why does Sourceror store the players by this pid hash (which might possibly collide)?  Why not just add the players to a list?
      pl[pid]=p;
      ps[pid]=st;
      tg[pid]=st;
      // BroadcastMessage("Player " $ p.PlayerReplicationInfo.PlayerName $ " on team " $ p.PlayerReplicationInfo.Team $ " has ip " $ PlayerPawn(p).GetPlayerNetworkAddress() $ " and score " $ p.PlayerReplicationInfo.Score $ ".");
      // BroadcastMessage("Player " $ p.getHumanName() $ " on team " $ p.PlayerReplicationInfo.Team $ " has ip " $ PlayerPawn(p).GetPlayerNetworkAddress() $ " and score " $ p.PlayerReplicationInfo.Score $ ".");
      Log("Player " $ p.getHumanName() $ " on team " $ p.PlayerReplicationInfo.Team $ " has ip " $ PlayerPawn(p).GetPlayerNetworkAddress() $ " and score " $ p.PlayerReplicationInfo.Score $ ".");
      BroadcastMessage("" $ p.getHumanName() $ " has " $st$ " cookies.");
    }
  }

  // sort players by strength
  n=0;
  do
  {
    pid=-1;
    mx=0;
    // find pid=i with max tg[i]
    for (i=0; i<64; i++)
    {
      if (tg[i]>mx)
      {
        pid=i;
        mx=tg[i];
      }
    }
    // If we found one, add him as the next player in the list
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

  CopyArraysIntoConfig();
  SaveConfig();

}

// Returns the strength of a player or a bot
function int GetPawnStrength(Pawn p) {
  local int st;

  if (p.IsA('PlayerPawn') && p.bIsHuman)
  {
    // a human player - get his strength
    st=GetPlayerStrength(PlayerPawn(p));
  } else {
    // a bot - use default strength
    st=BotStrength;
  }

  Log("AutoTeamBalance.GetPawnStrength(" $ p $ "): " $ st $ "");  

  return st;
}

// Returns the strength of a player
function int GetPlayerStrength(PlayerPawn p) {
  local int found;
  found = FindPlayerRecord(p);
  if (found == -1) {
    return UnknownStrength; // unknown player or player is too weak for list
  } else {
    return avg_score[found]; // player's average score, or best estimate of player
  }
  // return UnknownStrength;
}



// ==== nogginBasher's player records back-end: ==== //

// From playerData[] to ip[],nick[],avg_score[],... (should be done at the start)
function CopyConfigIntoArrays() {
  local int field;
  local int i;
  local String data;
  Log("AutoTeamBalance.CopyConfigIntoArrays() running");
  for (i=0; i<MaxPlayerData; i++) {
    data = playerData[i];
    if ( data == "" )
      continue;
    // I didn't try to work out the Divide() and Split() fns offered on http://udn.epicgames.com/Two/UnrealScriptReference.html
    field = InStr(data," ");
      ip[i] = Left(data,field);
    data = Mid(data,field+1);
    field = InStr(data," ");
      nick[i] = Left(data,field);
    data = Mid(data,field+1);
    field = InStr(data," ");
      avg_score[i] = Float(Left(data,field));
    data = Mid(data,field+1);
      hours_played[i] = Float(data);
  }
  Log("AutoTeamBalance.CopyConfigIntoArrays() done");
}

// From ip[],nick[],avg_score[],... to playerData[] (should be done before SaveConfig())
function CopyArraysIntoConfig() {
  local int i;
  Log("AutoTeamBalance.CopyArraysIntoConfig() running");
  for (i=0; i<MaxPlayerData; i++) {
     if ( ip[i] != "" ) {
       playerData[i] = ip[i] $ " " $ nick[i] $ " " $ avg_score[i] $ " " $ hours_played[i];
    }
  }
  Log("AutoTeamBalance.CopyArraysIntoConfig() done");
}

// Returns the strength of a player or a bot
function int FindPlayerRecord(PlayerPawn p) {
  local int found;
  local int i;
  found = -1;
  for (i=0;i<MaxPlayerData;i++) {
    // Exact match! return the index immediately
    if (p.getHumanName() == nick[i] && p.GetPlayerNetworkAddress() == ip[i]) {
      found = i;
      // Log("AutoTeamBalance.FindPlayerRecord(p) Exact match for " $nick$ ","$ip": " $ found);
      return found;
    } else {
      // Backups if we don't find the exact ip+nick
      if (p.GetPlayerNetworkAddress() == ip[i]) {
        found = i; // matching ip
        // Log("AutoTeamBalance.FindPlayerRecord(p) IP match for " $nick$ ","$ip": " $ found);
      }
      if (p.getHumanName() == nick[i] && found == -1) {
        // Log("AutoTeamBalance.FindPlayerRecord(p) nick match for " $nick$ ","$ip": " $ found);
        found = i; // if no matching ip yet, same nick on a different ip
      }
    }
    // Log("Player " $ p.getHumanName() $ " on team " $ p.PlayerReplicationInfo.Team $ " has ip " $ PlayerPawn(p).GetPlayerNetworkAddress() $ " and score " $ p.PlayerReplicationInfo.Score $ ".");
  }
  if (found == -1) {
    CreateNewPlayerRecord(p);
  }
  return found;
}

function int CreateNewPlayerRecord(PlayerPawn p) {
  local int pos;
  // Find an empty slot:
  for (pos=0;pos<MaxPlayerData;pos++) {
    if (ip[pos] == "" && nick[pos] == "") {
      break;
    }
  } // TODO: If all full, could add somewhere randomly in the last 100 spots (rather than just 1 spot which keeps getting re-used)
  // or, find the oldest record and replace it
  ip[pos] = p.GetPlayerNetworkAddress();
  nick[pos] = p.getHumanName();
  avg_score[pos] = 0;
  hours_played[pos] = 0;
  // SaveConfig();
  return pos;
}

// Code snippet from advwaad, taken for interest:
// for(P=level.pawnlist; P!=none ; P=P.nextpawn)
// if( P.IsA('TournamentPlayer') || P.IsA('Bot') )
// if( P.PlayerReplicationInfo.Team == 0 )
// Red++;
// else if( P.PlayerReplicationInfo.Team == 1 )
// Blue++;

// which team is bigger?
// local TournamentGameReplicationInfo TGRI;
// TGRI = TournamentGameReplicationInfo(Level.Game.GameReplicationInfo);
// if ( TGRI != none)
// {
// if (TGRI.Teams[0] > TGRI.Teams[1])
// //Do something.......Red Team > Blue Team
// if (TGRI.Teams[0] < TGRI.Teams[1])
// //Do something.......Blue Team > Red Team
// }

