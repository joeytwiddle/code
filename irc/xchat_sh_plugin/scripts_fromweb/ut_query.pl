#!/usr/local/bin/perl -w
# $Id: ut_query.pl,v 1.1 2007/05/20 21:07:54 joey Exp $
#========================================================================
# UT Query:   UDP query of a UT server.
#
#             Examples:
#               /ut help
#               /ut 66.150.208.208 7778
#               /ut 66.150.208.156
#               /ut frag1
#               /ut last
#               /ut show aliases
#               /ut show maps url
#               /ut show users
#               /ut show themes
#               /ut set theme [theme]
#
#             Listens to other users in the channel:
#               !ut help
#               (and so on...)
#
# Version:
#  05/15/2003.  Initial version.
#
#  05/16/2003.  Added !ut bot feature for other users.
#
#  05/18/2003.  Added "/ut last".
#               Added "/ut show users".
#
#  05/19/2003.  Added "/ut show maps [url]".
#
#  05/21/2003.  Added "/ut show themes".
#               Added "/ut set theme [theme]".
#
#  05/23/2003.  Added "/ut show remote".
#               Added "/ut set remote on|off".
#
# To do:      Maybe make it respond to "/msg blah" stuff like most bots.
#             Make it watch for your friends and say "hey! so-n-so is now playing wherever!".
#
#========================================================================

use strict;

use Time::Local;
use Socket;
use Sys::Hostname; 
use HTTP::Request::Common qw(POST);
use LWP::UserAgent;


#========================================================================

our $scriptName            =  "UT Query";
our $scriptVersion         =  "0.1";
our $scriptLongName        =  "$scriptName for XChat - v$scriptVersion";


#========================================================================

IRC::register($scriptName, $scriptVersion, "", "");
IRC::print "Loading $scriptLongName\n";
IRC::add_command_handler("ut", "localUT");
IRC::add_print_handler("Channel Message", "remoteUT");


#========================================================================

my %colors                  =  ( "white"   =>  "0",    "black"   =>  "1",
                                 "blue"    =>  "2",    "green"   =>  "3",
                                 "red"     =>  "4",    "brown"   =>  "5",
                                 "purple"  =>  "6",    "orange"  =>  "7",
                                 "yellow"  =>  "8",    "lime"    =>  "9",
                                 "teal"    =>  "10",   "aqua"    =>  "11",
                                 "royal"   =>  "12",   "pink"    =>  "13",
                                 "grey"    =>  "14",   "silver"  =>  "15",
                               );

my $ResetColor                =  "\cC";
my $ResetAll                  =  "\017";
my $Reverse                   =  "\026";
my $Bold                      =  "\002";
my $Underline                 =  "\037";

my $default_theme             =  "plain";
my %themes                    =  ( "plain"   => {"h1" => [],                  "h2" => [],                  "h3" => [],                 "h4" => []},
                                   "blue"    => {"h1" => ["blue"],            "h2" => ["aqua"],            "h3" => ["royal"],          "h4" => ["blue"]},
                                   "red"     => {"h1" => [],                  "h2" => ["red"],             "h3" => ["red"],            "h4" => ["blue"]},
                                   "odd"     => {"h1" => ["yellow"],          "h2" => ["aqua"],            "h3" => ["orange"],         "h4" => ["blue"]},
                                   "sunny"   => {"h1" => ["red","yellow"],    "h2" => ["red","orange"],    "h3" => ["red","yellow"],   "h4" => ["blue","yellow"]},
                                   "lime"    => {"h1" => [],                  "h2" => ["lime"],            "h3" => ["lime"],           "h4" => ["blue"]},
                                   "quake"   => {"h1" => ["silver","brown"],  "h2" => ["silver","brown"],  "h3" => ["silver","brown"], "h4" => ["blue","brown"]},
                                   "usa"     => {"h1" => ["red","white"],     "h2" => ["white","blue"],    "h3" => ["blue","red"],     "h4" => ["blue","white"]},
                                   "amber"   => {"h1" => ["yellow","black"],  "h2" => ["yellow","black"],  "h3" => ["yellow","black"], "h4" => ["blue"]},
                                   "ocean"   => {"h1" => ["lime","blue"],     "h2" => ["white","blue"],    "h3" => ["white","blue"],   "h4" => ["aqua","blue"]},
                                   "purple"  => {"h1" => [],                  "h2" => ["purple"],          "h3" => ["purple"],         "h4" => ["blue"]},
                                   "green"   => {"h1" => [],                  "h2" => ["green"],           "h3" => ["green"],          "h4" => ["blue"]},
                                   "pink"    => {"h1" => ["pink","black"],    "h2" => ["pink","black"],    "h3" => ["pink","black"],   "h4" => ["blue","black"]},
                                   "xmas"    => {"h1" => ["red","white"],     "h2" => ["green","white"],   "h3" => ["red","white"],    "h4" => ["blue","white"]},
                                   "nova"    => {"h1" => ["yellow","red"],    "h2" => ["yellow","red"],    "h3" => ["yellow","red"],   "h4" => ["aqua","red"]},
                                   "spaz"    => {"h1" => ["yellow","purple"], "h2" => ["red","orange"],    "h3" => ["purple","lime"],  "h4" => ["blue","lime"]},
                                 );

