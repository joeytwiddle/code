//// From PlayerPawn.ClientMessage():
// pawn.Player.Console.Message( pawn.PlayerReplicationInfo, "...", 'Event' );
// pawn.myHUD.Message( pawn.PlayerReplicationInfo, "...", 'Event' );

// TODO:
// AutoTeamBalance logging: 0=none, 1=to logfile, 2=broadcast in-game, 3=both
// // AutoTeamBalance logging level: 0=none, 1=hello etc, 2=details(show stats)
// AutoTeamBalance detailed stats logging: 0=none, 1=to logfile, 2=broadcast in-game, 3=both
// Cookies: 0=none, 1=console message to each player, 2=HUD message to each player, 3=broadcast message to all players

// TODO: we could now move to the list-of-gametype-strings method instead of all those bools

// TODO: test doing Disable('Tick'); when Tick() is no longer needed

//== AutoTeamBalance ==========================================================

// A mutator that makes fair teams at the beginning of each teamgame, by recording the relative strengths of players on the server (linked to their nick/ip).
// It also attempts to put a player joining the game on an appropriate team.
// It can only build player stats for regular players who stay until the end of the game.
// It will also take a while after running to build up accurate stats of the players.
// For the first week or so you may wish to collect stats but not attempt team-balancing: just set bAutoBalanceTeamsFor*=False but leave bUpdatePlayerStatsFor*=True.
// by Daniel Mastersourcerer at Kitana's Castle and F0X|nogginBasher of no fixed abode.
// (c)opyleft May 2007 under GNU Public Licence

// vim: tabstop=2 shiftwidth=2 expandtab

// NOTE: If your server has custom maps, it might be a good idea to increase NetWait to say 20 seconds, to give each player a better chance of downloading the map and joining the game before it starts, so that player can be included in the teambalancing.

// The field delimeter for playerData in the config files is a space " " since that can't appear in UT nicks (always _)

// DONE: don't do any evening of teams if bTournament=True; but atm it will still do end-game player stats updates in tournament mode; and why not?!

// DONE: catch the end-of-game event and collect scores then (check Level.Game.bGameEnded)

// HALF-DONE: when the playerData array gets full, old records are not recycled properly (atm the last is just overwritten repeatedly :| )   - ok now we recycle the record with shortest play hours.  TODO: It's not perfect because the new player's stats might get overwritten pretty soon.  We either need a little randomnity, or we could store date_last_played[]

// TODO: catch a player saying "!teams", maybe write some custom code to balance the teams then (by swapping 1/2 players only, maybe slightly randomised so it can be repeated if unsatisfactory; noo that could get too spammy :E)

// Done now: i shouldn't be taking averages over time, but over #polls :S  Actually either is fine, but I was doing it weirdly before.

// CONSIDER: in cases of a standoff (e.g. none of the players are found in the DB so all have UnknownStrength), choose something random!  What we are given may not be random enough (like bPlayersBalanceTeams).

// TODO: config option bRankBots (might be interesting to see how Visse compares to the humans ^^ )

// Half-Done: could also analyze TDM (DeathMatchPlus) scores, but without the CTF bonuses, these will be much lower (store in separate fields? e.g. avg_TDM_score TDM_hours_played)  What about a method to separate all teamgames?  OR Easier: make a separate player with nick+" "+ip+" "+gameType hash

// Current rankings method:
// We wait until the end of the game, then we update the stats for each player.
// Hence we collect each player's average endgame score.

// Half-Done: I'm not so sure about averaging end-scores of each game.  For example, a game might be very short with low scores, doesn't mean all the players strengths should be punished for that.  So I guess scores should be proportional to time.  So maybe we should use weightScore to calculate each player's average score-per-hour (I think XOL used SPH/FPH).

// OLD rankings method:
//
// At the moment we are polling the game every PollMinutes minutes, and updating the players according to their current in-game score.
// So we are basically measuring their average score during the whole (any,all) time they are on the server (playing CTF with 4+ players; and even teams?!).
//
// We are currently taking player snapshots about 6 times during each 15 minute game, and storing the average score (usually SmartCTF score, not default frags).
// This is NOT their average score at the end of the game, but their average score at "random" intervals during the game.
// This might seem unfair to players who join the server only for a few minutes, and get a low average score.  Screw them, they might leave halfway through the next game.  Regular players get rightly punished (and highly ranked!) for holding high scores during long games.  :)
//
// Thus recorded for each player is their average score over all the time they ever spent on the server (until they change nick or IP).
// Players will be forever punished/catching up if they started playing with low scores on the server.
// We could offer a MaxHoursPlayed or MaxPollsBeforeRecyclingStrength (or MaxGamesPlayed), after which their time_on_server does not increase, and their ranking becomes more oriented towards the players most recent scores (older scores get phased out).  E.g. with MaxPollsBeforeRecyclingStrength=99, new_score = ((old score * 99) + current_score) / 100 = .99*old_score + .01*current_score <-- This is still a very slow way of forgetting the past

