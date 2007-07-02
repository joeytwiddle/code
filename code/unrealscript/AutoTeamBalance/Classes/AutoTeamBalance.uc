/*

// TODO: test if switching players fails when UT's MaxTeamSize is low enough.  Solve it if neccessary.

// DONE: really we want (optional) automatic balancing, when 2 players from 1 team leave the game
//       or maybe just flash a warning that teams need to be balanced
//       yes, mid-game teambalance should always be instigated by a user

// When mid-game balancing, we try to find the player whos strength will make the teams most even.  But we may wish to give priority to switching the player who most recently joined the server, if the strengths are not so relevant.

// I don't think we are storing enough stats.
// We are only storing enough stats to perform teambalance according to my algorithms.
// Since we are storing stats, we may as well generate all interesting information.
// e.g.: deaths, suicides, last_time_played, frags (as opposed to score), most_used_weapon, ...

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
// Code snippets lifted from iDeFiX's team balancer, TeamBallancer, and the adwvaad thread

// vim: tabstop=2 shiftwidth=2 expandtab

// NOTE: If your server has custom maps, it might be a good idea to increase NetWait to 20 or 30 seconds, to give each player a better chance of downloading the map and joining the game before it starts, so that player can be included in the teambalancing.  BUG: Do not use NetWait<3; it may cause the teambalance to occur before anyone joins the server!

// The field delimeter for playerData in the config files is a space " " since that can't appear in UT nicks (always _)

// HALF-DONE TODO: when the playerData array gets full, old records are not recycled properly (atm the last is just overwritten repeatedly :| )   - ok now we recycle the record with shortest play hours.  TODO: It's not perfect because the new player's stats might get overwritten pretty soon.  We either need a little randomnity, or we could store date_last_played[] (if I knew how to get a date in unrealscript)

// Done now: i shouldn't be taking averages over time, but over #polls :S  Actually either is fine, but I was doing it weirdly before.

// CONSIDER: in cases of a standoff (e.g. none of the players are found in the DB so all have UnknownStrength), choose something random!  What we are given may not be random enough (like bPlayersBalanceTeams).

// DONE: config options bRankBots and bBalanceBots (might be interesting to see how Visse compares to the humans ^^ )

// Half-Done: could also analyze TDM (DeathMatchPlus) scores, but without the CTF bonuses, these will be much lower (store in separate fields? e.g. avg_TDM_score TDM_hours_played)  What about a method to separate all teamgames?  OR Easier: make a separate player with nick+" "+ip+" "+gameType hash

// Current rankings method:
// We wait until the end of the game, then we update the stats for each player.
// We optionally normalise their score, then multiply it up to (normalised) score-per-hour, then divide by 4 to get score per 1/4 hour.
// Hence we collect each player's average (normalised) score per 15 minutes.

// So what else could we try?
// - DONE Team Cap Bonus: regardless of frags, the team which played the best CTF will get the most caps.  All players on the winning team could be rewarded for having done so, even if their actual score was pretty low.
// - The actual scores might not be useful, but the distribution of those scores might be interesting.
//   For example, the order of players on the scoreboard (imagine if both teams were merged into one) should give an idea of the relative skills of certain players.  e.g. top player gets 100 points, all other players get less, bottom player gets 10.
//   DONE: Or the relative scores could be considered.  E.g. the scores from the game could be scaled so that they always have a mean of say 50 points.  So we can still use the game scores, but the scaling will "normalise" those scores so that as much benefit comes from doing well in a low-scoring game as in a high-scoring game.
//        Ofc this would mean the best noobs get similar scores to the best el33ts, if they the noobs and leets never actually play at the same time, which kinda makes sense.  ^^

// TODO: consider adding just a little randomnity.  If we have the same 8 players on the server for 4 games in a row, and their stats don't change enough to actually switch any of their positions in the ranking, won't Daniel's initial teambalance create identical teams for every game?  Can we find a way to avoid that?  Mix up the lower skilled players a bit, since that will have least impact?
// looks like FRand() might returns a number between 0 and 1.

// TODO: throughout the code and comments i have referred to strength,avg_score,ranking,rating,stats but often meant the same thing.  Daniel stuck to "Strength" so maybe I should consolidate around that name.
// consider using a new word "judge" as the process of observing and maintaining user stats (as opposed to balancing or other features)

// Note: at the moment no attempt is made to update the strengths of players who leave the server before the end of the game.
// Only players on the server when the game ends will get their strenghts updated.
// Maybe that's undesirable.  iDeFiX's teambalancer updates stats mid-game (but i think it caches the info and does more processesing at the end of the game).

// CONSIDER: If players manage to make the teams unfair anyway, and then go and cap 10 flags 3v1, then the scores from that game will hardly be an accurate representation of what they should have been.  So should we be detecting whether teams were actually fair when we update the stats?  Well, maybe it's ok: the lamers who teamed up will get punished by unrealistically high stats, so next time they play they will get balanced with weak players on their team.  Mwuhahaha!  And the guy who got bullied will obviously need a break, so him losing some strength stats will just mean he gets some stronger team-mates in future.  :)  So I guess we don't really need to detect whether teams were fair when updating stats!

// CONSIDER: should we record lots of stats, like frags-per-hour, score-per-hour, normalised-score-per-hour, num-games-played, num-games-won, num-games-lost (diff = num-drawn or left before end)
// HALF-DONE we optionally output the extra stats to the log

*/

//=============================================================================

class AutoTeamBalance expands Mutator config(AutoTeamBalance);

var string HelloBroadcast; // TODO CONSIDER: make this configurable, and make it say nothing if ""

var config bool bBroadcastStuff;   // Be noisy to in-game console
var config bool bBroadcastCookies; // Silly way to debug; each players strength is spammed at end of game as their number of cookies
var config bool bDebugLogging;     // logs are more verbose/spammy than usual; recommended only for developers
// bLogBalancing, bLogDatabase
// TODO: now we are doing p.ClientMessage() sometimes, we don't really need to BroadcastMessage as well (I only want it as a developer to see changes during the game.)
// var config bool bOnlyMoreCookies;  // only broadcast a players cookies when they have recently increased

var config bool bLetPlayersRebalance;
var config bool bWarnMidGameUnbalance;
var config bool bAllowSemiAdminKick;
var config bool bAllowSemiAdminForceTravel;
var config bool bBalanceBots; // Include bots in the rebalancing (CONSIDER: what happens if bBalanceBots is set but bRankBots is not?)
var config bool bRankBots; // Record stats for bots as if they were players.
var config int MinSecondsBeforeRebalance;
var config String SemiAdminPass;   // Needed for mid-game rebalancing with: "mutate teams <pass>"

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

var config bool bLogExtraStats;       // in case you want to analyze the end-game stats yourself, instead of leaving that to AutoTeamBalance
// var config float PollMinutes;    // e.g. every 2.4 minutes, update the player stats from the current game
var config float MaxHoursWhenCopyingOldRecord;     // If you have lots of fakenicklamers on your server, set this high.  If not, set it low, so that players who unluckily share the same IP or nick, don't get their stats confused.
// var config int MaxPollsBeforeRecyclingStrength;    // after this many polls, player's older scores are slowly phased out.  This feature is disabled by setting MaxPollsBeforeRecyclingStrength=0 // DONE: refactored this to HoursBeforeRecyclingStrength
var config float HoursBeforeRecyclingStrength;
var config int MinHumansForStats; // below this number of human players, stats will not be updated, i.e. current game scores will be ignored
var config bool bNormaliseScores;
var config bool bScalePlayerScoreToFullTime; // After much consideration, I got close to implementing this.  But my final argument is: Why should a player be punished because they didn't play the game from the start?  Answer: Because there was a 50:50 chance that they made teams uneven when they joined, becoming the extra man on a team which was closely matched to the opponent.  Therefore it is quite likely that they will score well, because their team will be slaughtering the opponents.  (Their whole team will score better though.)    OK so I implemented it, although a little untidily in two places; maybe it's good to punish those players who join a game and score well, by giving them a high ranking and putting them on a weak team in future.  Also, even if their score is magnified by their short time in the server, that score will only count towards their ranking relative to that size of time.  :)
// deprecated: var config bool bDoWeightedUpdates;

