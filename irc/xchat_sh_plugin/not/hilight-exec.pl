#!/usr/bin/perl -w
# VIM: set ts=4, set sw=4
#
# Author: Vetinari <iranitev@gmx.net>
# Date:   2002-04-30
#
package IRC::XChat::HilightExec;

my $script_name    = "hilight-exec.pl";
my $script_version = '0.1.0';

# config... just the command line ...
my $command = '/path/to/prog -with -some -args';
# end config 
$command .= ' &';
IRC::register($script_name,$script_version, "", "");
IRC::print("\cC0\cB$script_name\cB version\cC3 $script_version\cO "
           . "by \cB\cC4V\cC7etinari\cO loading...\n");

my $pkg = __PACKAGE__;

my @highlights = get_highlights();

IRC::add_message_handler("PRIVMSG", "${pkg}::msg_privmsg");

sub get_highlights {
    my $dir = "$ENV{'HOME'}/.xchat2";
    open CFG, "$dir/xchat.conf"
        or do {
            IRC::print("hilight-exec.pl: Could not open $dir/xchat.conf: $!\n");
            return ();
        };
    my $line = "";
    my @list = ();
    while (defined ($line = <CFG>)) {
        next unless $line =~ /bluestring\s*=/;
        $line =~ s/\s*bluestring\s*=\s*//;
        $line =~ s/\s*$//;
        @list = split /,/, $line;
        last;
    }
    close CFG;
    return @list;
}

sub trim {
    local $_ = shift;
    s/^\s*//;
    s/\s*$//;
    return $_;
}

sub msg_privmsg {
    my $line = shift;
    $line =~ /:.*?\s*([^!]*)!([^@]*@[^ ]*) +(.*?)\s*:(.*)/;
    my ($nick,$host,$chan,$text) = ($1,$2,$3,$4);
    return 0
        if $text =~ /^\cA.*\cA$/;    # CTCP's ...

    $chan =~ s/PRIVMSG\s*//;
    my $cur_nick = IRC::get_info(1);
    push @highlights, $cur_nick;
    foreach my $highlight (@highlights) {
        if ($text =~ /\Q$highlight\E/i) {
            IRC::print("HighLight '$nick' (on $chan)\n");
            system '/bin/sh', '-c', $command;
            last;
        }
    }
    (undef) = pop @highlights;
    return 0;
}

1;
#end
