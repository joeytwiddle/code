#!/usr/bin/perl -w
#
# XQF -> XChat Script
# Version 0.70 (Released Jun 28, 2003) 
# Written by Douglas Bollinger (Doogman) <doug@sdf.lonestar.org>
#
# Special thanks to Ludwig Nussel for bugfixes and additional features
# and Marcus Garcia (NiTrO) for ideas and testing.
#
# Also like to thank Gatewood Green (Woody) for Perl coding techniques for
# working within Xchat.
#
# This script is public domain and you are free to use this code as you like
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  Nevertheless, I'm
# still very interested in bug reports, feature requests and user feedback
# sent to the above email address.
#
# Based on the GameSpy -> mIrc Script by Peter Reinhold
#
# This script allows other IRC users to query for stats of a gameserver you
# are using by typing a command.  Requires XQF and XChat.
#
# Also included is optional Licq support.  When you start gaming, the
# script will set your ICQ status to a user-specified setting and update
# your autoresponse message with the stats of the game server you are
# currently using.  When you return from the game, your ICQ status will be
# set to available and your autoresponse message will be reset.
#
# NOTE: The Licq integration is no longer maintained, but you can enable it
# below.  If you have a patch to make Licq work better again, I would
# appreciate it.
#
# XQF is available at http://www.linuxgames.com/xqf/
# XChat is available at http://www.xchat.org
# Licq is available at http://www.licq.com
#
# The latest version of this script is available for download at the 
# XQF -> XChat homepage at http://doug.freeshell.org/xqf-xchat/
#
# Tested on Linux XChat 1.8.11, XQF 0.9.11.2 and Licq 1.0.3 
#
# To use, copy this script to ~/.xchat/ and enable LaunchInfo.txt support
# in XQF via preferences/appearance.

package Xqf;
use strict;
use vars qw (%message %filepath %flood_times %flood_hits %flood_game
  %config %flood_setting %nick %licq %script %scriptinfo @active_channels);

&IRC::register( "Xqf-XChat", "0.70", "", "" );
&IRC::add_message_handler( "PRIVMSG", "Xqf::privmsg_handler" );
&IRC::add_message_handler( "JOIN",    "Xqf::join_handler" );
&IRC::add_command_handler( "return",  "Xqf::return_handler" );
&IRC::add_command_handler( "xqf-xchathelp",  "Xqf::help_handler" );

# Global variables for option control
# Set to 1 to enable, 0 to disable
$config{'licq'}   = 0;    # Licq support
$config{'nick'}   = 0;    # Enable nick change that shows the player's gaming
                          # status ie: nick to nick[game]
$config{'away'}   = 0;    # Enable setting of /away when you start a game
$config{'join'}   = 1;    # When you are gaming, alert people who join the
                          # channel(s) you are using of what command to type
                          # for your game stats
$config{'notice'} = 0;    # Alert chatters with notice when you start a game
$config{'notice_return'} = 0;    # Alert chatters with notice when you return
                                 # from a game
$config{'wall'}        = 1; # Alert chatters with wallchan when you start a game
$config{'wall_return'} = 1; # Alert chatters with wallchan when you return
                            # from a game
                            # Note1: wallchan is less annoying to mIRC
                            # users and is preferred
							# Note2: wallchan messages are disabled when
							# using the active_channels feature				
$config{'licq_status'} = "na";           # Status for Licq while you are gaming
                                         # Choose from:  na, away, occupied, dnd
$script{'irccommand'}  = "!gameinfo";    # Command to receive stats
$script{'pingserver'}  = "!aping"; 		 # Command to activate server pinging

# Below is the setting to enable/disable server pinging.  Here's a typical
# use for this feature to describe it's function:  You and your friends
# meet in a IRC channel and decide to use a public server.  Someone uses
# XQF to find a nice server, uses copy or copy+ to grab the server info,
# types in xchat "!aping" (default, can be changed) and pastes the server
# information (the dotted quad IP address) after the command into a xchat
# window.  All the other xchat users who have this script running with the
# pingserver feature enabled will automatically ping the server and report
# the ping results back to that channel for the other users to see.  This
# way you can find a good server for all the people in the channel quickly
# and easily with a minimal amount of cut and pasting of IP numbers.
#
# Input examples:
# !aping 205.177.13.20
# !aping 205.177.13.20:27963
# !aping 66  205.177.13.20:27963   =JC= Just Cause  mp_assault  10 of 20
#
# Output example:
# 205.177.13.20= 64.219 ms
#
# Since this feature probably can be used for nefarious purposes, it is
# disabled by default; you can enable it below.  It is also recommended to
# use the active_channels feature to limit the script activity to just the
# channel(s) that you and your gaming buddies use for meetings.