// Defaults (Daniel's):
var config int NormalisedStrength; // The average forced when normalising scores.
var config int UnknownStrength;    // Default strength for unknown players
// var config float UnknownMinutes;   // Initial virtual time spend on server by new players
var config int BotStrength;        // Default strength for bots
var config int FlagStrength;       // Strength modifier for captured flags
var config int StrengthThreshold;
var config int WinningTeamBonus;   // Players on the winning team get these bonus points at the end of the game (they contribute to stats)
var config bool bClanWar;          // Make teams by clan tag
var config string clanTag;         // Clan tag of red team (all other players to blue)
// var config String RedTeam[16];     // Players on red team (unreferenced)
// var config String BlueTeam[16];    // Players on blue team (unreferenced)
var config bool bUseOnlyInGameScoresForRebalance;    // AKA bMidGameBalancingUsesInGameScoresNotPlayerRecords
var config bool bTesting;

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
  bWarnMidGameUnbalance=False // TODO: default this to false for release? (nahhh, just false on XOL :P) (disabled in Tournament mode)
  bAllowSemiAdminKick=True
  bAllowSemiAdminForceTravel=True
  MinSecondsBeforeRebalance=20 // must be at least 1, to avoid a bug with multiple calls to MutatorTeamMessage (or at least that used to be true, maybe not now)
  SemiAdminPass="defaults_to_admin_pass"
  bBalanceBots=False
  bRankBots=True
  bAutoBalanceTeamsForCTF=True
  bAutoBalanceTeamsForTDM=True
  bAutoBalanceTeamsForAS=True
  bAutoBalanceTeamsForOtherTeamGames=True
  // BalanceTeamsForGameTypes="CTFGame,TeamGamePlus,JailBreak,*"
  bUpdatePlayerStatsForCTF=True // Argh!  Won't this update scores from BT games too?!
  bUpdatePlayerStatsForTDM=True // If you are normalising scores, then updating stats for TDM should be ok.  But if you are not normalising scores, then the different bonuses in CTF will make stats from the different gametypes incompatible.  (Basically TDMers will get lower strengths because they never get the bonus points from caps/covers/etc.)  So in this case you are recommended only to build stats for your server's most popular gametype.
  bUpdatePlayerStatsForAS=False  // Probably best left False (unless you are running an AS-only server) because AS scores are crazy (one guy gets 100 for the last objective, even though it was a team effort)
  bUpdatePlayerStatsForOtherTeamGames=False
  bUpdatePlayerStatsForNonTeamGames=False // TODO: could happily be true; DM scores are a good indication of a player's strength
  // UpdateStatsForGameTypes="CTFGame"
  // bUpdateStatsForCTFOnly=True
  // OnlyUpdateStatsIfGametypeIsA='CTFGame' // Would have been nice to offer it this way, but I didn't get it working.
  // OnlyBalanceTeamsIfGametypeIsA='TeamGamePlus' // TODO: we CAN do it this way, e.g. using String(gametype.Class) == "Botpack.Assault"
  bLogExtraStats=False
  // PollMinutes=2.4
  MaxHoursWhenCopyingOldRecord=2.0
  // MaxPollsBeforeRecyclingStrength=200 // I think for a returning player with a previous average of 100(!), and a new skill of around 50, and with 24 polls an hour and MaxPollsBeforeRecyclingStrength=100, after 100 more polls (4 more hours), the player's new average will look like 60.5.  That seems too quick for me, so I've gone for 200.  ^^  btw this maths is wrong :| but approx i guess
  HoursBeforeRecyclingStrength=12.0
  MinHumansForStats=4     // DONE: for release, recommended 4
  bNormaliseScores=True     // Normalise scores so that the average score for every game is 50.  Recommended for servers where some games end with very high scores and some not (e.g. if you have different styles of map and game-modes, like mixing normal weapons clanwar maps with instagib action maps).  You can turn this off if your server has a fixed mapcycle and always the same game-mode.  Normalising results in a *relative* ranking of players who play the same games.  Not normalising would be better for separating weak and strong players who never actually played together.  If you have 10 strong players getting high scores on one game, and 10 noobs getting low scores during a different game, normalising would actually put the strongest noob up with the strongest pwnzor.  TODO CONSIDER: would it be a useful compromise to "half-normalise"?  And how would we do that?  I think some logarithmic maths might be required.
  bScalePlayerScoreToFullTime=True
  // deprecated: bDoWeightedUpdates=False  // Untested experimental stats updating method
  UnknownStrength=50      // New player records start with an initial strength of 50 (when scores are normalised, this is the average.  Otherwise it should be something around the average endgame-score-per-hour/4 of new players on your server.  Anyway it's only used briefly, if a new player stays until the end of the game then their stats will be generated, and this value forgotten.
  // UnknownMinutes=10       // New player records start with a virtual 10 minutes of time played already
  BotStrength=10          // maybe 20 or 30 is better, if we increase normal score to 100
  FlagStrength=20         // If it's 3:0, the winning team will get punished an extra 60 points; used when new players join the game and number of players on each team are even; DONE: could also be used when doing mid-game "!teams" balance
  StrengthThreshold=100    // If bWarnMidGameUnbalance and team strength difference is greater than this and stronger team has more player, warn those players of team inbalance.
  WinningTeamBonus=0
  bClanWar=False
  MaxPlayerData=4096
  // bHidden=True // what is this?  iDeFiX says it's only needed for ServerActors
  bUseOnlyInGameScoresForRebalance=False     // To reduce lag, do not look up player strengths from the database during gameplay, only at the start and end of the game; use in-game scores instead
  bTesting=False
}





// ==== HOOKS / OVERRIDES functions and events called externally. ==== //

// Initialize the system
function PostBeginPlay() {

  Super.PostBeginPlay();

  if (initialized) {
    if (bDebugLogging) { Log(Self$".PostBeginPlay() called with initialized already true; quitting."); }
    return;
  }

  if (bDebugLogging) { Log(Self$".PostBeginPlay() initialising"); }

  // If AutoTeamBalance was added as both a ServerActor and a Mutator, we only want one to run.
  // The Mutator's PostBeginPlay() gets called first, so we force the ServerActor to disable itself.

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

  // if (bLetPlayersRebalance) {
  // For the moment, we always want to register as a messenger, so that players may type "!red" or "!blue"
    Level.Game.RegisterMessageMutator(Self);
    if (bDebugLogging) { Log(Self$".PostBeginPlay() registered self as messenger"); }
  // }

  // This is how we detect the moment just before game-start (in CheckGameStart()), to do a final team balance:
  SetTimer(1,True);

  //// To detect the end-of-game ev.nt: (have not seen this working yet)
  // Enable('Trigger');
  // // Tag = 'EndGame';
  // Tag = 'GameEnded';

  // timeGameStarted = Level.TimeSeconds; // the game does not actually start until players and DoGameStart() is called.  This has been moved down.

  gameEndDone = false; // Kinda redundant, since it will have been default initialised to false anyway.

  // LoadConfig(); // TODO CONSIDER: If possible, we could also LoadConfig() here.  Then this mutator would be the only one I know that lets you edit the ini file without needing to restart the server!  OK well apparently LoadConfig() doesn't exist.  :P
  CopyConfigIntoArrays();  // First time the data is needed, we must convert it.

  initialized = true;

}

/* // TESTING: well this never got called
event PostLogin(PlayerPawn newPlayer) {
  if (bDebugLogging) { Log("event "$Self$".PostLogin("$newPlayer$") was called."); }
}
*/

/*
// TODO CONSIDER: if we make the rollback of old scores very fast, and put more weight into having won, then everyone should get an even number of wins.
//                if a player gets in 4 lost matches in a row, he will soon be placed on a strong team against a bunch of noobs who have been lucky enough to win some recent and are about to get their balance by getting pwned.  ^^
//                however, this is not the final goal: players who have put effort into learning to play UT should be rewarded for their effort: their effort and hopefully therefore increased skill should pay off in their ability to win games (and frag, score highly).
//                Thinking of the XOL ranking, it seemed that players who just spent tonnes of time on the server always got high in the rankings (i don't mean total score, i mean in-game scoreboards and FPH/SPH).  This seems reasonable, given that as well as skill, there was much to learn, with a rotation of a large set of large maps, with regular new maps which nobody had seen.
//                Indeed one way to keep noobs on the server, is to reward them for playing for longer.  (Possibly simply by learning noobish things about the crazy maps, but 
//                This could in fact be artificially encouraged, by using the teambalance to put players who have spent many hours on the server on the same team as strong players who would help them to win (and hence score highly).
//                (This situation would actually mean that the longer a person plays, the worse their teambalance rating would become.)
//                Maybe all these CTF reward bonuses (XOL's and SmartCTF's) actually only cause the team that win (own) to score even more highly than they deserve.  (This took random actions, but because of a slightly high overall skill, they managed to make covers and caps and all score well.)  <-- wtf does this mean?! i thought there was a valid point here! ;)
*/

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

// Tick now replaced by a Timer with 1 second, to detect just before the start of the game.
/*
event Tick(float DeltaTime) {
  if (!gameStartDone) CheckGameStart(); // this the boolean check should be efficient, and switched off before the game starts, but anyway we Disable('Tick()');
}
*/

/*
// TESTING Alternative, there is: event GameEnding() { ... } implemented in GameInfo.  Can we drop our own event catcher in here, without overriding the other?
// or ofc we can use a timer and check Level.Game.bGameEnded but the timer mustn't do this twice at the end of one game. :P
// From testing this: it never gets called!  (Maybe we must somehow register to receive events.)
// event GameEnding() {
  // if (bDebugLogging) { Log("AutoTeamBalance.GameEnding() even was CALLED!  bOverTime="$Level.Game.bOvertime$" bGameEnded="$Level.Game.bGameEnded); }
// }

//// TESTING: I have not seen this called yet
// event Trigger(Actor Other, Pawn EventInstigator) {
  // if (bDebugLogging) { Log("AutoTeamBalance.Trigger("$Other$","$EventInstigator$") was CALLED!"); }
  // CheckGameEnd();
  // // foreach AllActors(class'Actor', A, 'EndGame') A.trigger(self, none);
// }
*/

// Timer is initially set at 1 second to detect the moment before game-start for ForceFullTeamsRebalance().
// Then it is set to 10 seconds during play, to detect mid-game unbalance, if bWarnMidGameUnbalance is set.
// Also, after HandleEndGame() is called, detects real game end and called UpdateStatsAtEndOfGame().
event Timer() {
  if (!gameStartDone) CheckGameStart();
  if (gameStartDone) CheckGameEnd();
  if (bWarnMidGameUnbalance && gameStartDone && !gameEndDone && Level.Game.IsA('TeamGamePlus') && !DeathMatchPlus(Level.Game).bTournament) CheckMidGameBalance();
}

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

  // TODO TESTING: trying it as players join the start of a game
  // if (!gameStartDone) return;

  if (!ShouldBalance(Level.Game)) return;

  if (bDebugLogging) { Log("AutoTeamBalance.ModifyLogin("$SpawnClass$","$Portal$",\""$Options$"\")"); }

  // read this player's selected team
  selectedTeam=Level.Game.GetIntOption(Options,"Team",255);

  // TODO: shouldn't we just skip all this balancing if the player is logging in as a spectator?

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
          // I changed this from Daniel's version, so that bot strengths are not considered.
          // Since a player is joining, one of the bots will probably leave, so counting that bot's strength is inaccurate, and we don't know which bot it will be.  So let's just count player strengths.
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
      // teamnr=0; if (teamStr[0]>teamStr[1]) teamnr=1;
      teamnr=0; if (teamStr[0]>=teamStr[1]+Rand(2)) teamnr=1;
      if (bDebugLogging) { Log("AutoTeamBalance.ModifyLogin(): "$teamSize[0]$"v"$teamSize[1]$" and "$teamStr[0]$"v"$teamStr[1]$" so sending new player to WEAKER team "$getTeamName(teamnr)$"."); }
    } else {
      // send player to the team with fewer players
      // teamnr=0; if (teamSize[0]>teamSize[1]) teamnr=1;
      teamnr=0; if (teamSize[0]>=teamSize[1]+Rand(2)) teamnr=1;
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
  // Or maybe teams become inaccurate when we do the initial ForceFullTeamsRebalance(), but then Daniel would have probably put the check there.
  // I was thinking of stripping teamSizeWithBots because i have adwvaad on my server,
  // but it isn't used to make the decision of which team to join, it's only used for this bugfix.
  // Mmm when someone joins the server mid-game, the bot scores are used to balance the teams
  // With advwaad, if there is a bot, he'll be on the smaller team, so i guess it's fine the player will take the bots place
  // If there isn't a bot, the new player will go on the weaker team, and adwvaad will add a new bot to the stronger team

}

