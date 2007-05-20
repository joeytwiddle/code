Flash's X-Chat scripts:
        irc-flash@digdilem.org
        Flash_ at #3am irc.quakenet.org and #perl irc.oftn.net
        
Written on and for Windows's X-Chat versions, although should run fine on Linux too with
a small adjustment of the home path. (Nice to turn the tables on all the linux-only scripts
available for X-Chat... :x )  Works across multiple connected servers.

History:
0.9  - Update. Now matches on partial trigger, does not require full trigger.
       Aarcane suggested a code addition to allow codes to be included on the 
         triggers as well as on the replies - so now it can trigger on your nick etc.
       (Config file format unchanged, existing configs will work)
0.8c - First release.

Description:
Watches for incoming messages. Checks them against channel and trigger list. If matches,
replies automatically. Not restricted to "/say" - can "/me" actions and "/kick" etc. 
Can expand codes to nicks, channels etc. Quite versatile. 
Automatically reloads trigger config if it changes.
Toggle all triggers on or off with "/trigger"
Adds button to edit triggerlist (Windows)

Note: Not fully tested on multiple servers.
Note: Your reply appears before their trigger occurs, due to way Xchat works. They see it
        normally.
Note: Now triggers on strings /containing/ your trigger string.  
        Eg, "!example" will trigger, so will "!EXAmplE" and, since 0.9 -
        "this is my !example trigger" will do too. 
        
Eg:
[14:07] Pengbot> !example 
[14:07] <Flash_> Me: Flash_, You: Pengbot, Chan: #flashbots, Server: pelikaista.fi.quakenet.org, Topic: topic, blah blah blah blah


Config instructions in triggers.txt

Upgrading:
Copy the .pl over the previous version. Check this file for changes to any configuration
files. Don't overwrite your existing ones without backing up first.

Installation:


0a. WINDOWS: YOU MUST HAVE THE PERL PLUGIN ENABLED AND PERL INSTALLED!
0b. LINUX: YOU MUST HAVE PERL INSTALLED!

1.
Copy the following files to your X-Chat's HOME directory. (The user's dir, not where you installed xchat)
        Under Windows XP - this dir is: "C:\Documents and Settings\YOUR_USER_NAME\Application Data\X-Chat 2". 
         When you find the right one you'll see a bunch of .conf files. You can quick-find by opening Logfiles 
         dir from Xchats prefs menu/logging and ascend one level. Note "Application Data" is set to HIDDEN by 
         default so you may not find it unless you enable hidden files in Explorer. 
         Yeah, I know - MS sucks, news at eleven.
        
        Under linux, it's "~/.xchat/" by default. 
        
        Flash-trigger.pl   - The script
        triggers.txt     - The nicks of your friends that you want to op.
        
2.
If Linux, edit Flash-trigger.pl and amend $triggerfile to something like "~/.xchat/" or whatever your xchat user dir is.
Linux users will probably want to either disable buttons, or change the edit string at the bottom of the script to something vaguely useful instead of notepad.

If Windows, no need to edit (unless you want to disable buttons). UNLESS IT BREAKS! Some versions of windows don't supply
the user's home directory that this script needs. If that happens, you'll need to enter the full path to your trigger file
in the config. Note that windows' backslashes should be replaced with forward slashes, and any spaces preceeded by one 
backslash each. (I know it's annoying...)

3.
Load script in Xchat. Either use "/load "full path"" or the nice little popup menu. By placing .pl files in this 
folder, they are automatically loaded by Xchat on startup, so you could just restart Xchat and it should load.


On Success, you will notice "Loading Flashy's Trigger script" printed somewhere and a new button. Click that
button (windows) to load the trigger list for editing)