// Post-thoughts:
// Scores can vary greatly on different maps with different numbers of players and with different teams.
// On a server like iNzane or XOL, which has a fairly small mapcycle, and which has regular players, maybe this is not a problem.
// But on my server I have CTF, iCTF, tCTF, maps for 2 players and maps for 20 players, scores from different games will vary wildly.
// Recording average-score-per-hour on such a server might run into problems.
// For example maybe a lowskilled player asks a real noob player for a 1v1 on CTF-PureAction.
// In 10 minutes the stronger player might have made 15 flagkills, 10 caps, and a whole bunch of points, which when converted into a score-per-hour might make that player look very powerful.
// However his score was abnormally boosted by the low skill of his opponent.
// He might have increased his average score-per-hour to something comparable with those of highskill players, but who only play against other highskill players, on more serious maps, e.g. CTF-Terra.

// So what else could we try?
// - TODO Team Cap Bonus: regardless of frags, the team which played the best CTF will get the most caps.  All players on the winning team could be rewarded for having done so, even if their actual score was pretty low.
// - The actual scores might not be useful, but the distribution of those scores might be interesting.
//   For example, the order of players on the scoreboard (imagine if both teams were merged into one) should give an idea of the relative skills of certain players.  e.g. top player gets 100 points, all other players get less, bottom player gets 10.
//   TODO Or the relative scores could be considered.  E.g. the scores from the game could be scaled so that they always have a mean of say 50 points.  So we can still use the game scores, but the scaling will "normalise" those scores so that as much benefit comes from doing well in a low-scoring game as in a high-scoring game.
//        Ofc this would mean the best noobs get similar scores to the best el33ts, if they the noobs and leets never actually play at the same time, which kinda makes sense.  ^^

// TODO: consider adding just a little randomnity.  If we have the same 8 players on the server for 4 games in a row, and their stats don't change enough to actually switch any of their positions in the ranking, won't Daniel's initial teambalance create identical teams for every game?  Can we find a way to avoid that?  Mix up the lower skilled players a bit, since that will have least impact?
// looks like FRand() might returns a number between 0 and 1.

// TODO: throughout the code and comments i have referred to strength,avg_score,ranking,rating,stats but often meant the same thing.  Daniel stuck to "Strength" so maybe I should consolidate around that name.

// Note: at the moment no attempt is made to update the strengths of players who leave the server before the end of the game.
// Only players on the server when the game ends will get their strenghts updated.
// Maybe that's undesirable.  iDeFiX's teambalancer updates stats mid-game (but i think it caches the info and does more processesing at the end of the game).

// CONSIDER: If players manage to make the teams unfair anyway, and then go and cap 10 flags 3v1, then the scores from that game will hardly be an accurate representation of what they should have been.  So should we be detecting whether teams were actually fair when we update the stats?  Well, maybe it's ok: the lamers who teamed up will get punished by unrealistically high stats, so next time they play they will get balanced with weak players on their team.  Mwuhahaha!  And the guy who got bullied will obviously need a break, so him losing some strength stats will just mean he gets some stronger team-mates in future.  :)  So I guess we don't really need to detect whether teams were fair when updating stats!

//=============================================================================

class AutoTeamBalance expands Mutator config(AutoTeamBalance);

var string HelloBroadcast; // TODO CONSIDER: make this configurable, and make it say nothing if ""

var config bool bAutoBalanceTeamsForCTF;
var config bool bAutoBalanceTeamsForTDM;
var config bool bAutoBalanceTeamsForAS;
var config bool bAutoBalanceTeamsForOtherTeamGames;
// var config string BalanceTeamsForGameTypes; // TESTING_List_desired_gametypes
// For updating player strength in-game:
var config bool bUpdatePlayerStatsForCTF;
var config bool bUpdatePlayerStatsForTDM;
var config bool bUpdatePlayerStatsForAS;
var config bool bUpdatePlayerStatsForOtherTeamGames;
var config bool bUpdatePlayerStatsForNonTeamGames;
// var config string UpdateStatsForGameTypes; // TESTING_List_desired_gametypes
// var config bool bUpdateStatsForCTFOnly;  // Stats were updating during other gametypes, which yield entirely different scores.  (Maybe stats for different gametypes should be handled separately.)  If your server runs only one team gametype, or gametypes with comparably scores, you can set this to False.
//// These didn't work for me; maybe config vars can't be complex types like "name"
// var config name OnlyBalanceTeamsIfGametypeIsA; // Defaults to 'TeamGamePlus' so it will try to balance teams for all team games.
// var config name OnlyUpdateStatsIfGametypeIsA;  // Stats were updating during other gametypes than CTF, which yield entirely different scores.  (Maybe stats for different gametypes should be handled separately.)  You can set this to your own server's favourite gametype, or to 'TeamGamePlus' if you only host one gametype, or player scores are comparable across all your gametypes.
// var config float PollMinutes;    // e.g. every 2.4 minutes, update the player stats from the current game
var config int MaxPollsBeforeRecyclingStrength;    // after this many polls, player's older scores are slowly phased out.  This feature is disabled by setting MaxPollsBeforeRecyclingStrength=0 // TODO: refactor this to MaxHoursOfOldStats, more tangible unit for admin to edit
var config int MinHumansForStats; // below this number of human players, stats will not be updated, i.e. current game scores will be ignored
var config bool bNormaliseScores;
// deprecated: var config bool bDoWeightedUpdates;

