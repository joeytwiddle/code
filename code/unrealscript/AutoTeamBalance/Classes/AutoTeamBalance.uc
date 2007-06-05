//== AutoTeamBalance ==========================================================

// A mod that makes teams by player strength
// by Daniel Mastersourcerer at Kitana's Castle and nogginBasher
// (c)opyleft May 2007

// vim: tabstop=2 shiftwidth=2 expandtab

// At the moment we are doing mostly 

// The field delimeter for playerData in the config files is a space " " since that can't appear in UT nicks (always _)

// TODO: catch the end-of-game event and collect scores then (check Level.Game.bGameEnded)

// TODO: catch a player saying "!teams"

// Done now: i shouldn't be taking averages over time, but over #polls :S

// TODO when the playerData array gets full, old records are not recycled properly (atm the last is just overwritten repeatedly :| )

// CONSIDER: in cases of a standoff (e.g. all players are new and score UnknownStrength) choose something random!  What we are given may not be random enough (like bPlayersBalanceTeams).

// TODO: config option bRankBots

// Current rankings:
//
// At the moment we are polling the game every PollMinutes minutes, and updating the players according to their current in-game score.
// So we are basically measuring their average score during the whole (any,all) time they are on the server (playing CTF with 4+ players; TODO: and even teams?!).
//
// We are currently taking player snapshots about 6 times during each 15 minute game, and storing the average score (usually SmartCTF score, not default frags).
// This is NOT their average score at the end of the game, but their average score at "random" intervals during the game.
// This might seem unfair to players who join the server only for a few minutes, and get a low average score.  Screw them, they might leave halfway through the next game.  Regular players get rightly punished (and highly ranked!) for holding high scores during long games.  :)
//
// Thus recorded for each player is their average score over all the time they ever spent on the server (until they change nick or IP).
// Players will be forever punished/catching up if they started playing with low scores on the server.
// We could offer a MaxHoursPlayed or MaxPollsBeforeRecyclingStrength (or MaxGamesPlayed), after which their time_on_server does not increase, and their ranking becomes more oriented towards the players most recent scores (older scores get phased out).  E.g. with MaxPollsBeforeRecyclingStrength=99, new_score = ((old score * 99) + current_score) / 100 = .99*old_score + .01*current_score <-- This is still a very slow way of forgetting the past

// TODO: throughout the code i have referred to strength,avg_score,ranking,rating which are all the same thing.  Daniel stuck to "Strength" so maybe I should consolidate around that name.

//=============================================================================

class AutoTeamBalance expands Mutator config(AutoTeamBalance);

var string HelloBroadcast; // Don't want config; want to overwrite it

var config bool bAutoBalanceTeams;
// For updating player strength in-game:
var config bool bUpdatePlayerStats;
var config bool bUpdateStatsForCTFOnly;  // Stats were updating during other gametypes, which yield entirely different scores.  (Maybe stats for different gametypes should be handled separately.)  If your server runs only one team gametype, or gametypes with comparably scores, you can set this to False.
var config float PollMinutes;    // e.g. every 2.4 minutes, update the player stats from the current game
var config int MaxPollsBeforeRecyclingStrength;    // after this many polls, player's older scores are slowly phased out.  This feature is disabled by setting MaxPollsBeforeRecyclingStrength=0
var config int MinHumansForStats; // below this number of human players, stats will not be updated, i.e. current game scores will be ignored

var config bool bBroadcastStuff;   // Be noisy to in-game console
var config bool bDebugLogging;     // logs are more verbose/spammy than usual; recommended only for developers
var config bool bBroadcastCookies; // Silly way to debug; each players strength is spammed as their number of cookies
var config bool bOnlyMoreCookies;  // only broadcast a players cookies when they have recently increased

