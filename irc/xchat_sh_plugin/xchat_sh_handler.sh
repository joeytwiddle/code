export COMMAND_SCRIPTS_DIR="/home/joey/.xchat2.utb0t/plugin/xchat_sh_scripts"
# export COMMAND_SCRIPTS_DIR="/home/joey/linux/.xchat2.utb0t/plugin/xchat_sh_scripts"
# export COMMAND_SCRIPTS_DIR="/home/joey/j/code/other/irc/xchat/xchat_sh_scripts" ## hwi

export LOGDIR="$HOME/.xchat2.utb0t/logs"

SPLIT_LONG_LINES=true

## TODO: Logs to /tmp

log () {
	# printf "%s\n" "`geekdate -fine` $*" >> /tmp/xchat_sh_handler.log
	# printf "%s\n" "`geekdate -fine` $NETWORK/$CHANNEL ($NICK) $*" >> /tmp/xchat_sh_handler.log
	printf "%s\n" "`geekdate -fine` $NETWORK/$CHANNEL ($NICK) $*" >&2
}

# log "Shell received: $*"
# for ARG
# do log "ARG: $ARG"
# done

COMMAND="$1"
shift

# if [ ! "$COMMAND" = wut ]
# then
	# echo "Sorry I'm playing UT so my scripts are disabled."
	# exit 0
# fi

break_long_lines_but_retain_color () {

	## It seems something cannot handle strings > 250 chars, so...
	# sed "s+\(.\{0,245\}[^ ]\)\(  *\|$\)+\1$NL+g" |
	# sed "s+\(.\{0,245\}[^ ]\)\(  *\)+\1$NL +g" |
	## Moved elsewhere, because it breaks /notices!  That's bad because then they turn into /say!  :S
	# sed "s+\(.\{0,245\}[^ ]\)\(  *\|$\)+\0$NL +g" | grep -v "^ $" |
	# sed "s+^\(/notice [^ ]* \|\)\(.\{0,245\}[^ ]\)\(  *\|$\)+\0$NL\1 +g" | grep -v "^\(notice [^ ]*\|\) $" |
	# sed "s+^\(/notice [^ ]* \|\)\(.\{0,245\}[^ ]\)\(  *\|$\)+\0 ...$NL\1 ... +g" | grep -v "^\(notice [^ ]*\|\) ... $" |
	# sed "s+^\(/notice [^ ]* \|\)\(\(\(\([[:digit:],]\{1,5\}\)\|\).\)\{0,245\}[^ ]\)\(  *\|$\)+\0...$NL\1 >\5<... +g" | grep -v "^\(notice [^ ]*\|\) ... $" |
	# sed "s+^\(/notice [^ ]* \|\)\(\(.\(\([[:digit:],]\{1,5\}\)*\)\)\{0,245\}\)  *+\0...$NL\1 >\5<... +g" | grep -v "^\(notice [^ ]*\|\) ... $" |
	# sed "s+^\(/notice [^ ]* \|\)\(\(.\(\([[:digit:],]\{1,5\}\)*\)\)\{0,245\}\)+\0$NL\1 \5+g" | grep -v "^\(notice [^ ]*\|\) ... $" |
	# sed "s+^\(/notice [^ ]* \|\)\(\(.\(\([[:digit:],]\{1,5\}\)*\)\)\{0,80\}\)  *+\0$NL\1 \5+g" | grep -v "^\(notice [^ ]*\|\) ... $" |
	# sed "s+\(^/notice [^ ]* \|\)\([^]\(\([[:digit:],]\{1,5\}\)*\)\)\{0,40\}  *+\0$NL\1 \4+g" | grep -v "^\(notice [^ ]*\|\) ... $" |
	# sed "s+\(^/notice [^ ]* \|\)\([^]\(\([[:digit:],]\{5\}\)*\)\)\{0,245\}  *+\0$NL\1 \4+g" | grep -v "^\(notice [^ ]*\|\) ... $" |
	# sed "s+\(^/notice [^ ]* \|\)\([^]\(\([[:digit:],]\{5\}\)*\)\)\{0,245\}\(  *\|$\)+\0$NL\1 \4+g" | grep -v "^\(/notice [^ ]* *\([[:digit:],]\{5\}\|\)\) *$" |
	# sed "s+\(^/notice [^ ]* \|\)\([^]\(\([[:digit:],]\{5\}\)*\)\)\{0,245\}\(  *\|$\)+\0$NL\1 \4+g" | grep -v "^\(/notice [^ ]* *\|\)\([[:digit:],]\{5\}\| \)*$" |
	## I think the problem is, it would rather gather the external 40 by stealing .s from the \{1,5\}, than fill the \{1,5\} :s
	## To fix it I assume all my colour codes are exactly 5 in length (BUG TODO NOT TRUE!).
	## OK fixed notices.  NO TODO BUG: only works on first split!!  Could get round this by 1) inserting a special char where a newline is needed; 2) have another sed(?) which splits the stream into newlines at those chars, and adds /notice to them all if needed.  Or maybe that suffers the same problem!!  :S
	## OK catching the last colour, and re-writing it.  :)
	## We have to match the \|$ otherwise it always tries to do a last break on the last space, even if the line is short.
	## But doing the \|$ generates a final *empty* line, so we have to add the grep to remove it.
	# sed "s+\(^/notice [^ ]* \|\)\(\(\([[:digit:],]\{5\}\)*\)[^]\)\{0,245\}\(  *\|$\)+\0$NL\1 \4+g" | grep -v "^\(/notice [^ ]* *\|\)\([[:digit:],]\{5\}\| \)*$" |
	# sed "s+\(\(\([[:digit:],]\{5\}\)*\)[^]\)\{0,245\}\(  *\|$\)+\0$NL \3+g" | grep -v "^\([[:digit:],]\{5\}\| \)*$"
	## For some unknown reason, it was splitting my not-playing !wut response at *every* space!  So...
	sed -u "s+\(\(\([[:digit:],]\{5\}\)*\)[^]\)\{100,220\}\(  *\|$\)+\0$NL \3+g" | grep --line-buffered -v "^\([[:digit:],]\{5\}\| \)*$"
	## Small bug: each block of colour codes is treated as exactly 1 character; it should probably be 0.
	## BUG: now empty lines (or lines containing just one " ") are stripped, even if they were intended.  :|

}

