//== AutoTeamBalance ==========================================================

// A mutator that makes fair teams at the beginning of each teamgame, by recording the relative strengths of players on the server (linked to their nick/ip).
// It also attempts to put a player joining the game on an appropriate team.
// by Daniel Mastersourcerer at Kitana's Castle and nogginBasher
// (c)opyleft May 2007 under GNU Public Licence

// vim: tabstop=2 shiftwidth=2 expandtab

// The field delimeter for playerData in the config files is a space " " since that can't appear in UT nicks (always _)

// TODO: don't do any even-ing of teams if bTournament=True; also don't do any scanning (strength updating) either?

// TODO: catch the end-of-game event and collect scores then (check Level.Game.bGameEnded)

// TODO when the playerData array gets full, old records are not recycled properly (atm the last is just overwritten repeatedly :| )

// TODO: catch a player saying "!teams", maybe write some custom code to balance the teams then (by swapping 1/2 players only, maybe slightly randomised so it can be repeated if unsatisfactory; noo that could get too spammy :E)

// Done now: i shouldn't be taking averages over time, but over #polls :S

// CONSIDER: in cases of a standoff (e.g. none of the players are found in the DB so all have UnknownStrength), choose something random!  What we are given may not be random enough (like bPlayersBalanceTeams).

// TODO: config option bRankBots (might be interesting to see how Visse compares to the humans ^^ )

// Current rankings method:
// We wait until the end of the game, then we update the stats for each player.
// Hence we collect each player's average endgame score.

// OLD rankings method:
//
// At the moment we are polling the game every PollMinutes minutes, and updating the players according to their current in-game score.
// So we are basically measuring their average score during the whole (any,all) time they are on the server (playing CTF with 4+ players; and TODO: even teams?!).
//
// We are currently taking player snapshots about 6 times during each 15 minute game, and storing the average score (usually SmartCTF score, not default frags).
// This is NOT their average score at the end of the game, but their average score at "random" intervals during the game.
// This might seem unfair to players who join the server only for a few minutes, and get a low average score.  Screw them, they might leave halfway through the next game.  Regular players get rightly punished (and highly ranked!) for holding high scores during long games.  :)
//
// Thus recorded for each player is their average score over all the time they ever spent on the server (until they change nick or IP).
// Players will be forever punished/catching up if they started playing with low scores on the server.
// We could offer a MaxHoursPlayed or MaxPollsBeforeRecyclingStrength (or MaxGamesPlayed), after which their time_on_server does not increase, and their ranking becomes more oriented towards the players most recent scores (older scores get phased out).  E.g. with MaxPollsBeforeRecyclingStrength=99, new_score = ((old score * 99) + current_score) / 100 = .99*old_score + .01*current_score <-- This is still a very slow way of forgetting the past

// TODO: throughout the code and comments i have referred to strength,avg_score,ranking,rating,stats but often meant the same thing.  Daniel stuck to "Strength" so maybe I should consolidate around that name.

//=============================================================================

class AutoTeamBalance expands Mutator config(AutoTeamBalance);

var string HelloBroadcast; // Don't want config; want to overwrite it

var config bool bAutoBalanceTeamsForCTF;
var config bool bAutoBalanceTeamsForTDM;
var config bool bAutoBalanceTeamsForOtherTeamGames;
// var config string BalanceTeamsForGameTypes; // TESTING_List_desired_gametypes
// For updating player strength in-game:
var config bool bUpdatePlayerStatsForCTF;
var config bool bUpdatePlayerStatsForTDM;
var config bool bUpdatePlayerStatsForOtherTeamGames;
var config bool bUpdatePlayerStatsForNonTeamGames;
// var config string UpdateStatsForGameTypes; // TESTING_List_desired_gametypes
// var config bool bUpdateStatsForCTFOnly;  // Stats were updating during other gametypes, which yield entirely different scores.  (Maybe stats for different gametypes should be handled separately.)  If your server runs only one team gametype, or gametypes with comparably scores, you can set this to False.
//// These didn't work for me; maybe config vars can't be complex types like "name"
// var config name OnlyBalanceTeamsIfGametypeIsA; // Defaults to 'TeamGamePlus' so it will try to balance teams for all team games.
// var config name OnlyUpdateStatsIfGametypeIsA;  // Stats were updating during other gametypes than CTF, which yield entirely different scores.  (Maybe stats for different gametypes should be handled separately.)  You can set this to your own server's favourite gametype, or to 'TeamGamePlus' if you only host one gametype, or player scores are comparable across all your gametypes.
// TODO: var config bool bUpdateStatsAtGameEndOnly;
// var config float PollMinutes;    // e.g. every 2.4 minutes, update the player stats from the current game
var config int MaxPollsBeforeRecyclingStrength;    // after this many polls, player's older scores are slowly phased out.  This feature is disabled by setting MaxPollsBeforeRecyclingStrength=0
var config int MinHumansForStats; // below this number of human players, stats will not be updated, i.e. current game scores will be ignored