// Defaults (Daniel's):
var config int UnknownStrength;    // Default strength for unknown players
var config float UnknownMinutes;   // Initial virtual time spend on server by new players
var config int BotStrength;        // Default strength for bots
var config int FlagStrength;       // Strength modifier for captured flags
var config bool bClanWar;          // Make teams by clan tag
var config string clanTag;         // Clan tag of red team (all other players to blue)
// var config String RedTeam[16];     // Players on red team (unreferenced)
// var config String BlueTeam[16];    // Players on blue team (unreferenced)

// For storing player strength data:
var int MaxPlayerData; // The value 4096 is used in the following array declarations and the defaultproperties, but throughout the rest of the code, MaxPlayerData can be used to save duplication lol
var config String playerData[4096]; // String-format of the player data stored in the config (ini-file), including ip/nick/avg_score/time_played data

// Internal (parsed) player data:
var String ip[4096];
var String nick[4096];
var float avg_score[4096];
var float hours_played[4096];
// var int games_played[4096];
// TODO: var int date_last_played[4096];

// For local state caching:
var bool initialized;              // Mutator initialized flag
var bool gameStarted;              // Teams initialized flag

defaultproperties {
  HelloBroadcast="AutoTeamBalance (beta1) is attempting to balance the teams"
  bAutoBalanceTeams=True
  bUpdatePlayerStats=True
  bUpdateStatsForCTFOnly=True
  PollMinutes=2.4
  MaxPollsBeforeRecyclingStrength=100
  MinHumansForStats=1     // TODO: recommended 4
  bBroadcastStuff=True
  bDebugLogging=True      // TODO: recommended False
  bBroadcastCookies=True
  bOnlyMoreCookies=True
  UnknownStrength=40      // New player records start with an initial strength of avg_score 40
  UnknownMinutes=10       // New player records start with a virtual 10 minutes of time played already
  BotStrength=20
  FlagStrength=50         // If it's 3:0, the winning team will get punished an extra 150 points
  bClanWar=False
  MaxPlayerData=4096
  // bHidden=True // what is this?
}



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

   SetTimer(PollMinutes*60,True);
   // Call Timer() every PollMinutes.

  Log("AutoTeamBalance.PostBeginPlay(): Set Timer() for "$(PollMinutes*60)$" seconds.");
}

// Do something every tick
event Tick(float DeltaTime) {
  CheckGameStart();
}

// If a new player joins a game which has already started, this will send him to the most appropriate team (based on summed strength of each team, plus capbonuses).
function ModifyLogin(out class<playerpawn> SpawnClass, out string Portal, out string Options) {
  local int selectedTeam;
  local int teamSize[2];
  local int teamSizeWithBots[2];
  local int teamStr[2]; // each team's strength, only used if the #players on each team is equal
  local int teamnr;
  local String plname;
  local Pawn p;
  local TournamentGameReplicationInfo GRI;

  // (nogginBasher) as far as i can tell we don't actually have the pawn of the player we are moving
  //                we need his nick+ip to get his own strength.  Since we can't at the moment we just assume his strength is >0 and put him on the weaker team.

  if (NextMutator!= None) NextMutator.ModifyLogin(SpawnClass, Portal, Options);

  // check if this is a team game and if InitTeams has been passed
  // Done: don't we want to put this new player on the right team even if InitTeams has been passed?  so should be ignore gameStarted?  nooo, this check is that the game *has* started, because we don't need to switch the players when joining a new map, because InitTeams will do that.
  if (!bAutoBalanceTeams || !gameStarted || !Level.Game.IsA('TeamGamePlus')) return;

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

  // Check team balance of current players in game
  // Calculate sum of player strengths for each team (as well as the flagbonus above)
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
      teamnr=0; if (teamStr[0]>teamStr[1]) teamnr=1;
      Log("AutoTeamBalance.ModifyLogin(): "$teamSize[0]$"v"$teamSize[1]$" so sending new player to WEAKER team "$teamnr$".");
    } else {
      // send player to the team with fewer players
      teamnr=0; if (teamSize[0]>teamSize[1]) teamnr=1;
      Log("AutoTeamBalance.ModifyLogin(): "$teamSize[0]$"v"$teamSize[1]$" so sending new player to SMALLER team "$teamnr$".");
    }

  }

  // if selected team does not equal forced team then modify login
  if (teamnr!=selectedTeam) Options="?Team=" $ teamnr $ Options;

  // fix teamsize bug in Botpack.TeamGamePlus
  if (GRI.Teams[0].Size!=teamSizeWithBots[0] || GRI.Teams[1].Size!=teamSizeWithBots[1]) // this looks like a check that there are humans playing :o
  {
    Log("AutoTeamBalance.ModifyLogin(): Fixing team size (" $ GRI.Teams[0].Size $ "," $ GRI.Teams[1].Size $ ") should be (" $ teamSizeWithBots[0] $ "," $ teamSizeWithBots[1] $ ")");
    GRI.Teams[0].Size=teamSizeWithBots[0];
    GRI.Teams[1].Size=teamSizeWithBots[1];
  }
}