// MutatorBroadcastMessage and MutatorTeamMessage can be used to catch messages said by players during the game (or messages broadcast by the server).

// Catch messages from spectators:
function bool MutatorBroadcastMessage(Actor Sender, Pawn Receiver, out coerce string Msg, optional bool bBeep, out optional name Type) {
  // if (Sender == Receiver) { // Only process the message once.
  if (Sender == Receiver && Sender.IsA('Spectator')) { // Only process the message once.
    if (bDebugLogging) { Log("AutoTeamBalance.MutatorBroadcastMessage() Checking ("$Sender$" -> "$Receiver$") "$Msg$""); }
    // CheckMessage(Msg, Sender);
    // Note that spectator messages have the extra <nick>: before them.  We remove this.
    // Note we do not change Msg since it's an output variable.
    CheckMessage(Mid(Msg,InStr(Msg,":")+1), Sender);
  // } else {
    // if (bDebugLogging) { Log("AutoTeamBalance.MutatorBroadcastMessage(): Ignoring ("$Sender$" -> "$Receiver$") "$Msg$""); }
  }
  return Super.MutatorBroadcastMessage(Sender,Receiver,Msg,bBeep,Type);
}

// Catch messages from players:
function bool MutatorTeamMessage(Actor Sender, Pawn Receiver, PlayerReplicationInfo PRI, coerce string Msg, name Type, optional bool bBeep) {
  if (Sender == Receiver) { // Only process the message once.
    if (bDebugLogging) { Log("AutoTeamBalance.MutatorTeamMessage() Checking ("$Sender$" -> "$Receiver$") "$Msg$""); }
    CheckMessage(Msg, Sender);
  // } else {
    // TODO TESTING TO FIX BUG: After i switched team, and did a "mutate teams  full", I couldn't make say "!teams" work any more.  I think the timelimit has helped this.
    // if (bDebugLogging) { Log("AutoTeamBalance.MutatorTeamMessage(): Ignoring ("$Sender$" -> "$Receiver$") "$Msg$""); }
    // Mmm when the problem occurs, MutatorTeamMessage doesn't get called at all!
    // I'm sometimes getting it at the start of the map too, before making any team changes.
  }
  return Super.MutatorTeamMessage(Sender,Receiver,PRI,Msg,Type,bBeep);
}
  /*
  //// This gets called, and passed to chat, BUT still gets called 8 times!!
  // Isn't this the same as calling super?
  if ( NextMessageMutator != None ) {
    return NextMessageMutator.MutatorTeamMessage( Sender, Receiver, PRI, Msg, Type, bBeep ); // commenting this out does not stop the 8 repeats
  } else {
    return true; // this seems to be what's needed to ensure the message finally reaches the game (doesn't get swallowed)
  }
  */

// TODO: Allow semi-admins to log in with the password, and then be able to run commands without it for the rest of their stay in that game.
// TODO BUG: I thought I had a problem that this was not getting called if have AutoTeamBalance as a ServerActor, but that problem has either gone now, or I was getting this confused with MutatorTeamMessage().
// Mmmm sometimes it is not getting called.  Maybe it's the random other Mutators I have added.  Maybe it's a problem with player rejoining/swicthing teams, and getting lost wrt messaging.
// Or maybe it was just me rebooting the server before the module has finished compiling.  :P
// No can't be that.  When this happens, SmartCTF scoreboard does not appear either.
// Maybe AutoCannon is the problem; it does not appear to be doing anything.
function Mutate(String str, PlayerPawn Sender) {

  local String args[255];
  // local array<String> args;
  local int argcount;

  local String localPass; // we might not offer exactly what is in the config file, this is what we will really offer
  local String pass_if_needed; // for the help (to display whether pass is needed or not)

  local String msg;
  local int i;

  local Pawn p;

  if (bDebugLogging) { Log("AutoTeamBalance.Mutate("$str$","$sender$") was called."); }

  if (Sender.bAdmin)
    localPass = ""; // any or no pass is accepted
  else if (SemiAdminPass == "defaults_to_admin_pass")
    // localPass = Engine.GameInfo.AdminPassword // trying to access it directly did not work
    // localPass = Level.Game.ConsoleCommand("get engine.gameinfo AdminPassword");
    localPass = ConsoleCommand("get engine.gameinfo AdminPassword"); // local to this fn, no longer written to AutoTeamBalance.ini
  else
    localPass = SemiAdminPass;

  argcount = SplitString(str," ",args);

  if (localPass=="" || args[argcount-1]~=localPass) { // Semi-admin privilege commands:

    switch ( Caps(args[0]) ) {

      case "TEAMS":
        if (!Level.Game.GameReplicationInfo.bTeamGame) {
          Sender.ClientMessage("AutoTeamBalance cannot balance teams: this isn't a team game!");
        } else {
          // We let semiadmins override bTournament
          // if (DeathMatchPlus(Level.Game).bTournament) {
          // if (args[2]~="FULL" || args[2]~="FORCE") {
          // } else {
          // }
          // if (bBroadcastStuff) { BroadcastMessageAndLog(Sender.getHumanName()$" has requested teambalance."); }
          MidGameRebalance();
        }
      break;

      case "FORCETEAMS":
        // Sender.ClientMessage("AutoTeamBalance performing full teams rebalance...");
        if (bBroadcastStuff) { BroadcastMessageAndLog(Sender.getHumanName()$" has forced a full teams rebalance."); }
        // To make this balance as accurate as possible, we update the stats now, so we can use the scores from this game so-far.
        // But since this would mess up the end-game stats updating (counting this part of the game twice), we restore the stats from the config afterwards.
        // CopyArraysIntoConfig(); // Not actually needed; they should be identical at this stage
        UpdateStatsAtEndOfGame();
        ForceFullTeamsRebalance();
        CopyConfigIntoArrays();
      break;

      case "STRENGTHS":
        for (p=Level.PawnList; p!=None; p=p.NextPawn) {
          if (AllowedToBalance(p)) {
            Sender.ClientMessage(p.getHumanName()$" has strength "$GetPawnStrength(p));
          }
        }
        Sender.ClientMessage("Red team strength is "$Int(GetTeamStrength(0))$", Blue team strength is "$Int(GetTeamStrength(1))$".");
      break;

      case "STRENGTH":
        Sender.ClientMessage(FindPlayerNamed(args[1]).getHumanName()$" has strength "$GetPawnStrength(FindPlayerNamed(args[1])));
      break;

      case "TORED":
        // if (bBroadcastStuff) { BroadcastMessageAndLog(Sender.getHumanName()$" is trying to fix the teams."); }
        ChangePlayerToTeam(FindPlayerNamed(args[1]),0,true);
        Sender.ClientMessage("Red team strength is now "$Int(GetTeamStrength(0))$", Blue team strength is "$Int(GetTeamStrength(1))$".");
      break;

      case "TOBLUE":
        // if (bBroadcastStuff) { BroadcastMessageAndLog(Sender.getHumanName()$" is trying to fix the teams."); }
        ChangePlayerToTeam(FindPlayerNamed(args[1]),1,true);
        Sender.ClientMessage("Red team strength is now "$Int(GetTeamStrength(0))$", Blue team strength is "$Int(GetTeamStrength(1))$".");
      break;

      case "WARN":
        // FlashMessageToPlayer(FindPlayerNamed(args[1]),args[2]);
        msg=""; for (i=2;i<argcount;i++) { if (!(args[i]~=localPass)) msg = msg $ args[i] $ " "; } // hack to rebuild args without password
        FlashMessageToPlayer(FindPlayerNamed(args[1]),msg);
        FindPlayerNamed(args[1]).ShakeView(4.0,8000.0,12000.0);
      break;

      case "KICK":
        if (bAllowSemiAdminKick) {
          msg=""; for (i=2;i<argcount;i++) { if (!(args[i]~=localPass)) msg = msg $ args[i] $ " "; } // hack to rebuild args without password
          FlashMessageToPlayer(FindPlayerNamed(args[1]),msg);
          // Sender.Kick(args[1]);
          Sender.Kick(FindPlayerNamed(args[1]).getHumanName());
        }
      break;

      case "KICKBAN":
        if (bAllowSemiAdminKick) {
          msg=""; for (i=2;i<argcount;i++) { if (!(args[i]~=localPass)) msg = msg $ args[i] $ " "; } // hack to rebuild args without password
          FlashMessageToPlayer(FindPlayerNamed(args[1]),msg);
          // Sender.KickBan(args[1]);
          Sender.KickBan(FindPlayerNamed(args[1]).getHumanName());
        }
      break;

      case "FORCETRAVEL":
        if (bAllowSemiAdminForceTravel) {
          //// We may not want to publicise the password of the server we are forwarding to.  (e.g. server may have an irc reporter)
          // if (bBroadcastStuff) { BroadcastMessageAndLog("Admin has forced a Server Travel to: "$args[1]); }
          for (p=Level.PawnList; p!=None; p=p.NextPawn) {
            if (p.IsA('PlayerPawn')) { // yes we forward spectators too!
              PlayerPawn(p).PreClientTravel();
              PlayerPawn(p).ClientTravel(args[1], TRAVEL_Absolute, False);
            }
          }
        }
      break;

      Default:
      break;

    }

  }

  if (Sender.bAdmin) { // Admin only commands:

    // TODO: Experimental; comment out in final build
    switch ( Caps(args[0]) ) {

      // Allows semiadmins to read variables from the config files (and maybe some live variables too; untested)
      case "GET":
        Sender.ClientMessage( args[1] $ ":" $ args[2] $ " = " $ ConsoleCommand("get " $ args[1] $ " " $ args[2]) );
      break;

      // Allows semiadmins to write to config variables (probably equivalent to: admin set <package> <name> <value>)
      case "SET":
        ConsoleCommand("set " $ args[1] $ " " $ args[2] $ " " $ args[3]);
        Sender.ClientMessage( args[1] $ ":" $ args[2] $ " = " $ ConsoleCommand("get " $ args[1] $ " " $ args[2]) );
      break;

      case "GETPROP":
        Sender.ClientMessage( args[1] $ " = " $ GetPropertyText(args[1]) );
      break;

      // TODO TESTING: Allows semiadmins to write to in-game variables
      case "SETPROP":
        SetPropertyText(args[1],args[2]);
        Sender.ClientMessage( args[1] $ " = " $ GetPropertyText(args[1]) );
        Sender.ClientMessage(args[1] $ " = " $ ConsoleCommand("get " $ args[1] $ " " $ args[2])); // read it back to the user, to check it worked
      break;

      // Allows semiadmins to run any console command on the server
      case "CONSOLE":
        msg=""; for (i=2;i<argcount;i++) { if (!(args[i]~=localPass)) msg = msg $ args[i] $ " "; } // hack to rebuild args without password
        Sender.ClientMessage( "" $ ConsoleCommand(msg) );
      break;

      case "SAVECONFIG":
        UpdateStatsAtEndOfGame();
        CopyArraysIntoConfig(); // Already done for us
        SaveConfig();
        // CopyConfigIntoArrays(); // If the game ends after this, we will re-do this time period, but damn we can't copy back now
      break;

      case "GRANTADMIN":
        ToggleAdminOnPlayer( FindPlayerNamed(args[1]) );
      break;

      Default:
      break;

    }

  }

  if ( args[0]~="HELP" ) {
    if (localPass == "")
      pass_if_needed = "";
    else
      pass_if_needed = " [password]";
    Sender.ClientMessage("AutoTeamBalance semi-admin commands:");
    Sender.ClientMessage("    mutate teams" $ pass_if_needed);
    Sender.ClientMessage("    mutate forceteams" $ pass_if_needed);
    Sender.ClientMessage("    mutate strengths" $ pass_if_needed);
    Sender.ClientMessage("    mutate strength <part_of_nick>" $ pass_if_needed);
    Sender.ClientMessage("    mutate tored <player>" $ pass_if_needed);
    Sender.ClientMessage("    mutate toblue <player>" $ pass_if_needed);
    Sender.ClientMessage("    mutate warn <player> <message>" $ pass_if_needed);
    if (bAllowSemiAdminKick) {
      Sender.ClientMessage("    mutate kick <player> [<reason>]" $ pass_if_needed);
      Sender.ClientMessage("    mutate kickban <player> [<reason>]" $ pass_if_needed);
    }
    if (bAllowSemiAdminForceTravel) {
      Sender.ClientMessage("    mutate forcetravel <url>" $ pass_if_needed);
    }
    if (Sender.bAdmin) {
      Sender.ClientMessage("AutoTeamBalance admin-only commands:");
      Sender.ClientMessage("    mutate saveconfig");
      Sender.ClientMessage("    mutate grantadmin <player>");
      Sender.ClientMessage("    mutate get <package> <variable>");
      Sender.ClientMessage("    mutate set <package> <variable> <new_value>");
      Sender.ClientMessage("    mutate getprop <variable>");
      Sender.ClientMessage("    mutate setprop <variable> <new_value>");
      Sender.ClientMessage("    mutate console <command>");
    }
  }

  Super.Mutate(str,Sender);
}

