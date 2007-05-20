#!/usr/bin/perl
#
# Flashy's script. irc-flash@digdilem.org - poke Flash_ at irc.quakenet.org - #3am
# If used in your own work, please include my name in it somewhere, thanks.
#
# Trigger script. Reads triggers in from textfile and responds for you when those
# triggers are called in channel.
# Toggle off/on with "/trigger" (Default is ON when script loads)
###################################################################################
# Start user configuration (Not required under windows if you put triggers.txt in 
# the right place and your version of Windows supplies HOME environment...)
###################################################################################
# Where is the trigger file? (Default - Xchat's windows' home. 
# $triggerfile = "$ENV{'HOME'}/Application\ Data/X-Chat\ 2/triggers.txt";
$triggerfile = "$ENV{'HOME'}/.xchat2.nogginBasher/triggers.txt";
# Do you want an extra button to edit triggerfile?
$fltr_button=1;
###################################################################################
# End user configuration
###################################################################################
IRC::add_message_handler("PRIVMSG", "watch_triggers");
$fltr_debug=0;  # Toggle debug
$fltr_enabled=1; # Initial working state. yes or no. Toggleable with /trigger
$fltr_version="0.9";
Xchat::register( "Flashy's Trigger Script", "$fltr_version", "User Triggers", "" );
Xchat::print "Loading Flashy's Trigger Script $fltr_version (Use /trigger to toggle)";
if ($fltr_debug) { Xchat::print"Debug mode ON!"; }

$fl_tr_size = ( -s $triggerfile);
open (TF, "<$triggerfile") or ( Xchat::print("Flash-triggers: Cannot load trigger file: $triggerfile") );
@FLTF = <TF>;
close(TF);

sub watch_triggers {  # Called every time a message is put to a chan you are in
        if ($fltr_enabled) {
                if ($fl_tr_size ne (-s $triggerfile)) {  #Triggerlist has changed, reload
                                $fl_tr_size = (-s $triggerfile);
                                open (TF, "<$triggerfile"); # Don't warn if not there
                                @FLTF = <TF>;
                                close(TF);
                                Xchat::print "Trigger List Filesize changed, reloading..."; 
                                }
                $fltr_mynick = IRC::get_info(1);
                $fltr_server = IRC::get_info(3);
                $fltr_topic = IRC::get_info(8);
                
                foreach $line (@_) {
                        $line =~ m/\:(.*?)\!(.*?)\sPRIVMSG\s(.*?)\s\:(.*)?/;
                        
                        $m_nick = $1;
                        $m_send = $2;
                        $m_chan = $3;
                        $m_line = $4;
                        
                        $m_line =~ s/^\s+//; # Remove trailing whitespace
                        $m_line =~ s/\s+$//;
                        
                        if ($fltr_debug) { Xchat::print"String: [$m_line]"; }
                        
                        foreach(@FLTF) {
                                $fltr_first = substr($_,0,1);
                                if ($fltr_first ne '#') { # Ignore commented lines
                                        $_ =~ chomp;
                                        
                                        @fltr_cfg = split(/\|/); 
                                        $t_trig = $fltr_cfg[0];
                                        $t_chan = $fltr_cfg[1];
                                        $t_resp = $fltr_cfg[2];
                                        # 04/04/05 - Aarcane suggested adding codes to trigger string:
                                        $t_trig =~ s/%n/$m_nick/gi;
                                        $t_trig =~ s/%m/$fltr_mynick/gi;
                                        $t_trig =~ s/%c/$m_chan/gi;
                                        $t_trig =~ s/%s/$fltr_server/gi;
                                        $t_trig =~ s/%t/$fltr_topic/gi;
                                        
                                        if ($fltr_debug) { Xchat::print"\"$t_trig\"|$t_chan|$t_resp - $_"; }
                                        # Check each config entry for correct channel, or "ALL"
                                        if ((lc($m_chan) eq lc($t_chan)) or (lc($t_chan) eq 'all')) {                                            
                                                if ($fltr_debug) { Xchat::print"Channel Hit!"; } 
                                                # Now check to see if trigger matches
                                                #if (lc($t_trig) eq lc($m_line)) { # Check for exact triugger
                                                if (lc($m_line) =~ /$t_trig/i) { # Check to see whether it contains partial trigger
                                                        # Add stuff to parse responds
                                                        $t_resp =~ s/%n/$m_nick/gi;
                                                        $t_resp =~ s/%c/$m_chan/gi;
                                                        $t_resp =~ s/%m/$fltr_mynick/gi;
                                                        $t_resp =~ s/%s/$fltr_server/gi;
                                                        $t_resp =~ s/%t/$fltr_topic/gi;
                                                        
                                                        IRC::command("/$t_resp");
                                                        }
                                                } else { if ($fltr_debug) { Xchat::print"Channel Not Hit!"; } }
                                        }
                                }
                        }
                }
}

if ($fltr_button =~ /1/) { # Add button to edit friendlist
IRC::command "/delbutton Triggers";
IRC::command "/addbutton Triggers exec notepad $triggerfile";
}

# Add client control settings.
IRC::add_command_handler("trigger", "fltr_trigger_toggle");

sub fltr_trigger_toggle {
        if ($fltr_enabled) {
        Xchat::print"Triggers Disabled";
        $fltr_enabled=0;
        } else {
        Xchat::print"Triggers Enabled";
        $fltr_enabled=1;
        }
return 1;
}
