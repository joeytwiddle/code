//// From PlayerPawn.ClientMessage():
// pawn.Player.Console.Message( pawn.PlayerReplicationInfo, "...", 'Event' );
// pawn.myHUD.Message( pawn.PlayerReplicationInfo, "...", 'Event' );

// TODO: after a player has joined, we should message them to overwrite the default "you are on red" message, to tell them their team has not yet been assigned

// TODO: the defaults (for the XOL release at least) should be as close as possible to Sourceror's original XOL mod.  make options for other servers

// TODO: consider: should we normalise player scores in terms of time before we normalise the scores around the average?

// TODO: test: does this work ok as a ServerActor?

// TODO: i had a report thata game was left running for a long time, then a player entered for the last minute and made 1 cap
//       they got +3294724 points!  try to reproduce this problem, then fix it.

// TODO: sourceror recommends updating stats for any player who leaves the game part-way through.  maybe idefix's code knows how to detect this

// TODO: viking recommends averaging score and frags, to get a mix of a player's DM skill and CTF skill (make it a bool option imo)

// TODO: use in-game scores for "!teams" and balancing when a new player joins, by doing a mid-game new-stats-calculation of the current players on the server (altho not an update)

// TODO:
// AutoTeamBalance logging: 0=none, 1=to logfile, 2=broadcast in-game, 3=both
// // AutoTeamBalance logging level: 0=none, 1=hello etc, 2=details(show stats)
// AutoTeamBalance detailed stats logging: 0=none, 1=to logfile, 2=broadcast in-game, 3=both
// Cookies: 0=none, 1=console message to each player, 2=HUD message to each player, 3=broadcast message to all players
// CONSIDER:
// mm well we got 3 types of logging:
//   developer logging, we don't want live, except for major debugging, and it can be removed in final version
//   user(admin) friendly-logging
//   user(admin) friendly-logging, but it's inefficient so should be disabled except when admin is debugging

// TODO BUG: mid-game team balancing should NOT switch players who have the flag (this requires some adjustment of the teambalance algorithm)  LOL atm if it does, the player keeps the flag, and the flag does not change colour :P
// TODO: mid-game team balancing should make as few switches as possible: yeah let's just switch 1 or 2 players, based on their stats.  If a new player has just joined (no stats yet), we could update their stats, to judge whether they really pwn.  (This might cause a little error in their total_time_on_server when end-game stats are processed)

// TODO: we could now move to the list-of-gametype-strings method instead of all those bools

// TODO: if mid-game-player-join teambalance is still slow after my attempts to make it more efficient,
//       try doing it without player stats lookup, just with in-game scores
// CONSIDER doing this anyway :P
// TODO: let the option whether to use recorded stats or in-game scores for balancing be configurable mid-game, so admin can try both methods during play
// TODO: provide a few different algorithms for balancing teams, on different "!teams" commands, so they can be tested and evaluated

// DONE: watch for somebody saying "!teams" or "teams" and do mid-game balancing (TODO: based on current scores in case some players aren't in the stats)
// HALF-DONE-HALF-TODO: catch a player saying "!teams", maybe write some custom code to balance the teams then (by swapping 1/2 players only, maybe slightly randomised so it can be repeated if unsatisfactory; noo that could get too spammy :E)

// TODO: provide (semi-admin) commands to force players onto different teams, e.g. "mutate tored Tigz [<pass>]"
// DONE: if we do this, then make a "mutate teams [<pass>]" also

// TODO: configure hours_copied (if ppl change nick alot, we may want to keep all their hours!)
// TODO: add mid-game teambalance (on !teams, by scores plz)
// TODO: fix on-join teambalance (do it by scores if it's less laggy)

// NOTE: mid-game rebalancing puts me (when just 1 player) on a different team from the one it gives me at startup (lol because there are bots and the bots atm have better rankings than me :P )

// TODO: the balancing now says "X you have N cookies" which hides the message "You are on the Red team" which is kinda useful info, especially for mid-game balancing.

// DONE: add configurable winningteambonus score (e.g. +10 frags/points) for every player on the winning team
//       this will help ranking to demote non-CTF players, and balance teams for games with even caps, not just even scores
// CONSIDER: instead of +10 for winning, -10 for losing?  what difference does that make to the stats anyway?
// TODO CONSIDER: WinningTeamBonus could be combined with FlagStrength into just one config var.  With their current values, the bonus would need to be divided by #players on team.
// TODO: maybe the bonus should be hidden from the scoreboard.  DONE BUG it should definitely be disabled in tournament mode!
// CONSIDER: does WinningTeamBonus really help?  a DMer will end up on winning teams anyway, because his rating will be low, even tho he might frag well, so he'll be put with strong teammates.  true?  contradiction: why will his rating be low, if he's fragging well and getting on strong teams and hence winning the CTF?  :P

// TODO: when seeking player records, should we do a case-insensitive match on player names?

// Some forms of stats building might tend to make it harder and harder for frequent players on the server.  (They will always been trying to regain points lost by earlier mistakes, until their MaxPollsBeforeRecyclingStrength is reached.)
// Well not neccessarily frequent players, but players with good stats.

//== AutoTeamBalance ==========================================================

// A mutator that makes fair teams at the beginning of each teamgame, by recording the relative strengths of players on the server (linked to their nick/ip).
// It also attempts to put a player joining the game on an appropriate team.
// It can only build player stats for regular players who stay until the end of the game.
// It will also take a while after running to build up accurate stats of the players.
// For the first week or so you may wish to collect stats but not attempt team-balancing: just set bAutoBalanceTeamsFor*=False but leave bUpdatePlayerStatsFor*=True.
// by Daniel Mastersourcerer at Kitana's Castle and F0X|nogginBasher.
// (c)opyleft May 2007 under GNU Public Licence
// Thanks to iDeFiX, unrealadmin, Matt and the author of adwvaad
// Code snippets stolen from iDeFiX's team balancer, TeamBallancer, and the adwvaad thread

// vim: tabstop=2 shiftwidth=2 expandtab

// NOTE: If your server has custom maps, it might be a good idea to increase NetWait to say 20 seconds, to give each player a better chance of downloading the map and joining the game before it starts, so that player can be included in the teambalancing.

// The field delimeter for playerData in the config files is a space " " since that can't appear in UT nicks (always _)

// HALF-DONE TODO: when the playerData array gets full, old records are not recycled properly (atm the last is just overwritten repeatedly :| )   - ok now we recycle the record with shortest play hours.  TODO: It's not perfect because the new player's stats might get overwritten pretty soon.  We either need a little randomnity, or we could store date_last_played[]

// Done now: i shouldn't be taking averages over time, but over #polls :S  Actually either is fine, but I was doing it weirdly before.

// CONSIDER: in cases of a standoff (e.g. none of the players are found in the DB so all have UnknownStrength), choose something random!  What we are given may not be random enough (like bPlayersBalanceTeams).

// TODO: config option bRankBots or bIncludeBots (might be interesting to see how Visse compares to the humans ^^ )

// Half-Done: could also analyze TDM (DeathMatchPlus) scores, but without the CTF bonuses, these will be much lower (store in separate fields? e.g. avg_TDM_score TDM_hours_played)  What about a method to separate all teamgames?  OR Easier: make a separate player with nick+" "+ip+" "+gameType hash

// Current rankings method:
// We wait until the end of the game, then we update the stats for each player.
// We optionally normalise their score, then multiply it up to (normalised) score-per-hour, then divide by 4 to get score per 1/4 hour.
// Hence we collect each player's average (normalised) score per 15 minutes.

// So what else could we try?
// - DONE Team Cap Bonus: regardless of frags, the team which played the best CTF will get the most caps.  All players on the winning team could be rewarded for having done so, even if their actual score was pretty low.
// - The actual scores might not be useful, but the distribution of those scores might be interesting.
//   For example, the order of players on the scoreboard (imagine if both teams were merged into one) should give an idea of the relative skills of certain players.  e.g. top player gets 100 points, all other players get less, bottom player gets 10.
//   TODO Or the relative scores could be considered.  E.g. the scores from the game could be scaled so that they always have a mean of say 50 points.  So we can still use the game scores, but the scaling will "normalise" those scores so that as much benefit comes from doing well in a low-scoring game as in a high-scoring game.
//        Ofc this would mean the best noobs get similar scores to the best el33ts, if they the noobs and leets never actually play at the same time, which kinda makes sense.  ^^

// TODO: consider adding just a little randomnity.  If we have the same 8 players on the server for 4 games in a row, and their stats don't change enough to actually switch any of their positions in the ranking, won't Daniel's initial teambalance create identical teams for every game?  Can we find a way to avoid that?  Mix up the lower skilled players a bit, since that will have least impact?
// looks like FRand() might returns a number between 0 and 1.

// TODO: throughout the code and comments i have referred to strength,avg_score,ranking,rating,stats but often meant the same thing.  Daniel stuck to "Strength" so maybe I should consolidate around that name.
// consider using a new word "judge" as the process of observing and maintaining user stats (as opposed to balancing or other features)

// Note: at the moment no attempt is made to update the strengths of players who leave the server before the end of the game.
// Only players on the server when the game ends will get their strenghts updated.
// Maybe that's undesirable.  iDeFiX's teambalancer updates stats mid-game (but i think it caches the info and does more processesing at the end of the game).