$config{'pingserver'} = 0;	# Enable server pinging 
							# Set to 1 to enable, 0 to disable

# Below is the active channel setting.  Normally this script listens for
# commands and sends the join and notice/wall messages to every channel
# opened in the main Xchat window.  Sometimes you want to monitor channels
# while gaming whose participants aren't interested in your gaming
# information, so the active channels feature was added.  Any channels
# added to the active channels list will be the ONLY ones monitored for
# commands or to receive join or notice/wall messages.  Note:  Due to the
# nature of wall (write all), it will be completely disabled when using
# active channels.  If you want other chatters (in channels not in the
# active channel list) to be notified when you start or return from a game,
# you must activate notice and/or notice_return.  These are set off by
# default.
# 
# Please note that if you activate the "away" setting that people in
# channels other than the active channel list will see a message.  This is
# a feature of IRC.
#
# Put your active channels inbetween the parenthesis, with a single space
# between the channels and each channel starting with a pound (#) sign.
# Add as many channels as you like.  If no active channels are entered,
# _ALL_ channels will be monitored for commands and receive script output,
# which is the default setting.
# 
# Example:
# @active_channels = qw(#clanxqf #myotherclan);

@active_channels= qw();

# Flood protection settings
$flood_setting{'attempts'}   = 3;        # Number of irccommands before flood
                                         # protection is enabled and chatter
                                         # is ignored
$flood_setting{'timespan'}   = 30;       # Seconds before flood counter is
                                         # reset
$flood_setting{'timeignore'} = 120;      # Seconds before chatter is
                                         # un-ignored

# Set paths to important files
$filepath{'launchinfo'} = "$ENV{HOME}/.qf/LaunchInfo.txt";
$filepath{'licq_fifo'}  = "$ENV{HOME}/.licq/licq_fifo";

$script{'playing'}   = 0;                # You are not gaming by default
$script{'licq_time'} = 0;                # Reset Licq refresh time

# Global variables containing script info
$scriptinfo{'name'}    = "XQF -> XChat";
$scriptinfo{'version'} = "0.70";
$scriptinfo{'author'}  = "Douglas Bollinger";
$scriptinfo{'email'}   = "doug\@sdf.lonestar.org";
$scriptinfo{'URL'}     = "http://doug.freeshell.org/xqf-xchat/";

# Print script info 
&IRC::print( "\n\0033" . "-" x 45 . "\003\n" );
&IRC::print(
"Loading\002 $scriptinfo{'name'} \002 Version:\002 $scriptinfo{'version'}\002"
);
&IRC::print("By: $scriptinfo{'author'} \002<\002$scriptinfo{'email'}\002>\002");
&IRC::print("$scriptinfo{'URL'}");
&IRC::print("\nFor help: /xqf-xchathelp");
&IRC::print( "\0033" . "-" x 45 . "\003\n\n" );

# Remove old LaunchInfo.txt file
if ( -e $filepath{'launchinfo'} ) { unlink $filepath{'launchinfo'} }

# init messages
&Xqf::update_messages();

&IRC::add_timeout_handler( 2000, "Xqf::checklaunchinfo_handler" );
&IRC::add_timeout_handler( 3000, "Xqf::flushignorelist_handler" );

sub update_messages {
    $Xqf::nick{'default'} = &IRC::get_info(1);    # Gets current nick
         # If enabled, nick is changed to below while gaming
         # You can change default nick ending [game] to whatever
    $Xqf::nick{'game'} = $Xqf::nick{'default'} . "[game]";

    # Various script messages are below
    $Xqf::message{'ignore'} =
"$Xqf::nick{'default'}'s XQF -> XChat script is ignoring you for $Xqf::flood_setting{'timeignore'} seconds!";
    $Xqf::message{'ignore_ret'} =
      "$Xqf::nick{'default'}'s XQF -> XChat script is listening to you again";
    $Xqf::message{'away'} =
      "I'm gaming right now.  $Xqf::script{'irccommand'} for more info.";
    $Xqf::message{'notice'} =
"$Xqf::nick{'default'} is gaming.  $Xqf::script{'irccommand'} for more info.";
    $Xqf::message{'not_playing'} =
      "$Xqf::nick{'default'} is not gaming right now";
    $Xqf::message{'return'} = "I'm back from the game!";
}

