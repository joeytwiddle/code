==== nogginBasher's Unreal Tournament mods ====

Some stable .u .ini and .int files are available from:

  http://neuralyte.org/~joey/ut_releases/

All the others are here, but may be in developmental state:

  http://hwi.ath.cx/ut/dev/System/

If you still can't find it, you can get the latest source and compile it!
Look in the <PackageName>/Classes folder on:

  http://hwi.ath.cx/ut/dev/

Add the package name to EditorActors= in UT.ini, and then run ucc.exe make.

Some of them are experimental.  Some of them are overdeveloped, in which case
you may prefer to look for an earlier stable version from CVS:

  http://hwi.ath.cx/cgi-bin/cvsweb/code/unrealscript/




=== GREAT ===

* AutoTeamBalance

  Tracks player scores in order to balance the teams at the beginning of each
  game.

* kxWallJump

  This one is a lot of fun!  It's a mod of UT_DoubleJump restricted to work
  only near walls.  Trapped in a corner?  Want to get on top of that box?
  Bounce off the walls like a ninja, using boots that grip the walls.

* PubliciseScore

  Add info to the server's title as displayed in the UT server list.  Can show
  the number of players, current team scores, and time remaining in the game.
  Later versions can also cycle the server title.

* PostBox

  Allows players to leave messages for their friends which can be picked up
  later.

* FairLMS

  A mutator-style clone of the ELMS gametype from the No Downloads servers.
  LastManStanding with a twist: your health is dropping, only frags can get you
  more health, and 4 frags will give you a powerup!  Great fun.  Too bad the
  jolt server is empty these days.

* kxGrapple

  My advanced grappling hook for UT.  Simulates a strong rope with a strong
  winch, and realistic gravity, swinging the player, and wrapping around edges.
  Offers players the option of winch-in, hold or wind-out, offhand grappling,
  and toggling of behind-view and FOV when grapple is selected.  Based on
  Expert100, inspired by No Downloads.  A fun substitute for the Translocator
  on CTF maps, except those with no walls or ceiling, or where the player needs
  to translocate through a small window.

  NOTE: You should not download kxGrapple.u since this is my development
  version and subject to change (using this one is just asking for Version
  Mismatch).  Instead download the latest release, e.g. kxGrapple04rc2.u

  NOTE: Might be incompatible with UTPure.  I don't remember for sure.

  BUGS: Replication could be improved.  Nin really warps around a lot when he
  uses it in LMS!

  Some players like to toggle grapple on/off with either mouse button, but
  kxGrapple strictly toggles with left-throw, right-retract bindings, like a
  translocator does.  I have left it configurable (client or server side?), but
  I should probably make the defaults noob-friendly.

* TeamSwitcher

  Provides a few commands to players: !red !blue !green !spec !play !vote

* MapMuts

  Collects stats on maps into one big file, including approximate size
  (player-capacity) of map, number of times played, total frags, has it
  crashed.  Can also catch counts of players saying "good map" and "bad map".
  Can automatically switch to the "ideal" next map for servers without
  mapvote.

* MessageAdmin

  Allows players to leave a message for the server admin by saying in game
  "!admin xer0 just made me cry".  Basically a cut-down copy of PostBox.
  Not connected to IRC, Twitter or e-mail, just dumps the message in an ini
  file which the admin must check and clear.

* NDgrapnc.GrapplingMut

  A simplified configuration of kxGrapple, which acts similarly to the No
  Downloads grapple.  Shows a red "rope" between the player and the hook, which
  can be disabled.  Does not show team-colored glow on the hook like ND's does.



=== GOOD ===

+ ActorEditor

  Allows the admin to set some properties of game actors during play.
  Only top-level text properties can be adjusted.  (E.g. PlayerStart12.Team,
  ZoneInfo3.Gravity, Bot5.Health)
  Changes can be saved to be re-applied when map is next loaded!
  (This can be used to fix small map bugs server-side without a rebuild.)
  There is no support yet for spawning new actors.
  It is not very easy to use!

+ ChatMuts

  Simple mutator ChatMuts.Ignore allows players to type !ignore KGB so they
  won't receive further chat messages from KGB that game.  Unfortunately does
  not work on taunts.  (For that try semiadmin mod EasyMute.)

+ ClientConsoleLogger

  There are a couple of these already, but this one works in Linux!  Well
  actually it just logs to the standard log.  You have to grep out the
  [ChatLog] lines. :)

+ NDgrapnc.GrapplingMut

  An earlier release of my grappling hook, made as a clone of the No Downloads
  grapple, when ND CTF went down *sob*.

+ PainSounds

  Hitsounds made from grunts and groans in the male and female sound packs.
  Originally part of ELMS/FairLMS.  (Some earlier versions caused players to
  crash when I sent the sound on too many channels.)