function ToggleAdminOnPlayer(Pawn p) {
  local PlayerPawn player;
  if (p!=None && p.IsA('PlayerPawn')) {
    player = PlayerPawn(p);
    player.bAdmin = !player.bAdmin;
    player.PlayerReplicationInfo.bAdmin = player.bAdmin;
  }
}

/*
HandleEndGame gets called when the game time limit expires, BUT the game may go into overtime without us knowing (one of the earlier mutators, or the gametype itself, might decide this).
So at this point I set a Timer to check in 10 seconds whether the game really has ended or not.
TODO CONSIDER: the timer could be disabled if it finds the game has gone into overtime, then we could wait for this function to get called again at the real game end.
*/
function bool HandleEndGame() {
  local bool b;
  // if (bDebugLogging) { Log("AutoTeamBalance.HandleEndGame() bOverTime="$Level.Game.bOvertime$" bGameEnded="$Level.Game.bGameEnded); }

  // This isn't guaranteed to be the end of the game, since we may go into overtime now.
  // But the Timer() is quite efficient (checking bGameEnded first) so shouldn't cause any lag during overtime.
  // And we have to start the Timer() here, because I have no other way of detecting (getting called at) the end of the game!
  // SetTimer(10,False); // Now checking once a minute to see if game has ended; changed to 10 seconds since we lost our alternative MessageMutator hook
  // SetTimer(10,True); // Turned on now in case bWarnMidGameUnbalance is not set, but kept looping in case it is.
  SetTimer(10,bWarnMidGameUnbalance); // only loop if we need to check team balance into overtime (probably was set like this already)
  // if (!bWarnMidGameUnbalance) {
    // SetTimer(10,False);
  // }
  // Timer disabled; it is used by bWarnMidGameUnbalance, and HandleEndGame is now done by Tag='EndGame' and Trigger().
  if (bDebugLogging) { Log("AutoTeamBalance.HandleEndGame(): Set Timer() for 10 seconds. [bOverTime="$Level.Game.bOverTime$",bGameEnded="$Level.Game.bGameEnded$"]"); }

  if ( NextMutator != None ) {
    b = NextMutator.HandleEndGame();
    // if (bDebugLogging) { Log("AutoTeamBalance.HandleEndGame() NextMutator returned "$b$"  bOverTime="$Level.Game.bOvertime$" bGameEnded="$Level.Game.bGameEnded); }
    return b;
  }
  return false;
}



// =========== Our State Model =========== //

// Checks if the game has begun.
function CheckGameStart() {
  local int c,n,e;

  // if (gameStartDone) return; // We expect this to be checked by our caller.

  // We can disable the timer immediately, if AutoTeamBalance is not needed for this game.
  // If we are going to balance, then the timer waits for 1 second before the game starts.
  // If we are going to update stats, we need to record the time the game actually started at, so we wait the same way.
  if (!ShouldBalance(Level.Game) && !ShouldUpdateStats(Level.Game)) { // We do this early, to check at the very least that this is a teamgame
    DoGameStart();
    return;
  }

  // OK so we want to wait for more players to join.
  // Keep ticking until 2 seconds before game start.

  // Read starting countdown
  // Level.TimeSeconds counts up from the moment the server becomes ready to serve the map
  e = TeamGamePlus(Level.Game).ElapsedTime; // after the first player joins, appears to count up to NetWait, then game starts, and once all the bots join, it stops increasing
  n = TeamGamePlus(Level.Game).NetWait;
  c = TeamGamePlus(Level.Game).countdown; // appears to stay fixed at 10, but daniel checked it
  // should never get logged:
  if (gameStartDone && bDebugLogging) { Log("AutoTeamBalance.CheckGameStart(): c="$c$" n="$n$" e="$e$" t="$Level.TimeSeconds$""); }
  c = Min(c,n-e);

  // BUG: This can occasionally get called twice within one second (when the Timer was set to 1 second).
  // BUG: Also gets called at 0.
  if (bBroadcastStuff && (n-e)<n && ((n-e)/10*10)==(n-e) && (n-e)>0) {
    // BroadcastMessage((n-e)$" seconds until game starts...");
    BroadcastMessage((n-e)$" seconds until game starts...",True,'CriticalEvent');
  }

  // If we want to balance bots according to their stats at the start of the game, then we must wait for them to join (they seem to start joining at c=0).
  // if (bBalanceBots) { c += 8; } // wait an extra 8 seconds for bots.  BUG: doesn't work, since e stops increasing; we would have to compare the only changing value Level.TimeSeconds

  // Initialize teams 2 seconds before the game starts:
  // NOTE: when i set NetWait to 1 for testing, I think this 2 caused the ForceFullTeamsRebalance to happen before any bots or players had entered the game.  With NetWait 5 it seems ok.  So BUG: Do not set your NetWait below 3!
  if (c<2) {
    DoGameStart();
  }

}

function DoGameStart() {
  timeGameStarted = Level.TimeSeconds+1.5; // (since we are called on average 1.5 seconds before starting countdown ends)
  if (ShouldBalance(Level.Game)) {
    // if (bBroadcastStuff) { BroadcastMessageAndLog(HelloBroadcast); }
    //// I can't actually test this until I have human players; bots haven't joined yet.
    //// TESTING: how well can we do by just switching 1 or 2 players?
    // if (bTesting) { lastBalanceTime=0; MidGameRebalance(); }
    // if (bTesting) { lastBalanceTime=0; MidGameRebalance(); }
    // if (bTesting) { lastBalanceTime=0; MidGameRebalance(); }
    ForceFullTeamsRebalance();
  }
  gameStartDone=True; // Should ensure CheckGameStart() is never called again.
  Disable('Tick');
  // We disable the timer, unless it is needed to check mid-game teambalance.
  // HandleEndGame() will set it again, if it is needed for CheckGameEnd().
  if (bWarnMidGameUnbalance) {
    SetTimer(10,True); // the frequency of the "uneven teams" bleep is 10 seconds
  } else {
    SetTimer(0,False);
  }
}