sub checklaunchinfo_handler {
    my (@channels);
    if ( !-e $Xqf::filepath{'launchinfo'} ) {
        &IRC::add_timeout_handler( 2000, "Xqf::checklaunchinfo_handler" );
    }
    else {
        $Xqf::script{'playing'} = 1;
        &Xqf::update_messages();
        if ( $Xqf::config{'notice'} ) {
            @channels = &Xqf::retrieve_channels;
            foreach (@channels) {
                &IRC::command("/notice $_ $Xqf::message{'notice'}");
            }
        }
        if ( $Xqf::config{'wall'} ) {
			if ( @Xqf::active_channels) {
				&IRC::print("\n\002$Xqf::scriptinfo{'name'}: \0035Sorry,".
					" wallchan messages are disabled"); 
				&IRC::print("\002\0035when using a active channel list!");
			} else {
				&IRC::command("/WALLCHAN $Xqf::message{'notice'}");
			}
        }
        if ( $Xqf::config{'nick'} ) {
            &IRC::command("/nick $Xqf::nick{'game'}");

            # Make sure the nick change goes through
            unless ( &IRC::get_info(1) eq $Xqf::nick{'game'} ) {
                &IRC::print(
"\002$Xqf::scriptinfo{'name'}: \0035Waiting for nick change..."
                );
                sleep 2;
            }
        }
        if ( $Xqf::config{'away'} ) {
            &IRC::command("/away $Xqf::message{'away'}");
        }

        # Print the return instructions to the XChat user
        &IRC::print(
            "\n\002$Xqf::scriptinfo{'name'}: \0035" . "#" x 27 . "\n" );
        &IRC::print(
            "\002$Xqf::scriptinfo{'name'}: \0035Type /return to reset Xchat\n");
        &IRC::print(
            "\002$Xqf::scriptinfo{'name'}: \0035" . "#" x 27 . "\n\n" );
    }
    return 0;
}