my $default_alias             =  "frag1";
my %aliases                   =  ( "frag1"      => ["66.150.208.208",  7778],
                                   "frag2"      => ["66.150.208.156",  7778],
                                   "brutality"  => ["65.151.132.205",  7778],
                                   "skematic"   => ["66.92.186.80",    7778],
                                   "arc"        => ["216.178.1.47",    7778],
                                   "blossom1"   => ["66.151.183.197",  7778],
                                   "blossom2"   => ["12.212.72.186",  27581],
                                   "nthz"       => ["66.151.183.180",  7778],
                                   "hunter"     => ["66.151.132.66",   7778],
                                 );

my %players                   =  ();

my %globals                   =  ();

my $remote                    =  1;


#========================================================================

sub color {
  my ($nick, $h, $theme)      =  @_;

  if (not defined $theme) {
    $theme                    =  $globals{$nick}{theme};
  }

  my $hspecRef                =  $themes{$theme}{$h};
  my @hspec                   =  @$hspecRef;

  my $fg                      =  $hspec[0];
  my $bg                      =  $hspec[1];

  my $code                    =  "";

  if (defined $fg) {
    $code                    .=  $ResetColor . $colors{$fg};

    if (defined $bg) {
      $code                  .=  "," . $colors{$bg};
    }
  }

  return $code;
}


#========================================================================

sub fixUTF8 {
  my ($in)                    =  @_;

  my $out                     =  "";
  for (split //, $in) {
    my $code                  =  ord($_);
    if ($code < 128) {
      $out                   .=  $_;
    }
    elsif (($code >= 128) && ($code <= 2047)) {
      if ($code == 154) {
        # small s with caron fails the algorithm
        $out                 .=  chr(197) . chr(161);
      } else {
        $out                 .=  chr(192 + int($code / 64));
        $out                 .=  chr(128 + ($code % 64));
      }
    }
    elsif (($code >= 2048) && ($code <= 65535)) {
      $out                   .=  chr(224 + ($code / 4096));
      $out                   .=  chr(128 + (($code / 64) % 64));
      $out                   .=  chr(128 + ($code % 64));
    }
    elsif (($code >= 65536) && ($code <= 2097151)) {
      $out                   .=  chr(240 + ($code / 262144));
      $out                   .=  chr(128 + (($code / 4096) % 64));
      $out                   .=  chr(128 + (($code / 64) % 64));
      $out                   .=  chr(128 + ($code % 64));
    }
    elsif (($code >= 2097152) && ($code <= 67108863)) {
      $out                   .=  chr(248 + ($code / 16777216));
      $out                   .=  chr(128 + (($code / 262144) % 64));
      $out                   .=  chr(128 + (($code / 4096) % 64));
      $out                   .=  chr(128 + (($code / 64) % 64));
      $out                   .=  chr(128 + ($code % 64));
    }
    elsif (($code >= 67108864) && ($code <= 2147483647)) {
      $out                   .=  chr(252 + ($code / 1073741824));
      $out                   .=  chr(248 + (($code / 16777216) % 64));
      $out                   .=  chr(128 + (($code / 262144) % 64));
      $out                   .=  chr(128 + (($code / 4096) % 64));
      $out                   .=  chr(128 + (($code / 64) % 64));
      $out                   .=  chr(128 + ($code % 64));
    }
  }

  return $out;
}