function CheckMidGameBalance() {
  local int redTeamCount,blueTeamCount;
  local int redTeamStrength,blueTeamStrength;
  local int weakerTeam;
  local Pawn p;
  local String problem;
  weakerTeam = -1;
  // CONSIDER: we could include bots in the count and the rebalancing, in case the server does not have bBotsBalanceTeams==True
  redTeamCount = GetTeamSize(0);
  blueTeamCount = GetTeamSize(1);
  // if (gameStartDone && bDebugLogging) { Log("AutoTeamBalance.CheckMidGameBalance(): redTeamCount="$redTeamCount$" blueTeamCount="$blueTeamCount$""); }
  // Is one of the teams down 2 players?
  if (redTeamCount>=blueTeamCount+2) {
    weakerTeam = 1; problem=""; // problem = " ("$redTeamCount$"v"$blueTeamCount$") ";
  }
  if (redTeamCount<=blueTeamCount-2) {
    weakerTeam = 0; problem=""; // problem = " ("$redTeamCount$"v"$blueTeamCount$") ";
  }
  // If that check didn't find uneven teams, check if the weaker team is down just 1 player:
  // DONE BUG: When not counting bots, and only 1 player on server, if their score is >Threshold then they will always get uneven teams warning!  FIXED by checking at least 3 players.  Still, if all combinations of those 3 scores make >Threshold difference, warning will be unstoppable.
  if (weakerTeam == -1 && redTeamCount+blueTeamCount>=3) {
    redTeamStrength = GetTeamStrength(0);
    blueTeamStrength = GetTeamStrength(1);
    if (redTeamCount>blueTeamCount && redTeamStrength>blueTeamStrength+StrengthThreshold) {
      weakerTeam = 1; problem = " (strength "$redTeamStrength$" v "$blueTeamStrength$") ";
    }
    if (redTeamCount<blueTeamCount && blueTeamStrength>redTeamStrength+StrengthThreshold) {
      weakerTeam = 0; problem = " (strength "$redTeamStrength$" v "$blueTeamStrength$") ";
    }
  }
  if (weakerTeam == -1) {
    return;
  }
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.IsA('PlayerPawn') && !p.IsA('Spectator')) {
      if (p.PlayerReplicationInfo.Team == weakerTeam) {
        // p.ClientMessage("Teams look uneven!  ("$problem$")  Say !teams to fix them");
        p.ClientMessage("Teams look uneven! "$problem$" Type !teams to fix them",'CriticalEvent',True);
      } else {
        // p.ClientMessage("Teams look uneven!  ("$problem$")  Say !teams or !"$Caps(getTeamName(weakerTeam))$"");
        p.ClientMessage("Teams look uneven! "$problem$" Type !teams or !"$getTeamName(weakerTeam)$"",'CriticalEvent',False);
        p.ShakeView(1.0,2000.0,2000.0);
        // we play our own sound
        // PlaySound ( sound Sound, optional ESoundSlot Slot, optional float Volume, optional bool bNoOverride, optional float Radius, optional float Pitch 
        // sound'NewBeep'
        // p.PlaySound(sound'Beep', SLOT_Interface, 2.5, False, 32, 16); // a squirgier beep, reminds me of jailbreak or respawning on dark camper-sniper servers
        p.PlaySound(sound'FlyBuzz', SLOT_Interface, 2.5, False, 32, 16); // an annoying buzzing fly
      }
    }
  }
}

function CheckGameEnd() {
  if (Level.Game.bGameEnded) {
    if (gameEndDone) return;
    gameEndDone = true;
    // TODO CONSIDER: Could turn Timer off now
    if (ShouldUpdateStats(Level.Game)) {
      UpdateStatsAtEndOfGame();
      CopyArraysIntoConfig();
      SaveConfig();
    }
  }
}


function bool CheckMessage(String Msg, Actor Sender) {

  // if (Msg ~= "!HELP") {
    // PlayerPawn(Sender).ClientMessage("Commands are: !teams !red !blue !spec !play");
  // }

  if (Msg ~= "!RED") {
    ChangePlayerToTeam(PlayerPawn(Sender),0,false);
  }

  if (Msg ~= "!BLUE") {
    ChangePlayerToTeam(PlayerPawn(Sender),1,false);
  }

  if (Msg ~= "!SPEC" || Msg ~= "!SPECTATE") {
    if (!Sender.IsA('Spectator')) {
      PlayerPawn(Sender).PreClientTravel();
      PlayerPawn(Sender).ClientTravel(Level.GetAddressUrl()$"?OverrideClass=Botpack.CHSpectator",TRAVEL_Absolute, False);
      // PlayerPawn(Sender).ClientTravel("/?OverrideClass=Botpack.CHSpectator",TRAVEL_Relative, False); // did not work
    }
  }

  if (Msg ~= "!PLAY") {
    if (Sender.IsA('Spectator')) {
      // BroadcastMessageAndLog("Trying to reconnect "$Sender$" as a player...");
      PlayerPawn(Sender).PreClientTravel();
      PlayerPawn(Sender).ClientTravel(Level.GetAddressUrl()$"?OverrideClass=",TRAVEL_Absolute, False);
    }
  }

  if (Msg ~= "TEAMS" || Msg ~= "!TEAMS") {
    if (bLetPlayersRebalance && !DeathMatchPlus(Level.Game).bTournament) {
      // DONE: check this is not called during bTournament games.
      // Log("AutoTeamBalance.MutatorTeamMessage(): Calling ForceFullTeamsRebalance().");
      // ForceFullTeamsRebalance();
      if (bDebugLogging) { Log("AutoTeamBalance.MutatorTeamMessage(): Calling MidGameRebalance()."); }
      MidGameRebalance();
    }
  }

}