var config bool bBroadcastStuff;   // Be noisy to in-game console
var config bool bDebugLogging;     // logs are more verbose/spammy than usual; recommended only for developers
var config bool bBroadcastCookies; // Silly way to debug; each players strength is spammed as their number of cookies
var config bool bOnlyMoreCookies;  // only broadcast a players cookies when they have recently increased

// Defaults (Daniel's):
var config int UnknownStrength;    // Default strength for unknown players
// var config float UnknownMinutes;   // Initial virtual time spend on server by new players
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
// TODO: var int date_last_played[4096]; // would be good for recycling; otherwise recycle on lowest hours_played i guess, although if the server/playerData lasts 1billion years, it might be hard for the current generation of players to get into the ranking

// For local state caching (not repeating when called by Tick's or Timer's):
var bool initialized;              // Mutator initialized flag
var bool gameStartDone;            // Teams initialized flag (we never initialise this to False, but I guess Unreal does that for us)
var bool gameEndDone;

var int timeGameStarted;

defaultproperties {
  HelloBroadcast="AutoTeamBalance (beta) is attempting to balance the teams"
  bAutoBalanceTeamsForCTF=True
  bAutoBalanceTeamsForTDM=True
  bAutoBalanceTeamsForOtherTeamGames=True
  // BalanceTeamsForGameTypes="CTFGame,TeamGamePlus,JailBreak,*"
  bUpdatePlayerStatsForCTF=True
  bUpdatePlayerStatsForTDM=False
  bUpdatePlayerStatsForOtherTeamGames=False
  bUpdatePlayerStatsForNonTeamGames=False
  // UpdateStatsForGameTypes="CTFGame"
  // bUpdateStatsForCTFOnly=True
  // OnlyUpdateStatsIfGametypeIsA='CTFGame'
  // OnlyBalanceTeamsIfGametypeIsA='TeamGamePlus'
  // PollMinutes=2.4
  MaxPollsBeforeRecyclingStrength=200 // I think for a returning player with a previous average of 100(!), and a new skill of around 50, and with 24 polls an hour and MaxPollsBeforeRecyclingStrength=100, after 100 more polls (4 more hours), the player's new average will look like 60.5.  That seems too quick for me, so I've gone for 200.  ^^  btw this maths is wrong :| but approx i guess
  MinHumansForStats=1     // TODO: recommended 4
  bBroadcastStuff=True
  bDebugLogging=True      // TODO: recommended False
  bBroadcastCookies=True
  bOnlyMoreCookies=True
  UnknownStrength=40      // New player records start with an initial strength of avg_score 40
  // UnknownMinutes=10       // New player records start with a virtual 10 minutes of time played already
  BotStrength=20
  FlagStrength=50         // If it's 3:0, the winning team will get punished an extra 150 points
  bClanWar=False
  MaxPlayerData=4096
  // bHidden=True // what is this?  iDeFiX says it's only needed for ServerActors
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
  initialized = true;
  gameEndDone = false;

  timeGameStarted = Level.TimeSeconds;

  // Call Timer() every PollMinutes.
  // SetTimer(PollMinutes*60,True);
  SetTimer(60,True); // Now checking once a minute to see if game has ended

  Level.Game.RegisterMessageMutator( Self ); // TESTING Matt's MutatorBroadcastMessage hook below

  // Log("AutoTeamBalance.PostBeginPlay(): Set Timer() for "$(PollMinutes*60)$" seconds.");
  Log("AutoTeamBalance.PostBeginPlay(): Set Timer() for 60 seconds.");
}

