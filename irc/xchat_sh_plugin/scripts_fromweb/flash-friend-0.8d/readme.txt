Flash's X-Chat scripts:
        irc-flash@digdilem.org
        Flash_ at #3am irc.quakenet.org and #perl irc.oftn.net
        
Written on and for Windows's X-Chat versions, although should run fine on Linux too with
a small adjustment of the home path. (Nice to turn the tables on all the linux-only scripts
available for X-Chat... :x )  Works across multiple connected servers.

History:
0.8b - First release.
0.8d - Fixes issue that stopped it working with some servers. (There are different types of JOIN string 
depending on server, I didn't realise that some servers were broken... Should be fine now. No change to
config so it's a drop-in upgrade.)

Short Description:

Watches for people joining channels you are in. If one of those nicks matches a name
in your friend list, it quietly ops them. Obviously only works if you have ops in the first place... :)
Allows per-channel opping and global opping using wildcard "ALL". See example friends.txt for details.
Automatically reloads changed friends file.

Warnings: 
        1. Only checks nicks, not hostnames. Somebody could fake a nick on a non nickserv-controlled server 
                and be opped by you.

Eg:
[14:06] Friend: Pengbot found, opping.
[14:06] --> Pengbot (~Pengbot@Flashbot.users.quakenet.org) has joined #flashbots
[14:06] --- Flash_ sets modes [#flashbots +o Pengbot]

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
        
        Flash-friend.pl   - The script
        friends.txt     - The nicks of your friends that you want to op.
        
2.
If Linux, edit Flash-friend.pl and amend $friendfile to something like "~/.xchat/" or whatever your xchat user dir is.
If Windows, no need to edit (unless you want to disable buttons)

3.
Load script in Xchat. Either use "/load "full path"" or the nice little popup menu. By placing .pl files in this 
folder, they are automatically loaded by Xchat on startup, so you could just restart Xchat and it should load.


On Success, you will notice "Loading Flashy's Friend script" printed somewhere and a new button. Click that
button (windows) to load the friend list for editing)