/* Old comments from this method:

  // OK the 8 calls are because this gets called once per player, plus once for UTServerAdminSpectator, and once for ChatLogger
  // Solution: only process it when Sender = Receiver ^^
  // (Interestingly, despite having 2 bots on my team, TeamSay messages only get sent to myself!  (Maybe if there were other humans...))

    // CheckGameEnd(); // Does no harm to do this twice.  The broadcast from mapvote might make the stats parsing come sooner than waiting for the timer.

    // Failed Test: Nope that was never broadcast :P
    // if ( InStr(Msg,"game has ended.")>=0 ) {
      // Log("AutoTeamBalance.MutatorTeamMessage(\""$Msg$"\") detected \"game has ended.\" - DONE elsewhere: run UpdateStatsAtEndOfGame() here.");
    // }

    // if (Level.Game.bGameEnded) {
      // Log("AutoTeamBalance.MutatorTeamMessage(\""$Msg$"\") detected Level.Game.bEnded = True - could run UpdateStatsAtEndOfGame() here.");
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



// =========== Balancing Algorithms =========== //

// Balance the teams just before the start of a new game.  No need for FlagStrength here.
// This was originally Daniel's InitTeams() method, but I have renamed it.
// It can now be called mid-game using "mutate teams <pass> full"
// In this case, it doesn't check which players are holding flags, but hopefully the flag will at least drop or return when the player is restarted.
// BUG TODO: could be unfair if one team drops flag because their FC is switched, but the other team's FC keeps his.  :P
function ForceFullTeamsRebalance() {
  local Pawn p;
  local int st;
  local int pid;
  local Pawn pl[64]; // hashmap of playerpawns, with i = PlayerID%64
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

  // We can't balance if it's not a teamgame
  if (!Level.Game.GameReplicationInfo.bTeamGame) return;

  if (bDebugLogging) { Log("AutoTeamBalance.ForceFullTeamsRebalance(): Running..."); }

  // rate all players, and put them in a temporary structure:
  for (p=Level.PawnList; p!=None; p=p.NextPawn)
  {
    // ignore non-player pawns
    // if (p.bIsPlayer && !p.IsA('Spectator') && AllowedToBalance(p))
    if (AllowedToBalance(p))
    {
      st=GetPawnStrength(p);
      pid=p.PlayerReplicationInfo.PlayerID % 64;
      // Why does Sourceror store the players by this pid hash (which might possibly collide)?  Why not just add the players to a list?
      // Ah well ofc the PlayerID is documented to be unique.
      // Worth noting, from GameInfo.uc:
      // // Set the player's ID.
      // NewPlayer.PlayerReplicationInfo.PlayerID = CurrentID++;
      // So I guess hashing with modulus 64 might not be so dangerous.  :)
      pl[pid]=p;
      ps[pid]=st;
      // tg[pid]=st;
      moved[pid] = 0;
      // p.PlayerReplicationInfo.PlayerName
      if (bDebugLogging) { Log("AutoTeamBalance.ForceFullTeamsRebalance(): Player " $ p.getHumanName() $ " on team " $ p.PlayerReplicationInfo.Team $ " has ip " $ getIP(p) $ " and score " $ p.PlayerReplicationInfo.Score $ "."); }
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
      // if (bBroadcastCookies) { FlashMessageToPlayer(pl[pid],"You have " $ps[pid]$ " cookies."); } // gets hidden by team switches below
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
      ChangePlayerToTeam(pl[pid],teamnr,false);
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
      ChangePlayerToTeam(pl[pid],teamnr,gameStartDone && !gameEndDone);
      teamstr[teamnr]+=ps[pid];
    }

    // if there is an odd number of players put the last player in the weaker team
    if ((n&1)==1)
    {
      pid=plorder[i];
      teamnr=0; if (teamstr[0]>=teamstr[1]+Rand(2)) teamnr=1;
      if (bDebugLogging) { Log("AutoTeamBalance.ForceFullTeamsRebalance(): "$n$" is odd so sending last player to WEAKER team "$teamnr$"."); }
      ChangePlayerToTeam(pl[pid],teamnr,gameStartDone && !gameEndDone);
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
  // if (bBroadcastStuff && bDebugLogging) { BroadcastMessageAndLog("Red team strength is now "$Int(GetTeamStrength(0))$".  Blue team strength is "$Int(GetTeamStrength(1))$"."); }
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
      if (bBroadcastCookies && p.IsA('PlayerPawn') && !p.IsA('Spectator')) {
        FlashMessageToPlayer(p, p.getHumanName() $", you have "$ GetPawnStrength(p) $" cookies.");
      }
    }
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
  // CONSIDER: we could include bots in the count and the rebalancing, in case the server does not have bBotsBalanceTeams==True
  redTeamCount = GetTeamSize(0);
  blueTeamCount = GetTeamSize(1);
  // We assume bot skills are pretty much irrelevant, and the bots will auto-switch to balance teams after we move any players around.

  if (bDebugLogging) { Log("MidGameRebalance() "$redTeamCount$" v "$blueTeamCount$""); }

  // TODO: what if redTeamCount << blueTeamCount ?  e.g. it's 6v2 so we need to move two players.  we could balance in a while loop if it's guaranteed to end
  if (redTeamCount < blueTeamCount) {
    MidGameTeamBalanceSwitchOnePlayer(1,0);
  } else if (blueTeamCount < redTeamCount) {
    MidGameTeamBalanceSwitchOnePlayer(0,1);
  } else {
    MidGameTeamBalanceSwitchTwoPlayers();
  }

}

function bool MidGameTeamBalanceSwitchOnePlayer(int fromTeam, int toTeam) {
  local float fromTeamStrength, toTeamStrength, currentDifference, playerStrength;
  local Pawn p;
  local Pawn closestPlayer; // the most ideal potential player to switch
  local float newDifference; // the absolute strength difference between the two teams after the potential switch
  fromTeamStrength = GetTeamStrength(fromTeam);
  toTeamStrength = GetTeamStrength(toTeam);
  currentDifference = fromTeamStrength - toTeamStrength;
  if (currentDifference<0) {
    // if (bBroadcastStuff) { BroadcastMessageAndLog("AutoTeamBalance not adjusting teams because smaller team looks stronger."); }
    if (bBroadcastStuff) { BroadcastMessageAndLog("AutoTeamBalance refusing to help "$getTeamName(toTeam)$" because it is already stronger ("$Int(toTeamStrength)$">"$Int(fromTeamStrength)$")"); }
    // it says this when i'm the only player on the server, with 5 bots; embarassing
    return False;
  }
  // closestPlayer = None; // not needed, i believe
  // Find the player on fromTeam with strength closest to difference, and switch him/her
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (AllowedToBalance(p) // no not bots; we want even players!  ok so disable bBalanceBots
        // && !p.IsA('Bot')
        && p.PlayerReplicationInfo.Team==fromTeam && p.PlayerReplicationInfo.HasFlag==None) {
      playerStrength = GetPawnStrength(p);
      // Note we multiply playerStrength by 2 here, because switching him will cause -strength to fromTeam and +strength to toTeam.
      if (closestPlayer == None || Abs(currentDifference-playerStrength*2) < newDifference) {
        closestPlayer = p;
        newDifference = Abs(currentDifference-playerStrength*2);
      }
    }
  }
  if (closestPlayer == None) {
    if (bBroadcastStuff) { BroadcastMessageAndLog("AutoTeamBalance could not find any player on "$getTeamName(fromTeam)$" to switch."); }
    return False;
  }
  if (newDifference >= currentDifference) {
    if (bBroadcastStuff) { BroadcastMessageAndLog("AutoTeamBalance not switching "$closestPlayer.getHumanName()$" because that would make "$getTeamName(fromTeam)$" team too weak!"); }
    return False;
  } else {
    ChangePlayerToTeam(closestPlayer,toTeam,true);
    if (bBroadcastStuff) { BroadcastMessageAndLog("Red team strength is now "$Int(GetTeamStrength(0))$", Blue team strength is "$Int(GetTeamStrength(1))$"."); }
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

  redTeamStrength = GetTeamStrength(0);
  blueTeamStrength = GetTeamStrength(1);
  difference = blueTeamStrength - redTeamStrength; // positive implies Team 1 is stronger than Team 0
  newdifference = difference;

  // These repeated calls to GetPawnStrength() are going to be inefficient, possibly causing some lag while the server calculates.
  for (redP=Level.PawnList; redP!=None; redP=redP.NextPawn) {
    for (blueP=Level.PawnList; blueP!=None; blueP=blueP.NextPawn) {
      // if (isHumanPlayer(redP) && isHumanPlayer(blueP) && redP != blueP
      if (redP != blueP
          && redP.PlayerReplicationInfo.Team==0
          && blueP.PlayerReplicationInfo.Team==1
          // && redP.bIsPlayer && !redP.IsA('Spectator')
          // && blueP.bIsPlayer && !blueP.IsA('Spectator')
      // bIsPlayer may be redundant, handled by AllowedToBalance
          && AllowedToBalance(redP) // no not bots; we want even players!  ok so disable bBalanceBots
          && AllowedToBalance(blueP) // no not bots; we want even players!  ok so disable bBalanceBots
          // && !redP.IsA('Bot')
          // && !blueP.IsA('Bot')
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

  // TODO: if one of the players is a bot, we should probably move him last, because bots tend to switch back to the other team, if UT.ini is configured that way.  Alternatively, we could copy Daniel's temporary-ut-balance-disable code into ChangePlayerToTeam.
  if (redPlayerToMove != None && bluePlayerToMove != None) {
    ChangePlayerToTeam(redPlayerToMove,1,true);
    ChangePlayerToTeam(bluePlayerToMove,0,true);
    if (bBroadcastStuff) { BroadcastMessageAndLog("Red team strength is now "$Int(GetTeamStrength(0))$", Blue team strength is "$Int(GetTeamStrength(1))$"."); }
    return True;
  } else {
    if (bBroadcastStuff) { BroadcastMessageAndLog("AutoTeamBalance could not find two switches to improve the teams."); }
    return False;
  }
}


// ======== Change game or message players. ======== //
function ChangePlayerToTeam(Pawn p, int teamnum, bool bShake) {
  // Note: if ForceFullTeamsRebalance() is invoked mid-game; it's possible that this player is already on this team, in which case don't switch.
  if (teamnum == p.PlayerReplicationInfo.Team) {
    if (bDebugLogging) { Log("AutoTeamBalance.ChangePlayerToTeam("$p.getHumanName()$","$teamnum$"): doing nothing since player is already on team "$teamnum); }
    return;
  }
  if (p.IsA('Bot')) {
    Bot(p).ConsoleCommand("taunt wave");
  }
  if (bDebugLogging) { Log("AutoTeamBalance.ChangePlayerToTeam("$p.getHumanName()$"): "$p.PlayerReplicationInfo.Team$" -> "$teamnum); }
  Level.Game.ChangeTeam(p,teamnum);
  Level.Game.RestartPlayer(p); // i thought by doing this even before the game had started, it might fix problems with the player's team getting confused by the server; i don't think it worked, but it didn't do any harm either
  if (bShake) {
    // Level.Game.RestartPlayer(p);
    // FlashMessageToPlayer(p,"You have been moved to the "$getTeamName(teamnum)$" team.");
    // FlashMessageToPlayer(p,"YOU have been MOVED to the >> "$getTeamName(teamnum)$" << team for a fairer game.");
    // FlashMessageToPlayer(p,"You are now on the "$getTeamName(teamnum)$" team.");
    FlashMessageToPlayer(p,"You have been moved to the "$getTeamName(teamnum)$" team for a fairer game.");
    p.ShakeView(2.0,2000.0,0.0);
  }
}

/* I want to Log all calls to BroadcastMessage() so that I can see without playing how much the players are getting spammed by broadcasts.
   Eventually, they should be turned back to just BroadcastMessage() calls. */
function BroadcastMessageAndLog(string Msg) {
  if (bDebugLogging) { Log("AutoTeamBalance Broadcasting: "$Msg); }
  BroadcastMessage(Msg);
}

function FlashMessageToPlayer(Pawn p, string Msg) {
  if (bDebugLogging) { Log("AutoTeamBalance Sending message to "$p.getHumanName()$": "$Msg); }
  // NOTE: in case you thought otherwise, this message gets displayed in the console, but not in the chatarea.  It is also displayed on the HUD, but can be hidden by the scoreboard, or a following HUD message.
  // p.ClientMessage(Msg); // goes to chat and console
  // p.ClientMessage(Msg, 'Event', True); // TESTING (sends only to chat, or only to chat+console?)
  // p.ClientMessage(Msg, 'CriticalEvent', True); // goes to HUD and console
  p.ClientMessage(Msg, 'CriticalEvent', False); // goes to HUD and console, no beep
  p.PlaySound(sound'Beep', SLOT_Interface, 2.5, False, 32, 32); // we play our own sound
}
/*
In PlayerPawn, ClientMessage actually calls:
  Player.Console.Message( PlayerReplicationInfo, S, Type );
  PlayBeepSound();
  myHUD.Message( PlayerReplicationInfo, S, Type );
*/



//// ======== Library Functions which do not change state ======== ////

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

function bool AllowedToBalance(Pawn b) {
  if (b.IsA('Bot'))
    return bBalanceBots;
  else
    // return True;
    return b.IsA('PlayerPawn') && !b.IsA('Spectator');
}

/* Checks that the player is a human, or a bot when bRankBots is set.  Does not check whether the human player is a spectator. */
function bool AllowedToRank(Pawn b) {
  if (b.IsA('Bot'))
    return bRankBots;
  else
    return b.IsA('PlayerPawn'); // && !b.IsA('Spectator');
    // return True;
}

// This is used for checking and performing mid-game teambalance.  It counts bots if bBalanceBots is set to True.
function int GetTeamSize(int team) {
  local int count;
  local Pawn p;
  count = 0;
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    // if (isHumanPlayer(p) && p.PlayerReplicationInfo.Team == team) count++;
    // if (p.bIsPlayer && !p.IsA('Spectator') && AllowedToBalance(p) && p.PlayerReplicationInfo.Team == team) count++;
    if (AllowedToBalance(p) && p.PlayerReplicationInfo.Team == team) count++;
  }
  return count;
}

function int CountHumanPlayers() {
  local Pawn p;
  local int countHumanPlayers;
  // Do not update stats for games with <MinHumansForStats human players.
  countHumanPlayers = 0;
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.bIsPlayer && !p.IsA('Spectator') && !p.IsA('Bot') && p.IsA('PlayerPawn') && p.bIsHuman) { // maybe the last 2 are not needed
      countHumanPlayers++;
    }
  }
  return countHumanPlayers;
}

/* // Unused.
function bool isHumanPlayer(Pawn p) {
  return p.bIsPlayer && p.bIsHuman && !p.IsA('Spectator') && !p.IsA('Bot'); // seems this check for bot is needed!
}
*/

function String getTeamName(int teamNum) {
  return TeamGamePlus(Level.Game).Teams[teamNum].TeamName;
}

// We don't want to get bot strengths when we are doing MidGameRebalance, because if we do switch a player, a bot may also switch team, and our algorithm does not account for that, so it's better just to ignore the bots, and balance the players.
// Done: Maybe we should count bot strengths; maybe the bot is strong, and the team with the bot is pwning the other team!  But TODO currently bot strengths are all the same, the static config BotStrength, which doesn't really represent how well this particular bot is performing in this game, with respect to the other players.  We should really look at the bot's score-per-minute from the current game, or actually build stats for the server's bots.
function float GetTeamStrength(int teamNum) {
  local Pawn p;
  local float strength;
  strength = 0;
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    // if (p.bIsPlayer && !p.IsA('Spectator') && /* !p.IsA('Bot') && */ p.PlayerReplicationInfo.Team == teamNum) {
    if (p.bIsPlayer && !p.IsA('Spectator') && p.PlayerReplicationInfo.Team == teamNum) {
      strength += GetPawnStrength(p);
    }
  }
  // if (bDebugLogging) { Log("AutoTeamBalance.GetTeamStrength(): "$getTeamName(teamNum)$" team has strength "$strength); }
  // Add flagstrength:
  strength += TournamentGameReplicationInfo(Level.Game.GameReplicationInfo).Teams[teamNum].Score*FlagStrength;
  return strength;
}