split_long_lines_retaining_notice_or_msg () {

	while IFS="" read LINE
	do
		# if printf "%s" "$LINE" | grep -i "^/notice" >/dev/null
		if printf "%s" "$LINE" | grep --line-buffered -i "^/\(notice\|msg\)" >/dev/null
		then
			NOTICE=`printf "%s" "$LINE" | sed 's+^\([^ ]* *[^ ]* *\).*+\1+'`
			TOBREAK=`printf "%s" "$LINE" | sed 's+^[^ ]* *[^ ]* *++'`
		else
			NOTICE=""
			TOBREAK="$LINE"
		fi
		printf "%s" "$TOBREAK" |
		break_long_lines_but_retain_color |
		while IFS="" read BROKENLINE
		do printf "%s\n" "$NOTICE$BROKENLINE"
		done
	done

}

abort_if_needed () {

	CHANNEL=`echo "$CHANNEL" | tolowercase` ## TODO: since we are in a function, this will get kept forever!

	## My filters: testing only responds to whereis, and only testing responds to whereis
	# if startswith "$LASTCMD" xchat-test && [ ! "$COMMAND" = "whereis" ]
	# then exit 0
	# fi
	# if [ "$COMMAND" = "whereis" ] && ! startswith "$LASTCMD" xchat-test
	# then exit 0
	# fi
	## New filter: testing never runs any scripts:
	# if startswith "$LASTCMD" xchat-test || contains "$LASTCMD" ".xchat2.testing"
	# then exit 0
	# fi

	## No bots allowed in irc.utchat.com/#ut (turned this off 07/03/2007)
	# if [ "$CHANNEL" = "#ut" ] && [ ! "$NETWORK" = "irc.xyxyx.org" ]
	# then
		# # jshinfo "Refusing to spam on $NETWORK $CHANNEL"
		# exit 0
	# fi

	## Reno was given a 24hr ban for abuse:
	# if contains "$NICK" "Reno" || contains "$NICK" "reno"
	# then exit 0
	# fi

	## For emergencies:
	# exit 0

	## Disable all scripts on freenode network:
	if [ "$NETWORK" = freenode.net ] || [ "$NETWORK" = irc.freenode.net ] || [ "$NETWORK" = irc.freenode.com ] || [ "$NETWORK" = irc.freenode.org ]
	then exit 0
	fi

	## Commands not wanted on irc.utchat.com/#ut:
	if [ "$CHANNEL" = "#ut" ] && [ "$COMMAND" = u ]
	then exit 0
	fi
	if [ "$SERVER" = irc.utchat.com ] || [ "$NETWORK" = "ChaoticNetworks.Com" ] || [ "$NETWORK" = irc.globalgamers.net ]
	then

		## Blacklist:
		# u|op|deop|utp|rapid|spam|cowsay)
		case "$COMMAND" in
			u|op|deop|invite|rapid)
				exit 0
			;;
		esac

		## Avoid collision with W|S's t3h_bot:
		# if [ "$CHANNEL" = "#{W|S}" ]
		if [ "$CHANNEL" = "#}w|s{" ]
		then exit 0
		fi

		if [ "$CHANNEL" = "#uthelp" ]
		then exit 0
		fi

		if [ "$CHANNEL" = "#ut" ] || [ "$CHANNEL" = "#testing" ]
		then

			## Whitelist:
			case "$COMMAND" in
				whereis|lastseen|searchserv|xol|f0x|nzp|inz|of|smo|focr|terror|dm|ctf|jb|as|bt|face|lms|mh|siege)
					: ## !invitebot was too spammy for #ut; consider /notice response
				;;
				*)
					exit 0
				;;
			esac

			## VampyrCeil prefers /notice responses
			export NOTICE_STYLE="notice"

		fi

		## This nob exploits the bot in #ut whenever he can, to get it banned (succeeded twice)
		case "$NICK" in
			Taerom)
				exit 0
			;;
		esac

	fi

}

