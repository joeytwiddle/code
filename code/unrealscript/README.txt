= nogginBasher's Unreal Tournament mods =



== Downloads ==

Some stable releases are available (either as zip or as .u .ini and .int):

  http://neuralyte.org/~joey/ut_releases/

Most of the others are here, but some may be in developmental state:

  http://hwi.ath.cx/ut/dev/System/

If you still can't find it, you can get the latest source and compile it!
Look in the <PackageName>/Classes folder on:

  http://hwi.ath.cx/ut/dev/

Add the package name to EditorActors= in UT.ini, and then run ucc.exe make.

Some of them are experimental.  Some of them are overdeveloped, in which case
you may prefer to look for an earlier stable version from CVS:

  http://hwi.ath.cx/cgi-bin/cvsweb/code/unrealscript/

If you are really lazy, you can search the site with Google:

  https://www.google.co.uk/search?q=grapple%20site%3Ahwi.ath.cx%2Fut%2F

I write some of the classes using # preprocessor directives.  These classes are
stored in .uc.jpp files, and jpp is used to process them into .uc files.  You
can pick up and work on the .uc, or squeeze "gcc -E" into your build process.



== GREAT ==

* AutoTeamBalance

  Tracks player scores in order to balance the teams at the beginning of each
  game.  Could be better at aiding voluntary mid-game rebalance.

* kxWallJump (ServerPackage *and* Mutator)

  This one is a lot of fun!  It's a mod of UT_DoubleJump restricted to work
  only near walls.  Trapped in a corner, or want to get on top of a box?
  Bounce off the walls like a ninja, as if your boots could grip the walls.

  Admins or single-players can "mutate jumptype" to cycle the jumping modes.
  The default is a difficult/useful.  I even managed to set suitable meshes!

* PubliciseScore

  Add info to the server's title as displayed in the UT server list.  Can show
  the number of players, current team scores, and time remaining in the game.
  Later versions can also cycle the server title, e.g. to present news ticker.

* PostBox

  Allows players to leave messages for their friends which can be picked up
  later.

* FairLMS

  A mutator-style clone of the ELMS gametype from the No Downloads servers.
  LastManStanding with a twist: your health is dropping, only frags can get you
  more health, and 4 frags will give you a powerup!  Great fun.  Too bad the
  jolt server is empty these days.

* kxGrapple (ServerPackage *and* mutator)

  My advanced grappling hook for UT.  Simulates a strong rope with a strong
  winch, and realistic gravity, swinging the player, and wrapping around edges.
  Offers players the option of winch-in, hold or wind-out, offhand grappling,
  and toggling of behind-view and FOV when grapple is selected.  Based on
  Expert100, inspired by No Downloads.  A fun substitute for the Translocator
  on CTF maps, *except* those with no walls or ceiling, or where the player needs
  to translocate through a small window.

  NOTE: You should not download kxGrapple.u since this is my development
  version and subject to change (using this one is just asking for Version
  Mismatch).  Instead download the latest release, e.g. kxGrapple04rc2.u

  NOTE: Might be incompatible with UTPure.  I don't remember for sure.

  BUGS: Replication could be improved.  Laggers can warp around a lot when
  using it rapidly.

  Some players like to toggle grapple on/off with either mouse button, but
  kxGrapple strictly toggles with left-throw, right-retract bindings, like a
  translocator does.  I have left it configurable (client or server side?), but
  I should probably make the defaults noob-friendly.

  If enabled, offhand grapple is available on: MUTATE TOGGLEHOOK

* NDgrapnc

  An earlier rigid-line grappling hook, made as a clone of the No Downloads
  grapple, when their CTF disappeared.  The red line may be disabled in the
  ini.

  Must be installed as a ServerPackage.  Run the mutator NDgrapnc.GrapplingMut.

* TeamSwitcher

  Provides a few commands to players: !red !blue !green !spec !play !vote

* MapMuts

  Collects stats on maps into one big file, including approximate size
  (player-capacity) of map, number of times played, total frags, has it
  crashed.  Can also catch counts of players saying "good map" and "bad map".

  Can automatically choose a suitable next map for the current number of
  players, providing a nice map cycle for servers without mapvote.

* MessageAdmin

  Allows players to leave a message for the server admin by saying in game
  "!admin xer0 just made me cry".  Not connected to IRC, Twitter or e-mail,
  just dumps the message in an ini file which the admin must read and clear.



== GOOD ==

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

+ ClientConsoleLogger (Client package)

  There are a couple of these already, but this one works in Linux!  Well
  actually it just logs to the standard log.  You have to grep out the
  [ChatLog] lines. :)

+ FixWeaponBalance (3rd generation)

  Allows admin to tweak the damage dealt by various weapons.  Now applies
  changes *before* armor absorption.

  Admins who are logged in can try "mutate help".  The mod can be toggled with
  "mutate fwb" and display live damage changes with "mutate fwbreport".
  A summary of the current configuration is available on "mutate status".

  All normal weapon damage can be configured in terms of "scaling".  There is
  also a new feature bScaleSniperByDistance which aims to reward close-range
  snipers and discourage long-range sniping.

  See the mod's README for more info.

  Released into the wild late Nov 2012.  Bugfixes may need to follow...  :)

