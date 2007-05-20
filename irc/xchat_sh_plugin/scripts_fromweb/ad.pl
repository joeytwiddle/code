#!/usr/bin/perl -w
#Ad Script 1.0 for Xchat
#By: Kevin Durst (2004)
#Note: I spent a lot of time on this script, please do not change it and claim its yours and release it on the net.
#Thank You
#Another Note: the ad store files are ad1 and ad2 in your home dir( ~/ad1 | ~/ad2)

#pre-dfined time in seconds
$ad1t="800";
$ad2t="800";

#command subs
sub ad1_handler;
sub ad2_handler;
sub ado1_handler;
sub ado2_handler;
sub adof1_handler;
sub adof2_handler;
sub arev_handler;
sub arevo_handler;

#int
$hreason=$hdefreason;
#commands
IRC::register ("Ad Script", "1.0", "", "");
IRC::print "\n\002\0034:: Ad Script 1.0 (Kevin Durst, 2004) ::\003\n";
IRC::print "\n\002\0034 Defualt Timer = 800(s) \003\n";
IRC::print "\n\002\0034 Commmands: \003\n";
IRC::print "\n\002\0034 ad1 (set ad1 msg) \003\n";
IRC::print "\n\002\0034 ad2 (set ad2 msg) \003\n";
IRC::print "\n\002\0034 ado1 (turn on ad1) \003\n";
IRC::print "\n\002\0034 ado2 (turn on ad2) \003\n";
IRC::print "\n\002\0034 adof1 (turn off ad1) \003\n";
IRC::print "\n\002\0034 adof2 (turn off ad2) \003\n";
IRC::print "\n\002\0034 arev (turn on animerev.org ad) \003\n";
IRC::print "\n\002\0034 arev0 (turn off animerev.org ad) \003\n";

IRC::add_command_handler("ad1", "ad1_handler");
IRC::add_command_handler("ad2", "ad2_handler");
IRC::add_command_handler("ado1", "ado1_handler");
IRC::add_command_handler("ado2", "ado2_handler");
IRC::add_command_handler("adof1", "adof1_handler");
IRC::add_command_handler("adof2", "adof2_handler");
IRC::add_command_handler("arev", "arev_handler");
IRC::add_command_handler("arevo", "arevo_handler");

#subcommand commands

sub arev_handler {
 IRC::command "/say \002\0034[\003AD\0034]\003 For Awesome Anime Reviews: http://animerev.org";
 IRC::command "/timer -refnum 3 -repeat 0 $ad1t say \002\0034[\003AD\0034]\003 For Awesome Anime Reviews: http://animerev.org";
 IRC::print "\n\002\0034:: AnimeRev.org Ad Turned On.";
 return 1;
}
sub ad1_handler {
 my $u = shift(@_);
 unlink "ad1";
 open(AD1, ">> ad1");
 	print AD1 "$u";
 close(AD1);
 IRC::print "\n\002\0034:: Ad 1 stored with the following msg: $u";
 return 1;
}

sub ad2_handler {
 my $u = shift(@_);
 unlink "ad2";
 open(AD1, ">> ad2");
 	print AD1 "$u";
 close(AD1);
 IRC::print "\n\002\0034:: Ad 2 stored with the following msg: $u";
 return 1;
}

sub ado1_handler {

open(AD1, "ad1") or warn "Cannot open ad1: $!\n";
	$input = <AD1>;
close(AD1);

 IRC::command "/say \002\0034[\003AD\0034]\003 $input";
 IRC::command "/timer -refnum 1 -repeat 0 $ad1t say \002\0034[\003AD\0034]\003 $input";
 IRC::print "\n\002\0034:: Ad 1 Turned on with the following msg: $input";

 return 1;
}

sub ado2_handler {
open(AD2, "ad2") or warn "Cannot open ad1: $!\n";
	$input = <AD2>;
close(AD2);

 IRC::command "/say \002\0034[\003AD\0034]\003 $input";
 IRC::command "/timer -refnum 2 -repeat 0 $ad1t say \002\0034[\003AD\0034]\003 $input";
 IRC::print "\n\002\0034:: Ad 2 Turned on with the following msg: $input";
 return 1;
}

sub adof1_handler {
 my $u = shift(@_);
 IRC::command "/timer -delete 1";
 IRC::print "\n\002\0034:: Ad 1 Turned off";
 return 1;
}

sub adof2_handler {
 my $u = shift(@_);
 IRC::command "/timer -delete 2";
 IRC::print "\n\002\0034:: Ad 2 Turned off";
 return 1;
}

sub arevo_handler {
 IRC::command "/timer -delete 3";
 IRC::print "\n\002\0034:: AnimeRev.org Ad Turned Off.";
 return 1;
}