sub timeStamp {
  my ($sec,$min,$hour,$tday,$mon,$year,$wday,$yday,$isdst)  =  localtime(time);

  return sprintf("%02s/%02s/%04s at %02s:%02s:%02s", $mon + 1, $tday, $year + 1900, $hour, $min, $sec);
}


sub preventFlood {
  my ($nick)                  =  @_;

  $globals{$nick}{flood}++;
  if ($globals{$nick}{flood} > 10) {
    $globals{$nick}{flood}    =  0;
    sleep 1;
  }
}


sub messageToUser {
  my ($nick, $message)        =  @_;

  if ($nick eq IRC::get_info(1)) {
    IRC::print $message;
  } else {
    my @a                     =  split /\n/, $message;
    for $b (@a) {
      IRC::command "/msg $nick $b";
    }
  }
}


#========================================================================

sub byFragsThenName
{
   my $playerT                =  $players{$a};
   my @playerA                =  @$playerT;
      $playerT                =  $players{$b};
   my @playerB                =  @$playerT;

   $playerB[0]  <=>  $playerA[0]
     ||
   $a           cmp  $b;
}


#========================================================================

sub prepGlobals {
  my ($nick)                  =  @_;

  if (defined $globals{$nick}{useCount}) {
    return;
  }

  $globals{$nick}{useCount}   =  0;
  $globals{$nick}{lastTime}   =  "";
  $globals{$nick}{lastAct}    =  "";
  $globals{$nick}{lastIP}     =  "";
  $globals{$nick}{lastPort}   =  0;
  $globals{$nick}{theme}      =  $default_theme;
  $globals{$nick}{flood}      =  0;
}


#========================================================================

sub doHelp {
  my ($nick)                  =  @_;

  my $msg                     =  color($nick, "h1") . $Bold . "$scriptLongName\n"                 .
                                 color($nick, "h3") .         "Like so: /ut [alias | ip [port]] (alias defaults to $default_alias)\n"  .
                                 color($nick, "h3") .         "Or:      /ut last\n"               .
                                 color($nick, "h3") .         "Or:      /ut set remote on|off\n"  .
                                 color($nick, "h3") .         "Or:      /ut set theme [theme]\n"  .
                                 color($nick, "h3") .         "Or:      /ut show aliases\n"       .
                                 color($nick, "h3") .         "Or:      /ut show maps [url]\n"    .
                                 color($nick, "h3") .         "Or:      /ut show remote\n"        .
                                 color($nick, "h3") .         "Or:      /ut show themes\n"        .
                                 color($nick, "h3") .         "Or:      /ut show users\n";
  messageToUser($nick, $msg);

  return 1;
}


#========================================================================

sub doShowAliases {
  my ($nick)                  =  @_;

  IRC::command    color($nick, "h1") . $Bold .              "$scriptLongName";
  IRC::command    color($nick, "h2") .                      "Available aliases:";
  IRC::command    color($nick, "h3") .                      "  "                  .
                  color($nick, "h3") . $Bold . $Underline . "Alias          "     . $Underline . "  " .
                  color($nick, "h3") .         $Underline . "IP             "     . $Underline . "  " .
                  color($nick, "h3") .         $Underline . "Port  ";
  foreach (sort keys %aliases) {
    preventFlood($nick);
    IRC::command  color($nick, "h3") .                      "  $_"    .
      " " x (15 - (length $_))                                        .
      "  " . $aliases{$_}[0]                                          .
      " " x (15 - (length $aliases{$_}[0]))                           .
      "  " . $aliases{$_}[1];
  }
  if ($globals{$nick}{lastIP} ne "") {
    IRC::command  color($nick, "h3") .                      "* last"  .
      " " x 11                                                        .
      "  " . $globals{$nick}{lastIP}                                  .
      " " x (15 - (length $globals{$nick}{lastIP}))                   .
      "  " . $globals{$nick}{lastPort};
  }
}