+ PainSounds

  Hitsounds made from grunts and groans in the male and female sound packs.
  Originally part of ELMS/FairLMS.  (Some earlier versions caused players to
  crash when I sent the sound on too many channels.)

+ RedirectPlayers

  Put this mutator on your old server and players will be automatically sent to
  the new server.  (Does not update their favourites but it can display a
  message.)

+ ReduceBalls, PowerfulWeapons, NoPickups and RechargeAmmo

  Four mutators I wrote on request for scarface's ZeroPing ComboGib server.
  Use them alongside UTPure's ZP_ShockArena.  (May also work with ZPP103.)

  Not as good as some other ComboGib mods, but purely mutators, no
  ServerPackages.  iloveut99 released a ZP version on UnrealAdmin in 2011.

  ReduceBalls BUGS: it only removes balls after they have spawned, so sometimes
  you can see a ball briefly appear then disappear, and if you are standing
  right next to someone, you can kill them with a ball before it gets removed!
  (Increasing tickrate might help with this.)
  BUG: Cooldown is a bit naff; should have a proper overheat/refuel pattern.

  PowerfulWeapons: Increases strength of shock primary so it acts like
  instagib, but BUG: also makes unpopped balls, landing on someone's head and
  grapple hits deadly!

  Optional config for UnrealTournament.ini:

    [PowerfulWeapons.PowerfulWeapons]
    DamageScale=20.0



== MAYBE ==

~ EasyMuteV16

  Updated Rush's mod a bit so it records the IP addresses of muted players.
  This means they will remain muted if they rejoin, and after mapswitch.

  Did not call it V17 - he can suck in my changes if he wants.

~ NerfAmmo

  So far only one mutator: NerfAmmo.NerfSniper

  Allows configuration of StartingAmmo, AmmoPerPack, and MaxAmmo.  Can stop
  camper sprees from lasting too long, e.g. by setting MaxAmmo=20.

  TODO: This could be done in a structured config that would allow admins to
  add settings for new weapons without needing a rebuild.  The same applies to
  FixWeaponBalance.

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
  even without the mutator, shock balls will fly slowly.  You should really do
  a server restart after using it!

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

  TrackMuts.TrackPlayers can be used to display hints for noobs: messages about
  your Flag Carrier's location, for servers without SmartCTF.  It can also be
  used to track zone-changes by specific players.

~ WeirdMuts

  WeirdMuts.ForceBehindView - You can force everyone to play a map in
  behindview mode!

  (There's also a dodgy GrowthMutator which spawns a new piece of flora
  wherever a player dies, sometimes half-embedded in the wall or floor.)

~ kxDoubleJump

  Minor changes to DoubleJump to make it more organic (input velocity partially
  retained, you can jump to higher spots but it's difficult!).  This package
  later became kxWallJump which I prefer!

~ kxForceGun

  A crazy weapon which allows you to pick up weapons, siege buildings and bots,
  and move them to other places (or throw them into space!).  Like the gun from
  Half Life.  Still some bugs in Siege (meshes do not update for other
  players), and bots get stuck.  No sound effects, but a proof-of-concept.

~ sgGrapple

  This addon to Siege provides build options for the kxGrapple, kxForceGun and
  kxDoubleJumpBoots items under Items and Teleport categories.

~ sgGrapple2f

  A build for SiegeXXL2d.  That's right, 2d, not 2f.

~ sgGrappleXtreme02

  A build for SiegeXtreme2.

~ JLib

  Some common functions I use in UnrealScripts, mostly String related.

~ ut_magic_redirect

  Find this on github.  Needs to have node.js installed!



== DODGY ==

- ArenaFallback

  Instagib maps often don't include weapon pickups.  If run in normal mode,
  players have only enforcer and impact hammer.  This mutator will only kick in
  on maps where no weapons are provided, creating an instagib or flak arena.
  (The mutator has been overdeveloped and broken, but one of the earlier
  versions worked fine.)

- CrouchBlocksDamage

  My first ever mutator, for protection against shock balls on XOL.  Never
  actually tried it there!  Reduces boost as well as damage.  With some config
  tweaks, I could boost myself over Face with my own ripper, wheeee!

- RandomMutators

  Given a big list of mutators, will choose a random 3 to use at the beginning
  of each map.  This mutator is classically whack.

- ScreenEditor.deprecated

  On maps using Mychael's Screen actor, allows players to write messages onto
  the screens, or link nearby screens+teleporters to certain servers.  In the
  end I moved this into ActorEditor, and work on the code in ScreenEditor was
  discontinued.  I embedded it in the later crashy ThornsPortal map.

- TranslocBots

  Makes bots translocate around a bit more.  Not very well.

- nBAutoSpectate

  My terrible modification of the AutoSpectate mod.  Attempts to setup
  interesting camera angles when spectating or replaying demos.  Results are
  sometimes interesting, and sometimes buggy.  Does not work well on demos
  you recorded as a spectator.



== Some Future Ideas? ==

- RealCrouching

  Make crouching actually reduce a player's height, so they can fit under
  smaller spaces.  The fact that UT does not already do this is a little too
  unreal-istic for me.

  We could even offer an even lower and slower flatten/crawl stance, but we
  don't have meshes for it.  (Perhaps we could re-animate/switch between some
  of the existing feign death sequences.)