sub make_message_serverstats {
    my (
        @launchinfo,       $gamestart,        $server_game,
        $server_name,      $server_ip,        $server_mod,
        $server_time_mins, $server_time_secs, @message
    );
    open( LAUNCHINFO, $Xqf::filepath{'launchinfo'} );    # Import server stats
    chomp( @launchinfo = <LAUNCHINFO> );
    close(LAUNCHINFO);
    foreach (@launchinfo) { s/^\w+\s// }    # Weed out unnecessary info
    ( $server_game, $server_name, $server_ip, $server_mod ) =
      (@launchinfo)[ 0, 1, 2, 3 ];          # Set variables from array
    ($gamestart) = ( stat( $Xqf::filepath{'launchinfo'} ) )[9];
    $server_time_mins = int( ( time - $gamestart ) / 60 );
    $server_time_secs = ( time - $gamestart ) % 60;
    $message[0] =
      "$Xqf::nick{'default'} is playing $server_game " . "/ $server_mod";
    $message[1] = "Servername: $server_name";
    $message[2] = "ServerIP: $server_ip";
    $message[3] =
"$Xqf::nick{'default'} started playing $server_time_mins mins $server_time_secs secs ago";
    return @message;
}

sub privmsg_handler {
    my ( $line, $nick, $channel, $message, @message_outgoing );
    $line = shift (@_);
	# Is the next line crash proof?
	$line =~ /:(.*)!(.*@.*) .* (#.*) :(.*)/;
    $nick = $1;
	$channel = $3;
	$message = $4;

	# Don't do anything if the nick is being ignored
	unless ( exists( $Xqf::flood_ignore{$nick} )) {
        # If the command comes from a channel in the active_channels list
		# or the active_channels list is empty, process command
		if (grep { /^$channel$/ } @Xqf::active_channels or
			! @Xqf::active_channels) {
			# Process the gameserver stat command
			if ( $message =~ /^$Xqf::script{'irccommand'}/i ) {
				if ( !$Xqf::script{'playing'} ) {
					&IRC::command(
						"/notice $nick $Xqf::message{'not_playing'}")
				} else {
					@message_outgoing = &Xqf::make_message_serverstats();
					# Send server stats to person who asked
					foreach (@message_outgoing) {
						&IRC::command("/notice $nick $_");
					}
					# Start floodprotection on user who called
					&Xqf::floodprotect($nick);
				}
			}
			
			# Process the server ping command
			if ($message =~ /^$Xqf::script{'pingserver'}/i and 
				$Xqf::config{'pingserver'}){
				if ($Xqf::script{'playing'}) {
					&IRC::command("/notice $nick No pinging while gaming")
				} else {
					@message_outgoing = &Xqf::make_message_ping($message);	
					# Send ping stats to room
					foreach (@message_outgoing) {
						&IRC::command("/notice $channel $_");
					}
					# Start floodprotection on user who called
					&Xqf::floodprotect($nick);
				}
			}
		}
	}
	return 0;
}

sub make_message_ping {

	my ($line, $ip_number, @ping_output, $average_ping, $average_line,
	@message);
	$line = shift (@_);
	# This regexp should find the dotted quad in the message line, if not
	# it messages an error to the person who sent the command
	if ($line !~ /.*?(\d+\.\d+\.\d+\.\d+).*/) {
		@message = "Can't understand this IP number: $line";
	} else {
		$ip_number = $1;
        &IRC::print("\002$scriptinfo{'name'}: \0035Pinging $ip_number");
		# This is the command that will run ping for the server latency.
		# The -w flag is the maximum seconds ping will run, the -i flag is
		# how long ping will wait in seconds between packets and the -c
		# flag is the maximum amount of pings it will send to the ip number
		# If you seem to have trouble with pingserver, try using -i 1
		@ping_output = `ping -w 2 -i .5 -c 3 $ip_number`;
		$average_line= $ping_output[-1];
		# Take the last line and use a regexp to grab the average while
		# checking to make sure the ping output is legit
		if ($average_line !~ m#^.+= \S+/(\S+)/\S+/.*#) {
			# Uh-oh, 100% packet loss
			if ($average_line !~ /^rtt.*/	) {
				$average_ping = "Cannot connect";
			} else {
				# Catch all error message
				&IRC::print("\002$scriptinfo{'name'}: \0035Having trouble parsing the ping command?!?");
			}
		} else {
			$average_ping = "$1 ms";
		}
			@message = "$ip_number= $average_ping";
	}
	return @message;
}

sub help_handler {
	&IRC::print( "\n\0033" . "-" x 55 . "\003\n" );
	&IRC::print( "Make sure you enabled LaunchInfo.txt support in XQF" );
	&IRC::print( "via Preferences / Appearance." );
	&IRC::print( "\nAlso, your Licq (if running) status and autoresponse" );
	&IRC::print( "message will be changed to reflect your gaming status." );
	&IRC::print( "This can be defeated by editing this script file." );
	&IRC::print( "\nWhen you start a game via XQF, the script will" );
	&IRC::print( "automatically start running.  When you are done gaming, ");
	&IRC::print( "type /return in a XChat window to inform the script you ");
	&IRC::print( "have returned. ");
	&IRC::print( "\0033" . "-" x 55 . "\003\n\n" );
	return 1;
}

sub return_handler {
    my (@channels);
    unless ( $Xqf::script{'playing'} ) {
        &IRC::print("\002$scriptinfo{'name'}: \0035You are not gaming!");
        return 1;
    }
    if ( $Xqf::config{'nick'} ) {
        &IRC::command("/nick $Xqf::nick{'default'}");
		# Make sure the nick change goes through
        unless ( &IRC::get_info(1) eq $Xqf::nick{'default'} ) {
            &IRC::print(
                "\002$scriptinfo{'name'}: \0035Waiting for nick change...");
            sleep 2;
        }
    }
    if ( $Xqf::config{'away'} )          { &IRC::command("/away") }
    if ( $Xqf::config{'notice_return'} ) {
        # Tell all valid channels user is returning
        @channels = &Xqf::retrieve_channels;
        foreach (@channels) {
            &IRC::command("/notice $_ $Xqf::message{'return'}");
        }
    }
    if ( $Xqf::config{'wall_return'} ) {
		if ( @Xqf::active_channels) {
			&IRC::print("\n\002$Xqf::scriptinfo{'name'}: \0035Sorry,".
				" wallchan messages are disabled"); 
			&IRC::print("\002\0035when using a active channel list!");
		} else {
	        &IRC::command("/WALLCHAN $Xqf::message{'return'}");
		}
    }

    # Delete LaunchInfo.txt and reset playing variable
    unlink $Xqf::filepath{'launchinfo'};
    $Xqf::script{'playing'} = 0;

    # Reset Licq auto-response message and set status to online
    if ( &Xqf::check_licq() ) {
        system("echo auto_response > $Xqf::filepath{'licq_fifo'}");
        system("echo status online > $Xqf::filepath{'licq_fifo'}");
    }

    # Start looking for LaunchInfo.txt again
    &IRC::add_timeout_handler( 2000, "Xqf::checklaunchinfo_handler" );
    return 1;
}

sub join_handler {
    my ( $line, $nick, $channel );
    $line = shift (@_);
    $line =~ /:(.*)!(.*@.*) .*:(.*)/;
    $nick = $1;
	$channel = $3;

    # If user is playing, send message to channel joiners
    if ( $Xqf::script{'playing'} and $Xqf::config{'join'} ) {
        # If the join comes from a channel in the active_channels list
		# or the active_channels list is empty, send message 
        if (grep { /^$channel$/ } @Xqf::active_channels or
			! @Xqf::active_channels) {
			&IRC::command("/notice $nick $Xqf::message{'notice'}");
		}
    }
    return 0;
}

sub floodprotect {
    my ($nick);
    $nick = $_[0];

    # Increment tally for nick 
    $Xqf::flood_hits{$nick}++;

    # Sets the time of first query.  
    if ( !exists( $Xqf::flood_times{$nick} ) ) {
        $Xqf::flood_times{$nick} = time + $Xqf::flood_setting{'timespan'};
    }

    # Nick exceeded queries/time.  Add nick to flood ignore 
    if ( $Xqf::flood_hits{$nick} > ( $Xqf::flood_setting{'attempts'} ) ) {
        &IRC::command("/notice $nick $Xqf::message{'ignore'}");
        $Xqf::flood_ignore{$nick} = time + $Xqf::flood_setting{'timeignore'};
    }
}

sub flushignorelist_handler {
    my ( $nick, $time );
    if ( keys(%Xqf::flood_times) ) {
        while ( ( $nick, $time ) = each(%Xqf::flood_times) ) {
            if ( $time < time ) {
                # Times up.  Remove nick to reset query timer
                delete( $Xqf::flood_times{$nick} );
                delete( $Xqf::flood_hits{$nick} );
            }
        }
    }
    if ( keys(%Xqf::flood_ignore) ) {
        while ( ( $nick, $time ) = each(%Xqf::flood_ignore) ) {
            if ( $time < time ) {
                # Times up.  Un-ignore nick & send mess.
                delete( $Xqf::flood_ignore{$nick} );
                &IRC::command( "/notice $nick "
                    . "$nick, $Xqf::message{'ignore_ret'}" );
            }
        }
    }

    # A player might switch servers without restarting the script.
    # For the Licq message to be accurate we have to check for a
    # server change at regular intervals
    if ( $Xqf::script{'playing'} and &Xqf::check_licq() ) {
        &Xqf::freshen_licq_awaymessage;
    }
    &IRC::add_timeout_handler( 3000, "Xqf::flushignorelist_handler" );
    return 1;
}

sub check_licq {
    if ( $Xqf::config{'licq'} and ( grep { /licq/ } `ps -ax` ) ) {
        return 1;
    }
    return 0;
}

sub retrieve_channels {
	my (@channels, @valid_channels, %seen, $room);

	@channels = grep { /^#/ } &IRC::channel_list();
	@seen{@Xqf::active_channels} = ();
	# If we have active channels, go through all the channels we have open
	# in Xchat and only copy into valid_channel channels that are both
	# being used and in active_channel.  This way you can put channels in
	# active_channel that you aren't using every session and not have
	# errors when the script try to access them and the window isn't open
	if (@Xqf::active_channels) {
		foreach $room (@channels) {
			push (@valid_channels, $room) if exists $seen{$room};
		}
	} else {
		# Empty active_channel?  Then all open channels are fair game
		@valid_channels = @Xqf::active_channels;
	}
	return @valid_channels;

}

sub freshen_licq_awaymessage {
    my ( $gamestart, @mesg, $output );
    ($gamestart) = ( stat( $Xqf::filepath{'launchinfo'} ) )[9];

    # Is Licq autoresponse messsage outdated?
    if ( $Xqf::script{'licq_time'} < $gamestart ) {

        # Get server stat messages
        @mesg = &Xqf::make_message_serverstats();

		# The problem is here:  The shell escaping is off somehow.  It used
		# to work but now it doesn't. :(
		#
        # Need to get \n's to Licq but Perl wants to expand them.
        # To make Perl ignore them we need to use double \'s
        # Also need to use \" to get quotes around message so 
        # shell doesn't choke on special characters. ie '|> etc.
		$output = "\"status $Xqf::config{'licq_status'} $mesg[0]\\n$mesg[1]\\n$mesg[2]\"";

        # Send status & autoresponse message string to Licq
        system("echo $output > $Xqf::filepath{'licq_fifo'}");

        # Set time of last Licq away message
        $Xqf::script{'licq_time'} = $gamestart;

        # Print what's going to X-Chat user
        &IRC::print(
"\002$Xqf::scriptinfo{'name'}: \0035Updating Licq autoresponse message & setting status to $Xqf::config{'licq_status'}"
        );
    }
}