// Do something every tick
// TODO: Determine whether this check every Tickrate is more or less efficient than using SetTimer() and Timer(), then merge the two CheckGameStart() and UpdateStatsAtEndOfGame() into the more efficient method.
//       We may not need to check more often than once every -2 + 270 seconds.
//       If TickRate 20 means 20 calls to Tick per second, presumably Timer() is (an Engine (non UScript) event? and) more efficient.
//       Ahhh but maybe Daniel used Tick() because it's before game start, too early to use a timer.  To look into...
event Tick(float DeltaTime) {
  if (!gameStartDone) CheckGameStart();
}

function bool ShouldBalance(GameInfo game) {

  //// TESTING_List_desired_gametypes
  // local string[20] gametypes;
  // local int n,i;

  // if (bDebugLogging) { Log("AutoTeamBalance.ShouldBalance("$game$") Game.Name="$Game.Name$" Game.Class="$Game.Class$""); }
  // Never balance in tournament mode
  if (DeathMatchPlus(Level.Game).bTournament)
    return False;
  // We can't balance if it's not a teamgame
  if (!Level.Game.GameReplicationInfo.bTeamGame)
    return False;

  // We only balance CTF games if asked
  // if (Level.Game.Name == 'CTFGame')
  if (String(Level.Game.Class) == "Botpack.CTFGame")
    return bAutoBalanceTeamsForCTF;
  // We only balance TDM games if asked (NOTE: we don't use IsA here, because other teamgames might be a subclass of TeamGamePlus)
  if (String(Level.Game.Class) == "Botpack.TeamGamePlus")
    return bAutoBalanceTeamsForTDM;

  //// TESTING_List_desired_gametypes
  // n = Split(BalanceTeamsForGameTypes,",",gametypes);
  // for (i=0;i<n;i++) {
    // if (gametypes[i] == "*")
      // return True;
    // if (gametypes[i] == String(Level.Game.Name))
      // return True;
  // }
  // if (bDebugLogging) { Log("AutoTeamBalance.ShouldBalance("$game$"): Did not match any of the specified gametypes: "$BalanceTeamsForGameTypes); }

  // OK so it's an unknown teamgame
  if (bDebugLogging) { Log("AutoTeamBalance.ShouldBalance("$game$") Game.Name="$Game.Name$" Game.Class="$Game.Class$" returning "$bAutoBalanceTeamsForOtherTeamGames); }
  return bAutoBalanceTeamsForOtherTeamGames;
}

function bool ShouldUpdateStats(GameInfo game) {
  if (bDebugLogging) { Log("AutoTeamBalance.ShouldUpdateStats("$game$") Game.Name="$Game.Name$" Game.Class="$Game.Class$""); }
  // Never balance in tournament mode
  // if (DeathMatchPlus(Level.Game).bTournament)
    // return False;
  // We only build stats for CTF games if asked
  if (String(Level.Game.Class) == "Botpack.CTFGame")
    return bUpdatePlayerStatsForCTF;
  // We only build stats for TDM games if asked (NOTE: we don't use IsA here, because other teamgames might be a subclass of TeamGamePlus)
  if (String(Level.Game.Class) == "Botpack.TeamGamePlus")
    return bUpdatePlayerStatsForTDM;
  // OK so it's not CTF or TDM, but is it another type of team game?
  if (Level.Game.GameReplicationInfo.bTeamGame)
    return bUpdatePlayerStatsForOtherTeamGames;
  // It's not a team game.  Build stats because it's a non teamgame?  (For admins more interested in player stats than balancing teams.)
  return bUpdatePlayerStatsForNonTeamGames;
}

// If a new player joins a game which has already started, this will send him to the most appropriate ("weaker") team (based on summed strength of each team, plus capbonuses).
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
  // Done: don't we want to put this new player on the right team even if InitTeams has been passed?  so should be ignore gameStartDone?  nooo, this check is that the game *has* started, because we don't need to switch the players when joining a new map, because InitTeams will do that.
  if (!gameStartDone) return;

  if (!ShouldBalance(Level.Game)) return;

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
  if (GRI.Teams[0].Size!=teamSizeWithBots[0] || GRI.Teams[1].Size!=teamSizeWithBots[1])
  {
    Log("AutoTeamBalance.ModifyLogin(): Fixing team size (" $ GRI.Teams[0].Size $ "," $ GRI.Teams[1].Size $ ") should be (" $ teamSizeWithBots[0] $ "," $ teamSizeWithBots[1] $ ")");
    GRI.Teams[0].Size=teamSizeWithBots[0];
    GRI.Teams[1].Size=teamSizeWithBots[1];
  }
  // nogginBasher: I don't understand this bit, but maybe it's because we modified the player's login
  // I was thinking of stripping teamSizeWithBots because i have adwvaad on my server,
  // but since it isn't used to make the decision of which team to join, it's fine to leave it in for this bugfix.
  // Mmm when someone joins the server mid-game, the bot scores are used to balance the teams
  // With advwaad, if there is a bot, he'll be on the smaller team, so i guess it's fine the player will take the bots place
  // If there isn't a bot, the new player will go on the weaker team, and adwvaad will add a new bot to the stronger team
}

