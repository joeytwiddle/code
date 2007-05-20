#!/usr/bin/perl -w

## Yes we do hate mirc, so if its mentioned we will kick =)
## Ok, we cant be that evil, so we will give a couple of warnings before 
## Written by zarck of #amiga.se 29/10 2002 (zarck@linux.nu)
## If you rewrite this code please send me a copy =) 

$firstwarn="mirc, mirk, mirka är inte tillåtet! ush! VARNING!"; #first warning 
$warn="Anti-mIRC Varning "; #warning message
$warnings=5; #how many times will we tolerate it before kick? 
$kickmsg="Våga säga NEJ till mIRC! (det heter IRC:a inte mirkka)"; #selfexplained i guess

$version="0.9";
IRC::register ("mIRC killer" , "$version" ,"","");
IRC::print ("mIRC-killer $version");
IRC::add_message_handler ("PRIVMSG",find);

sub find
{
	foreach (@_)
	{
		$_ =~/^\:(.*)\!.*\@.*PRIVMSG\s(\#.*)\s\:(.*)$/; #pretty poor regexp
		$nick=$1; $chan=$2; $line=$3;
		$lcline=lc($line); #lowercase, easier that way =)
		if ($lcline=~/mirc/ || $lcline=~/mirk/)
		{
			if ($lcline=~/amir/) 
			{
			}
			else
			{
				if ($nicks{$nick})
				{
					$nicks{$nick}++;
					if ($nicks{$nick} == $warnings)
					{
						IRC::command ("/kick $nick $kickmsg");
						$nicks{$nick}=0;
					}
					else
					{
						IRC::command ("/notice $nick $warn [ $nicks{$nick}/$warnings ]");
					}
				}
				else
				{
					$nicks{$nick}=1;
					IRC::command ("/notice $nick $firstwarn [ $nicks{$nick}/$warnings ]");
				}
				
			}
		}
	}

}
	
