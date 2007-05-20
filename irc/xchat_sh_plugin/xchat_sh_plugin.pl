package IRC::Xchat::XChatShPlugin;

## Hmmm, from b0at on irc:
# use strict; use warnings; use Xchat ':all'; ... hook_print($_, sub { ... exists $commands{$foo} ? ... }) for "Channel Message", "Channel Msg Hilight";

use strict;
use warnings;
use Xchat ':all';

use Thread;

IRC::register("xchat sh plugin", "0.1", "", "");

my $confdir = IRC::get_info(4);

my $IRC::Xchat::XChatShPlugin::xchat_sh_exec = "/home/joey/.xchat2/plugin/xchat_sh_handler.sh";
my $IRC::Xchat::XChatShPlugin::command_scripts_dir = "/home/joey/.xchat2/plugin/xchat_sh_scripts";
# my $IRC::Xchat::XChatShPlugin::xchat_sh_exec = "/home/joey/j/code/other/irc/xchat/xchat_sh_handler.sh";
# my $IRC::Xchat::XChatShPlugin::command_scripts_dir = "/home/joey/j/code/other/irc/xchat/xchat_sh_scripts";

# $locked = 0;

sub xchat_sh_command_trigger {

	my ( @a, @b, @c ) = @_;

	# Xchat::print "\$a[0][0]=$a[0][0]";
	# Xchat::print "\$a[0][1]=$a[0][1]";
	# Xchat::print "\$a[1][0]=$a[1][0]";
	# Xchat::print "\$b[0][0]=$b[0][0]";
	# Xchat::print "\$b[0][1]=$b[0][1]";
	# Xchat::print "\$b[1][0]=$b[1][0]";
	# Xchat::print "\$c=$c";

	# # Xchat::print "\@_=@_";
	# Xchat::print "a \$_[0]=$_[0]";
	# Xchat::print "b \$_[0][0]=$_[0][0]";
	# Xchat::print "c \$_[0][1]=$_[0][1]";
	# Xchat::print "d \$_[0][2]=$_[0][2]";
	# Xchat::print "e \$_[1][0]=$_[1][0]";

	my $line = $_[1][0];
	my @temp = split /\ /, $line;
	my $cmd  = $temp[0];
	my $rest = join(" ", @temp[1..$#temp]);

	my $nick = IRC::get_info(1);

	# xchat_sh_message_trigger($_[0]);
	# exec_shell_command($_[1][0],$_[0][1]);
	exec_shell_command("$cmd","$rest","$nick");

}

sub xchat_sh_message_trigger {

	# Xchat::print "A \$_[0]=$_[0]";
	# Xchat::print "B \$_[0][1]=$_[0][1]";

	## rainbow just does:
	# $_=shift@_;

	# Xchat::print "\$_=$_";
	# Xchat::print "\$\_=$_";
	# Xchat::print "\$_[0]=$_[0]";
	# Xchat::print "\$_[0][0]=$_[0][0]";
	# Xchat::print "\$_[0][1]=$_[0][1]";
	# Xchat::print "\$_[1]=$_[1]";
	# Xchat::print "\$_[2]=$_[2]";

	my $nick = $_[0][0];

	## When using add_command_handler:
	# my $line = $_[2];
	## When using hook_print:
	my $line = $_[0][1];
	my @temp = split /\ /, $line;
	# my $cmd  = @temp[0];
	my $cmd  = $temp[0];
	my $rest = join(" ", @temp[1..$#temp]);

	# if ($locked) {
		# return 0;
		# # Xchat::print "sorry i cannot i am busy"
	# }

	## Allowed characters:
	if ( $cmd =~ /^[!.@-><_]/ ) { # && !$locked )) {
		# $locked = 1;
		$cmd =~ s/^.//;

		## I want to start the process in a new shell, so that I can return,
		## and the line which triggered me can be outputted in XChat before
		## the output from this call!
		## Also, this prevents XChat from freezing whilst the shellscript is running!

		## TODO BUG!  In a detached thread, it might send it to the wrong channel.  :|
		## It was sending some responses to #ut (presumably XChat focused it because there was chat there).
		## But I think it happens even if I don't detach until after calling IRC::command.
		## So maybe it just happens if other chat windows are busy.
		## Maybe we can avoid it by not starting a new thread, or by passing the channel
		## to the thread so it knows where to respond to.

		## TESTING: proposed solution is to start the new thread, but respond to the channel we received it from.  :)
		## Sob.  Still not good enough.  If XChat focus is switched to another server, it messages on that server.  :|

		if ( "$cmd" ) { ## ignores empty commands

			exec_shell_command("$cmd","$rest","$nick");

		}

	}

	# if ( $cmd =~ /^[0-9]*$/ ) {
		# Xchat::command("That's numberwang!", "$respond_to", "$server");
	# }

	# $locked = 0;

	return 0;
}

sub exec_shell_command {

	my ( $cmd, $rest, $nick ) = @_;

	my $version = IRC::get_info(0);
	my $mynick = IRC::get_info(1);
	my $channel = IRC::get_info(2);
	my $server = IRC::get_info(3);
	my $var5 = IRC::get_info(5);
	my $var6 = IRC::get_info(6);
	my $network = IRC::get_info(7); ## TODO: this is actually better known as "network"
	my $topic = IRC::get_info(8);
	# Xchat::print "nick=$nick channel=$channel server=$server confdir=$confdir 5=$var5 6=$var6 network=$network topic=$topic";
	# Xchat::print "cmd=$cmd rest=$rest";
	# # Xchat::print "line=$line";

	my @start_args = ( $cmd, $rest, $nick, $channel, $server, $network );
	# call_shell(@start_args);
	# my $t = Thread->new(\&call_shell, @start_args);
	# $t->detach;
	# # my $result = t->join();

	## Suggested by Khisanth:
	# Xchat::hook_timer( 0, sub { do you thing; } );
	Xchat::hook_timer( 0, sub { call_shell(@start_args); } );
	# Xchat::hook_timer( 0, call_shell(@start_args); );
	## <b0at> Don't forget to return REMOVE

}

sub call_shell {

	my ( $cmd, $rest, $nick, $respond_to, $server, $network ) = @_;

	# Xchat::print "Calling shell: $xchat_sh_exec \"$cmd\" \"$rest\"";
	# open SPOOLER, "sh $xchat_sh_exec $line |"
	# open(SPOOLER, "sh $xchat_sh_exec $line |")
	# # open SPOOLER, "sh \$xchat_sh_exec \$line |"
	# open SPOOLER, "<", "sh", "$xchat_sh_exec", "$line", "|"
	# open(SPOOLER, '-|', "env", "NICK=$nick", "sh", "$IRC::Xchat::XChatShPlugin::xchat_sh_exec", "$cmd", "$rest")
	open(SPOOLER, '-|', "env", "NETWORK=$network", "env", "SERVER=$server", "env", "NICK=$nick", "env", "CHANNEL=$respond_to", "sh", "$IRC::Xchat::XChatShPlugin::xchat_sh_exec", "$cmd", "$rest")
	# open(SPOOLER, '-|', "sh", "$xchat_sh_exec", "$cmd", "$rest")
	or print "open failed: $!\n";
	while (<SPOOLER>) {
		s/\n//g;      # Strip newline
		s/^([^\/])/\/say $1/;
		# s/^([^\/])/\/msg $respond_to \1/;
		# s/\n//g;      # Strip newline
		# Xchat::print "Running IRC::command(\"$_\", \"$respond_to\", \"$server\");";
		# IRC::command ("$_");
		# IRC::command("$_", "$respond_to", "$server");
		s/^\///;
		# Xchat::command ("$_");
		Xchat::command("$_", "$respond_to", "$server");
		# Xchat::print("$_", "$respond_to", "$server");
		# sleep 1; ## avoid flooding
	}
	close SPOOLER or print "close failed: $! $?\n";

	# Thread->self.detach;
	# Thread->self.detach;

	# }

	# return 0;
	return Xchat::REMOVE; ## for timer
}

### When I upgraded the commands system, this stopped working, although idk why cos commands -l should still do the same.  Anyway I disabled the whole lot.
# ## Problem is, when the trigger is called, the actual command types is not passed down!
# # # open(SPOOLER, "-|", "find", "$command_scripts_dir", "-type", "f", "-maxdepth", "1", "|", "afterfirst", "$command_scripts_dir")
# # # open SPOOLER, "find $command_scripts_dir -type f -maxdepth 1 | afterfirst $command_scripts_dir/ |"
# # open SPOOLER, "$command_scripts_dir/commands | sed 's+^[^:]*:++' | tr ' ' '\n' | grep -v '^\$' |"
# open SPOOLER, "$command_scripts_dir/commands -l | grep -v '^\$' |"
# or print "open failed: $!\n";
# while (<SPOOLER>) {
	# s/\n//g;      # Strip newline
	# # Xchat::print "Adding: $_";
	# # IRC::add_command_handler("$_", "IRC::Xchat::XChatShPlugin::xchat_sh_command_trigger");
	# # hook_command("$_", "IRC::Xchat::XChatShPlugin::xchat_sh_command_trigger", ">>"."$_");
	# hook_command("$_", "IRC::Xchat::XChatShPlugin::xchat_sh_command_trigger");
# }
# close SPOOLER or print "close failed: $! $?\n";
# # close SPOOLER or print "close failed: $! $?\n";

# IRC::add_message_handler("PRIVMSG", "IRC::Xchat::XChatShPlugin::xchat_sh_message_trigger");
# IRC::add_print_handler("Channel Msg Hilight", "IRC::Xchat::XChatShPlugin::xchat_sh_message_trigger");
# IRC::add_print_handler("Channel Message", "IRC::Xchat::XChatShPlugin::xchat_sh_message_trigger");
# IRC::add_print_handler("Private Message to Dialog", "IRC::Xchat::XChatShPlugin::xchat_sh_message_trigger");
# IRC::add_print_handler("Your Message", "IRC::Xchat::XChatShPlugin::xchat_sh_message_trigger");
hook_print("Channel Msg Hilight", "IRC::Xchat::XChatShPlugin::xchat_sh_message_trigger");
hook_print("Channel Message", "IRC::Xchat::XChatShPlugin::xchat_sh_message_trigger");
hook_print("Private Message to Dialog", "IRC::Xchat::XChatShPlugin::xchat_sh_message_trigger");
hook_print("Your Message", "IRC::Xchat::XChatShPlugin::xchat_sh_message_trigger");
# hook_print("Your Message", sub { ... exists $commands{$foo} ? ... } );

IRC::print('Loaded XChat sh plugin.');
