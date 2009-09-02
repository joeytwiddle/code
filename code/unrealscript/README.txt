==== nogginBasher's mods ====


=== GREAT ===

* ActorEditor

  Allows the admin to set some properties of game actors during play.
  Only top-level text properties can be adjusted.  (E.g. PlayerStart12.Team,
  ZoneInfo3.Gravity, Bot5.Health)
  Changes can be saved to be re-applied when map is next loaded.  (Can
  occasionally be used to fix small map bugs.)
  Cannot yet spawn new actors (or can it?).

* MapMuts/

  Collects stats on maps into one big file, including approximate size
  (player-capacity) of map, number of times played, total frags, has it
  crashed.  Can also catch counts of players saying "good map" and "bad map".
  Can automatically switch to the "ideal" next map for servers without
  mapvote.

* MessageAdmin/

  Allows players to leave a message for the server admin by saying in game
  "!admin xer0 just made me cry".  Basically a cut-down copy of PostBox.
  Not connected to IRC, just dumps the message in an ini file.

* PostBox/

  Allows players to leave messages for their friends which can be picked up
  later.

* PubliciseScore/

  Add info to the server's title as displayed in the UT server list.  Can show
  the number of players, current team scores, and time remaining in the game.

* FairLMS/

  A mutator-style copy of the ELMS gametype from the No Downloads servers.
  LastManStanding with a twist: your health is dropping, only frags can get
  you more health, and 4 frags will give you a powerup!  Great fun.  Too bad
  the jolt server is quiet now.

* kxGrapple/

  My advanced grappling hook for UT, with a realistic swinging and wrapping
  line.  Can offer players winch-in and wind-out ability.  Based on Expert100,
  inspired by No Downloads, lovingly refined.  A fun substitute for the
  Translocator on CTF maps with any headroom.  Also available in rigid-line
  mode.

* TeamSwitcher/

  Provides a few commands to players: !red !blue !green !spec !play !vote



=== GOOD ===

+ ChatMuts/

  Simple mutator ChatMuts.Ignore allows players to type !ignore KGB so they
  won't receive further say messages from KGB that game.  Unfortunately does
  not work on taunts.

+ ClientConsoleLogger/

  There are a couple of these already, but this one works in Linux!  Well
  actually it just logs to the standard log.  You have to grep out the
  [ChatLog] lines. :)

+ CrouchBlocksDamage/

  My first ever mutator, for protection against shock balls on XOL.  Never
  really tried it, you can boost over Face crouching with your own ripper.

+ PainSounds/

  Hitsounds made from grunts and groans in the male and female sound packs.
  Originally part of ELMS/FairLMS.  (Some earlier versions caused players to
  crash when I sent the sound on too many channels.)

+ RedirectPlayers/

  Put this mutator on your old server and players will be automatically sent
  to the new server.  (Does not update their favourites but I think it does
  spam them.)

+ Resize/

  Resizes a map during play!  Well actually it attempts to resize everything
  in play except for the map.  ;)  Has a few small bugs but does work in many
  ways.  Could be useful for mappers considering using MapScale and wanting to
  test out some scale values.  Should not be a mutator, cannot increate splash
  radius.



=== MAYBE ===

~ ArenaFallback/

  Instagib maps often don't include weapon pickups.  If run in normal mode,
  players have only enforcer and impact hammer.  If your server has a mixture
  of modes and maps, this mutator will kick in on maps where no weapons are
  provided, creating an instagib or flak arena.  The mutator has been
  overdeveloped and broken, but one of the earlier versions was good.

~ JLib/

  Some common functions I use in UnrealScripts.

~ PureSwitcher/

  Enables and disables UTPure when playing certain maps/modes/mutators.
  Might be a fiddle to configure.

~ Resurector/

  If a player disconnects from the server, will bring them back with some of
  their stats intact.  Does not work for SmartCTF.  I just fixed a couple of
  bugs in this version, and stopped it from fragging AutoTeamBalance stats.

~ RocketArenaMutator/

  Allows you to boost yourself with your own weapons, like you can in
  RocketArena.  I actually changed some of the parameters so the boosts are
  more like CTF warmup mode.  Does not yet provide all weapons like
  RocketArena maps do.  Does not break the game up into 1v1 or 2v2 rounds.

~ SiegeAnywhere/

  An attempt to provide Siege features into the default UT gametypes.  This
  makes no sense and isn't particularly fun.  But the package does include a
  mutator which can be used to save the load the buildings currently on the
  map!  Last built for XXL2g.

~ SiegePlusDev/

  Attempts at ways of getting noobs out of the supplier *$&!*$"

~ TrackMuts/

  Can show you how much damage you are inflicting on other players, or how
  much you are receiving.  Unfortunately this mutator cannot see damage
  absorbed by armour - you actually only see health decrease.

~ WeirdMuts/

  You can force everyone to play a map in behindview mode!

~ kxForceGun/

  Allows you to pick up weapons, siege buildings and bots, and move them to
  other places.  Intended as a half-life gun.  Still some bugs.

~ sgGrapple/

  This addon to siege can provide build options for the kxGrapple and
  kxForceGun weapons.

~ sgGrapple2f/

  A build for SiegeXXL2d

~ sgGrappleXtreme02/

  A build for SiegeXtreme2



=== DODGY ===

- FixWeaponBalance/

  In zeroping with all weapons, sniper and shock primary become more powerful
  than the other weapons.  This mutator attempted to restore the normal
  balance, by reducing the cylinder radius of the player's head and body.
  FAILED: Mutator cannot do this.

- RandomMutators/

  Given a big list of mutators, will choose a random 3 to use at the beginning
  of each map.  This mutator is classically whack.

- ScreenEditor.deprecated/

  On maps using Mychael's Screen actor, allows players to write messages onto
  the screens, or link nearby screens+teleporters to certain servers.
  This was moved into ActorEditor in the end, making ScreenEdtior obsolete.

- ScreenExtensions/

  Provides triggers to switch or pause Screen slides.  For embedding into maps.

- TranslocBots/

  Makes bots translocate around a bit more.  Not very well.

- nBAutoSpectate/

  My terrible modification of the fanatstic AutoSpectate mod.  Attempts to
  setup interesting camera angles when spectating or replaying demos.  Pretty
  random, sometimes great, generally rubbish.