var config bool bBroadcastStuff;   // Be noisy to in-game console
var config bool bExtraDebugLogging;     // logs are more verbose/spammy than usual; recommended only for developers
var config bool bBroadcastCookies; // Silly way to debug; each players strength is spammed at end of game as their number of cookies
// TODO: now we are doing p.ClientMessage() sometimes, we don't really need to BroadcastMessage as well (I only want it as a developer to see changes during the game.)
// var config bool bOnlyMoreCookies;  // only broadcast a players cookies when they have recently increased
var config bool bBroadcastLostCookies;  // should we broadcast when someone has moved down the ranking?

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
// TODO: var int date_last_played[4096]; // would be good for recycling old stats; otherwise recycle on lowest hours_played I guess, although if the server/playerData lasts 1billion years, it might be hard for the current generation of players to get into the ranking

// For local state caching (not repeating when called by Tick's or Timer's):
var bool initialized;              // Mutator initialized flag
var bool gameStartDone;            // Teams initialized flag (we never initialise this to False, but I guess Unreal does that for us)
var bool gameEndDone;

var int timeGameStarted;

defaultproperties {
  HelloBroadcast="AutoTeamBalance (beta) is attempting to balance the teams"
  bAutoBalanceTeamsForCTF=True
  bAutoBalanceTeamsForTDM=True
  bAutoBalanceTeamsForAS=True
  bAutoBalanceTeamsForOtherTeamGames=True
  // BalanceTeamsForGameTypes="CTFGame,TeamGamePlus,JailBreak,*"
  bUpdatePlayerStatsForCTF=True
  bUpdatePlayerStatsForTDM=True // If you are normalising scores, then updating stats for TDM should be ok.  But if you are not normalising scores, then the different bonuses in CTF will make stats from the different gametypes incompatible.  (Basically TDMers will get lower strengths because they never get the bonus points from caps/covers/etc.)  So in this case you are recommended only to build stats for your server's most popular gametype.
  bUpdatePlayerStatsForAS=False  // Probably best left False (unless you are running an AS-only server) because AS scores are crazy (one guy gets 100 for the last objective, even though it was a team effort)
  bUpdatePlayerStatsForOtherTeamGames=False
  bUpdatePlayerStatsForNonTeamGames=False
  // UpdateStatsForGameTypes="CTFGame"
  // bUpdateStatsForCTFOnly=True
  // OnlyUpdateStatsIfGametypeIsA='CTFGame' // Would have been nice to offer it this way, but I didn't get it working.
  // OnlyBalanceTeamsIfGametypeIsA='TeamGamePlus' // TODO: we CAN do it this way, e.g. using String(gametype.Class) == "Botpack.Assault"
  // PollMinutes=2.4
  MaxPollsBeforeRecyclingStrength=200 // I think for a returning player with a previous average of 100(!), and a new skill of around 50, and with 24 polls an hour and MaxPollsBeforeRecyclingStrength=100, after 100 more polls (4 more hours), the player's new average will look like 60.5.  That seems too quick for me, so I've gone for 200.  ^^  btw this maths is wrong :| but approx i guess
  MinHumansForStats=1     // TODO: for release, recommended 4
  bNormaliseScores=True     // Normalise scores so that the average score for every game is 50.  Recommended for servers where some games end with very high scores and some not (e.g. if you have different styles of map and game-modes, like mixing normal weapons clanwar maps with instagib action maps).  You can turn this off if your server has a fixed mapcycle and always the same game-mode.  Normalising results in a *relative* ranking of players who play the same games.  Not normalising would be better for separating weak and strong players who never actually played together.  If you have 10 strong players getting high scores on one game, and 10 noobs getting low scores during a different game, normalising would actually put the strongest noob up with the strongest pwnzor.  TODO CONSIDER: would it be a useful compromise to "half-normalise"?  And how would we do that?  I think some logarithmic maths might be required.
  // deprecated: bDoWeightedUpdates=False  // Untested experimental stats updating method
  bBroadcastStuff=True
  bExtraDebugLogging=True      // TODO: for release, recommended False (some logging is ok tho!)
  bBroadcastCookies=True
  // bOnlyMoreCookies=False
  bBroadcastLostCookies=True // TODO: for release, default to False
  UnknownStrength=50      // New player records start with an initial strength of avg_score 40 (I changed this to 30 when I started normalising scores) (I changed it to 50 since most new players who join the server are about as good as the known players)
  // UnknownMinutes=10       // New player records start with a virtual 10 minutes of time played already
  BotStrength=20
  FlagStrength=50         // If it's 3:0, the winning team will get punished an extra 150 points; used when new players join the game and number of players on each team are even
  bClanWar=False
  MaxPlayerData=4096
  // bHidden=True // what is this?  iDeFiX says it's only needed for ServerActors
}



// ==== Daniel's HOOKS / OVERRIDES ==== //

