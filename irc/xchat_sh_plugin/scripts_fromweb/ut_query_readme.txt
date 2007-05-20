ut_query.pl (5/23/2003)
Joe Trofimczuk, xchat@joe2net.com

Thanks for your interest in my little script.

License
-------
This script is protected by copyright, I suppose...

But, it is released under the "I Don't Care" license.

This means that I don't much care what you do with it. Also, I don't
really want to hear about your wonderful improvements or the nasty
bugs that you've found. OK, maybe...but I won't go bowling with you.

Also, I might like to learn some of your good Thai food recipes.


What is it?
-----------
With this script you can query an Unreal Tournament server and see the
results in your IRC discussion.


How to install it?
------------------
Just put the script in your X-Chat scripts directory. Use the "/load"
command to load the script. Or, if you prefer, restart your X-Chat.

Keep in mind that the Win32 port of X-Chat has a bummer "/load" command.
The "/ut" command will be triggered once for each time you've loaded the
script.


How to use it?
--------------
Like so:
   /ut ip port
Or, if the script is configured with your favorites, then:
   /ut alias
Or, if you want to repeat the same query you did last time, then:
   /ut last
Or, if you want to see who has been using the script, then:
   /ut show users
Or, if you want to see the other stuff it can do, then:
   /ut help


Just a couple of thoughts...
----------------------------
It would be good to have some option of displaying the query results
only to yourself (IRC::print) instead of always to the channel (IRC::command).
I'll probably add this as a "/ut set var" command.

The script also listens to other users. If anyone else says "!ut blah" then
it will behave as normal. However, if the remote user says "!ut help" then
they will receive the help text in a privmsg window. That's because probably
not everyone in the channel wants to know the help.

If more than a single user in the channel has the script loaded then it's
a good idea if all but one turn off their remote feature: "/ut set remote off".
Otherwise, when someone says "!ut frag1", for example, you will see lots
of repeated stuff.

I may make the thing respond to "/msg blah" instead of "!ut blah". Or,
maybe both.

As written, the script favors the UT community that I prefer. The command
"/ut show maps" scrapes the Yahoo! Briefcase site where the folks I play
with can find their map downloads ("/ut show maps url" will also display
the URLs to those maps).

All output is throttled to prevent a channel from thinking that it is
flooding. It rests 1 second after every ten lines of output.

Each user is given their own memory for things such as color theme. But,
all users see the channel output, so it's a good idea if users select a
color theme that is acceptable to most. For example, X-Chat defaults to
text on black, while mIRC defaults to text on white. Not all of the color
themes use a background color. In fact, maybe the "plain" theme is best.

I have no interest in UT2K3 or any other game. So, please don't ask me to
make this thing generic enough to support your favorite thing.

If this script doesn't do what you want, then please consider it useless,
or maybe, nothing more than sample Perl code for your evening reading.

Have fun!

If this script means half as much to you as it does to me...
...then it means twice as much to me.


As always, I gladly accept cash grants for no particular reason.