// CONSIDER: If players manage to make the teams unfair anyway, and then go and cap 10 flags 3v1, then the scores from that game will hardly be an accurate representation of what they should have been.  So should we be detecting whether teams were actually fair when we update the stats?  Well, maybe it's ok: the lamers who teamed up will get punished by unrealistically high stats, so next time they play they will get balanced with weak players on their team.  Mwuhahaha!  And the guy who got bullied will obviously need a break, so him losing some strength stats will just mean he gets some stronger team-mates in future.  :)  So I guess we don't really need to detect whether teams were fair when updating stats!

// TODO CONSIDER: should we record lots of stats, like frags-per-hour, score-per-hour, normalised-score-per-hour, num-games-played, num-games-won, num-games-lost (diff = num-drawn or left before end)

//=============================================================================

class AutoTeamBalance expands Mutator config(AutoTeamBalance);

var string HelloBroadcast; // TODO CONSIDER: make this configurable, and make it say nothing if ""

var config bool bBroadcastStuff;   // Be noisy to in-game console
var config bool bBroadcastCookies; // Silly way to debug; each players strength is spammed at end of game as their number of cookies
var config bool bDebugLogging;     // logs are more verbose/spammy than usual; recommended only for developers
// TODO: now we are doing p.ClientMessage() sometimes, we don't really need to BroadcastMessage as well (I only want it as a developer to see changes during the game.)
// var config bool bOnlyMoreCookies;  // only broadcast a players cookies when they have recently increased

var config bool bLetPlayersRebalance;
var config int MinSecondsBeforeRebalance;
var config String AdminPassword;   // Needed for mid-game rebalancing with: "mutate teams <pass>"

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

var config bool bLogEndStats;
// var config float PollMinutes;    // e.g. every 2.4 minutes, update the player stats from the current game
var config float MaxHoursWhenCopyingOldRecord;     // If you have lots of fakenicklamers on your server, set this high.  If not, set it low, so that players who unluckily share the same IP or nick, don't get their stats confused.
// var config int MaxPollsBeforeRecyclingStrength;    // TODO later on this line! after this many polls, player's older scores are slowly phased out.  This feature is disabled by setting MaxPollsBeforeRecyclingStrength=0 // TODO: refactor this to MaxHoursOfOldStats, more tangible unit for admin to edit
var config float HoursBeforeRecyclingStrength;
var config int MinHumansForStats; // below this number of human players, stats will not be updated, i.e. current game scores will be ignored
var config bool bNormaliseScores;
// var config bool bScalePlayerScoreToFullTime; // After much consideration, I got close to implementing this.  But my final argument is: Why should a player be punished because they didn't play the game from the start?  Answer: Because there was a 50:50 chance that they made teams uneven when they joined, becoming the extra man on a team which was closely matched to the opponent.  Therefore it is quite likely that they will score well, because their team will be slaughtering the opponents.
// deprecated: var config bool bDoWeightedUpdates;

// Defaults (Daniel's):
var config int UnknownStrength;    // Default strength for unknown players
// var config float UnknownMinutes;   // Initial virtual time spend on server by new players
var config int BotStrength;        // Default strength for bots
var config int FlagStrength;       // Strength modifier for captured flags
var config int WinningTeamBonus;   // Players on the winning team get these bonus points at the end of the game (they contribute to stats)
var config bool bClanWar;          // Make teams by clan tag
var config string clanTag;         // Clan tag of red team (all other players to blue)
// var config String RedTeam[16];     // Players on red team (unreferenced)
// var config String BlueTeam[16];    // Players on blue team (unreferenced)

// For storing player strength data:
var int MaxPlayerData; // The value 4096 is used in the following array declarations and the defaultproperties, but throughout the rest of the code, MaxPlayerData can be used to save duplication lol
var config String playerData[4096]; // String-format of the player data stored in the config (ini-file), including ip/nick/avg_score/time_played data

// Internal (parsed) player data:
var bool CopyConfigDone; // set to true after the arrays have been populated (so we don't do it twice)
var String ip[4096]; // We could consider using instead the default struct Guid { var int A, B, C, D; };
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
var int lastBalanceTime;

defaultproperties {
  HelloBroadcast="AutoTeamBalance (beta) is attempting to balance the teams"
  bBroadcastStuff=True
  bDebugLogging=False      // DONE: for release, recommended False (some logging is ok tho!)
  bBroadcastCookies=False   // DONE: for release, recommended False (it's fun and useful for debugging, but not that great :P )
  // bOnlyMoreCookies=False
  bLetPlayersRebalance=True // TODO: default this to false for release? (nahhh, just false on XOL :P)
  MinSecondsBeforeRebalance=20 // must be at least 1, to avoid a bug with multiple calls to MutatorTeamMessage
  AdminPassword="defaults_to_admin_pass"
  bAutoBalanceTeamsForCTF=True
  bAutoBalanceTeamsForTDM=True
  bAutoBalanceTeamsForAS=True
  bAutoBalanceTeamsForOtherTeamGames=True
  // BalanceTeamsForGameTypes="CTFGame,TeamGamePlus,JailBreak,*"
  bUpdatePlayerStatsForCTF=True
  bUpdatePlayerStatsForTDM=True // If you are normalising scores, then updating stats for TDM should be ok.  But if you are not normalising scores, then the different bonuses in CTF will make stats from the different gametypes incompatible.  (Basically TDMers will get lower strengths because they never get the bonus points from caps/covers/etc.)  So in this case you are recommended only to build stats for your server's most popular gametype.
  bUpdatePlayerStatsForAS=False  // Probably best left False (unless you are running an AS-only server) because AS scores are crazy (one guy gets 100 for the last objective, even though it was a team effort)
  bUpdatePlayerStatsForOtherTeamGames=False
  bUpdatePlayerStatsForNonTeamGames=False // TODO: could happily be true; DM scores are a good indication of a player's strength
  // UpdateStatsForGameTypes="CTFGame"
  // bUpdateStatsForCTFOnly=True
  // OnlyUpdateStatsIfGametypeIsA='CTFGame' // Would have been nice to offer it this way, but I didn't get it working.
  // OnlyBalanceTeamsIfGametypeIsA='TeamGamePlus' // TODO: we CAN do it this way, e.g. using String(gametype.Class) == "Botpack.Assault"
  bLogEndStats=True
  // PollMinutes=2.4
  MaxHoursWhenCopyingOldRecord=2.0
  // MaxPollsBeforeRecyclingStrength=200 // I think for a returning player with a previous average of 100(!), and a new skill of around 50, and with 24 polls an hour and MaxPollsBeforeRecyclingStrength=100, after 100 more polls (4 more hours), the player's new average will look like 60.5.  That seems too quick for me, so I've gone for 200.  ^^  btw this maths is wrong :| but approx i guess
  HoursBeforeRecyclingStrength=12.0
  MinHumansForStats=4     // DONE: for release, recommended 4
  bNormaliseScores=True     // Normalise scores so that the average score for every game is 50.  Recommended for servers where some games end with very high scores and some not (e.g. if you have different styles of map and game-modes, like mixing normal weapons clanwar maps with instagib action maps).  You can turn this off if your server has a fixed mapcycle and always the same game-mode.  Normalising results in a *relative* ranking of players who play the same games.  Not normalising would be better for separating weak and strong players who never actually played together.  If you have 10 strong players getting high scores on one game, and 10 noobs getting low scores during a different game, normalising would actually put the strongest noob up with the strongest pwnzor.  TODO CONSIDER: would it be a useful compromise to "half-normalise"?  And how would we do that?  I think some logarithmic maths might be required.
  // bScalePlayerScoreToFullTime=True
  // deprecated: bDoWeightedUpdates=False  // Untested experimental stats updating method
  UnknownStrength=50      // New player records start with an initial strength of 50 (when scores are normalised, this is the average)
  // UnknownMinutes=10       // New player records start with a virtual 10 minutes of time played already
  BotStrength=20
  FlagStrength=50         // If it's 3:0, the winning team will get punished an extra 150 points; used when new players join the game and number of players on each team are even; TODO: could also be used when doing mid-game "!teams" balance
  WinningTeamBonus=10
  bClanWar=False
  MaxPlayerData=4096
  // bHidden=True // what is this?  iDeFiX says it's only needed for ServerActors
}



// ==== Daniel's HOOKS / OVERRIDES ==== //

// If AutoTeamBalance was added as both a ServerActor and a Mutator, we only want one to run.
// The Mutator's PostBeginPlay() gets called first, so we force the ServerActor to disable itself.