// Initialize the system
function PostBeginPlay() {
  if (initialized) return;

  if (bExtraDebugLogging) { Log("AutoTeamBalance.PostBeginPlay() WAS CALLED NOW"); }

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
  // Log("AutoTeamBalance.PostBeginPlay(): Set Timer() for "$(PollMinutes*60)$" seconds.");
  // SetTimer(10,True); // Now checking once a minute to see if game has ended; changed to 10 seconds since we lost our alternative MessageMutator hook
  // if (bExtraDebugLogging) { Log("AutoTeamBalance.PostBeginPlay(): Set Timer() for 10 seconds."); }
  // This has been moved to HandleEndGame, so no checks need to be made during game-play (ok well maybe it's checked during overtime)!  :)

  // Level.Game.RegisterMessageMutator( Self ); // TESTING Matt's MutatorBroadcastMessage hook below
  // deprecated because it was hiding server broadcasts (like adwvaad used to; might be fixed by making it a serveractor, although may or may not be possible for what we are doing with this mutator)

}

// Check if game is about to start, and if so, balance the teams before it does.  (We want to do this as late as possible before the game starts, to give players time to join the server.)
// DONE: Determine whether this check every Tickrate is more or less efficient than using SetTimer() and Timer(), then merge the two CheckGameStart() and UpdateStatsAtEndOfGame() into the more efficient method.
//       We may not need to check more often than once every -2 + 270 seconds.
//       If TickRate 20 means 20 calls to Tick per second, presumably Timer() is (an Engine (non UScript) event? and) more efficient.
//       Ahhh but maybe Daniel used Tick() because it's before game start, too early to use a timer.  To look into...
//       Yes that's the reason, can't use Timer().  But what about PreBeginPlay()?
//       Anyway, I inlined the bool check into Tick() so no extra function call is needed.  And UpdateStatsAtEndOfGame() is dealt with by Timer().
//       TODO: well this bool check must be pretty fast, but consider overriding PreBeginPlay() and see if that can help us.
event Tick(float DeltaTime) {
  if (!gameStartDone) CheckGameStart();
}

// TODO: just a TEST, remove it if it can't help us.
function PreBeginPlay() {
  if (bExtraDebugLogging) { Log("AutoTeamBalance.PreBeginPlay() WAS CALLED NOW"); }
}

function bool ShouldBalance(GameInfo game) {

  // TODO: AS is a teamgame, so ok to balance, BUT ONLY on the first half-game.
  //       Teams should remain the same for the second half-game.  (I fear if it does the balancing for both half-games, the same ppl will be defending twice!)
  // Also TODO: make AS a separate option in the booleans above; atm it's classified under Other.

  //// TESTING_List_desired_gametypes
  // local string[20] gametypes;
  // local int n,i;

  // if (bExtraDebugLogging) { Log("AutoTeamBalance.ShouldBalance("$game$") Game.Name="$Game.Name$" Game.Class="$Game.Class$""); }
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
  if (String(Level.Game.Class) == "Botpack.Assault")
    return bAutoBalanceTeamsForAS;

  //// TESTING_List_desired_gametypes
  // n = Split(BalanceTeamsForGameTypes,",",gametypes);
  // for (i=0;i<n;i++) {
    // if (gametypes[i] == "*")
      // return True;
    // if (gametypes[i] == String(Level.Game.Name))
      // return True;
  // }
  // if (bExtraDebugLogging) { Log("AutoTeamBalance.ShouldBalance("$game$"): Did not match any of the specified gametypes: "$BalanceTeamsForGameTypes); }

  // OK so it's an unknown teamgame
  if (bExtraDebugLogging) { Log("AutoTeamBalance.ShouldBalance("$game$") Game.Name="$Game.Name$" Game.Class="$Game.Class$" returning "$bAutoBalanceTeamsForOtherTeamGames); }
  return bAutoBalanceTeamsForOtherTeamGames;
}

function bool ShouldUpdateStats(GameInfo game) {
  if (bExtraDebugLogging) { Log("AutoTeamBalance.ShouldUpdateStats("$game$") Game.Name="$Game.Name$" Game.Class="$Game.Class$""); }
  // We only build stats for CTF games if asked
  if (String(Level.Game.Class) == "Botpack.CTFGame")
    return bUpdatePlayerStatsForCTF;
  // We only build stats for TDM games if asked (NOTE: we don't use IsA here, because other teamgames might be a subclass of TeamGamePlus)
  if (String(Level.Game.Class) == "Botpack.TeamGamePlus")
    return bUpdatePlayerStatsForTDM;
  if (String(Level.Game.Class) == "Botpack.Assault")
    return bUpdatePlayerStatsForAS;
  // OK so it's not CTF or TDM or AS, but is it another type of team game?
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

  if (bExtraDebugLogging) { Log("AutoTeamBalance.ModifyLogin()"); }

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
  if (bExtraDebugLogging) { Log("AutoTeamBalance.HandleEndGame() bOverTime="$Level.Game.bOvertime$" bGameEnded="$Level.Game.bGameEnded); }

  SetTimer(10,True); // Now checking once a minute to see if game has ended; changed to 10 seconds since we lost our alternative MessageMutator hook
  if (bExtraDebugLogging) { Log("AutoTeamBalance.HandleEndGame(): Set Timer() for 10 seconds."); }

  if ( NextMutator != None ) {
    b = NextMutator.HandleEndGame();
    if (bExtraDebugLogging) { Log("AutoTeamBalance.HandleEndGame() NextMutator returned "$b$"  bOverTime="$Level.Game.bOvertime$" bGameEnded="$Level.Game.bGameEnded); }
    return b;
  }
  return false;
}