## Drop the first part of the hostname, to determine which network we are on:
# NETWORK=`echo "$NETWORK" | sed 's+.*\.\([^\.]*\.[^\.]*\)$+\1+'` ## trim server to the last two words
NETWORK=`echo "$NETWORK" | sed 's+^irc\.\([^.]*\)\.\([^.]*\)\.org+irc.\2.org+'`
## TODO: need to globally swap NETWORK and SERVER!

abort_if_needed

## Dodgy hack to strip IRC colour-codes off the front of the nick:
## Errr when was I getting IRC colour-codes on the start of nicks?! :O
# NICK=`echo "$NICK" | striptermchars`
if echo "$NICK" | grep "^" >/dev/null
then NICK=`echo "$NICK" | colrm 1 3`
fi

# if [ "$NICK" = zbe ]
# then
	# echo "Leave me alone, you are mean."
	# exit 0
# fi

# SCRIPT="$COMMAND_SCRIPTS_DIR/$COMMAND"
# SCRIPT=`find "$COMMAND_SCRIPTS_DIR"/ -type f -or -type l -iname "$COMMAND"` # COMMAND_SCRIPTS_DIR should be absolute
cd "$COMMAND_SCRIPTS_DIR"
# SCRIPT=`find -type f -or -type l -iname "$COMMAND" | grep -v "^\." | grep -v "/\." | head -n 1`
# SCRIPT=`find -iname "$COMMAND" | grep -v "^\." | grep -v "/\." | head -n 1`
ESCAPED_COMMAND=` printf "%s" "$COMMAND" | sed ' s+*+\\\*+g ; s+?+\\\\?+g ' `
SCRIPTS=`
	# find "$PWD"/ -iname "$COMMAND" -type f -or -type l | ## for some reason i can't fathom, that didn't work!
	find "$PWD"/ -iname "$ESCAPED_COMMAND" -not -type d |
	grep -v "/CVS/" |
	grep -v "/disabled/" |
	# grep -v "^\." |
	# grep -v "/\." |
	head -n 5
`

NL="\\
"

for SCRIPT in $SCRIPTS
do

	if [ -f "$SCRIPT" ] && [ -x "$SCRIPT" ]
	then
		log "Calling: sh \"$SCRIPT\" \"$*\""


		## Decided this was nicer for the scripts:
		# cd "$COMMAND_SCRIPTS_DIR/" ## already done

		export NICK
		export CHANNEL
		export NETWORK ## actually it seeems this was already done

		## This doesn't work if user has not set mode +x themself.  Auths now checked in Java using /whois.
		# if echo "$HOST" | grep "\.users\.quakenet\.org$" >/dev/null
		# then export AUTH="`echo "$HOST" | sed 's+\..*++'`"
		# fi

		# set -x

		## nice added for my own convenience ;p
		# nice -n 15 sh "$SCRIPT" $* |
		# nice -n 15 sh "$SCRIPT" "$@" |
		## We don't want to quote "$*" because we want to provide each word as a separate argument, so...
		set -f ## This turns off globbing so * and ? won't be expanded
		# nice -n 17 sh "$SCRIPT" $* |
		sh "$SCRIPT" $* |
		# sed "s+'+_+g" |
		# sed "s+^................................................................................+\0$NL+g" |
		# tr -d "'" | ## don't know why this was on
		# tee /tmp/xchat_sh_handler.mid | ## For debugging
		# head -100 | ## Dirty hack to reduce danger of accidental flooding!  ## But I had to remove this because it was the cause of the buffering :|

		if [ "$SPLIT_LONG_LINES" ]
		then split_long_lines_retaining_notice_or_msg
		else cat
		fi |

		## Unless a / irc command is provided, we just respond with /say:
		## But this is now handled by parent perl plugin.
		# sed 's+^[^/]+/say \0+'

		### Disabled 07/03/2007:
		# ## If we are in certain channels, then respond with /notice instead of /say.
		# ## Note this doesn't work if the script returns a /command, only if it returns text to say (or notice).
		# if [ "$CHANNEL" = "#ut" ] && [ ! "$NETWORK" = "irc.xyxyx.org" ] # || [ "$CHANNEL" = "#test" ] # || [ "$CHANNEL" = "#whereisbot" ]
		# then sed "s+^[^/]+/notice $NICK \0+"
		# ## Doesn't work: "Cannot send to channel"
		# # else sed "s+^[^/]+/notice $CHANNEL \0+" ## note CHANNEL actualy == $respond_to i.e. it might be a nick
		# else cat
		# fi # | tee -a /tmp/xchat_sh_handler.later ## For debugging
		cat

	fi

done

if [[ ! "$SCRIPTS" ]]
then
		log "Ignoring: $COMMAND"
		:
		# echo "No such command: $COMMAND"
fi