sub doShowMaps {
  my ($nick, $param)          =  @_;

  my $URL                     =  "briefcase.yahoo.com/bc/bunnehbiscuit/lst?.dir=/&.begin=9999&.view=1";
  my $ua                      =  LWP::UserAgent->new();
  $ua->agent("Mozilla/4.7 [en] (X11; U; Linux 2.2.17 i686)");
  my $req                     =  HTTP::Request->new(GET => "http://".$URL);
  $req->header(Accept  => "text/html");
  my $contentResponse         =  $ua->request($req);

  IRC::command      color($nick, "h1") . $Bold .              "$scriptLongName";
  IRC::command      color($nick, "h2") .                      "Maps available from Buni\'s Yahoo briefcase:";
  IRC::command      color($nick, "h3") .                      "  "                              .
                    color($nick, "h3") . $Bold . $Underline . "File                          "  . $Underline . "  " .
                    color($nick, "h3") .         $Underline . "Type"                            . $Underline . "  " .
                    color($nick, "h3") .         $Underline . "Size    ";
  my $count                   =  0;

  my $stuff                   =  $contentResponse->content;
  while ($stuff  =~ m/.*<a\s*?href="(.*c24e83c9\/bc\/.*?)">/gi)
  {
    my $url                   =  $1;
    my ($name, $ext)          =  ($url     =~ m/.*c24e83c9\/bc\/(.*)\.(.*)\?/gi);
    my ($size)                =  (($stuff  =~ m/size=-1>(.*\s*?(K|M)B)<\/font>/gi) ? $1 : "?");

    preventFlood($nick);
    $count++;
    IRC::command    color($nick, "h3") .         "  $name" .
      " " x (30 - (length $name))                          .
      "  " . $ext                                          .
      " " x (4 - (length $ext))                            .
      "  " . $size;
    if (lc $param eq "url") {
      IRC::command  color($nick, "h4") .         "  ($url)";
    }
  }
  if ($count < 1) {
    IRC::command " None";
  }
}


sub doShowRemote {
  my ($nick)                  =  @_;

  my $msg                     =  "remote is turned ";

  my $status                  =  ($remote == 1) ? "on" : "off";

  messageToUser($nick, "<" . $scriptName . "> $msg $status" . ".");
}


sub doShowThemes {
  my ($nick)                  =  @_;

  IRC::command    color($nick, "h1") . $Bold .              "$scriptLongName";
  IRC::command    color($nick, "h2") .                      "Available color themes:";
  IRC::command    color($nick, "h3") .                      "  "               . 
                  color($nick, "h3") . $Bold . $Underline . "Theme          "  . $Underline . "  " .
                  color($nick, "h3") .         $Underline . "Sample                                   ";
  foreach (sort keys %themes) {
    preventFlood($nick);
    my $flag                  =  ($globals{$nick}{theme} eq $_) ? "*" : " ";
    IRC::command                      "$flag $_"           .
      " " x (15 - (length $_))                             .
      "  " . color($nick, "h1", $_) . "[ half ]"           . $ResetAll .
      "  " . color($nick, "h2", $_) . "[ light ]"          . $ResetAll .
      "  " . color($nick, "h3", $_) . "[ epoch ]"          . $ResetAll .
      "  " . color($nick, "h4", $_) . "[ phase ]";
  }
}


sub doShowUsers {
  my ($nick)                  =  @_;

  IRC::command      color($nick, "h1") . $Bold .              "$scriptLongName";
  IRC::command      color($nick, "h2") .                      "Recently used by:";
  IRC::command      color($nick, "h3") .                      "  "                        . 
                    color($nick, "h3") . $Bold . $Underline . "User           "           . $Underline . "  " .
                    color($nick, "h3") .         $Underline . "Last Activity            " . $Underline . "  " .
                    color($nick, "h3") .         $Underline . "At                    "    . $Underline . "  " .
                    color($nick, "h3") .         $Underline . "Count";
  my $count                  =  0;
# foreach (sort byUseCountThenNick keys %globals) {
  foreach (sort keys %globals) {
    if ($globals{$_}{useCount} > 0) {
      preventFlood($nick);
      $count++;
      my $flag                =  ($nick eq $_) ? "*" : " ";
      IRC::command  color($nick, "h3") .         "$flag $_"  .
        " " x (15 - (length $_))                             .
        "  " . $globals{$_}{lastAct}                         .
        " " x (25 - (length $globals{$_}{lastAct}))          .
        "  " . $globals{$_}{lastTime}                        .
        " " x (22 - (length $globals{$_}{lastTime}))         .
        "  " . $globals{$_}{useCount};
    }
  }
  if ($count < 1) {
    IRC::command "  Nobody";
  }

}