// TESTING Alternative, there is: event GameEnding() { ... } implemented in GameInfo.  Can we drop our own event catcher in here, without overriding the other?
// or ofc we can use a timer and check Level.Game.bGameEnded but the timer mustn't do this twice at the end of one game. :P
// From testing this: it never gets called!  (Maybe we must somehow register to receive events.)
// event GameEnding() {
  // if (bExtraDebugLogging) { Log("AutoTeamBalance.GameEnding() even was CALLED!  bOverTime="$Level.Game.bOvertime$" bGameEnded="$Level.Game.bGameEnded); }
// }



// ==== Daniel's other functions: ==== //

// Checks if the game has begun (CONSIDER/TEST: optimise this and move/inline the check right into Tick() above.  ATM it is called every tick, although exits fast.)
function CheckGameStart() {
  local int c,n,e;

  // if (gameStartDone) return; // This was moved up to Tick()

  // this mod works on team games only
  // only TeamGamePlus has the "countdown" variable and of course teams
  // we now check this in ShouldBalance
  // if (!bAutoBalanceTeams || !Level.Game.IsA(OnlyBalanceTeamsIfGametypeIsA) || !Level.Game.GameReplicationInfo.bTeamGame || DeathMatchPlus(Level.Game).bTournament) {
  if (!ShouldBalance(Level.Game)) { // We do this early, to check at the very least that this is a teamgame
    gameStartDone=True;
    return;
  }

  // read starting countdown
  c = TeamGamePlus(Level.Game).countdown;
  n = TeamGamePlus(Level.Game).NetWait;
  e = TeamGamePlus(Level.Game).ElapsedTime;
  // if (bExtraDebugLogging) { Log("AutoTeamBalance.CheckGameStart(): c="$c$" n="$n$" e="$e$" t="$Level.TimeSeconds$""); }
  c = Min(c,n-e);

  // initialize teams 1 second before game is starting
  if (c<2) {
  // if (Level.TimeSeconds >= 20) {
    if (bBroadcastStuff) { BroadcastMessageAndLog(HelloBroadcast); }
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
  // local int tg[64]; // their strengths, but they get zeroed during the player sorting/ranking
  // local bool moved[64]; // now i've decided to use a bool to say whether a player has been moved (because some players might actually have strength zero!)
  local int moved[64]; // dammit bool arrays are not allowed!  so 0=false 1=true :P
  local int plorder[32];
  local int i;
  local int n;
  local int mx;
  local int teamnr;
  local int teamstr[2];
  local TeamGamePlus g; // my linux ucc make had trouble with TeamGamePlus :|
  local int oldMaxTeamSize;
  local bool oldbPlayersBalanceTeams, oldbNoTeamChanges;

  // LoadConfig(); // TODO CONSIDER: If possible, we could also LoadConfig() here.  Then this mutator would be the only one I know that lets you edit the ini file without needing to restart the server!  OK well apparently LoadConfig() doesn't exist.  :P
  CopyConfigIntoArrays();  // First time the data is needed, we must convert it.

  if (bExtraDebugLogging) { Log("AutoTeamBalance.InitTeams(): Running..."); }

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
      // tg[pid]=st;
      moved[pid] = 0;
      // p.PlayerReplicationInfo.PlayerName
      Log("AutoTeamBalance.InitTeams(): Player " $ p.getHumanName() $ " on team " $ p.PlayerReplicationInfo.Team $ " has ip+port " $ PlayerPawn(p).GetPlayerNetworkAddress() $ " and score " $ p.PlayerReplicationInfo.Score $ ".");
      // if (bBroadcastCookies && !bOnlyMoreCookies) { BroadcastMessageAndLog("" $ p.getHumanName() $ " has " $st$ " cookies."); }
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
      // if (tg[i]>mx) // BUG FIXED: if a player has strength <= 0, they will never get into the plorder list, which might really confuse the teams, since they will be sitting on a team but not handled by teambalance.
      // if ( (pid == -1 && pl[i] != None && tg[pid] == ps[pid]) || (tg[i]>mx) ) // OK added if we haven't yet found a player, and we have one right here, use him regardless of his strength.  NO this will confuse below, we use td[pid]=0 to "clear" this record when he's moved to ranking.  OK so added test that tg[pid] has not been reset.  But, what if it was 0 to begin with?!
      // OK so now: is it real player?  not yet moved?  first we encountered this scan (grab him!) or stronger than last encountered?
      if ( pl[i] != None && moved[i]==0 && (pid == -1 || ps[i]>mx) )
      {
        pid=i;
        mx=ps[i];
      }
    }
    // If we found one, add him as the next player in the list
    if (pid != -1)
    {
      plorder[n]=pid;
      // ps[pid]=0;
      moved[pid] = 1;
      n++;
      Log("AutoTeamBalance.InitTeams(): [Ranking] "$ps[pid]$" "$ pl[pid].getHumanName() $"");
      if (bBroadcastCookies) { BroadcastMessageAndLog(""$ pl[pid].getHumanName() $" has " $ps[pid]$ " cookies."); }
      // if (bBroadcastCookies) { pl[pid].ClientMessage("You have " $ps[pid]$ " cookies.", 'CriticalEvent', True); }
      // if (bBroadcastCookies) { SendClientMessage(pl[pid],"You have " $ps[pid]$ " cookies."); } // gets hidden by team switches below
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

  // Little point doing this here; wait until we update the player strengths.
  // CopyArraysIntoConfig();
  // SaveConfig();

  // Tell each player how many cookies they have
  // (This must come after the team switching, otherwise the "xxx is on Red" will overwrite this text.)
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.bIsPlayer && !p.IsA('Spectator')) {
      if (bBroadcastCookies) { SendClientMessage(p, p.getHumanName() $", you have "$ GetPawnStrength(p) $" cookies."); }
    }
  }

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

  if (bExtraDebugLogging) { Log("AutoTeamBalance.GetPawnStrength(" $ p $ "): " $ st $ ""); }

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
  if (bExtraDebugLogging) {
    c = TeamGamePlus(Level.Game).countdown;
    n = TeamGamePlus(Level.Game).NetWait;
    e = TeamGamePlus(Level.Game).ElapsedTime;
    l = TeamGamePlus(Level.Game).TimeLimit;
    t = Level.TimeSeconds;
    s = Level.Game.StartTime;
    Log("AutoTeamBalance.Timer() DEBUG c="$c$" b="$n$" e="$e$" l="$l$" t="$t$" s="$s$" bGameEnded="$Level.Game.bGameEnded);
  }
  // if (bUpdatePlayerStats) {
    // Stats were updating during a game of DM ffa, 3 players, low scores.  This gives very different scores than CTF games.
    // Presumably we have not checked that this is *really* a team-game we are getting stats from.
    // For now, have optionally limited stats to CTF games only:
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
  /*
  local int c,n,e,l,t,s;
  if (bExtraDebugLogging) {
    c = TeamGamePlus(Level.Game).countdown;
    n = TeamGamePlus(Level.Game).NetWait;
    e = TeamGamePlus(Level.Game).ElapsedTime;
    l = TeamGamePlus(Level.Game).TimeLimit;
    t = Level.TimeSeconds;
    s = Level.Game.StartTime;
    Log("AutoTeamBalance.Timer() DEBUG c="$c$" b="$n$" e="$e$" l="$l$" t="$t$" s="$s$" bGameEnded="$Level.Game.bGameEnded);
  }
  */
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
        Log("AutoTeamBalance.FindPlayerRecord(p) IP match for " $p.getHumanName()$ ","$stripPort(p.GetPlayerNetworkAddress())$": ["$found$"] "$nick[i]$" ("$avg_score[i]$")");
      }
      if (p.getHumanName() == nick[i] && found == -1) {
        Log("AutoTeamBalance.FindPlayerRecord(p) nick match for " $nick[i]$ ","$p.GetPlayerNetworkAddress()$": ["$found$"] "$ip[i]$" ("$avg_score[i]$")");
        found = i; // if not yet matching an ip, match the same nick on a different ip
      }
      // TODO: if an uneven match, choose a match with more experience (hours_played)
      // TODO: if we have little experience (<10mins) of a player, assume default score?
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
  }
  // TODO: If all full, could add somewhere randomly in the last 100 (ordered) spots (rather than just 1 spot which keeps getting re-used)
  // DONE: find the record with lowest hours_played and replace that one
  // better: or, find the oldest record and replace it
  if (pos == MaxPlayerData) { // all records were full
    pos = FindShortestPlayerRecord();
  }
  ip[pos] = stripPort(p.GetPlayerNetworkAddress());
  nick[pos] = p.getHumanName();
  // initialise each player as having played for UnknownMinutes (e.g. 10 or 0.1) minutes already, and already earned an average UnknownStrength (e.g. 40) frags
  avg_score[pos] = UnknownStrength; // DO NOT set this to 0; it will screw with InitTeams()!
  hours_played[pos] = 0; // UnknownMinutes/60;
  Log("AutoTeamBalance.CreateNewPlayerRecord("$p$") ["$pos$"] "$ nick[pos] $" "$ ip[pos] $" "$ avg_score[pos] $" "$ hours_played[pos] $".");
  // if (bBroadcastCookies) { BroadcastMessageAndLog("Welcome "$ nick[pos] $"!  You have "$ avg_score[pos] $" cookies."); }
  // if (bBroadcastCookies) { BroadcastMessageAndLog("Welcome to the server "$ nick[pos] $"!  Have a cookie.  :)"); }
  // if (bBroadcastCookies) { p.ClientMessage("Welcome to the server "$ nick[pos] $"!  Have a cookie.  :)", 'CriticalEvent', True); }
  if (bBroadcastCookies) { SendClientMessage(p,"Welcome to the server "$ nick[pos] $"!  Have a cookie.  :)"); }
  // SaveConfig();
  return pos;
}