// Initialize the system
function PostBeginPlay() {

  Super.PostBeginPlay();

  if (initialized) {
    if (bDebugLogging) { Log(Self$".PostBeginPlay() called with initialized already true; quitting."); }
    return;
  }

  if (bDebugLogging) { Log(Self$".PostBeginPlay() initialising"); }

  // If AutoTeamBalance was included as a ServerActor, register it as a game mutator:
  // I was getting an infinite recursion error, so I removed this.
  // Maybe because I had already added the mutator
  // Maybe this was designed for ServerActor
  // Before uncommenting, consider moving the initialized=true; to the line before.
  // DONE: turn this into add-if-not-already-added, and we have ourself a mutator/serveractor

  // If AutoTeamBalance was installed as a ServerActor, we need to register it as a mutator:
  // Made safe by our custom implementation of AddMutator below
  Level.Game.BaseMutator.AddMutator(Self);
  if (initialized) { // Another instance of the AutoTeamBalance mutator has notified me that I need/should not activate.
    if (bDebugLogging) { Log(Self$".PostBeginPlay() disabling self on request"); }
    gameStartDone=True;
    Disable('Tick');
    return;
  }
  if (bDebugLogging) { Log(Self$".PostBeginPlay() added self as mutator"); }

  // btw just interesting to note: if i have AutoTeamBalance as a ServerActor and a mutator on startup arguments, the mutator gets its PostBeginPlay called before the ServerActor's, and the ServerActor mut doesn't seem to get called at all.

  // NOTE: If I move the "if (initialized) return;" down to here, then the ServerActor AutoTeamBalance0 destroys the mutator AutoTeamBalance1
  //       I also get a couple of AddMutator Accessed None errors in the log.
  // If I leave it high, then the Mutator effectively turns off the ServerActor, by setting initialized=true before the ServerActor gets called.
  // Er no that's such a lie, the position of "if (initialized) return;" makes no difference!!
  // So presumably the Mutator does just switch off the ServerActor.

  // NOTE: one big disadvantage of using ServerPackages, seems to be that the .u is sent to the client (maybe for simulation purposes),
  //       and then successive releases get a version mismatch.  :f
  //       Or was that more related to the .u symlink included in my local client UT install?
  //       Anyway, I don't think it ever needs to be a ServerPackage, but some admins may prefer to install it as a ServerActor than a Mutator.

  // TODO: When I was testing both ServerActor and mutator, it seemed "!teams" was not working

  if (bLetPlayersRebalance) {
    Level.Game.RegisterMessageMutator(Self);
    if (bDebugLogging) { Log(Self$".PostBeginPlay() registered self as messenger"); }
  }

  // timeGameStarted = Level.TimeSeconds; // the game does not actually start until players and DoGameStart() is called.  This has been moved down.

  gameEndDone = false; // Kinda redundant, since it will have been default initialised to false anyway.

  // LoadConfig(); // TODO CONSIDER: If possible, we could also LoadConfig() here.  Then this mutator would be the only one I know that lets you edit the ini file without needing to restart the server!  OK well apparently LoadConfig() doesn't exist.  :P
  CopyConfigIntoArrays();  // First time the data is needed, we must convert it.

  initialized = true;

}

// TESTING
event PostLogin(PlayerPawn newPlayer) {
  if (bDebugLogging) { Log("event "$Self$".PostLogin("$newPlayer$") was called."); }
}

// TODO CONSIDER: if we make the rollback of old scores very fast, and put more weight into having won, then everyone should get an even number of wins.
//                if a player gets in 4 lost matches in a row, he will soon be placed on a strong team against a bunch of noobs who have been lucky enough to win some recent and are about to get their balance by getting pwned.  ^^
//                however, this is not the final goal: players who have put effort into learning to play UT should be rewarded for their effort: their effort and hopefully therefore increased skill should pay off in their ability to win games (and frag, score highly).
//                Thinking of the XOL ranking, it seemed that players who just spent tonnes of time on the server always got high in the rankings (i don't mean total score, i mean in-game scoreboards and FPH/SPH).  This seems reasonable, given that as well as skill, there was much to learn, with a rotation of a large set of large maps, with regular new maps which nobody had seen.
//                Indeed one way to keep noobs on the server, is to reward them for playing for longer.  (Possibly simply by learning noobish things about the crazy maps, but 
//                This could in fact be artificially encouraged, by using the teambalance to put players who have spent many hours on the server on the same team as strong players who would help them to win (and hence score highly).
//                (This situation would actually mean that the longer a person plays, the worse their teambalance rating would become.)
//                Maybe all these CTF reward bonuses (XOL's and SmartCTF's) actually only cause the team that win (own) to score even more highly than they deserve.  (This took random actions, but because of a slightly high overall skill, they managed to make covers and caps and all score well.)  <-- wtf does this mean?! i thought there was a valid point here! ;)

// Implementation of AddMutator which prevents double or recursive adding:
function AddMutator(Mutator Other) {
  if (bDebugLogging) { Log(Self$".AddMutator("$Other$") called."); }
  if (Other != None && Other.Class == Self.Class) { // The check for None prevents some "Accessed: None" errors in the logs.
    // Check whether this mutator has already been added.
    if (Other == Self) {
      // This exact instance has already been added.  (When does this happen?  When we've been included as a mutator rather than a server actor?)
      if (bDebugLogging) { Log(Self$".AddMutator("$Other$"): not adding mutator self again!"); }
    } else {
      // This is a different instance of the same mutator.  (Maybe the admin made it a ServerActor *and* added it as a mutator.)
      // We won't add the second instance, so we may as well destroy it:
      if (bDebugLogging) { Log(Self$".AddMutator("$Other$"): destroying other instance with "$Other$".Destroy()"); }
      AutoTeamBalance(Other).initialized = true; // tell the other mutator (probably the serveractor) it should not initialize
      Other.Destroy(); // seems to do nothing useful; the mutator continues to run through PostBeginPlay().
    }
  } else {
    // This is another mutator entirely, do what we would normally do
    Super.AddMutator(Other);
  }
}

// Check if game is about to start, and if so, balance the teams before it does.  (We want to do this as late as possible before the game starts, to give players time to join the server.)
event Tick(float DeltaTime) {
  if (!gameStartDone) CheckGameStart(); // this the boolean check should be efficient, and switched off before the game starts, but anyway we Disable('Tick()');
}

function bool ShouldBalance(GameInfo game) {

  // DONE: AS is a teamgame, so ok to balance, BUT ONLY on the first half-game.
  //       Teams should remain the same for the second half-game.  (I fear if it does the balancing for both half-games, the same ppl will be defending twice!)  OK well it wasn't SO bad, because Red attacks the first half, defends the second half.
  // DONE: make AS a separate option in the booleans above; atm it's classified under Other.

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
  if (String(Level.Game.Class) == "Botpack.Assault") {
    // Do not balance AS game if we're in the second half of the game
    if (Assault(Level.Game).Part != 1)
      return False;
    else
      return bAutoBalanceTeamsForAS;
  }

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
  // if (bDebugLogging) { Log("AutoTeamBalance.ShouldBalance("$game$") Game.Name="$Game.Name$" Game.Class="$Game.Class$" returning "$bAutoBalanceTeamsForOtherTeamGames); }
  return bAutoBalanceTeamsForOtherTeamGames;
}

function bool ShouldUpdateStats(GameInfo game) {
  // if (bDebugLogging) { Log("AutoTeamBalance.ShouldUpdateStats("$game$") Game.Name="$Game.Name$" Game.Class="$Game.Class$""); }
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
  // It's not a team game.  Build stats because it's a non teamgame?  (For admins more interested in player stats than balancing teams., or if you think stats from DM games are useful)
  return bUpdatePlayerStatsForNonTeamGames;
}

// TESTING Alternative, there is: event GameEnding() { ... } implemented in GameInfo.  Can we drop our own event catcher in here, without overriding the other?
// or ofc we can use a timer and check Level.Game.bGameEnded but the timer mustn't do this twice at the end of one game. :P
// From testing this: it never gets called!  (Maybe we must somehow register to receive events.)
// event GameEnding() {
  // if (bDebugLogging) { Log("AutoTeamBalance.GameEnding() even was CALLED!  bOverTime="$Level.Game.bOvertime$" bGameEnded="$Level.Game.bGameEnded); }
// }

// Checks if the game has begun (CONSIDER/TEST: optimise this and move/inline the check right into Tick() above.  ATM it is called every tick, although exits fast.)
function CheckGameStart() {
  local int c,n,e;

  // if (gameStartDone) return; // This was moved up to Tick()

  // this mod works on team games only
  // there is only a countdown in deathmatchplus games
  // but anyway we will only balance or rate on a teamgameplus
  // we now check this in ShouldBalance
  // if (!bAutoBalanceTeams || !Level.Game.IsA(OnlyBalanceTeamsIfGametypeIsA) || !Level.Game.GameReplicationInfo.bTeamGame || DeathMatchPlus(Level.Game).bTournament) {
  // Previously we were only checking ShouldBalance, but since ShouldUpdateStats uses timeGameStarted, we still need to wait for the gamestart if we are updating stats but not balancing.  The check for balancing has been moved below.
  if (!ShouldBalance(Level.Game) && !ShouldUpdateStats(Level.Game)) { // We do this early, to check at the very least that this is a teamgame
    gameStartDone=True;
    Disable('Tick');
    return;
  }

  // Read starting countdown
  c = TeamGamePlus(Level.Game).countdown;
  n = TeamGamePlus(Level.Game).NetWait;
  e = TeamGamePlus(Level.Game).ElapsedTime;
  // if (bDebugLogging) { Log("AutoTeamBalance.CheckGameStart(): c="$c$" n="$n$" e="$e$" t="$Level.TimeSeconds$""); }
  c = Min(c,n-e);

  // Initialize teams 2 seconds before the game starts:
  if (c<2) {
  // if (Level.TimeSeconds >= 20) {
    DoGameStart();
    gameStartDone=True;
    Disable('Tick');
  }

  // Log("AutoTeamBalance.CheckGameStart(): Done.");  // Too noisy; gets called many times before the balancing+start

}