sub doShow {
  my ($nick, $what, $param)   =  @_;

  if      (lc $what eq "aliases") {
    doShowAliases($nick);
  } elsif (lc $what eq "maps")    {
    doShowMaps($nick, $param);
  } elsif (lc $what eq "remote")  {
    doShowRemote($nick);
  } elsif (lc $what eq "themes")  {
    doShowThemes($nick);
  } elsif (lc $what eq "users")   {
    doShowUsers($nick);
  } else {
    messageToUser($nick, "<" . $scriptName . "> Don't know how to show \'$what\'.");
  }

  return 1;
}


#========================================================================

sub doSetRemote {
  my ($nick, $param)          =  @_;

  my $msg                     =  "remote turned";

  if      (lc $param eq "on")  {
    $remote                   =  1;
  } elsif (lc $param eq "off") {
    $remote                   =  0;
  } else {
    $msg                      =  "Please say \'on\' or \'off\', not ";
  }

  messageToUser($nick, "<" . $scriptName . "> $msg $param" . ".");
}


sub doSetTheme {
  my ($nick, $param)          =  @_;

  if (defined $themes{$param}) {
    $globals{$nick}{theme}    =  $param;

    messageToUser($nick, "<" . $scriptName . "> Your theme is now $param" . ".");
  } else {
    messageToUser($nick, "<" . $scriptName . "> $param is not a defined theme.");
  }
}


sub doSet {
  my ($nick, $what, $param)   =  @_;

  if      (lc $what eq "remote") {
    doSetRemote($nick, $param);
  } elsif (lc $what eq "theme")  {
    doSetTheme($nick, $param);
  } else {
    messageToUser($nick, "<" . $scriptName . "> Don't know how to set \'$what\'.");
  }

  return 1;
}


#========================================================================