+ RedirectPlayers

  Put this mutator on your old server and players will be automatically sent to
  the new server.  (Does not update their favourites but it can display a
  message.)

+ ReduceBalls, PowerfulWeapons, NoPickups and RechargeAmmo

  Four mutators I wrote on request for scarface's  ZeroPing ComboGib server.
  Use them alongside ZP_ShockArena.

  Not as good as some other ComboGib mods!  The original non-zp may have been
  from No Downloads.  iloveut99 released a ZP version on UnrealAdmin in 2011.

  ReduceBalls BUGS: it only removes balls after they have spawned, so sometimes
  you can see a ball briefly appear then disappear, and if you are standing
  right next to someone, you can kill them with a ball before it gets removed!
  (Increasing tickrate might help with this.)
  TODO: Sucks.  Cannot distinguish the 3 different shock hit-types (other
  versions give 0 damage for unpopped balls.)  Cooldown is rubbish; should have
  a proper overheat/cooldown/refuel pattern.

  PowerfulWeapons BUGS: increases strength of shock primary so it acts like
  instagib, but also makes unpopped balls and landing on someone's heads
  deadly!

  Grapple hits kill players.  This may or may not be desirable.

  Optional config for UnrealTournament.ini:

    [PowerfulWeapons.PowerfulWeapons]
    DamageScale=20.0



=== MAYBE ===

~ EasyMuteV16

  Updated Rush's mod a bit so it records the IP addresses of muted players.
  This means they will remain muted if they rejoin, and after mapswitch.

  Did not call it V17 - he can suck in my changes if he wants.

~ FixWeaponBalance

  Attempts to fix weapon unbalance caused by ZeroPing and non-hardcore mode.

  The ZeroPing mod makes hitscan weapons work properly for the client.  (That's
  the sniper, enforcer, shock primary and of course instagib gun.)  But by
  increasing realism, it makes it possible to make more hits than would usually
  occur on a 40 ping national non-zp server.  This has the effect of making the
  non-hitscan weapons less powerful relative to the ones zeroping fixed.

  Furthermore, Siege games are played in non-hardcore mode, in order for the
  jetpack to work properly.  This mode reduces the power of all the weapons,
  but on a player with no armour or vials, a headshot is still a headshot, so
  there is no real reduction.  That's why Siege games turn into a sniper arena,
  because no other weapon really works as well.

  This mutator attempts to restore the balance, and has gone through a number
  of iterations.

  NOTE) I believe the ideal solution is to make a new ZeroPing which slightly
     reduces the headshot and body radius to create accuracy like a 40 ping
     ClanBase server.

  NOTE) Alternatively we can fix the jetpack, and play Siege in hardcore mode.
     Headshots will still be easy, but the other weapons will do their proper
     damage too.  (We could also consider TIW for pulse and minigun.)

  Attempt 1) just an experiment

    Attempted to reduce the cylinder radius of the player's head and body,
    which I think is the ideal solution.  Unfortunately as a mutator using
    Trace(), this suffers from the very lag that zp tries to avoid, so it was
    horridly inaccurate and high-pingers could rarely make a headshot!
    (An American kindly tested it for me and got into a fit doing it. ;)

  Attempt 2) * CURRENT! *

    Reduces the damage caused by hitscan shots.  Amount configurable.

    Works well in zp CTF and LMS games, which are usually played in hardcore
    mode, and give 150 damage for a headshot.  You can scale down to 0.825 or
    even 0.78, and still kill a 0:100 player with the same number of shots as
    normal.  You only actually need to make more hits than usual if the enemy
    has armour/vials.

    But less perfect for Siege (non-hardcore) which only gives 100 for a
    headshot (two-thirds the hardcore damage).  If we reduce that we can never
    make a one-shot headshot on a newly spawned player!  It will require 2
    shots (unless they have been hit by a SuperProtector or team-mate).  This
    is not the same behaviour as in non-zp mode!

    Unfortunately mutators cannot affect armour damage, which is not reduced at
    all, only damage done to health after armour is reduced.  It would need to
    be done by altering the Damage taken in the GameType (out of mutator reach).

  Attempt 3) + Next! +

    Simply make damage for all weapons configurable.

    This would allow us to increase the damage of all of the weapons to
    hardcore mode levels in non-hardcore games.  ZP would still make the sniper
    more accurate, but that could be reduced in damage if desired.

  Aside: Due to the nature of Siege gameplay, and the large flat maps, ripper
  spamming has become quite popular, and one day it might be nice to address
  this.  Perhaps a bouncy ripper (primary) should cost double ammo, or the
  weapon could overheat and pause (lock up, cooldown).  Or we could ... tweak
  its damage.  :P

  Aside: Even when the sniper is back at ClanBase level, I still hate it!  I
  prefer weapons where you have to predict the opponents's future movement, and
  there is some chance involved.  Random shit can be funny.

