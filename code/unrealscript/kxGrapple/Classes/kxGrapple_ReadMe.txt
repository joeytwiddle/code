kxGrapplingHook

Install:

Drop the .u .int .ini files into System folder.
Add ServerPackages=kxGrapple to UnrealTournament.ini.
Use the kxGrapple.kxMutator mutator in a new game.

Overview:

kxGrapplingHook adds realism to the grappling weapon by making you swing according to gravity, momentum and air control (bSwingPhysics,bLinePhysics).  This is slightly harder to control than the original straight-line travel, but with a little practice you can learn some swinging skills and start pulling off some sweet moves!

Allows you shorten or lengthen the line to gain or lose height (bPrimaryWinch,bCrouchReleases).  If you switch to another weapon once you have fired the hook, and then you can just Jump to get off the line.

The line can now be shown in the game (bShowLine).  The current mesh is not very realistic, I need to find a simple straight rope mesh!  But the line does do semi-realistic wrapping around corners (bLineOfSight,bLineFolding)!  :)

I found behindview very useful when using this weapon, so I have included automatic behindview and FOV switching when switching to and from the weapon.  This can be disabled by the admin (bDefaultBehindView,bDefaultChangeFOV), or by the player during the game with "mutate BV off".

By default flag-carriers will drop the flag if they try to use the grappling weapon (bDropFlag).

Other options: You may wish to increase GrappleSpeed, or set bExtraPower to make it faster at *distance* (eh not yet with bLinePhysics).

Tips:

Aim your grapple at the ceiling and dodge/run forwards to swing upwards.  For fast swings without losing height, aim at the ceiling halfway to your destination, and make a fast wide swing outwards.  (I practiced those on Bleak.)

To scale the outside of the tower in Face, aim your grapple at the wall and swing sideways, when you reach the top of your swing, throw the grapple at the wall again, this time a little higher.  To quickly cross the map in Face, you can aim at the distant sky above, and make a long swing.  To get back up from below the Asteroid on Face, just get your hook into one of the towers, and winch up.

History:

adminthis from No Downloads released a nice update of the Expert 100 grappling hook, and increased the speed so it started to become as powerful as a translocator.  With nicely planned routes you could make use of air control and get some extra speed, so I found the weapon very nice to learn.  But the unrealistic physics just bugged me no end, so I just had to implement this feature.  Thanks also to Newton and my mechanics teacher Mr. O'Boyle.