function DoGameStart() {
  timeGameStarted = Level.TimeSeconds+2; // (since we are called 2 seconds before starting countdown ends)
  if (ShouldBalance(Level.Game)) {
    if (bBroadcastStuff) { BroadcastMessageAndLog(HelloBroadcast); }
    ForceFullTeamsRebalance();
  }
}

// Balance the teams just before the start of a new game.  No need for FlagStrength here.
// This was originally Daniel's InitTeams() method, but I have renamed it.
// It can now be called mid-game using "mutate teams <pass> full"
// In this case, it doesn't check which players are holding flags, but hopefully the flag will at least drop or return when the player is restarted.
// BUG TODO: could be unfair if one team drops flag because their FC is switched, but the other team's FC keeps his.  :P
function ForceFullTeamsRebalance() {
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

  // DONE: now that this can be run mid-game by saying "!teams", this fn should again check whether it's ok to balance (e.g. is this a team game?!)
  // We can't balance if it's not a teamgame
  if (!Level.Game.GameReplicationInfo.bTeamGame) return;

  if (bDebugLogging) { Log("AutoTeamBalance.ForceFullTeamsRebalance(): Running..."); }

  // rate all players, and put them in a temporary structure:
  for (p=Level.PawnList; p!=None; p=p.NextPawn)
  {
    // ignore non-player pawns
    if (p.bIsPlayer && !p.IsA('Spectator') && !p.IsA('Bot'))
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
      if (bDebugLogging) { Log("AutoTeamBalance.ForceFullTeamsRebalance(): Player " $ p.getHumanName() $ " on team " $ p.PlayerReplicationInfo.Team $ " has ip+port " $ PlayerPawn(p).GetPlayerNetworkAddress() $ " and score " $ p.PlayerReplicationInfo.Score $ "."); }
      // if (bBroadcastCookies && !bOnlyMoreCookies) { BroadcastMessageAndLog("" $ p.getHumanName() $ " has " $st$ " cookies."); }
    }
  }

  // sort players by strength (move them out of the structure, into plorder[])
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
      if (bDebugLogging) { Log("AutoTeamBalance.ForceFullTeamsRebalance(): [Ranking] "$ps[pid]$" "$ pl[pid].getHumanName() $""); }
      // if (bBroadcastCookies) { BroadcastMessageAndLog(""$ pl[pid].getHumanName() $" has " $ps[pid]$ " cookies."); }
      // if (bBroadcastCookies) { SendClientMessage(pl[pid],"You have " $ps[pid]$ " cookies."); } // gets hidden by team switches below
    }
  } until (pid==-1);

  // save team changing rules before we override them
  g=TeamGamePlus(Level.Game);
  oldMaxTeamSize=g.MaxTeamSize;
  oldbPlayersBalanceTeams=g.bPlayersBalanceTeams;
  oldbNoTeamChanges=g.bNoTeamChanges;
  // nogginBasher: i never actually worked out what bBalanceTeams and bPlayersBalanceTeams does.
  //               note that some parts of this mod assume/require/are-weird-without bBotsBalanceTeams = true

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
      ChangePlayerToTeam(pl[pid],teamnr);
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
      if (bDebugLogging) { Log("AutoTeamBalance.ForceFullTeamsRebalance(): i="$i$" Putting pid="$pid$" pl="$pl[pid].getHumanName()$" into team "$teamnr$"."); }
      ChangePlayerToTeam(pl[pid],teamnr);
      teamstr[teamnr]+=ps[pid];
    }

    // if there is an odd number of players put the last player in the weaker team
    if ((n&1)==1)
    {
      pid=plorder[i];
      teamnr=0; if (teamstr[0]>teamstr[1]) teamnr=1;
      if (bDebugLogging) { Log("AutoTeamBalance.ForceFullTeamsRebalance(): "$n$" is odd so sending last player to WEAKER team "$teamnr$"."); }
      ChangePlayerToTeam(pl[pid],teamnr);
      teamstr[teamnr]+=ps[pid];
    }

  }

  // restore team changing rules
  g.MaxTeamSize=oldMaxTeamSize;
  g.bPlayersBalanceTeams=oldbPlayersBalanceTeams;
  g.bNoTeamChanges=oldbNoTeamChanges;

  // Show team strengths to all players
  // Log("AutoTeamBalance.ForceFullTeamsRebalance(): Red team strength is " $ teamstr[0] $ ".  Blue team strength is " $ teamstr[1] $ ".");
  // Human strength:
  // if (bBroadcastStuff) { BroadcastMessageAndLog("Red team strength is now "$Int(GetTeamStrengthWithoutBotsOrCaps(0))$".  Blue team strength is "$Int(GetTeamStrengthWithoutBotsOrCaps(1))$"."); }
  // Non-human strength: But then, this was non-human balancing!
  if (bBroadcastStuff) { BroadcastMessageAndLog("Red team strength is " $ teamstr[0] $ ".  Blue team strength is " $ teamstr[1] $ "."); }

  // Little point doing this here; wait until we update the player strengths.
  // CopyArraysIntoConfig();
  // SaveConfig();

  // At game start, tell each player how many cookies they have
  if (!gameStartDone) {
    // (This must come after the team switching, otherwise the default start-game "xxx is on Red" will overwrite this text.)
    // TODO CONSIDER BUG: isn't it more important that the player sees what team they were moved to?!
    for (p=Level.PawnList; p!=None; p=p.NextPawn) {
      if (bBroadcastCookies && p.bIsPlayer && !p.IsA('Spectator') && !p.IsA('Bot')) {
        SendClientMessage(p, p.getHumanName() $", you have "$ GetPawnStrength(p) $" cookies.");
      }
    }
  }

}