// nogginBasher TESTING hook: HandleEndGame() is a Mutator function called by GameInfo.EndGame().
// OK well it did get called! :)
// BUT later mutators in the list have the right to force the game into overtime (we should pass the call onto them here I think), so it may not actually BE the end of the game!
// Maybe better just to wait with a timer until bGameEnded == True.
// Or start that timer here? ^^ (so it doesn't need to check during the game)
// Hmm from tests I found this function gets called twice for overtime games, but both times bGameEnded=False
function bool HandleEndGame() {
  local bool b;
  if (bDebugLogging) { Log("AutoTeamBalance.HandleEndGame() was CALLED!  bOverTime="$Level.Game.bOvertime$" bGameEnded="$Level.Game.bGameEnded); }
  if ( NextMutator != None ) {
    b = NextMutator.HandleEndGame();
    if (bDebugLogging) { Log("AutoTeamBalance.HandleEndGame() NextMutator returned "$b$"  bOverTime="$Level.Game.bOvertime$" bGameEnded="$Level.Game.bGameEnded); }
    return b;
  }
  return false;
}

// TESTING Alternative, there is: event GameEnding() { ... } implemented in GameInfo.  Can we drop our own event catcher in here, without overriding the other?
// or ofc we can use a timer and check Level.Game.bGameEnded but the timer mustn't do this twice at the end of one game. :P
// From testing this: it never gets called!
event GameEnding() {
  if (bDebugLogging) { Log("AutoTeamBalance.GameEnding() even was CALLED!  bOverTime="$Level.Game.bOvertime$" bGameEnded="$Level.Game.bGameEnded); }
}



// ==== Daniel's other functions: ==== //

// Checks if the game has begun (CONSIDER/TEST: optimise this and move/inline the check right into Tick() above.  ATM it is called every tick, although exits fast.)
function CheckGameStart() {
  local int c,n,e;

  // if (gameStartDone) return; // This was moved up to Tick()

  // this mod works on team games only
  // only TeamGamePlus has the "countdown" variable and of course teams
  // we now check this in ShouldBalance
  // if (!bAutoBalanceTeams || !Level.Game.IsA(OnlyBalanceTeamsIfGametypeIsA) || !Level.Game.GameReplicationInfo.bTeamGame || DeathMatchPlus(Level.Game).bTournament) {
  if (!ShouldBalance(Level.Game)) {
    gameStartDone=True;
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
    gameStartDone=True;
  }

  // Log("AutoTeamBalance.CheckGameStart(): Done.");  // Too noisy; gets called many times before the balancing+start

}