sub remoteUT {
  my ($nick, $rest)           =  ($_[0] =~ m/^(.*?)\s+(.*)\s+\(null\)\Z/);

  $nick                       =~ s/^\cC[0-9]{1,2}//;     # strip color off nick (xchat bug)

  my @temp                    =  split /\ /, $rest;
  my $cmd                     =  @temp[0];
     $rest                    =  join(" ", @temp[1..$#temp]);

  if ((lc $cmd eq "!ut") && ($remote == 1)) {

    my @ARGV                  =  split /\ /, "$rest";
    return doUT($nick, @ARGV);
  }

  return 0;
}


sub localUT
{
  my @ARGV                    =  split /\ /, "@_";

  my $nick                    =  IRC::get_info(1);

  return doUT($nick, @ARGV);
}


sub doUT
{
  my $nick                    =  shift(@_);

  prepGlobals($nick);
  my $activity                =  "@_";

  my $param1                  =  shift(@_) || "$default_alias";
  my $param2                  =  shift(@_);
  my $param3                  =  shift(@_);

  if (defined $aliases{$param1}) {
    $param2                   =  $aliases{$param1}[1];
    $param1                   =  $aliases{$param1}[0];
  }

  my $result                  =  0;

  if      (lc $param1 eq "help") {
    $result                   =  doHelp($nick);
  } elsif (lc $param1 eq "show") {
    $result                   =  doShow($nick, $param2, $param3);
  } elsif (lc $param1 eq "set")  {
    $result                   =  doSet($nick, $param2, $param3);
  }

  $globals{$nick}{lastTime}   =  timeStamp();
  $globals{$nick}{lastAct}    =  $activity;
  $globals{$nick}{useCount}++;

  if ($result > 0) {
    return 1;
  }

  if (lc $param1 eq "last") {
    if ($globals{$nick}{lastIP} ne "") {
      $param1                 =  $globals{$nick}{lastIP};
      $param2                 =  $globals{$nick}{lastPort};
    } else {
      messageToUser($nick, "<" . $scriptName . "> Can\'t specify \'last\' when there wasn\'t one.");
      return 1;
    }
  }

  $globals{$nick}{lastIP}     =  $param1;
  $globals{$nick}{lastPort}   =  $param2;

  my $protocol                =  getprotobyname 'udp';
  my $addressClient           =  sockaddr_in 0, scalar gethostbyname hostname;
  my $addressServer;
  if (not socket SERVER, PF_INET, SOCK_DGRAM, $protocol) {
    messageToUser($nick, "<" . $scriptName . "> Failed socket: $!");
    return 1;
  }
  if (not bind SERVER, $addressClient) {
    messageToUser($nick, "<" . $scriptName . "> Failed bind: $!");
    return 1;
  }
  if (not ($addressServer  =  sockaddr_in $param2, inet_aton $param1)) {
    messageToUser($nick, "<" . $scriptName . "> Server not found: $!");
    return 1;
  }

  send SERVER, '\\status\\', 0, $addressServer;
  my $handleRead              =  "";
  vec($handleRead, fileno SERVER, 1) = 1;
  my $serverInfo              =  "";
  while (select $handleRead, undef, undef, 0.5) {
    my $chunk;
    if (not defined(my $srcpaddr = recv SERVER, $chunk, 0x1000, 0)) {
      messageToUser($nick, "<" . $scriptName . "> Failed recv: $!");
      return 1;
    }
    $serverInfo              .=  $chunk;
  }
  close SERVER;

  if ($serverInfo eq "") {
    messageToUser($nick, "<" . $scriptName . "> Nothing recv: $!");
    return 1;
  }

  $serverInfo                 =~ s/\\//;
  $serverInfo                 =~ s/\\final\\$//;

  $serverInfo                 =  $serverInfo;

  my ($hostname)              =  fixUTF8(($serverInfo  =~ m/hostname\\\s?(.*?)\s?\\/i)   ? $1 : "N/A");
  my ($maptitle)              =  fixUTF8(($serverInfo  =~ m/maptitle\\\s?(.*?)\s?\\/i)   ? $1 : "N/A");
  my ($mapname)               =  fixUTF8(($serverInfo  =~ m/mapname\\\s?(.*?)\s?\\/i)    ? $1 : "N/A");
  my ($fraglimit)             =         (($serverInfo  =~ m/fraglimit\\\s?(.*?)\s?\\/i)  ? $1 : "--");
  my ($numplayers)            =         (($serverInfo  =~ m/numplayers\\\s?(.*?)\s?\\/i) ? $1 : "??");
  my ($maxplayers)            =         (($serverInfo  =~ m/maxplayers\\\s?(.*?)\s?\\/i) ? $1 : "??");

  IRC::command  color($nick, "h1") . $Bold . "$scriptLongName";
  IRC::command  color($nick, "h2") . $Bold . "$hostname";
  IRC::command  color($nick, "h2") .         "There are $numplayers of $maxplayers currently playing \"$maptitle\" ($mapname)";

  %players                    =  ();     # fyi, xchat treats globals in a module as static. clear before use.
  if ($numplayers ne "0") {
    IRC::command  color($nick, "h3") .                      "  "                        . 
                  color($nick, "h3") . $Bold . $Underline . "Player                   " . $Underline . "  " .
                  color($nick, "h3") .         $Underline . "Frags "                    . $Underline . "  " .
                  color($nick, "h3") .         $Underline . "Ping ";

    my $player                =  "0";
    for (;;) {
      last if ($player eq $numplayers);

      my ($pname)             =  ($serverInfo  =~ m/player_$player\\\s?(.*?)\s?\\/i);
      my ($uname)             =  fixUTF8($pname);
      my ($pfrag)             =  ($serverInfo  =~ m/frags_$player\\\s?(.*?)\s?\\/i);
      my ($pping)             =  ($serverInfo  =~ m/ping_$player\\\s?(.*?)\s?\\/i);

      $players{$pname}        =  [$pfrag, $pping, $uname];

      $player++;
    }

    my $count                 =  0;
    foreach (sort byFragsThenName keys %players) {
      preventFlood($nick);
      my $pname               =  $_;
      my $playerT             =  $players{$pname};
      my @player              =  @$playerT;
      my $pfrag               =  $player[0];
      my $pping               =  $player[1];
      my $uname               =  $player[2];

      if ($count == 0) {
        $pfrag               .=  "/$fraglimit";
      }

      IRC::command  color($nick, "h3") . "  $uname"  .
        " " x (25 - (length $pname))                 .
        "  " . $pfrag                                .
        " " x ( 6 - (length $pfrag))                 .
        "  " . $pping;

      $count++;
    }
  }

  return 1;
}

1;