// nogginBasher TESTING hook: HandleEndGame() is a Mutator function called by GameInfo.EndGame().
function bool HandleEndGame() {
  Log("AutoTeamBalance.HandleEndGame() was CALLED!");
}
// Alternative, there is: event GameEnding() { ... } implemented in GameInfo.  Can we drop our own event catcher in here, without overriding the other?
// or ofc we can use a timer and check Level.Game.bGameEnded but the timer mustn't do this twice at the end of one game. :P



// ==== Daniel's other functions: ==== //

// Checks if the game has begun (CONSIDER/TEST: optimise this and move/inline the check right into Tick() above.  ATM it is called every tick, although exits fast.)
function CheckGameStart() {
  local int c,n,e;

  // only TeamGamePlus has the "countdown" variable and of course teams
  if (gameStarted || !bAutoBalanceTeams || !Level.Game.IsA('TeamGamePlus')) return;

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
    if (bBroadcastStuff) { Log("AutoTeamBalance.CheckGameStart() Broadcasting: "$HelloBroadcast); }
    if (bBroadcastStuff) { BroadcastMessage(HelloBroadcast); }
    InitTeams();
    gameStarted=True;
  }

  // Log("AutoTeamBalance.CheckGameStart(): Done.");  // Too noisy; gets called many times before the balancing+start

}

