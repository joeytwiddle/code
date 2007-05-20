#!/usr/bin/perl -w

# Usage: /german <english text> or /tgerman <german text>
# Languages Are: German, Dutch, Spanish, French, Portuguese, Italian
# Author: epoch @ irc.cyberchat.org

use warnings;
use Lingua::Translate;
use Xchat qw(:all);

register('Translator Script','1.0');
Xchat::print"* Translator Script Loaded...";
hook_command('german', \&german);
hook_command('dutch', \&dutch);
hook_command('spanish', \&spanish);
hook_command('french', \&french);
hook_command('portuguese', \&portuguese);
hook_command('italian', \&italian);
hook_command('tgerman', \&tgerman);
hook_command('tdutch', \&tdutch);
hook_command('tspanish', \&tspanish);
hook_command('tfrench', \&tfrench);
hook_command('tportuguese', \&tportuguese);
hook_command('titalian', \&titalian);

sub german {
      my $gitrdun = Lingua::Translate->new(src => "en",dest => "de")       or die "No translation server available for en -> de";
      my $english = $_[1][1];
      my $german = $gitrdun->translate($english);
      Xchat::command("say $german");
      return Xchat::EAT_ALL;
}
sub dutch {
      my $gitrdun = Lingua::Translate->new(src => "en",dest => "nl") or die "No translation server available for en -> nl";
      my $english = $_[1][1];
      my $dutch = $gitrdun->translate($english);
      Xchat::command("say $dutch");
      return Xchat::EAT_ALL;
}
sub french {
      my $gitrdun = Lingua::Translate->new(src => "en",dest => "fr") or die "No translation server available for en -> de";
      my $english = $_[1][1];
      my $french = $gitrdun->translate($english);
      Xchat::command("say $french");
      return Xchat::EAT_ALL;
}
sub italian {
      my $gitrdun = Lingua::Translate->new(src => "en",dest => "it") or die "No translation server available for en -> de";
      my $english = $_[1][1];
      my $italian = $gitrdun->translate($english);
      Xchat::command("say $italian");
      return Xchat::EAT_ALL;
}
sub portuguese {
      my $gitrdun = Lingua::Translate->new(src => "en",dest => "pt") or die "No translation server available for en -> de";
      my $english = $_[1][1];
      my $portuguese = $gitrdun->translate($english);
      Xchat::command("say $portuguese");
      return Xchat::EAT_ALL;
}
sub spanish {
      my $gitrdun = Lingua::Translate->new(src => "en",dest => "es") or die "No translation server available for en -> de";
      my $english = $_[1][1];
      my $spanish = $gitrdun->translate($english);
      Xchat::command("say $spanish");
      return Xchat::EAT_ALL;
}
sub tgerman {
      my $gitrdun = Lingua::Translate->new(src => "de",dest => "en") or die "No translation server available for en -> de";
      my $english = $_[1][1];
      my $german = $gitrdun->translate($english);
      Xchat::command("say $german");
      return Xchat::EAT_ALL;
}
sub tdutch {
      my $gitrdun = Lingua::Translate->new(src => "nl",dest => "en") or die "No translation server available for en -> nl";
      my $english = $_[1][1];
      my $dutch = $gitrdun->translate($english);
      Xchat::command("say $dutch");
      return Xchat::EAT_ALL;
}
sub tfrench {
      my $gitrdun = Lingua::Translate->new(src => "fr",dest => "en") or die "No translation server available for en -> de";
      my $english = $_[1][1];
      my $french = $gitrdun->translate($english);
      Xchat::command("say $french");
      return Xchat::EAT_ALL;}
sub titalian {
      my $gitrdun = Lingua::Translate->new(src => "it",dest => "en") or die "No translation server available for en -> de";
      my $english = $_[1][1];
      my $italian = $gitrdun->translate($english);
      Xchat::command("say $italian");
      return Xchat::EAT_ALL;
}
sub tportuguese {
      my $gitrdun = Lingua::Translate->new(src => "pt",dest => "en") or die "No translation server available for en -> de";
      my $english = $_[1][1];
      my $portuguese = $gitrdun->translate($english);
      Xchat::command("say $portuguese");
      return Xchat::EAT_ALL;
}
sub tspanish {
      my $gitrdun = Lingua::Translate->new(src => "es",dest => "en") or die "No translation server available for en -> de";
      my $english = $_[1][1];
      my $spanish = $gitrdun->translate($english);
      Xchat::command("say $spanish");
      return Xchat::EAT_ALL;
}
 