// If a new player joins a game which has already started, this will send him to the most appropriate ("weaker") team (based on summed strength of each team, plus capbonuses).
// TODO: can cause lag on the server when a new player joins; consider using current game scores, or just caps, rather than looking up player stats
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

  if (!gameStartDone) return;

  if (!ShouldBalance(Level.Game)) return;

  if (bDebugLogging) { Log("AutoTeamBalance.ModifyLogin()"); }

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
  // TODO BUG: is it really fair to count bot strengths?  Once this player joins the server, one of the bots will probably leave.
  for (p=Level.PawnList; p!=None; p=p.NextPawn)
  {
    // ignore non-player pawns
    if (p.bIsPlayer && !p.IsA('Spectator'))
    {
      teamnr=p.PlayerReplicationInfo.Team;
      if (teamnr<2)
      {
        if (!p.IsA('Bot')) {
          teamSize[teamnr]++;
          teamStr[teamnr] += GetPawnStrength(p);
          // I changed this from Daniel's version, so that bot strengths are not considered.  (Chances are one of the bots is going to leave anyway; we only really want to balance human players.)
        }
        teamSizeWithBots[teamnr]++;
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
      if (bDebugLogging) { Log("AutoTeamBalance.ModifyLogin(): "$teamSize[0]$"v"$teamSize[1]$" so sending new player to WEAKER team "$getTeamName(teamnr)$"."); }
    } else {
      // send player to the team with fewer players
      teamnr=0; if (teamSize[0]>teamSize[1]) teamnr=1;
      if (bDebugLogging) { Log("AutoTeamBalance.ModifyLogin(): "$teamSize[0]$"v"$teamSize[1]$" so sending new player to SMALLER team "$getTeamName(teamnr)$"."); }
    }

  }

  // if selected team does not equal forced team then modify login
  if (teamnr!=selectedTeam) Options="?Team=" $ teamnr $ Options;

  // fix teamsize bug in Botpack.TeamGamePlus
  if (GRI.Teams[0].Size!=teamSizeWithBots[0] || GRI.Teams[1].Size!=teamSizeWithBots[1])
  {
    if (bDebugLogging) { Log("AutoTeamBalance.ModifyLogin(): Fixing team size (" $ GRI.Teams[0].Size $ "," $ GRI.Teams[1].Size $ ") should be (" $ teamSizeWithBots[0] $ "," $ teamSizeWithBots[1] $ ")"); }
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

function ChangePlayerToTeam(Pawn p, int teamnum) {
  // Note: if ForceFullTeamsRebalance() is invoked mid-game; it's possible that this player is already on this team, in which case don't switch.
  if (teamnum == p.PlayerReplicationInfo.Team) {
    if (bDebugLogging) { Log("AutoTeamBalance.ChangePlayerToTeam("$p.getHumanName()$","$teamnum$"): doing nothing since player is already on team "$teamnum); }
    return;
  }
  if (bDebugLogging) { Log("AutoTeamBalance.ChangePlayerToTeam("$p.getHumanName()$"): "$p.PlayerReplicationInfo.Team$" -> "$teamnum); }
  Level.Game.ChangeTeam(p,teamnum);
  Level.Game.RestartPlayer(p); // i thought by doing this even before the game had started, it might fix problems with the player's team getting confused by the server; i don't think it worked, but it didn't do any harm either
  if (gameStartDone) {
    // Level.Game.RestartPlayer(p);
    // SendClientMessage(p,"You have been moved to the "$getTeamName(teamnum)$" team.");
    // SendClientMessage(p,"YOU have been MOVED to the >> "$getTeamName(teamnum)$" << team for a fairer game.");
    SendClientMessage(p,"You are now on the "$getTeamName(teamnum)$" team.");
    p.ShakeView(2.0,2000.0,0.0);
  }
}

function MidGameRebalance() {
  local int redTeamCount,blueTeamCount;

  // if (bDebugLogging) { Log("MidGameRebalance() tgp="$Level.Game.IsA('TeamGamePlus')$" bTG="$Level.Game.bTeamGame); }

  // Surely ShouldBalance() has already been tested.  No, because mid game teambalance is called by player or admin.
  if (!Level.Game.IsA('TeamGamePlus') || !Level.Game.bTeamGame)
    return;

  // Refuse to balance teams more than once every MinSecondsBeforeRebalance seconds:
  // This also fixed the bug that if the player who said "!teams" was switched, a second call to MutatorTeamMessage was made, and MidGameRebalance was getting called again.
  if (lastBalanceTime + MinSecondsBeforeRebalance > Level.TimeSeconds) {
    if (bDebugLogging) { Log("MidGameRebalance() refusing to rebalance since lastBalanceTime="$lastBalanceTime$" is too close to current time "$Level.TimeSeconds); }
    return;
  }
  lastBalanceTime = Level.TimeSeconds;

  // CONSIDER: team sizes are not so important as team strengths.
  // we could always do MidGameTeamBalanceSwitchTwoPlayers, but with the option of one of those players being "None"
  // in the case of 3 elites versus 4 noobs, this should swap the best elite for the worst noob

  // DONE: need count WITHOUT bots!
  // redTeamCount = TeamGamePlus(Level.Game).Teams[0].Size;
  // blueTeamCount = TeamGamePlus(Level.Game).Teams[1].Size;
  redTeamCount = CountHumansOnTeam(0);
  blueTeamCount = CountHumansOnTeam(1);
  // We assume bot skills are pretty much irrelevant, and the bots will auto-switch to balance teams after we move any players around.

  if (bDebugLogging) { Log("MidGameRebalance() "$redTeamCount$" v "$blueTeamCount$""); }

  // TODO: what if redTeamCount << blueTeamCount ?  e.g. it's 6v2 so we need to move to players.  we could balance in a while loop if it's guaranteed to end
  if (redTeamCount < blueTeamCount) {
    MidGameTeamBalanceSwitchOnePlayer(1,0);
  } else if (blueTeamCount < redTeamCount) {
    MidGameTeamBalanceSwitchOnePlayer(0,1);
  } else {
    MidGameTeamBalanceSwitchTwoPlayers();
  }

}

function int CountHumansOnTeam(int team) {
  local int count;
  local Pawn p;
  count = 0;
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (isHumanPlayer(p) && p.PlayerReplicationInfo.Team == team) count++;
  }
  return count;
}

function bool isHumanPlayer(Pawn p) {
  return p.bIsPlayer && p.bIsHuman && !p.IsA('Spectator') && !p.IsA('Bot'); // seems this check for bot is needed!
}

function String getTeamName(int teamNum) {
  return TeamGamePlus(Level.Game).Teams[teamNum].TeamName;
}

function bool MidGameTeamBalanceSwitchOnePlayer(int fromTeam, int toTeam) {
  local float fromTeamStrength, toTeamStrength, difference, playerStrength;
  local Pawn p;
  local Pawn closestPlayer; // the most ideal potential player to switch
  local float closest; // the absolute strength difference between the two teams after the potential switch
  fromTeamStrength = GetTeamStrengthWithoutBotsOrCaps(fromTeam);
  toTeamStrength = GetTeamStrengthWithoutBotsOrCaps(toTeam);
  difference = fromTeamStrength - toTeamStrength;
  if (difference<0) {
    if (bBroadcastStuff) { BroadcastMessageAndLog("AutoTeamBalance not adjusting teams because smaller team looks stronger."); }
    return False;
  }
  // closestPlayer = None; // not needed, i believe
  // Find the player on fromTeam with strength closest to difference, and switch him/her
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.bIsPlayer && !p.IsA('Spectator') && !p.IsA('Bot')
        && p.PlayerReplicationInfo.Team==fromTeam && p.PlayerReplicationInfo.HasFlag==None) {
      playerStrength = GetPawnStrength(p);
      // Note we multiply playerStrength by 2 here, because switching him will cause -strength to fromTeam and +strength to toTeam.
      if (closestPlayer == None || Abs(playerStrength*2-difference) < closest) {
        closestPlayer = p;
        closest = Abs(playerStrength*2-difference);
      }
    }
  }
  if (closestPlayer == None) {
    if (bBroadcastStuff) { BroadcastMessageAndLog("AutoTeamBalance could not find any player on "$getTeamName(fromTeam)$" to switch."); }
    return False;
  }
  if (closest >= difference) {
    if (bBroadcastStuff) { BroadcastMessageAndLog("AutoTeamBalance not switching "$closestPlayer.getHumanName()$" because that would make "$getTeamName(fromTeam)$" team too weak!"); }
    return False;
  } else {
    ChangePlayerToTeam(closestPlayer,toTeam);
    if (bBroadcastStuff) { BroadcastMessageAndLog("Red team strength is now "$Int(GetTeamStrengthWithoutBotsOrCaps(0))$", Blue team strength is "$Int(GetTeamStrengthWithoutBotsOrCaps(1))$"."); }
    return True;
  }
}

function bool MidGameTeamBalanceSwitchTwoPlayers() {
  // initial:
  local float redTeamStrength, blueTeamStrength, difference;
  // during loop:
  local Pawn redP,blueP;
  local float redPStrength, bluePStrength;
  local float potentialNewDifference; // the strength difference between the two teams after switching these two players
  // best found:
  local Pawn redPlayerToMove,bluePlayerToMove; // the best two players found so far
  local float newdifference; // the strength difference between the two teams after switching these players

  redTeamStrength = GetTeamStrengthWithoutBotsOrCaps(0);
  blueTeamStrength = GetTeamStrengthWithoutBotsOrCaps(1);
  difference = blueTeamStrength - redTeamStrength; // positive implies Team 1 is stronger than Team 0
  newdifference = difference;

  // These repeated calls to GetPawnStrength() are going to be inefficient, possibly causing some lag while the server calculates.
  for (redP=Level.PawnList; redP!=None; redP=redP.NextPawn) {
    for (blueP=Level.PawnList; blueP!=None; blueP=blueP.NextPawn) {
      if (isHumanPlayer(redP) && isHumanPlayer(blueP) && redP != blueP
          && redP.PlayerReplicationInfo.HasFlag == None
          && blueP.PlayerReplicationInfo.HasFlag == None
      ) {
        redPStrength = GetPawnStrength(redP);
        bluePStrength = GetPawnStrength(blueP);
        // Note we multiply playerStrength by 2 here, because switching him will cause -strength to fromTeam and +strength to toTeam.
        potentialNewDifference = blueTeamStrength + redPStrength*2 - redTeamStrength - bluePStrength*2;
        if (Abs(potentialNewDifference) < Abs(newdifference)) {
          newdifference = potentialNewDifference;
          redPlayerToMove = redP;
          bluePlayerToMove = blueP;
        }
      }
    }
  }

  if (redPlayerToMove != None && bluePlayerToMove != None) {
    ChangePlayerToTeam(redPlayerToMove,1);
    ChangePlayerToTeam(bluePlayerToMove,0);
    if (bBroadcastStuff) { BroadcastMessageAndLog("Red team strength is now "$Int(GetTeamStrengthWithoutBotsOrCaps(0))$", Blue team strength is "$Int(GetTeamStrengthWithoutBotsOrCaps(1))$"."); }
    return True;
  } else {
    if (bBroadcastStuff) { BroadcastMessageAndLog("AutoTeamBalance could not find two switches to improve the teams."); }
    return False;
  }
}

function float GetTeamStrengthWithoutBotsOrCaps(int teamNum) {
  local Pawn p;
  local float strength;
  strength = 0;
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.bIsPlayer && !p.IsA('Spectator') && !p.IsA('Bot') && p.PlayerReplicationInfo.Team == teamNum) {
      strength += GetPawnStrength(p);
    }
  }
  // if (bDebugLogging) { Log(getTeamName(teamNum)$" team has human strength "$strength); }
  return strength;
}

// Returns the strength of a player or a bot
// Why do we ever want bot strengths?  Most of the time we intend to balance the players, then let rubbish bots automatically fill the empty slot(s).  We assume they are not very dangerous bots.
function int GetPawnStrength(Pawn p) {
  local int st;

  if (p.IsA('PlayerPawn') && p.bIsHuman)
  {
    // a human player - get his strength
    st = GetPlayerStrength(PlayerPawn(p));
  } else {
    // a bot - use default strength
    st = BotStrength;
  }

  if (bDebugLogging) { Log("AutoTeamBalance.GetPawnStrength(" $ p $ "): " $ st $ ""); }

  return st;
}