// Returns the strength of a player or a bot
// Why do we ever want bot strengths?  Most of the time we intend to balance the players, then let rubbish bots automatically fill the empty slot(s).  We assume they are not very dangerous bots.
// OK now we can include bots in ranking and balancing by setting bRankBots.  Without it, BotStrength is used for all bots (but only in situations where bot strengths are relevant).
function int GetPawnStrength(Pawn p) {
  local int st;

  if (AllowedToRank(p) || AllowedToBalance(p)) // TODO: is this good?
  {
    // a human player - get his strength
    // st = GetPlayerStrength(PlayerPawn(p));
    st = GetPlayerStrength(p);
  } else {
    // a bot - use default strength
    st = BotStrength;
  }

  // if (bDebugLogging) { Log("AutoTeamBalance.GetPawnStrength(" $ p $ "): " $ st $ ""); }

  return st;
}

// Returns the strength of a player
function int GetPlayerStrength(Pawn p) {
  local int found;
  if (bUseOnlyInGameScoresForRebalance && gameStartDone && !gameEndDone) {
    // if (bDebugLogging) { Log("Doing speedy lookup: "$p.getHumanName()$": "$p.PlayerReplicationInfo.Score); }
    return p.PlayerReplicationInfo.Score;
  }
  found = FindPlayerRecord(p);
  if (found == -1) {
    // TODO: if gameStartDone and/or gametime>1minute then guess the player's strength from their current score
    return UnknownStrength; // unknown player or player is too weak for list
  } else {
    return avg_score[found]; // player's average score (or best estimate of player)
  }
  // return UnknownStrength;
}

function Pawn FindPlayerNamed(String name) {
  local Pawn p;
  local Pawn found;
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    if (p.IsA('PlayerPawn') || p.IsA('Bot')) {
      if (p.getHumanName() ~= name) { // exact case insensitive match, return player
        return p;
      }
      if (Instr(Caps(p.getHumanName()),Caps(name))>=0) { // partial match, remember it but keep searching for exact match
        found = p;
      }
    }
  }
  return found; // return partial match, or None
}

/*
//// CONSIDER: we could add support for squeezing multiple delimiters into 1
// function array<String> SplitString(String str, String divider) {
// function int SplitString(String str, String divider, out array<String> parts) {
*/
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
			str = Mid(str,nextSplit+Len(divider));
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



// ======== Player database: ======== //

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

function String getIP(Pawn p) {
	if (p.IsA('PlayerPawn')) {
		return stripPort(PlayerPawn(p).GetPlayerNetworkAddress());
	} else {
		return "0.0.0.0";
	}
}

/*
// CONSIDER: a simple method of trying to squeeze some of the largest inefficiency out of this search:
//           when a player's record is updated (or found?), move it one record up in the list (unless it's already at the top)
//           this way, the most frequent, and regular players will be nearer the top of the list, so it will be faster to retrieve their records
//           side-effect: the positions of the records will hold meta-info: each game a player plays, will move him 1 spot up on the ladder
//           players near the bottom will be infrequent or not recent records, so they are good for recycling :)
//           note A,B -> B,A -> A,B if swap order != ladder order; will moving 2 spots stop this? no
//           the meta-info will ~ HoursPlayed
// return index i into playerData[] and ip[]/nick[]/... arrays, but not always an exact player match!
*/
// DONE: for more efficient searching: hash by player id, and swap into that position if neccessary
//       this makes the initial search for each new player in the game linear, but all later searches immediate
// side-affect: order in database is no longer by creation, but if time_last_seen field was added, you could see the last time that N people came through the server on one map
// Will always return a valid exact record, creating a new record if neccessary.  (This is a requirement to avoid re-scanning the db repeatedly).
function int FindPlayerRecord(Pawn p) {
  local int i;
  local int found;
  local string tmp_player_nick, tmp_player_ip;
  local float tmp_avg_score, tmp_hours_played;

  i = p.PlayerReplicationInfo.PlayerID % MaxPlayerData;
  if (p.getHumanName() == nick[i] && getIP(p) == ip[i]) {
    // if (bDebugLogging) { Log("AutoTeamBalance.FindPlayerRecord(): Fast lookup for "$p.getHumanName()$" @ "$getIP(p)$"."); }
    return i;
  }

  found = FindPlayerRecordNoFastHash(p);

  // If an exact record for the player was found, move it to index i for the rest of this game (by swapping it with whichever record is there)
  if (found != -1 && p.getHumanName() == nick[found] && getIP(p) == ip[found]) {
    if (bDebugLogging) { Log("AutoTeamBalance.FindPlayerRecord(): Optimising lookup ("$i$"<->"$found$") for "$p.getHumanName()$" @ "$getIP(p)$"."); }
    tmp_player_nick = nick[i];
    tmp_player_ip = ip[i];
    tmp_avg_score = avg_score[i];
    tmp_hours_played = hours_played[i];
    nick[i] = nick[found];
    ip[i] = ip[found];
    avg_score[i] = avg_score[found];
    hours_played[i] = hours_played[found];
    nick[found] = tmp_player_nick;
    ip[found] = tmp_player_ip;
    avg_score[found] = tmp_avg_score;
    hours_played[found] = tmp_hours_played;
    return i;
  }

  // No exact record for the player was found; we have performed a full search of the database :|
  if (bDebugLogging) { Log("AutoTeamBalance.FindPlayerRecord(): SLOW lookup for "$p.getHumanName()$" @ "$getIP(p)$"."); }

  // return found;

  // Let's create a new record for this player, to avoid doing that again.
  i = CreateNewPlayerRecord(p); // i=random, but the new record will be optimally indexed once FindPlayerRecord() is called again.

  if (found > -1) {
    // Copy over strength from the partial-match player, but only make that strength last for 2 hours.
    // SO: changing nick will Not reset your avg_score immediately, but eventually
    avg_score[i] = avg_score[found]; // Copy score from partial record max
    hours_played[i] = Min(MaxHoursWhenCopyingOldRecord,hours_played[found]); // but in case this is a different player (or maybe the same player but in a different environment), give the new record max 2 hours, so it won't take long to get an accurate idea of this new player's strength
    // if (bLogFakenickers) { Log("Fakenicker "$p.getHumanName()$" is "$nick[i]$" ip "$ip[i]); }
    Log("AutoTeamBalance: Fakenicker "$p.getHumanName()$" is "$nick[i]$" ip "$ip[i]);
  }

  return i; // if we didn't copy any stats over, he will have UnknownStrength, the same as if we return -1

}

// If an exact match for the player exists, return the index
// If not, return the index of a record with matching nick, or (preferably) matching ip
// If not, return -1
function int FindPlayerRecordNoFastHash(Pawn p) {
  local int found;
  local int i;
  local string player_nick;
  local string player_ip;

  player_nick = p.getHumanName();
  player_ip = getIP(p);

  found = -1;
  for (i=0;i<MaxPlayerData;i++) {
    // Exact match! return the index immediately
    if (player_nick == nick[i] && player_ip == ip[i]) {
      found = i;
      if (bDebugLogging) { Log("AutoTeamBalance.FindPlayerRecordNoFastHash(p) Exact match for " $nick[i]$ ","$ip[i]$": ["$found$"] ("$avg_score[i]$")"); }
      return found;
    } else if (player_ip == ip[i]) {
      found = i; // matching ip
      // if (bDebugLogging) { Log("AutoTeamBalance.FindPlayerRecordNoFastHash(p) IP match for " $p.getHumanName()$ ","$getIP(p)$": ["$found$"] "$nick[i]$" ("$avg_score[i]$")"); }
    } else if (player_nick == nick[i] && found == -1) {
      if (bDebugLogging) { Log("AutoTeamBalance.FindPlayerRecordNoFastHash(p) nick match for " $nick[i]$ ","$getIP(p)$": ["$found$"] "$ip[i]$" ("$avg_score[i]$")"); }
      found = i; // if not yet matching an ip, match the same nick on any ip
    }
    // TODO: if an uneven match, choose a match with more experience (hours_played)
    // TODO: if we have little experience (<10mins) of a player, assume default score?
  }

  if (bDebugLogging && found == -1) { Log("AutoTeamBalance.FindPlayerRecordNoFastHash("$p$") failed to return a record."); }
  return found;
}

function int CreateNewPlayerRecord(Pawn p) {
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
  ip[pos] = getIP(p);
  nick[pos] = p.getHumanName();
  // initialise each player as having played for UnknownMinutes (e.g. 10 or 0.1) minutes already, and already earned an average UnknownStrength (e.g. 40) frags
  avg_score[pos] = UnknownStrength; // DO NOT set this to 0; it will screw with InitTeams()!
  hours_played[pos] = 0; // UnknownMinutes/60;
  if (bDebugLogging) { Log("AutoTeamBalance.CreateNewPlayerRecord("$p$") ["$pos$"] "$ nick[pos] $" "$ ip[pos] $" "$ avg_score[pos] $" "$ hours_played[pos] $"."); }
  // if (bBroadcastCookies) { BroadcastMessageAndLog("Welcome "$ nick[pos] $"!  You have "$ avg_score[pos] $" cookies."); }
  // if (bBroadcastCookies) { BroadcastMessageAndLog("Welcome to the server "$ nick[pos] $"!  Have a cookie.  :)"); }
  // TODO: disabled to reduce lag at mid-game joins: if (bBroadcastCookies) { FlashMessageToPlayer(p,"Welcome to the server "$ nick[pos] $"!  Have a cookie.  :)"); }
  // SaveConfig();
  return pos;
}