// Balance the teams just before the start of a new game.  No need for FlagStrength here.
function InitTeams() {
  local Pawn p;
  local int st;
  local int pid;
  local Pawn pl[64]; // list of pawns, with i = a hash of PlayerID
  local int ps[64]; // their strengths
  local int tg[64]; // their strengths, but they get removed during the player sorting/ranking
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
      // Worth noting, from GameInfo.uc:
      // // Set the player's ID.
      // NewPlayer.PlayerReplicationInfo.PlayerID = CurrentID++;
      pl[pid]=p;
      ps[pid]=st;
      tg[pid]=st;
      // p.PlayerReplicationInfo.PlayerName
      Log("AutoTeamBalance.InitTeams(): Player " $ p.getHumanName() $ " on team " $ p.PlayerReplicationInfo.Team $ " has ip+port " $ PlayerPawn(p).GetPlayerNetworkAddress() $ " and score " $ p.PlayerReplicationInfo.Score $ ".");
      if (bBroadcastCookies && !bOnlyMoreCookies) { Log("AutoTeamBalance.InitTeams() Broadcasting: " $ p.getHumanName() $ " has " $st$ " cookies."); }
      if (bBroadcastCookies && !bOnlyMoreCookies) { BroadcastMessage("" $ p.getHumanName() $ " has " $st$ " cookies."); }
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
      Log("AutoTeamBalance.InitTeams(): [Ranking] "$ps[pid]$" " $ ((pl[pid]).getHumanName()) $ "");
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
      // NOTE: here we are using Playername, in other places we've used getHumanName.
      if (Instr(Caps(pl[pid].PlayerReplicationInfo.Playername),Caps(clanTag))==-1) teamnr=1;
      Level.Game.ChangeTeam(pl[pid],teamnr);
      teamstr[teamnr]+=ps[pid];
    }

  } else {

    // Rebuild teams from strength order 1-2-2-1-1-2-2-1 ...
    // (On the way we also calculate total team strengths)
    teamstr[0]=0;
    teamstr[1]=0;
    for (i=0; i<(n&254); i++)
    {
      pid=plorder[i];
      teamnr=0;
      if ((i&3)==1 || (i&3)==2) teamnr=1;
      Log("AutoTeamBalance.InitTeams(): i="$i$" Putting pid="$pid$" pl="$pl[pid].getHumanName()$" into team "$teamnr$".");
      Level.Game.ChangeTeam(pl[pid],teamnr);
      teamstr[teamnr]+=ps[pid];
    }

    // if there is an odd number of players put the last player in the weaker team
    if ((n&1)==1)
    {
      pid=plorder[i];
      teamnr=0; if (teamstr[0]>teamstr[1]) teamnr=1;
      Log("AutoTeamBalance.InitTeams(): "$n$" is odd so sending last player to WEAKER team "$teamnr$".");
      Level.Game.ChangeTeam(pl[pid],teamnr);
      teamstr[teamnr]+=ps[pid];
    }

  }

  // restore team changing rules
  g.MaxTeamSize=oldMaxTeamSize;
  g.bPlayersBalanceTeams=oldbPlayersBalanceTeams;
  g.bNoTeamChanges=oldbNoTeamChanges;

  // Show team strengths to all players
  if (bBroadcastStuff) { BroadcastMessage("Red team strength is " $ teamstr[0] $ ".  Blue team strength is " $ teamstr[1] $ "."); }
  Log("AutoTeamBalance.InitTeams(): Red team strength is " $ teamstr[0] $ ".  Blue team strength is " $ teamstr[1] $ ".");

  // Little point doing this here; moved to timer.
  // CopyArraysIntoConfig();
  // SaveConfig();

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
    return avg_score[found]; // player's average score (or best estimate of player)
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

// guaranteed to return index i into playerData[] and ip[]/nick[]/... arrays, but not always an exact player match!
function int FindPlayerRecord(PlayerPawn p) {
  local int found;
  local int i;
  found = -1;
  for (i=0;i<MaxPlayerData;i++) {
    // Exact match! return the index immediately
    if (p.getHumanName() == nick[i] && stripPort(p.GetPlayerNetworkAddress()) == ip[i]) {
      found = i;
      Log("AutoTeamBalance.FindPlayerRecord(p) Exact match for " $nick[i]$ ","$ip[i]$": ["$found$"] ("$avg_score[i]$")");
      return found;
    } else {
      // Backups if we don't find the exact ip+nick
      if (stripPort(p.GetPlayerNetworkAddress()) == ip[i]) {
        found = i; // matching ip
        Log("AutoTeamBalance.FindPlayerRecord(p) IP match for " $p.getHumanName()$ ","$p.GetPlayerNetworkAddress()$": ["$found$"] "$nick[i]$" ("$avg_score[i]$")");
      }
      if (p.getHumanName() == nick[i] && found == -1) {
        Log("AutoTeamBalance.FindPlayerRecord(p) nick match for " $nick[i]$ ","$p.GetPlayerNetworkAddress()$": ["$found$"] "$ip[i]$" ("$avg_score[i]$")");
        found = i; // if not yet matching an ip, match the same nick on a different ip
      }
      // TODO: if an uneven match, choose a match with more experience
      // TODO: if we have little experience of a player, assume default score?
    }
  }
  if (found == -1) {
    found = CreateNewPlayerRecord(p);
  }
  return found;
}

