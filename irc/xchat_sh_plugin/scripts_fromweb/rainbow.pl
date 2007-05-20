package IRC::Xchat::Rainbow;

IRC::register("xchat rainbow", "0.1", "", "");

sub rainbow{
	$i=0;
$_=shift@_;
chomp;
s{(.)}{       "\cC" . (($i++%14)+2) . "$1"      }eg;
#$_=$_."\n";
IRC::command ("/say $_");
        return 1;
}


IRC::add_command_handler("rb", "IRC::Xchat::Rainbow::rainbow");
IRC::print('Loaded rainbow Script by Axioplase.');