// Returns the strength of a player
function int GetPlayerStrength(PlayerPawn p) {
  local int found;
  found = FindPlayerRecord(p);
  if (found == -1) {
    // TODO: if gameStartDone and/or gametime>1minute then guess the player's strength from their current score
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
  local String args[255];

  // Now that I'm calling this from MutatorTeamMessage as well as InitTeams via Tick, I cache whether or not it's already been done:
  // if (CopyConfigDone)
    // return;

  CopyConfigDone=True;

  if (bDebugLogging) { Log("AutoTeamBalance.CopyConfigIntoArrays() running"); }
  for (i=0; i<MaxPlayerData; i++) {

    /*
    data = playerData[i];
    if (data == "")
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
    */

    if (playerData[i] == "") continue;
    SplitString(playerData[i]," ",args);
    ip[i] = args[0];
    nick[i] = args[1];
    avg_score[i] = Float(args[2]);
    hours_played[i] = Float(args[3]);

  }
  if (bDebugLogging) { Log("AutoTeamBalance.CopyConfigIntoArrays() done"); }
}

// From ip[],nick[],avg_score[],... to playerData[] (should be done before SaveConfig())
function CopyArraysIntoConfig() {
  local int i;
  if (bDebugLogging) { Log("AutoTeamBalance.CopyArraysIntoConfig() running"); }
  for (i=0; i<MaxPlayerData; i++) {
     if (ip[i] != "") {
       playerData[i] = ip[i] $ " " $ nick[i] $ " " $ avg_score[i] $ " " $ hours_played[i];
    }
  }
  if (bDebugLogging) { Log("AutoTeamBalance.CopyArraysIntoConfig() done"); }
}

// CONSIDER: a simple method of trying to squeeze some of the largest inefficiency out of this search:
//           when a player's record is updated (or found?), move it one record up in the list (unless it's already at the top)
//           this way, the most frequent, and regular players will be nearer the top of the list, so it will be faster to retrieve their records
//           side-effect: the positions of the records will hold meta-info: each game a player plays, will move him 1 spot up on the ladder
//           players near the bottom will be infrequent or not recent records, so they are good for recycling :)
//           note A,B -> B,A -> A,B if swap order != ladder order; will moving 2 spots stop this? no
//           the meta-info will ~ HoursPlayed
// return index i into playerData[] and ip[]/nick[]/... arrays, but not always an exact player match!
function int FindPlayerRecord(PlayerPawn p) {
  local int found;
  local int i;
  local string player_nick;
  local string player_ip;

  player_nick = p.getHumanName();
  player_ip = stripPort(p.GetPlayerNetworkAddress());

  found = -1;
  for (i=0;i<MaxPlayerData;i++) {
    // Exact match! return the index immediately
    if (player_nick == nick[i] && player_ip == ip[i]) {
      found = i;
      if (bDebugLogging) { Log("AutoTeamBalance.FindPlayerRecord(p) Exact match for " $nick[i]$ ","$ip[i]$": ["$found$"] ("$avg_score[i]$")"); }
      return found;
    } else if (player_ip == ip[i]) {
      found = i; // matching ip
      if (bDebugLogging) { Log("AutoTeamBalance.FindPlayerRecord(p) IP match for " $p.getHumanName()$ ","$stripPort(p.GetPlayerNetworkAddress())$": ["$found$"] "$nick[i]$" ("$avg_score[i]$")"); }
    } else if (player_nick == nick[i] && found == -1) {
      if (bDebugLogging) { Log("AutoTeamBalance.FindPlayerRecord(p) nick match for " $nick[i]$ ","$p.GetPlayerNetworkAddress()$": ["$found$"] "$ip[i]$" ("$avg_score[i]$")"); }
      found = i; // if not yet matching an ip, match the same nick on any ip
    }
    // TODO: if an uneven match, choose a match with more experience (hurs_played)
    // TODO: if we have little experience (<10mins) of a player, assume default score?
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
  if (bDebugLogging) { Log("AutoTeamBalance.CreateNewPlayerRecord("$p$") ["$pos$"] "$ nick[pos] $" "$ ip[pos] $" "$ avg_score[pos] $" "$ hours_played[pos] $"."); }
  // if (bBroadcastCookies) { BroadcastMessageAndLog("Welcome "$ nick[pos] $"!  You have "$ avg_score[pos] $" cookies."); }
  // if (bBroadcastCookies) { BroadcastMessageAndLog("Welcome to the server "$ nick[pos] $"!  Have a cookie.  :)"); }
  // if (bBroadcastCookies) { p.ClientMessage("Welcome to the server "$ nick[pos] $"!  Have a cookie.  :)", 'CriticalEvent', True); }
  // TODO: disabled to reduce lag at mid-game joins: if (bBroadcastCookies) { SendClientMessage(p,"Welcome to the server "$ nick[pos] $"!  Have a cookie.  :)"); }
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

// nogginBasher TESTING hook: HandleEndGame() is a Mutator function called by GameInfo.EndGame().
// OK well it did get called! :)
// BUT later mutators in the list have the right to force the game into overtime (we should pass the call onto them here I think), so it may not actually BE the end of the game!
// Maybe better just to wait with a timer until bGameEnded == True.
// Or start that timer here? ^^ (so it doesn't need to check during the game)
// Hmm from tests I found this function gets called twice for overtime games, but both times bGameEnded=False
// I hope it doesn't matter that we call SetTimer() twice; I imagine it does not start a second timer.
function bool HandleEndGame() {
  local bool b;
  // if (bDebugLogging) { Log("AutoTeamBalance.HandleEndGame() bOverTime="$Level.Game.bOvertime$" bGameEnded="$Level.Game.bGameEnded); }

  // This isn't guaranteed to be the end of the game, since we may go into overtime now.
  // But the Timer() is quite efficient (checking bGameEnded first) so shouldn't cause any lag during overtime.
  // And we have to start the Timer() here, because I have no other way of detecting (getting called at) the end of the game!
  SetTimer(10,True); // Now checking once a minute to see if game has ended; changed to 10 seconds since we lost our alternative MessageMutator hook
  if (bDebugLogging) { Log("AutoTeamBalance.HandleEndGame(): Set Timer() for 10 seconds."); }

  if ( NextMutator != None ) {
    b = NextMutator.HandleEndGame();
    // if (bDebugLogging) { Log("AutoTeamBalance.HandleEndGame() NextMutator returned "$b$"  bOverTime="$Level.Game.bOvertime$" bGameEnded="$Level.Game.bGameEnded); }
    return b;
  }
  return false;
}

// New Timer which is just looking for the end of the game
event Timer() {
  /*
  local int c,n,e,l,t,s;
  if (bDebugLogging) {
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

function CheckGameEnd() {
  if (Level.Game.bGameEnded) {
    if (gameEndDone) return;
    gameEndDone = true;
    if (ShouldUpdateStats(Level.Game)) {
      UpdateStatsAtEndOfGame();
    }
  }
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
    if (bDebugLogging) { Log("AutoTeamBalance.UpdateStatsAtEndOfGame(): not updating stats since countHumanPlayers "$countHumanPlayers$" < "$MinHumansForStats$"."); }
    return;
  }

  if (WinningTeamBonus != 0 && !DeathMatchPlus(Level.Game).bTournament) {
    GiveBonusToWinningTeamPlayers();
  }

  // Update stats for all players in game
  // if (bDebugLogging) { Log("AutoTeamBalance.UpdateStatsAtEndOfGame(): Updating player stats."); }
  if (bBroadcastStuff) { BroadcastMessageAndLog("AutoTeamBalance is updating player stats."); }
  // TEST considered when stats were being updated mid-game: make lag here on purpose and see how bad we can get it / how we can fix it.
  if (bLogEndStats) { Log("AutoTeamBalance.LogEndStats: NAME IP PING SCORE FRAGS DEATHS"); }
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.bIsPlayer && !p.IsA('Spectator') && !p.IsA('Bot') && p.IsA('PlayerPawn') && p.bIsHuman) { // lol
      UpdateStatsForPlayer(PlayerPawn(p));
    }
    if (bLogEndStats) { Log("AutoTeamBalance.LogEndStats: "$p.getHumanName()$" "$PlayerPawn(p).GetPlayerNetworkAddress()$" "$PlayerPawn(p).PlayerReplicationInfo.Ping$" "$PlayerPawn(p).PlayerReplicationInfo.Score$" ? "$PlayerPawn(p).PlayerReplicationInfo.Deaths$""); }
  }

  // Save the new stats in the config/ini file:
  Log("AutoTeamBalance.UpdateStatsAtEndOfGame(): saving stats to file");
  CopyArraysIntoConfig();
  SaveConfig(); // This is the only place this gets done atm!

  Log("AutoTeamBalance.UpdateStatsAtEndOfGame(): done");

}

function GiveBonusToWinningTeamPlayers() {
  local TeamInfo WinningTeam;
  local int i;
  local Pawn p;
  local TeamGamePlus thisTeamGame;

  // We can't find a winning team if it's not a teamgame!
  if (!Level.Game.GameReplicationInfo.bTeamGame) return;
  // if (!Level.Game.IsA('TeamGamePlus')) return;

  thisTeamGame = TeamGamePlus(Level.Game);

  // Which team won?
  // if (String(Level.Game.Class) == "Botpack.CTFGame") {
  // }

  // Copied from CTFGame.SetEndCams(), and looks functionally identical to the method in TeamGamePlus.
  for ( i=0; i<thisTeamGame.MaxTeams; i++ )
    if ( (WinningTeam == None) || (thisTeamGame.Teams[i].Score > WinningTeam.Score) )
      WinningTeam = thisTeamGame.Teams[i];
  // Check for tie:
  for ( i=0; i<thisTeamGame.MaxTeams; i++ ) {
    if ( (WinningTeam.TeamIndex != i) && (WinningTeam.Score == thisTeamGame.Teams[i].Score) ) {
      WinningTeam = None;
      break;
    }
  }

  if (WinningTeam == None) return; // game ended in a tie

  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.bIsPlayer && !p.IsA('Spectator') && p.IsA('PlayerPawn')) {
      if (p.PlayerReplicationInfo.Team == WinningTeam.TeamIndex) {
        if (bDebugLogging) { Log("AutoTeamBalance.GiveBonusToWinningTeamPlayers(): giving bonus to p.getHumanName()."); }
        p.PlayerReplicationInfo.Score += WinningTeamBonus;
        SendClientMessage(p,"You got "$WinningTeamBonus$" bonus points for finishing on the winning team.");
      }
    }
  }

}

function UpdateStatsForPlayer(PlayerPawn p) {
  local int i,j;
  local float current_score;
  local float old_hours_played;
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
    j = CreateNewPlayerRecord(p); // OLD BUG FIXED: is it inefficient to repeatedly create a PlayerPawn from the same Pawn?
    if (i > -1) {
      // Copy over strength from the partial-match player, but only make that strength last for 2 hours.
      // SO: changing nick will Not reset your avg_score immediately, but eventually
      avg_score[j] = avg_score[i]; // Copy score from partial record max
      hours_played[j] = Min(MaxHoursWhenCopyingOldRecord,hours_played[i]); // but in case this is a different player (or maybe the same player but in a different environment), give the new record max 2 hours, so it won't take long to get an accurate idea of this new player's strength
    }
    i = j;
  }

  current_score = p.PlayerReplicationInfo.Score;
  if (bNormaliseScores) {
    current_score = NormaliseScore(current_score);
  }
  // Ideally we would like to check how long this player has been on the server DONE
  gameDuration = Level.TimeSeconds - timeGameStarted;
  timeInGame = Level.TimeSeconds - p.PlayerReplicationInfo.StartTime;
  if (timeInGame>gameDuration)
    timeInGame = gameDuration;
  if (bDebugLogging) { Log("AutoTeamBalance.UpdateStatsForPlayer("$p.getHumanName()$") timeGameStarted="$timeGameStarted$" Game.StartTime="$Level.Game.StartTime$" Level.TimeSeconds="$Level.TimeSeconds$" Player.StartTime="$p.PlayerReplicationInfo.StartTime$" timeInGame="$timeInGame$" gameDuration="$gameDuration); }
  // Well if this player was only in the server for 5 minutes, we could multiply his score up so that he gets a score proportional to the other players.  (Ofc if he was lucky or unlucky, that luck will be magnified.)
  if (timeInGame < 60) { // The player has been in the game for less than 1 minute.
    if (bDebugLogging) { Log("AutoTeamBalance.UpdateStatsForPlayer("$p$") Not updating this player since his timeInGame "$timeInGame$" < 60s."); }
    return;
  }

  old_hours_played = hours_played[i];
  if (old_hours_played > HoursBeforeRecyclingStrength) {
    old_hours_played = HoursBeforeRecyclingStrength;
  }
  new_hours_played = old_hours_played + (Float(timeInGame) / 60 / 60);

  previous_average = avg_score[i];

  // TODO CONSIDER: should we give bonus points for being on the winning team?
  //                otherwise we might get a group of good team players who often have low scores but their team always wins.
  //                well that's unlikely, due to SmartCTFStats bonuses

  // Mmm we can forget all the weird weighting and just update the player's average_score_per_hour:
  if (bDebugLogging) { Log("AutoTeamBalance.UpdateStatsForPlayer(p) ["$i$"] "$p.getHumanName()$" avg_score = ( ("$avg_score[i]$" * "$old_hours_played$") + "$current_score$"/4.0) / "$(new_hours_played)); }
  avg_score[i] = ( (avg_score[i] * old_hours_played) + current_score/4.0) / new_hours_played;
  // I'm dividing every score here by 4 so that the actual averages stored in the config will be score-per-quarter-hour, which should be close to actual end-game scores (at least on my 15minute game server).  Just makes them easier to read; average scores in the stats will be around the normal.
  // We don't need to worry about how long he spent on the server wrt other players, or how long the game was.

  hours_played[i] += (Float(timeInGame) / 60 / 60);

  if (bBroadcastCookies) {
    if (avg_score[i]>previous_average+1) {
      BroadcastMessageAndLog(""$ p.getHumanName() $" has earned "$ Int(avg_score[i]-previous_average) $" cookies!");
      // p.ClientMessage("You earned "$ Int(avg_score[i]-previous_average) $" cookies this game.", 'CriticalEvent', True);
      SendClientMessage(p,"You earned "$ Int(avg_score[i]-previous_average) $" cookies this game."); // BUG: hidden by scoreboard
    }
    else if (previous_average>avg_score[i]+1) {
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

  // TODO CONSIDER: to make normalisation fair, should a player who joined late's score be scaled up by his time to the full game before normalisation? otherwise he may increase the scores of say a noob who fought a bot for 30 minutes.
  // If a player has joined the game at the very end.
  // He will reduce the overall average score, hence increasing the normalised scores of other players, and making his bad.
  // If a player played 1/2 the game, ...

  // Kinda inefficient to calculate the average once for every player, but who cares? :P
  // Could it cause confusion if a player leaves during the updating, or is that unlikely?  (Maybe this thread is synchronized wrt players leaving the game.)
  // We ignore bots scores and count, so it is irrelevant whether the bots have scored nothing, or have pwned the humans, or have performed somewhere inbetween.  Only player's relative scores are taken into account.
  averageGameScore = 0.0;
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.bIsPlayer && !p.IsA('Spectator') && !p.IsA('Bot') && p.IsA('PlayerPawn') && p.bIsHuman) { // lol
      averageGameScore += PlayerPawn(p).PlayerReplicationInfo.Score;
      playerCount++;
    }
  }
  averageGameScore = averageGameScore / Float(playerCount);

  // Avoid division-by-zero error here.  You guys got average <2 frags?  Screw you I'm not scaling that up to 50!
  if (averageGameScore < 2.0) { // TODO: kinda nasty to have such a sharp cutoff though; average 3 frags will be scaled up
    averageGameScore = 50;
  }
  // Mm so what if the average score is 3?
  // Well that suggests a short game, in which case the player's stats won't change much anyway.  But if it was a long game, and they still scored low, maybe these noobs will get rewarded more than they should.  Otoh scores may be low simply because it was 2v2 on a huge map, so reward is fair.
  // If the average is 100?  Wow high-scoring game!  Everyone's score will get scaled down.  But I guess it was a long game, so they will get the full time multiplier at least (more if it went into overtime).

  if (bDebugLogging) { Log("Normalising from average "$averageGameScore$": "$score$""); }

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
// After looking at TeamBallancer.uc, I changed this from MutatorBroadcastMessage to MutatorTeamMessage
// function bool MutatorBroadcastMessage( Actor Sender, Pawn Receiver, out coerce string Msg, optional bool bBeep, out optional name Type ) {
function bool MutatorTeamMessage(Actor Sender, Pawn Receiver, PlayerReplicationInfo PRI, coerce string Msg, name Type, optional bool bBeep) {

  // TODO TESTING TO FIX BUG: After i switched team, and did a "mutate teams  full", I couldn't make say "!teams" work any more.
  if (Sender != Receiver && bDebugLogging) {
    Log("AutoTeamBalance.MutatorBroadcast/TeamMessage(): Ignoring ("$Sender$" -> "$Receiver$") "$Msg$"");
  }
  // Mmm when the problem occurs, MutatorTeamMessage doesn't get called at all!
  // I'm sometimes getting it at the start of the map too, before making any team changes.

  if (Sender == Receiver) { // Only process the message once.

    if (bDebugLogging) { Log("AutoTeamBalance.MutatorBroadcast/TeamMessage() Checking ("$Sender$" -> "$Receiver$") "$Msg$""); }

    if (bLetPlayersRebalance && !DeathMatchPlus(Level.Game).bTournament) {
      if (Msg ~= "TEAMS" || Msg ~= "!TEAMS") {
        // DONE: check this is not called during bTournament games.
        // Log("AutoTeamBalance.MutatorBroadcast/TeamMessage(): Calling ForceFullTeamsRebalance().");
        // ForceFullTeamsRebalance();
        if (bDebugLogging) { Log("AutoTeamBalance.MutatorBroadcast/TeamMessage(): Calling MidGameRebalance()."); }
        MidGameRebalance();
      }
    }

  }

  //// This gets called, and passed to chat, BUT still gets called 8 times!!
  // Isn't this the same as calling super?
  if ( NextMessageMutator != None ) {
    return NextMessageMutator.MutatorTeamMessage( Sender, Receiver, PRI, Msg, Type, bBeep ); // commenting this out does not stop the 8 repeats
  } else {
    return true; // this seems to be what's needed to ensure the message finally reaches the game (doesn't get swallowed)
  }

}

/* Old comments from this method:

  // OK the 8 calls are because this gets called once per player, plus once for UTServerAdminSpectator, and once for ChatLogger
  // Solution: only process it when Sender = Receiver ^^
  // (Interestingly, despite having 2 bots on my team, TeamSay messages only get sent to myself!  (Maybe if there were other humans...))

    // CheckGameEnd(); // Does no harm to do this twice.  The broadcast from mapvote might make the stats parsing come sooner than waiting for the timer.

    // Failed Test: Nope that was never broadcast :P
    // if ( InStr(Msg,"game has ended.")>=0 ) {
      // Log("AutoTeamBalance.MutatorBroadcast/TeamMessage(\""$Msg$"\") detected \"game has ended.\" - DONE elsewhere: run UpdateStatsAtEndOfGame() here.");
    // }

    // if (Level.Game.bGameEnded) {
      // Log("AutoTeamBalance.MutatorBroadcast/TeamMessage(\""$Msg$"\") detected Level.Game.bEnded = True - could run UpdateStatsAtEndOfGame() here.");
    // }

    // if ( InStr(Msg," entered the game.")>=0 ) { // Can we find the new player pawn, and report his #cookies ?
      // PlayerJoinedShowInfo(Msg);
      // OK well that didn't work, apparently the player didn't have an IP address when he first joined the server.  (Maybe he gets assigned one once this stack is returned.)
    // }
    // if ( InStr(Msg,"left the game.")>=0 ) {

  // NOTE: read "8 times" below as "once per mutator".  NO don't!  It gets called 8 times even with only 4 muts!  :P

  // Allows messages through, but this fn never gets called!
  // if ( NextMessageMutator != None ) {
    // return NextMessageMutator.MutatorBroadcastMessage( Sender, Receiver, Msg, bBeep, Type );
  // } else {
    // return false;
  // }

  // As above, allows messages through, but this fn never gets called!
  // if ( NextMessageMutator != None ) {
    // return NextMessageMutator.MutatorBroadcastMessage( Sender, Receiver, Msg, bBeep, Type );
  // } else {
    // return true;
  // }

  //// Called 8 times, but never reaches game:
  // if ( NextMessageMutator != None ) {
    // return NextMessageMutator.MutatorTeamMessage( Sender, Receiver, PRI, Msg, Type, bBeep );
  // } else {
    // return false;
  // }

  //// This allows messages through, but this method doesn't seem to get called.  (Didn't it used to though? :o )
  // return Super.MutatorBroadcastMessage(Sender,Receiver,Msg,bBeep,Type);
  //// This method does get called, but 8 times :E and the messages don't reach the game :f
  // return Super.MutatorTeamMessage(Sender,Receiver,PRI,Msg,Type,bBeep);

  // BroadcastMessage(Msg);

*/

// I thought I had a problem that this was not getting called if have AutoTeamBalance as a ServerActor, but that problem has either gone now, or I was getting this confused with MutatorTeamMessage().
function Mutate(String str, PlayerPawn Sender) {
  local String args[255];
  // local array<String> args;
  local String admin_pass;

  if (bDebugLogging) { Log("AutoTeamBalance.Mutate("$str$","$sender$") was called."); }

  if (Sender.bAdmin)
    admin_pass = ""; // any or no pass is accepted
  else if (AdminPassword == "defaults_to_admin_pass")
    // admin_pass = Engine.GameInfo.AdminPassword // trying to access it directly did not work
    // admin_pass = Level.Game.ConsoleCommand("get engine.gameinfo AdminPassword");
    admin_pass = ConsoleCommand("get engine.gameinfo AdminPassword"); // local to this fn, no longer written to AutoTeamBalance.ini
  else
    admin_pass = AdminPassword;

  SplitString(str," ",args);

  if ( args[0]~="TEAMS" && ( admin_pass=="" || args[1]~=admin_pass ) ) {
    if (!Level.Game.GameReplicationInfo.bTeamGame) {
      Sender.ClientMessage("AutoTeamBalance cannot balance teams: this isn't a team game!");
    } else {
      // if (DeathMatchPlus(Level.Game).bTournament) {
      // if (args[2]~="FULL" || args[2]~="FORCE") {
      // } else {
      // }
      MidGameRebalance();
    }

  } else if ( args[0]~="FORCETEAMS" && ( admin_pass=="" || args[1]~=admin_pass ) ) {
    Sender.ClientMessage("AutoTeamBalance performing full teams rebalance...");
    // To make this balance as accurate as possible, we update the stats now, so we can use the scores from this game so-far.
    // But since this would mess up the end-game stats updating (counting this part of the game twice), we restore the stats from the config afterwards.
    // CopyArraysIntoConfig(); // Not actually needed; they should be identical at this stage
    UpdateStatsAtEndOfGame();
    ForceFullTeamsRebalance();
    CopyConfigIntoArrays();

  } else if ( args[0]~="TORED" && ( admin_pass=="" || args[2]~=admin_pass ) ) {
    ChangePlayerToTeam(FindPlayerNamed(args[1]),0);
  } else if ( args[0]~="TOBLUE" && ( admin_pass=="" || args[2]~=admin_pass ) ) {
    ChangePlayerToTeam(FindPlayerNamed(args[1]),1);

  // TODO: Experimental; comment out in final build
  // Allows semiadmins to read variables from the config files (and maybe some live variables too; untested)
  } else if (args[0]~="GET" && ( admin_pass=="" || args[3]~=admin_pass ) ) {
    Sender.ClientMessage(args[1] $ " = " $ ConsoleCommand("get " $ args[1] $ " " $ args[2]));
  /*
  // Allows semiadmins to write to variables (probably equivalent to: admin set <package> <name> <value>)
  } else if (args[0]~="SET" && ( admin_pass=="" || args[4]~=admin_pass ) ) {
    ConsoleCommand("set " $ args[1] $ " " $ args[2] $ " " $ args[3]);
    Sender.ClientMessage(args[1] $ " = " $ ConsoleCommand("get " $ args[1] $ " " $ args[2])); // read it back to the user, to check it worked
  */
  } else if ( args[0]~="SAVECONFIG" && ( admin_pass=="" || args[1]~=admin_pass ) ) {
    SaveConfig();

  } else {
    Super.Mutate(str,Sender);
  }
}

function Pawn FindPlayerNamed(String name) {
  local Pawn p;
  local Pawn found;
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.IsA('PlayerPawn') || p.IsA('Bot')) {
      if (p.getHumanName() ~= name) {
        return p;
      }
      if (Instr(Caps(p.getHumanName()),Caps(name))>=0) {
        found = p;
      }
    }
  }
  return found;
}

//// CONSIDER: we could add support for squeezing multiple delimiters into 1
// function array<String> SplitString(String str, String divider) {
// function int SplitString(String str, String divider, out array<String> parts) {
function int SplitString(String str, String divider, out String parts[255]) {
	// local String parts[255];
	// local array<String> parts;
	local int i,nextSplit;
	i=0;
	while (true) {
		nextSplit = InStr(str,divider);
		if (nextSplit >= 0) {
			// parts.insert(i,1);
			parts[i] = Left(str,nextSplit);
			str = Mid(str,nextSplit+1);
			i++;
		} else {
			// parts.insert(i,1);
			parts[i] = str;
			i++;
			break;
		}
	}
	// return parts;
	return i;
}


// I want to Log all calls to BroadcastMessage() so that I can see without playing how much the players are getting spammed by broadcasts.
// Eventually, they should be turned back to just BroadcastMessage() calls.
function BroadcastMessageAndLog(string Msg) {
  if (bDebugLogging) { Log("AutoTeamBalance Broadcasting: "$Msg); }
  BroadcastMessage(Msg);
}

function SendClientMessage(Pawn p, string Msg) {
  if (bDebugLogging) { Log("AutoTeamBalance Sending message to "$p.getHumanName()$": "$Msg); }
  // NOTE: in case you thought otherwise, this message gets displayed in the console, but not in the chatarea.  It is also displayed on the HUD, but can be hidden by the scoreboard, or a following HUD message.
  p.ClientMessage(Msg, 'CriticalEvent', True);
  // To just message the player's chat and console, try skipping the last 2 arguments.
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
  if (bBroadcastCookies) { BroadcastMessageAndLog(nick$" has "$Int(avg_score[i])$" cookies after "$hours_played[i]$" hours on the server."); }
}
*/

/*
function bool PreventDeath(Pawn Killed, Pawn Killer, name damageType, vector HitLocation) {
  Log("AutoTeamBalance.Pr.ventDeath("$Killed$","$Killer$","$damageType$")");
}

function MutatorTakeDamage( out int ActualDamage, Pawn Victim, Pawn InstigatedBy, out Vector HitLocation, out Vector Momentum, name DamageType) {
  // Log("AutoTeamBalance.MutatorTakeDamage("$Victim$","$InstigatedBy$","$DamageType$")");
}
*/