/*
// Finds an old player record which we can replace.  Actually since we don't have a last_seen field, we'll just have to remove the "shortest" record.  (Player didn't spend long on server; their stats don't mean a lot)
// Only problem, if the database really is saturated (but I think that's unlikely), this new player will probably be the next record to be replaced!  To keep his record in the database, the new player just has to play for longer than the now "shortest" record before another new player joins.
// Actually one nice side-effect of the particular algorithm we're using below (<lowest instead of <=lowest): if a few records share the "shortest record" time (actually this was more likely when our hours_played were incremented in fixed-size steps), it will be the first of them that gets replaced first.  :)  Down-side: the new player now in that early position in the stats-table was not an early player on the server, so he breaks this very pattern.
*/
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



// =========== Updating Stats: =========== //

function UpdateStatsAtEndOfGame() {
  local Pawn p;
  local int i;

  if (CountHumanPlayers() < MinHumansForStats) {
    if (bDebugLogging) { Log("AutoTeamBalance.UpdateStatsAtEndOfGame(): not updating stats since CountHumanPlayers "$CountHumanPlayers()$" < "$MinHumansForStats$"."); }
    return;
  }

  if (WinningTeamBonus != 0 && !DeathMatchPlus(Level.Game).bTournament) {
    GiveBonusToWinningTeamPlayers();
  }

  // Update stats for all players in game
  if (bDebugLogging) { Log("AutoTeamBalance.UpdateStatsAtEndOfGame(): Updating player stats."); }
  if (bBroadcastStuff) { BroadcastMessage("AutoTeamBalance is updating player stats."); }
  // TEST considered when stats were being updated mid-game: make lag here on purpose and see how bad we can get it / how we can fix it.
  // if (bLogExtraStats) { Log("AutoTeamBalance.LogEndStats: NAME IP TEAM PING PKTLOSS SCORE FRAGS DEATHS ITEMS SPREE SECRET TIME"); }
  if (bLogExtraStats) { Log("AutoTeamBalance.LogEndStats: Team Name IP Ping PktLoss Rank Hours Score Frags Deaths Items Spree Secret Time"); }
  for (p=Level.PawnList; p!=None; p=p.NextPawn) {
    // if (p.bIsPlayer && !p.IsA('Spectator') && AllowedToRank(p) && p.IsA('PlayerPawn')) {
    if (!p.IsA('Spectator') && AllowedToRank(p)) {
      i = UpdateStatsForPlayer(p);
      if (bLogExtraStats) { Log("AutoTeamBalance.LogEndStats: "$p.PlayerReplicationInfo.Team$" "$p.getHumanName()$" "$getIP(p)$" "$p.PlayerReplicationInfo.Ping$" "$p.PlayerReplicationInfo.PacketLoss$" "$avg_score[i]$" "$hours_played[i]$" "$p.PlayerReplicationInfo.Score$" ? "$p.PlayerReplicationInfo.Deaths$" "$p.ItemCount$" "$p.Spree$" "$p.SecretCount$" "$(Level.TimeSeconds - p.PlayerReplicationInfo.StartTime)$""); }
    }
  }

  // Save the new stats in the config/ini file:
  // Log("AutoTeamBalance.UpdateStatsAtEndOfGame(): saving stats to file");
  // CopyArraysIntoConfig();
  // SaveConfig(); // This is the only place this gets done atm!

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
    // if (p.bIsPlayer && !p.IsA('Spectator') && p.IsA('PlayerPawn')) {
    if (!p.IsA('Spectator') && AllowedToRank(p)) {
      if (p.PlayerReplicationInfo.Team == WinningTeam.TeamIndex) {
        if (bDebugLogging) { Log("AutoTeamBalance.GiveBonusToWinningTeamPlayers(): giving bonus to "$p.getHumanName()$"."); }
        p.PlayerReplicationInfo.Score += WinningTeamBonus;
        p.ClientMessage("You got "$WinningTeamBonus$" bonus points for finishing on the winning team.",'Pickup',False);
      }
    }
  }

}

function int UpdateStatsForPlayer(Pawn p) {
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

  // For efficiency, FindPlayerRecord() is now guaranteed to return a record.
  // The code below has moved in there.
  /*
  if (i == -1 || ip[i] != getIP(p) || nick[i] != p.getHumanName()) {
    // This is not an exact player match, so we should not update its stats
    // since we didn't find this actual ip+nick, we create a new entry
    j = CreateNewPlayerRecord(p); // OLD BUG FIXED: is it inefficient to repeatedly create a PlayerPawn from the same Pawn?
    if (i > -1) {
      // Copy over strength from the partial-match player, but only make that strength last for 2 hours.
      // SO: changing nick will Not reset your avg_score immediately, but eventually
      avg_score[j] = avg_score[i]; // Copy score from partial record max
      hours_played[j] = Min(MaxHoursWhenCopyingOldRecord,hours_played[i]); // but in case this is a different player (or maybe the same player but in a different environment), give the new record max 2 hours, so it won't take long to get an accurate idea of this new player's strength
      // if (bLogFakenickers) { Log("Fakenicker "$p.getHumanName()$" is "$nick[i]$" ip "$ip[i]); }
      Log("Fakenicker "$p.getHumanName()$" is "$nick[i]$" ip "$ip[i]);
    }
    i = j;
  }
  */

  current_score = p.PlayerReplicationInfo.Score;
  if (bScalePlayerScoreToFullTime) {
    current_score = current_score * (Level.TimeSeconds - timeGameStarted) / (Level.TimeSeconds - p.PlayerReplicationInfo.StartTime);
    // if (bDebugLogging) { Log("AutoTeamBalance.UpdateStatsForPlayer("$p.getHumanName()$") Scaled by time from "$p.PlayerReplicationInfo.Score$" to "$current_score$""); }
  }
  if (bNormaliseScores) {
    current_score = NormaliseScore(current_score);
  }
  // Ideally we would like to check how long this player has been on the server DONE
  gameDuration = Level.TimeSeconds - timeGameStarted;
  timeInGame = Level.TimeSeconds - p.PlayerReplicationInfo.StartTime;
  if (timeInGame>gameDuration)
    timeInGame = gameDuration;
  // if (bDebugLogging) { Log("AutoTeamBalance.UpdateStatsForPlayer("$p.getHumanName()$") timeGameStarted="$timeGameStarted$" Game.StartTime="$Level.Game.StartTime$" Level.TimeSeconds="$Level.TimeSeconds$" Player.StartTime="$p.PlayerReplicationInfo.StartTime$" timeInGame="$timeInGame$" gameDuration="$gameDuration); }
  // Well if this player was only in the server for 5 minutes, we could multiply his score up so that he gets a score proportional to the other players.  (Ofc if he was lucky or unlucky, that luck will be magnified.)
  if (timeInGame < 60) { // The player has been in the game for less than 1 minute.
    if (bDebugLogging) { Log("AutoTeamBalance.UpdateStatsForPlayer("$p$") Not updating this player since his timeInGame "$timeInGame$" < 60s."); }
    return i;
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
      if (bBroadcastStuff) { BroadcastMessageAndLog(""$ p.getHumanName() $" has earned "$ Int(avg_score[i]-previous_average) $" cookies!"); }
      FlashMessageToPlayer(p,"You earned "$ Int(avg_score[i]-previous_average) $" cookies this game."); // BUG: hidden by scoreboard
    }
    else if (previous_average>avg_score[i]+1) {
      if (bBroadcastStuff) { BroadcastMessageAndLog(""$ p.getHumanName() $" has lost "$ Int(previous_average-avg_score[i]) $" cookies."); }
      FlashMessageToPlayer(p,"You lost "$ Int(previous_average-avg_score[i]) $" cookies this game."); // BUG: hidden by scoreboard
    }
  }
  return i;
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
    if (!p.IsA('Spectator') && AllowedToRank(p)) { // lol
      if (bScalePlayerScoreToFullTime) {
        averageGameScore += p.PlayerReplicationInfo.Score * (Level.TimeSeconds - timeGameStarted) / (Level.TimeSeconds - p.PlayerReplicationInfo.StartTime);
      } else {
        averageGameScore += p.PlayerReplicationInfo.Score;
      }
      playerCount++;
    }
  }
  averageGameScore = averageGameScore / Float(playerCount);

  // Avoid division-by-zero error here.  You guys got average <2 frags?  Screw you I'm not scaling that up to NormalisedStrength!
  if (averageGameScore < 2.0) { // TODO: kinda nasty to have such a sharp cutoff though; average 3 frags will be scaled up
    averageGameScore = NormalisedStrength; // CONSIDER: maybe just better not to update
  } // BT games will tend to have a lot of -ve scores.
  // Mm so what if the average score is 3?
  // Well that suggests a short game, in which case the player's stats won't change much anyway.  But if it was a long game, and they still scored low, maybe these noobs will get rewarded more than they should.  Otoh scores may be low simply because it was 2v2 on a huge map, so reward is fair.
  // If the average is 100?  Wow high-scoring game!  Everyone's score will get scaled down.  But I guess it was a long game, so they will get the full time multiplier at least (more if it went into overtime).

  // if (bDebugLogging) { Log("AutoTeamBalance.NormaliseScore("$score$"): Average is "$averageGameScore$""); }

  return score * NormalisedStrength / averageGameScore; // TODO: i think this NormalisedStrength should be scaled if the game was longer/shorter than usual  fewer points for shorter games?  remember their relative score will be scaled up by their time, so is the scale really needed?  :o  Mmm I conclude we don't need to scale the NormalisedStrength.

}

// Takes everything before the first ":" - you should almost always use this when getting PlayerPawn.GetPlayerNetworkAddress(); at least in my experience the client's port number changed frequently.
function string stripPort(string ip_and_port) {
  if ((""$ip_and_port)=="None" || ip_and_port=="") {
    if (bDebugLogging) { Log("stripPort(); ip_and_port="$ip_and_port); }
    return "0.0.0.0";
  }
  return Left(ip_and_port,InStr(ip_and_port,":"));
}



// TESTING:

/*
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

/*
function bool PreventDeath(Pawn Killed, Pawn Killer, name damageType, vector HitLocation) {
  Log("AutoTeamBalance.Pr.ventDeath("$Killed$","$Killer$","$damageType$")");
}

function MutatorTakeDamage( out int ActualDamage, Pawn Victim, Pawn InstigatedBy, out Vector HitLocation, out Vector Momentum, name DamageType) {
  // Log("AutoTeamBalance.MutatorTakeDamage("$Victim$","$InstigatedBy$","$DamageType$")");
}
*/