function CheckGameEnd() {
  if (gameEndDone) return;
  if (Level.Game.bGameEnded) {
    gameEndDone = true;
    if (ShouldUpdateStats(Level.Game)) {
      UpdateStatsAtEndOfGame();
    }
  }
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

  CopyConfigIntoArrays();  // First time the data is needed, we must convert it.

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
      // So I guess hashing with modulus 64 might not be so dangerous.  :)
      pl[pid]=p;
      ps[pid]=st;
      tg[pid]=st;
      // p.PlayerReplicationInfo.PlayerName
      Log("AutoTeamBalance.InitTeams(): Player " $ p.getHumanName() $ " on team " $ p.PlayerReplicationInfo.Team $ " has ip+port " $ PlayerPawn(p).GetPlayerNetworkAddress() $ " and score " $ p.PlayerReplicationInfo.Score $ ".");
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
  Log("AutoTeamBalance.InitTeams(): Red team strength is " $ teamstr[0] $ ".  Blue team strength is " $ teamstr[1] $ ".");
  if (bBroadcastStuff) { BroadcastMessage("Red team strength is " $ teamstr[0] $ ".  Blue team strength is " $ teamstr[1] $ "."); }

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



/*
// Old Timer which was updating stats during the game
event Timer() { // this may be a reasonably hard work process; i hope it's been given it's own thread!
  // TESTING these counters; really i want to know how far after the end of the game we are
  local int c,n,e,l,t,s;
  if (bDebugLogging) {
    c = TeamGamePlus(Level.Game).countdown;
    n = TeamGamePlus(Level.Game).NetWait;
    e = TeamGamePlus(Level.Game).ElapsedTime;
    l = TeamGamePlus(Level.Game).TimeLimit;
    t = Level.TimeSeconds;
    s = Level.StartTime;
    Log("AutoTeamBalance.Timer() DEBUG Ending   c="$c$" b="$n$" e="$e$" l="$l$" t="$t$" s="$s$" bGameEnded="$Level.Game.bGameEnded);
  }
  // if (bUpdatePlayerStats) {
    // Stats were updating during a game of DM ffa, 3 players, low scores.  This gives very different scores than CTF games.
    // Presumably we have not checked that this is *really* a team-game we are getting stats from.
    // For now, have optionally limited stats to CTF games only:
    // TODO: could also analyze TDM (DeathMatchPlus) scores, but without the CTF bonuses, these will be much lower (store in separate fields? e.g. avg_TDM_score TDM_hours_played)  What about a method to separate all teamgames?  OR Easier: make a separate player with nick+" "+ip+" "+gameType hash
    // if (Level.Game.IsA('CTFGame') || !bUpdateStatsForCTFOnly) {
    // if (Level.Game.IsA(OnlyUpdateStatsIfGametypeIsA)) {
    if (ShouldUpdateStats(Level.Game)) {
      UpdateStatsAtEndOfGame();
    } else {
      // Log("AutoTeamBalance.Timer(): not running UpdateStatsAtEndOfGame() since Level.Game = "$Level.Game$" != "$OnlyUpdateStatsIfGametypeIsA$".");
      Log("AutoTeamBalance.Timer(): Refusing to run UpdateStatsAtEndOfGame().");
    }
  // }
}
*/

// New Timer which is just looking for the end of the game
event Timer() {
  CheckGameEnd();
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
  avg_score[pos] = 0; // UnknownStrength;
  hours_played[pos] = 0; // UnknownMinutes/60;
  Log("AutoTeamBalance.CreateNewPlayerRecord("$p$"): "$nick[pos]$" "$ip[pos]$" "$avg_score[pos]$" "$hours_played[pos]$".");
  // if (bBroadcastCookies) { BroadcastMessage("Welcome "$nick[pos]$".  You have "$avg_score[pos]$" cookies."); }
  // SaveConfig();
  return pos;
}

function UpdateStatsAtEndOfGame() {
  local int countHumanPlayers;
  local Pawn p;

  // Do not update stats for games with <MinHumansForStats human players.
  countHumanPlayers = 0;
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.bIsPlayer && !p.IsA('Spectator') && !p.IsA('Bot') && p.IsA('PlayerPawn') && p.bIsHuman) { // maybe the last 2 are not needed
      countHumanPlayers++;
    }
  }
  if (countHumanPlayers<MinHumansForStats) {
    Log("AutoTeamBalance.UpdateStatsAtEndOfGame(): not updating stats since countHumanPlayers "$countHumanPlayers$" < "$MinHumansForStats$".");
    return;
  }

  // Update stats for all players in game
  Log("AutoTeamBalance.UpdateStatsAtEndOfGame(): Updating player stats now.");
  if (bBroadcastStuff) { BroadcastMessage("AutoTeamBalance is updating player stats now."); }
  // TEST considered when stats were being updated mid-game: make lag here on purpose and see how bad we can get it / how we can fix it.
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.bIsPlayer && !p.IsA('Spectator') && !p.IsA('Bot') && p.IsA('PlayerPawn') && p.bIsHuman) { // lol
      UpdateStatsForPlayer(PlayerPawn(p));
    }
  }

  // Save the new stats in the config/ini file:
  Log("AutoTeamBalance.UpdateStatsAtEndOfGame(): saving stats to file");
  CopyArraysIntoConfig();
  SaveConfig(); // This is the only place this gets done atm!

  Log("AutoTeamBalance.UpdateStatsAtEndOfGame(): done");

}