~ RocketArenaMutator

  Allows you to boost yourself with your own weapons, like you can in
  RocketArena.  I actually changed some of the parameters so the boosts are
  more like CTF warmup mode.  Does not yet provide all weapons like RocketArena
  maps do.  Does not break the game up into 1v1 or 2v2 rounds, it's only a
  simple damage/boost mutator.  Perhaps rocket count may need to be log scaled
  to match the real RA mod.

~ PureSwitcher

  Enables and disables UTPure when using certain maps/modes/mutators/classes.
  Might be a fiddle to configure.  But our server runs Pure whenever it can!

~ Resize

  Resizes a map during play!  Well actually it attempts to resize everything in
  play except for the map, including physics.  Has a few small bugs but does
  work in most ways.  Useful for mappers considering using MapScale but wanting
  to test out some scale values first.  Should not have been written as a
  mutator: cannot increase splash radius.

  BUG: Will adjust projectile speeds "permanently".  I.e. after a map switch,
  even without the mutator, shock balls will fly slowly.  You had better do a
  server restart after using it!

~ Resurector

  If a player disconnects from the server, will bring them back with some of
  their stats intact.  Does not work for SmartCTF.  I just finished a couple of
  missing features in this mod, to stop it from fragging AutoTeamBalance stats.
  Sorry if I wasn't supposed to share it.  :P

~ ScreenExtensions

  Provides triggers to switch or pause Screen slides.  For embedding into maps.
  (Used in SG-GlacierXXLp to fire through Siege tutorial slides.)

~ SiegeAnywhere

  An attempt to provide Siege features into the default UT gametypes.  This
  makes no sense and isn't particularly fun.  But the package *does* include
  SiegeAnywhere.SiegeForever, a mutator which can be used to save and load the
  buildings currently on the map!  Last built for XXL2g.

~ SiegePlusDev

  Attempts at ways of getting noobs out of the supplier *$&!*$"
  Try SiegePlusDev.SupKick, SupBounce or SupFloat.
  SupKick is version agnostic.  Others you may need to recompile.

~ TrackMuts

  TrackMuts.TrackDamage can show you how much damage you are inflicting on
  other players, or how much you are receiving.  Unfortunately mutators cannot
  see damage absorbed by armour/shield - you only see the final health
  reduction.

  TrackMuts.TrackPlayers can be used to display messages about your Flag
  Carrier's location (ZoneName), for servers without SmartCTF.  (Admins can
  also use it for more general player location tracking.)

~ WeirdMuts

  WeirdMuts.ForceBehindView - You can force everyone to play a map in
  behindview mode!

  (There's also a dodgy GrowthMutator which spawns a new piece of flora
  wherever a player dies, sometimes half-embedded in the wall or floor.)

~ kxForceGun

  A crazy weapon which allows you to pick up weapons, siege buildings and bots,
  and move them to other places (or throw them into space!).  Like the gun from
  Half Life.  Still some bugs in Siege (meshes do not update for other
  players), and bots get stuck.

~ sgGrapple

  This patch to Siege provides build options for the kxGrapple, kxForceGun and
  kxDoubleJump items under Items and Teleport categories.

~ sgGrapple2f

  A build for SiegeXXL2d

~ sgGrappleXtreme02

  A build for SiegeXtreme2

~ JLib

  Some common functions I use in UnrealScripts, mostly String related.



=== DODGY ===

- ArenaFallback

  Instagib maps often don't include weapon pickups.  If run in normal mode,
  players have only enforcer and impact hammer.  This mutator will only kick in
  on maps where no weapons are provided, creating an instagib or flak arena.
  (The mutator has been overdeveloped and broken, but one of the earlier
  versions worked fine.)

- CrouchBlocksDamage

  My first ever mutator, for protection against shock balls on XOL.  Never
  actually tried it there!  With some configs, I could boost over Face with
  your own ripper, wheeee!

- RandomMutators

  Given a big list of mutators, will choose a random 3 to use at the beginning
  of each map.  This mutator is classically whack.

- ScreenEditor.deprecated

  On maps using Mychael's Screen actor, allows players to write messages onto
  the screens, or link nearby screens+teleporters to certain servers.  In the
  end I moved this into ActorEditor, and work on the code in ScreenEditor was
  discontinued.

- TranslocBots

  Makes bots translocate around a bit more.  Not very well.

- nBAutoSpectate

  My terrible modification of the AutoSpectate mod.  Attempts to setup
  interesting camera angles when spectating or replaying demos.  Results are
  sometimes interesting, but can be a bit jerky.  Does not work well on demos
  you recorded as a spectator.



=== Some Future Ideas? ===

- RealCrouching

  Make crouching actually reduce a player's height, so they can fit under
  smaller spaces.

  We could even offer a even lower and slower flatten/crawl stance, but we
  don't have meshes for it.  (Perhaps we could re-animate/switch between some
  of the existing feign deaeth sequences.)