// Finds an old player record which we can replace.  Actually since we don't have a last_seen field, we'll just have to remove the "shortest" record.  (Player didn't spend long on server; their stats don't mean a lot)
// Only problem, if the database really is saturated (but I think that's unlikely), this new player will probably be the next record to be replaced!  To keep his record in the database, the new player just has to play for longer than the now "shortest" record before another new player joins.
// Actually one nice side-effect of the particular algorithm we're using below (<lowest instead of <=lowest): if a few records share the "shortest record" time (actually this was more likely when our hours_played were incremented in fixed-size steps), it will be the first of them that gets replaced first.  :)  Down-side: the new player now in that early position in the stats-table was not an early player on the server, so he breaks this very pattern.
function int FindShortestPlayerRecord() {
  local int i,found;
  found = 0;
  for (i=1;i<MaxPlayerData;i++) {
    if (hours_played[i] < hours_played[found]) {
      found = i;
    }
  }
  return found;
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
  Log("AutoTeamBalance.UpdateStatsAtEndOfGame(): Updating player stats.");
  if (bBroadcastStuff) { BroadcastMessageAndLog("AutoTeamBalance is updating player stats."); }
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
  local int i,j;
  local float current_score;
  local float new_hours_played;
  local int previousPolls;
  local int gameDuration;
  local int timeInGame;
  local float weightScore;
  local float previous_average;

  i = FindPlayerRecord(p);

  if (i == -1 || ip[i] != stripPort(p.GetPlayerNetworkAddress()) || nick[i] != p.getHumanName()) {
    // This is not an exact player match, so we should not update its stats
    // since we didn't find this actual ip+nick, we create a new entry
    // DONE: should we base the new players stats on the default (below), or copy over the estimated player's stats?  (But maybe reset their time_on_server.)
    j = CreateNewPlayerRecord(p); // OLD BUG FIXED: is it inefficient to repeatedly create a PlayerPawn from the same Pawn?
    // Copy over strength from the partial-match player, but only make that strength last for 2 hours.
    // SO: changing nick will Not reset your avg_score immediately, but eventually
    avg_score[j] = avg_score[i]; // Copy score from partial record max
    hours_played[j] = Min(2.0,hours_played[i]); // but in case this is a different player (or maybe the same player but in a different environment), give the new record max 2 hours
    i = j;
  }

  current_score = p.PlayerReplicationInfo.Score;
  if (bNormaliseScores) {
    current_score = NormaliseScore(current_score);
  }
  // Ideally we would like to check how long this player has been on the server DONE i don't know how to get that yet ^^ I'm hoping it's somewhere in the code otherwise I have to remember the times that players joined    Ahh got it from iDeFiX's code, ofc it's in PlayerReplicationInfo, like everything else I can't find in PlayerPawn :>
  // For the moment, assume all players were on server the whole game:
  gameDuration = Level.TimeSeconds - timeGameStarted;
  timeInGame = Level.TimeSeconds - p.PlayerReplicationInfo.StartTime;
  // I don't know if this will ever happen, but I was thinking in tournament mode, we don't want to calculate a player as having played for 24 minutes if they were waiting 4 minutes for the game to start :P
  if (timeInGame>gameDuration)
    timeInGame = gameDuration;
  if (bExtraDebugLogging) { Log("AutoTeamBalance.UpdateStatsForPlayer(p) timeInGame="$timeInGame$" gameDuration="$gameDuration$" Level.Game.StartTime="$Level.Game.StartTime$" Level.TimeSeconds="$Level.TimeSeconds$""); }
  // Well if this player was only in the server for 5 minutes, we could multiply his score up so that he gets a score proportional to the other players.  (Ofc if he was lucky or unlucky, that luck will be magnified.)
  if (timeInGame < 60) { // The player has been in the game for less than 1 minute.
    Log("AutoTeamBalance.UpdateStatsForPlayer("$p$") Not updating this player since his timeInGame "$timeInGame$" < 60s.");
    return;
  }
  new_hours_played = hours_played[i] + (Float(timeInGame) / 60 / 60);

  previous_average = avg_score[i];

  /* I never tried this, but the method below is superior because it's simpler!
  if (bDoWeightedUpdates) {

    weightScore = Float(gameDuration) / Float(timeInGame);
    // Let's weight the scores more, so that instead of becoming score-per-endgame it becomes score-per-hour (in case this was a short game with low frags, or overtime with many frags).
    weightScore = weightScore * 60 * 60 / Float(gameDuration) / 4; // I'm dividing by 4 here to make it score-per-quarter-hour, which should be close to actual end-game scores, at least on my 15minute game server.
    previousPolls = hours_played[i] / 4;  // This is approx #times we've updated this player's stats before (since my server usually has 15 minute games).  But it's not too bad if your server is different.  It's just used to measure the significance of their current score relative to the number of scores we've seen before from this player.  Servers with longer game-times will make new scores slightly less significant.
    if (MaxPollsBeforeRecyclingStrength>0 && previousPolls > MaxPollsBeforeRecyclingStrength) {
      previousPolls = MaxPollsBeforeRecyclingStrength - 1;
    }
    Log("AutoTeamBalance.UpdateStatsForPlayer(p) ["$i$"] "$p.getHumanName()$" avg_score = ( ("$avg_score[i]$" * "$previousPolls$") + "$current_score$"*"$weightScore$") / "$(previousPolls+1));
    // avg_score[i] = ( (avg_score[i] * previousPolls) + current_score*weightScore) / (previousPolls+1);

  } else {
  */

  // TODO CONSIDER: should we give bonus points for being on the winning team
  //                otherwise we might get a group of good team players who often have low scores but their team always wins.
  //                well that's unlikely, due to SmartCTFStats bonuses

  // Mmm we can forget all the weird weighting and just update the player's average_score_per_hour:
  Log("AutoTeamBalance.UpdateStatsForPlayer(p) ["$i$"] "$p.getHumanName()$" avg_score = ( ("$avg_score[i]$" * "$hours_played[i]$") + "$current_score$"/4.0) / "$(new_hours_played));
  avg_score[i] = ( (avg_score[i] * hours_played[i]) + current_score/4.0) / new_hours_played; // I'm dividing every score here by 4 so that the actual averages stored in the config will be score-per-quarter-hour, which should be close to actual end-game scores, at least on my 15minute game server.  Just makes it easier to read.
  // We don't need to worry about how long he spent on the server wrt other players, or how long the game was.

  // }

  hours_played[i] = new_hours_played;

  if (bBroadcastCookies) {
    if (avg_score[i]>previous_average+1) {
      BroadcastMessageAndLog(""$ p.getHumanName() $" has earned "$ Int(avg_score[i]-previous_average) $" cookies!");
      // p.ClientMessage("You earned "$ Int(avg_score[i]-previous_average) $" cookies this game.", 'CriticalEvent', True);
      SendClientMessage(p,"You earned "$ Int(avg_score[i]-previous_average) $" cookies this game."); // BUG: hidden by scoreboard
    }
    else if (previous_average>avg_score[i]+1 && bBroadcastLostCookies) {
      BroadcastMessageAndLog(""$ p.getHumanName() $" has lost "$ Int(previous_average-avg_score[i]) $" cookies.");
      // p.ClientMessage("You lost "$ Int(previous_average-avg_score[i]) $" cookies this game.", 'CriticalEvent', True);
      SendClientMessage(p,"You lost "$ Int(previous_average-avg_score[i]) $" cookies this game."); // BUG: hidden by scoreboard
    }
  }
}