// Currently new players override the one at the bottom of the list.  At the moment you are guaranteed a valid record after calling FindPlayerRecord() or CreateNewPlayerRecord().
function int CreateNewPlayerRecord(PlayerPawn p) {
  local int pos;
  // Find an empty slot:
  for (pos=0;pos<MaxPlayerData;pos++) {
    if (ip[pos] == "" && nick[pos] == "") {
      break;
    }
  } // TODO: If all full, could add somewhere randomly in the last 100 spots (rather than just 1 spot which keeps getting re-used)
  // or, find the oldest record and replace it
  ip[pos] = stripPort(p.GetPlayerNetworkAddress());
  nick[pos] = p.getHumanName();
  // initialise each player as having played for UnknownMinutes (e.g. 10 or 0.1) minutes already, and already earned an average UnknownStrength (e.g. 40) frags
  avg_score[pos] = UnknownStrength;
  hours_played[pos] = UnknownMinutes/60;
  Log("AutoTeamBalance.CreateNewPlayerRecord("$p$"): "$nick[pos]$" "$ip[pos]$" "$avg_score[pos]$" "$hours_played[pos]$".");
  if (bBroadcastCookies) { Log("AutoTeamBalance.CreateNewPlayerRecord() Broadcasting: Welcome "$nick[pos]$".  You have "$avg_score[pos]$" cookies."); }
  if (bBroadcastCookies) { BroadcastMessage("Welcome "$nick[pos]$".  You have "$avg_score[pos]$" cookies."); }
  // SaveConfig();
  return pos;
}

event Timer() { // this may be a reasonably hard work process; i hope it's been given it's own thread!
  // TESTING these counters; really i want to know how far after the end of the game we are
  local int c,n,e,l,t;
  if (bDebugLogging) {
    c = TeamGamePlus(Level.Game).countdown;
    n = TeamGamePlus(Level.Game).NetWait;
    e = TeamGamePlus(Level.Game).ElapsedTime;
    l = TeamGamePlus(Level.Game).TimeLimit;
    t = Level.TimeSeconds;
    Log("AutoTeamBalance.Timer() Ending   c="$c$" b="$n$" e="$e$" l="$l$" t="$t$" bGameEnded="$Level.Game.bGameEnded);
  }
  if (bUpdatePlayerStats) {
    // Stats were updating during a game of DM ffa, 3 players, low scores.  This gives very different scores than CTF games.
    // Presumably we have not checked that this is *really* a team-game we are getting stats from.
    // For now, have optionally limited stats to CTF games only:
    // TODO: could also analyze TDM (DeathMatchPlus) scores, but without the CTF bonuses, these will be much lower (store in separate fields? e.g. avg_TDM_score TDM_hours_played)  What about a method to separate all teamgames?  OR Easier: make a separate player with nick+" "+ip+" "+gameType hash
    if (Level.Game.IsA('CTFGame') || !bUpdateStatsForCTFOnly) {
      UpdateStatsFromCurrentGame();
    } else {
      Log("AutoTeamBalance.Timer(): not running since Level.Game "$Level.Game$" != CTFGame and config has bUpdatePlayerStats=True.");
    }
  }
  if (bDebugLogging) {
    c = TeamGamePlus(Level.Game).countdown;
    n = TeamGamePlus(Level.Game).NetWait;
    e = TeamGamePlus(Level.Game).ElapsedTime;
    l = TeamGamePlus(Level.Game).TimeLimit;
    t = Level.TimeSeconds;
    Log("AutoTeamBalance.Timer() Ending   c="$c$" b="$n$" e="$e$" l="$l$" t="$t$" bGameEnded="$Level.Game.bGameEnded);
  }
}

