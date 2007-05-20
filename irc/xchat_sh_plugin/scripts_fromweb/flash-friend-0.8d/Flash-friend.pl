#!/usr/bin/perl
#
# Flashy's script. irc-flash@digdilem.org - poke Flash_ at irc.quakenet.org - #3am
# If used in your own work, please include my name in it somewhere, thanks.
# 
# Auto-Op: If a friend's nick matches one found in "friends.txt", This script will
# op them when they join a channel you are in, if you have ops. (Simplistic, no hostname
# check, so 'ware of somebody using anothers' nick.)
# Also adds "Friends" button below userlist, so you can easily edit the list in notepad

# Where is the friends.txt file? (Default - Xchat's windows' home. 
$friendfile = "$ENV{'HOME'}/Application\ Data/X-Chat\ 2/friends.txt";

# Change this to 0 if you are linux or don't want the friendlist button
$fl_frbutton=1;

#================== End of user config===========
$fl_frdebug=0; # Toggles debug mode
$fl_frversion = "0.8d";
Xchat::register( "Flashy's Friend Script", "$fl_frversion", "Friend Opper", "" );
Xchat::print "Loading Flashy's Friend script $fl_frversion";

IRC::add_message_handler("JOIN", "watch_joins");

$fl_fr_size = ( -s $friendfile);
open (FR, "<$friendfile"); # Don't warn if not there
@FL = <FR>;
close(FR);

if ($fl_frdebug) { Xchat::print "Filesize is: $fl_fr_size"; }

sub watch_joins { # called every time someone joins a chan
        foreach $line (@_) {
                if ($fl_fr_size ne (-s $friendfile)) {  #Friendlist has changed, reload
                        $fl_fr_size = (-s $friendfile);
                        open (FR, "<$friendfile"); # Don't warn if not there
                        @FL = <FR>;
                        close(FR);
                        Xchat::print "Friends List Filesize changed, reloading $friendfile"; 
                        }
                if ($fl_frdebug) { Xchat::print "Filesize is: $fl_fr_size"; }
                        
                if( $line =~ /^:([^!]+)!.+ JOIN :?(#.+)$/ ) {
                
                    $m_nick = $1;
                    $m_chan = $2;
                    }                    
                    
                if ($fl_frdebug) { Xchat::print"Derived nick: [$m_nick]"; }
                if ($fl_frdebug) { Xchat::print"Derived chan: [$m_chan]"; }
         
                foreach (@FL) {
                        chomp;
                        @friends = split(/,/,$_);                        
                        my $friend = @friends[0];
                        my $fr_chan = @friends[1];
                        $flfrfirst = substr($friend,0,1);
                        if ($flfrfirst ne '#') { # Ignore commented lines
                                if (($fr_chan eq $m_chan) or ($fr_chan eq uc('ALL'))) { # In right channel
                                        if ($fl_frdebug) { Xchat::print "Channel match [$m_chan] and [$fr_chan]"; }
                                                
                                        if ($m_nick eq $friend) {
                                                Xchat::print ("Friend: $friend found, opping in $m_chan.");
                                                IRC::command("/mode $m_chan +o $m_nick");
                                                } 
                                                else {
                                                if ($fl_frdebug) { Xchat::print "Check: '$friend' != '$m_nick' (Chan $m_chan)"; }
                                                        }
                                        } else  { if ($fl_frdebug) { Xchat::print "Channel NOT match [$m_chan] and [$fr_chan]"; } }
                                }
                        }
                }
}

if ($fl_frbutton =~ /1/) { # Add button to edit friendlist
IRC::command "/delbutton Friends";
IRC::command "/addbutton Friends exec notepad \"$ENV{'HOME'}\\Application Data\\X-Chat 2\\friends.txt\"";
}