function UpdateStatsForPlayer(PlayerPawn p) {
  local int i;
  local float current_score;
  local float new_hours_played;
  local int previousPolls;
  local int gameDurationSeconds;

  i = FindPlayerRecord(p);
  if (i == -1 || ip[i] != stripPort(p.GetPlayerNetworkAddress()) || nick[i] != p.getHumanName()) {
    // This is not an exact player match, so we should not update its stats
    // since we didn't find this actual ip+nick, we create a new entry
    // TODO CONSIDER: should we base the new players stats on the default (below), or copy over the estimated player's stats?  (But maybe reset their time_on_server.)
    i = CreateNewPlayerRecord(p); // OLD BUG FIXED: is it inefficient to repeatedly create a PlayerPawn from the same Pawn?
  }
  current_score = p.PlayerReplicationInfo.Score;
  // Ideally we would like to check how long this player has been on the server TODO i don't know how to get that yet ^^ I'm hoping it's somewhere in the code otherwise I have to remember the times that players joined
  // For the moment, assume all players were on server the whole game:
  gameDurationSeconds = Level.TimeSeconds - timeGameStarted;
  new_hours_played = hours_played[i] + (gameDurationSeconds / 60);
  previousPolls = hours_played[i] / 4;  // This is approx #times we've updated this player's stats before.  It's just used to measure the significance of their current score relative to the number of scores we've seen before from this player.
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

// MORE TESTING:

// Matt's method of catch player joined / left game events:
// Mutator broadcast message is called when the server broadcasts out
// UT will look for this function, and then you can do whatever you want
// After your stuff is done, then it has to pass on the message to
// the next mutator in line, so that it can then do it's stuff too
function bool MutatorBroadcastMessage( Actor Sender, Pawn Receiver, out coerce string Msg, optional bool bBeep, out optional name Type ) {

  CheckGameEnd(); // Does no harm to do this twice.  The broadcast from mapvote might make the stats parsing come sooner than waiting for the timer.

  if (bDebugLogging) { // TESTING I want to see if we can detect a player saying "!teams" this way... Answer: no! For that we need a MessagingSpectator
    Log("AutoTeamBalance.MutatorBroadcastMessage(\""$Msg$"\") was called.");
  }

  // Failed Test: Nope that was never broadcast :P
  // if ( InStr(Msg,"game has ended.")>=0 ) {
    // Log("AutoTeamBalance.MutatorBroadcastMessage(\""$Msg$"\") detected \"game has ended.\" - TODO run UpdateStatsAtEndOfGame() here.");
  // }

  // if (Level.Game.bGameEnded) {
    // Log("AutoTeamBalance.MutatorBroadcastMessage(\""$Msg$"\") detected Level.Game.bEnded = True - could run UpdateStatsAtEndOfGame() here.");
  // }

  // if ( InStr(Msg," entered the game.")>=0 ) { // Can we find the new player pawn, and report his #cookies ?
    // PlayerJoinedShowInfo(Msg);
    // OK well that didn't work, apparently the player didn't have an IP address when he first joined the server.  (Maybe he gets assigned one once this stack is returned.)
  // }
  // if ( InStr(Msg,"left the game.")>=0 ) {

  if ( NextMessageMutator != None ) {
    return NextMessageMutator.MutatorBroadcastMessage( Sender, Receiver, Msg, bBeep, Type );
  } else {
    return false;
  }

}

/*
function PlayerJoinedShowInfo(string Msg) {
  local int i;
  local String nick;
  local Pawn p;
  // Extract player's nick from the game broadcast message, e.g.: "nogginBasher entered the game."
  i = Instr(Msg," entered the game.");
  nick = Left(Msg,i);
  // Find player in current game list
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.bIsPlayer && !p.IsA('Spectator') && !p.IsA('Bot') && p.IsA('PlayerPawn') && p.bIsHuman) {
      if (p.getHumanName() == nick) {
        break; // Found him!
      }
    }
  }
  if (p == None) {
    Log("AutoTeamBalance.PlayerJoinedShowInfo(): could not find "$nick$" in current game.");
    return;
  }
  i = FindPlayerRecord(PlayerPawn(p));
  if (bBroadcastCookies) { BroadcastMessage(nick$" has "$Int(avg_score[i])$" cookies after "$hours_played[i]$" hours on the server."); }
}
*/
