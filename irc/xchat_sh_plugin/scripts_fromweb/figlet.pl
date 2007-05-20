#!/usr/bin/perl -w
IRC::register("SteveCs figlet script", "0.1", "", "");
IRC::print "Loading SteveCs figlet script 0.1\n";
IRC::add_command_handler("fig", "stevec_figlet");

sub stevec_figlet
{
	@a=`figlet @_`;
	for $b (@a){
		chop $b;
		IRC::command(" $b");    #don't just print $b if it starts
	}                               #with a / or \ then the server
	return 1;                       #might get angry and screw us up
}
