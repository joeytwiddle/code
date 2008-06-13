. includepath /home/joey/public_html/utportal/qstat
. includepath $HOME/myroots/cowsay
. includepath $HOME/myroots/figlet

function teegz () {
	## I got output to stdout, but the file was always empty.
	pipeboth --line-buffered | gzip -c > "$1" 2>&1
	# tee "$1"
	# pipeboth
}

java -cp bin:lib/pircbot.jar org.neuralyte.irc.IRCBot | tee ircbot.log
# java -cp bin:lib/pircbot.jar org.neuralyte.irc.IRCBot | teegz ircbot.log.gz