// Normalises a player's score so that average of all scores on server will be 50.
// This is to fix the problem that some games (e.g. 2v2 w00t or PureAction or iG) have much higher scores than others, which will confuse the stats.
// Now your stats are changed by about the same amount for each map, regardless of your actual frags, but relative to the other players on the server.
function float NormaliseScore(float score) {
  local Pawn p;
  local int playerCount;
  local float averageGameScore;

  // Kinda inefficient to calculate the average once for every player, but who cares? :P
  // Mmm well maybe it could cause some confusions if a player leaves during the updating, but that's unlikely.
  averageGameScore = 0.0;
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.bIsPlayer && !p.IsA('Spectator') && !p.IsA('Bot') && p.IsA('PlayerPawn') && p.bIsHuman) { // lol
      averageGameScore += PlayerPawn(p).PlayerReplicationInfo.Score;
      playerCount++;
    }
  }
  averageGameScore = averageGameScore / Float(playerCount);

  // Avoid division-by-zero error here.  You guys got average <2 frags?  Screw you I'm not scaling that up to 50!
  if (averageGameScore < 2.0) {
    averageGameScore = 50;
  }

  if (bExtraDebugLogging) { Log("Normalising from average "$averageGameScore$": "$score$""); }

  return score * 50 / averageGameScore; // TODO: i think this 50 should be scaled if the game was longer/shorter than usual  fewer points for shorter games?  remember their relative score will be scaled up by their time, so is the scale really needed?  :o  Mmm I conclude we don't need to scale the 50.

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
// nogginBasher: this seemed to be suppressing the broadcasts on the client's screens (same as adwvaad), so at the end I'm calling BroadcastMessage to send them out again.  Infinite loop?  We'll see... ^^
// Well that didn't work either.  :(
// So I'm gonna take it out entirely
/*
function bool MutatorBroadcastMessage( Actor Sender, Pawn Receiver, out coerce string Msg, optional bool bBeep, out optional name Type ) {

  CheckGameEnd(); // Does no harm to do this twice.  The broadcast from mapvote might make the stats parsing come sooner than waiting for the timer.

  if (bExtraDebugLogging) { // TESTING I want to see if we can detect a player saying "!teams" this way... Answer: no! For that we need a MessagingSpectator
    Log("AutoTeamBalance.MutatorBroadcastMessage(\""$Msg$"\") was called.");
  }

  // Failed Test: Nope that was never broadcast :P
  // if ( InStr(Msg,"game has ended.")>=0 ) {
    // Log("AutoTeamBalance.MutatorBroadcastMessage(\""$Msg$"\") detected \"game has ended.\" - DONE elsewhere: run UpdateStatsAtEndOfGame() here.");
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

  BroadcastMessage(Msg);

}
*/

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
  if (bBroadcastCookies) { BroadcastMessageAndLog(nick$" has "$Int(avg_score[i])$" cookies after "$hours_played[i]$" hours on the server."); }
}
*/

// I want to Log all calls to BroadcastMessage() so that I can see without playing how much the players are getting spammed by broadcasts.
function BroadcastMessageAndLog(string Msg) {
  Log("AutoTeamBalance Broadcasting: "$Msg);
  BroadcastMessage(Msg);
}

function SendClientMessage(Pawn pawn, string Msg) {
  Log("AutoTeamBalance Sending message to "$pawn.getHumanName()$": "$Msg);
  pawn.ClientMessage(Msg, 'CriticalEvent', True);
}