function UpdateStatsFromCurrentGame() {
  local int countHumanPlayers;

  local Pawn p;

  // Do not update stats for games with <MinHumansForStats human players.
  countHumanPlayers = 0;
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.bIsPlayer && !p.IsA('Spectator') && !p.IsA('Bot') && p.IsA('PlayerPawn') && p.bIsHuman) { // lol
      countHumanPlayers++;
    }
  }
  if (countHumanPlayers<MinHumansForStats) {
    Log("AutoTeamBalance.UpdateStatsFromCurrentGame(): not running since countHumanPlayers "$countHumanPlayers$" < "$MinHumansForStats$".");
    return;
  }

  // Update stats for all players in game
  if (bBroadcastStuff) { Log("AutoTeamBalance.Timer() Broadcasting: Updating stats now - Please report any lags"); }
  if (bBroadcastStuff) { BroadcastMessage("AutoTeamBalance.Timer(): Updating stats now - Please report any lags"); }
  // TODO: TEST: make lag here on purpose and see how bad we can get it / how we can fix it.
  Log("AutoTeamBalance.UpdateStatsFromCurrentGame(): updating stats");
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.bIsPlayer && !p.IsA('Spectator') && !p.IsA('Bot') && p.IsA('PlayerPawn') && p.bIsHuman) { // lol
      UpdateStatsForPlayer(PlayerPawn(p));
    }
  }

  // Save the new stats in the config/ini file:
  // TODO: Instead of doing the calculations and a config-file save mid-game, we could try to do this only at the end of each game:
  Log("AutoTeamBalance.UpdateStatsFromCurrentGame(): saving stats to file");
  CopyArraysIntoConfig();
  SaveConfig(); // This is the only place this gets done ATM!

  Log("AutoTeamBalance.UpdateStatsFromCurrentGame(): done");

}

function UpdateStatsForPlayer(PlayerPawn p) {
  local int i;
  local float current_score;
  local float new_hours_played;
  local int previousPolls;

  i = FindPlayerRecord(p);
  if (i == -1 || ip[i] != stripPort(p.GetPlayerNetworkAddress()) || nick[i] != p.getHumanName()) {
    // This is not an exact player match, so we should not update its stats
    // since we didn't find this actual ip+nick, we create a new entry
    // TODO CONSIDER: should we base the new players stats on the default (below), or copy over the estimated player's stats?
    i = CreateNewPlayerRecord(p); // OLD BUG FIXED: is it inefficient to repeatedly create a PlayerPawn from the same Pawn?
  }
  current_score = p.PlayerReplicationInfo.Score;
  new_hours_played = hours_played[i] + (PollMinutes / 60);
  previousPolls = hours_played[i] / (PollMinutes/60);
  if (MaxPollsBeforeRecyclingStrength>0 && previousPolls > MaxPollsBeforeRecyclingStrength) {
    previousPolls = MaxPollsBeforeRecyclingStrength - 1;
  }
  // Log("AutoTeamBalance.UpdateStatsForPlayer(p) ["$i$"] "$p.getHumanName()$" avg_score = ( ("$avg_score[i]$" * "$hours_played[i]$") + "$current_score$") / "$new_hours_played$"");
  // avg_score[i] = ( (avg_score[i] * hours_played[i]) + current_score) / new_hours_played;
  Log("AutoTeamBalance.UpdateStatsForPlayer(p) ["$i$"] "$p.getHumanName()$" avg_score = ( ("$avg_score[i]$" * "$previousPolls$") + "$current_score$") / "$(previousPolls+1)$"");
  avg_score[i] = ( (avg_score[i] * previousPolls) + current_score) / (previousPolls+1);
  hours_played[i] = new_hours_played;
  if (bBroadcastCookies && ((!bOnlyMoreCookies) || current_score>avg_score[i])) { Log("AutoTeamBalance.UpdateStatsForPlayer() Broadcasting: " $ p.getHumanName() $ " has " $Int(avg_score[i])$ " cookies!"); }
  if (bBroadcastCookies && ((!bOnlyMoreCookies) || current_score>avg_score[i])) { BroadcastMessage("" $ p.getHumanName() $ " has " $Int(avg_score[i])$ " cookies!"); }
}

// Takes everything before the first ":" - you should almost always use this when getting PlayerPawn.GetPlayerNetworkAddress(); at least in my experience the client's port number changed frequently.
function string stripPort(string ip_and_port) {
  return Left(ip_and_port,InStr(ip_and_port,":"));
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

