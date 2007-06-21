LOGFILE="ut-server/Logs/ucc.init.log"

## For ALWAYS_MUTATORS_REGEXP:
. ut_server.config

NL="\\
"

rm -f /tmp/considering_start /tmp/cancelled

web_say () {
	echo "`curseblue;cursebold`NJLA responding: `curseyellow`$*`cursenorm`"
	TXT=`echo "$*" | sed 's/ /+/g'`
	URL="http://`ppp-getip`:5080/ServerAdmin/current_console?SendText=say+$TXT"
	# echo "Hitting $URL"
	wget --http-user=nogginbasher --http-passwd=wibble -nv --tries=5 --timeout=5 -w 1 "$URL" -O /tmp/out.html 2>/tmp/wget.err || error "wget failed"
	# sleep 1
}

respond () {
	# echo "`curseblue;cursebold`NJLA responding: `curseyellow`$*`cursenorm`"
	# web_say "[NJ] $@"
	## This is a dodgy attempt to not exceed 60 chars per line:
	# echo "[NJ] $*" | sed 's+........................................[^ ]*+\0\
	# echo "[NJ] $*" | sed "s+........................................[^ ]*+\0$NL+g" |
	# echo "$*" | sed "s+........................................[^ ]*+\0$NL+g" |
	# echo "$*" | sed "s+........................................[^ ]*+\0$NL...+g" | grep -v "^\.\.\.$" |
	# echo "$*" | sed "s+........................................[^ ]*+\0$NL>+g" | grep -v "^>$" |
	# echo "$*" | sed "s+.{0,55}[^ ] +\0$NL+g" |
	echo "$*" | sed "s+.\{0,57\}[^ ]\( \|$\)+\0$NL> +g" | grep -v "^> $" |
	# echo "$*" | sed "s+\(.{0,55}[^ ] \|.{55}\)+\0$NL+g" |
	while read LINE
	do web_say "$LINE"
	done
	sleep 1
	# done &
	# sleep 2
}

WAITTIME=10
do_with_cancel_option () {

	## do_with_cancel_option "<nice_description>" <command> <args>...

	NICENAME="$1"; shift

	if [ "$1" = -consider-doing ]
	then

		shift
		sleep "$WAITTIME" ## Could make this a repeated sleep 1 so it can drop out as soon as it is cancelled
		if [ ! -f /tmp/cancelled ]
		then
			respond "Doing >$NICENAME< now..."
			"$@"
			# sleep 60 ## does this stop http dying when doing soft/hard restarts?
			respond ">$NICENAME< done."
		else
			respond ">$NICENAME< was cancelled."
		fi
		rm -f /tmp/considering_start
		rm -f /tmp/cancelled

	else

		if [ -f /tmp/considering_start ]
		then
			respond "NEGATIVE cannot suggest >$NICENAME<; another process is being considered."
			return
		fi ## todo: either cancel this new process request or cancel the existing process in favour of this one
		touch /tmp/considering_start
		do_with_cancel_option "$NICENAME" -consider-doing "$@" &
		respond "Will perform >$NICENAME< in $WAITTIME seconds..."
		respond 'Say "!no" to cancel.'

	fi

}

respond_list () {
	NAME="$1"
	shift
	LIST=`cat | sed 's+$+, +' | tr -d '\n' | sed 's+..$++'`
	respond "$NAME: $LIST"
}

respond_stream () {
	while read LINE; do respond "$LINE"; done
}

mutate_muts () {
	sh ./mutate_via_http.sh randommuts "$@" | respond_stream
}

mutate_map () {
	sh ./mutate_via_http.sh randommap "$@" | respond_stream
}

mutate_all () {
	mutate_muts "$@"
	mutate_map "$@"
}

respond 'n00bjuiceLiveAdmin started.  Say "!help" for more info.'

## TODO: optionally, allow spectator to execute nj commands.
## Spectator does not have surrounding (...) in the chatlog:
# ChatLog: nogginBasher:nj mutate
## Playuer does:
# ChatLog (nogginBasher): nj newmap

# tail -1000000f "$LOGFILE" |
tail -0f "$LOGFILE" |

## Catch just players:
# grep --line-buffered "^ChatLog (" |
# sed -u 's+^[^:]*: ++' |

## Allow spectators: (these 2 lines not working, 2 previous 2 re-enabled)
# grep --line-buffered "^ChatLog" |
# sed -u 's+^[^:(][:(][^:]*:[ ]*++' |
# pipeboth |
# grep --line-buffered "^nj " |

## Both:
grep --line-buffered "^ChatLog" |
sed -u 's+^ChatLog: .*:!+!+' |
sed -u 's+^ChatLog (.*): !+!+' |

grep --line-buffered '^!' |

while read COMMAND ARGA ARGSREST
do

	COMMAND=`echo "$COMMAND" | sed 's+^!++'`
	echo "njadmin_watch: Got: $COMMAND $ARGA $ARGSREST"

	case "$COMMAND" in

		status)
			sh ./mutate_via_http.sh listactive > active_mutators.now
			MUTATORSIG=`cat active_mutators.now | grep -v "$ALWAYS_MUTATORS_REGEXP" | afterfirst '\.' | sed 's+[a-z]++g' | tr '\n' ' ' | sed 's+ $++'`
			respond "[$MUTATORSIG]"
			# REGEXP=`echo "$ALWAYS_MUTATORS" | grep -v "^$" | list2regexp`
			# cat active_mutators.now | grep -v "$REGEXP" |
			cat active_mutators.now | grep -v "$ALWAYS_MUTATORS_REGEXP" |
			respond_list "Active mutators"
			jfcsh active_mutators.all.lastboot active_mutators.lastboot |
			respond_list "Secret mutators"
			jfcsh active_mutators.lastboot active_mutators.now |
			respond_list "Dropped mutators"
			# INACTIVE_MUTS=`sh ./mutate_via_http.sh listinactive | sed 's+$+, +' | tr -d '\n' | sed 's+..$++'`
			# respond "Inactive mutators: $INACTIVE_MUTS"
		;;

		listmuts)
			sh ./mutate_via_http.sh listactive | respond_list "Active mutators"
			sh ./mutate_via_http.sh listinactive | respond_list "Inactive mutators"
		;;

		addmut)
			sh ./mutate_via_http.sh listinactive | grep -i "$ARGA" |
			withalldo sh ./mutate_via_http.sh addmut |
			respond_stream
		;;

		delmut)
			sh ./mutate_via_http.sh listactive | grep -i "$ARGA" |
			withalldo sh ./mutate_via_http.sh delmut |
			respond_stream
		;;

		clearmuts)
			sh ./mutate_via_http.sh clearmuts |
			respond_stream
		;;

		mutate|randomise|randomize)
			case "$ARGA" in
				muts)
					## We don't need to delay this, since it doesn't perform restart
					## User can check if they like mutators, or run it again.  :)
					# do_with_cancel_option "Mutate mutators" mutate_muts
					mutate_muts $ARGSREST
				;;
				map)
					do_with_cancel_option "Mutate map" mutate_map $ARGSREST
				;;
				*)
					do_with_cancel_option "Mutate all" mutate_all $ARGA $ARGSREST
				;;
			esac
		;;

		newmap)
			do_with_cancel_option "Mutate map" mutate_map $ARGA $ARGSREST
		;;

		newmaps)
			respond "Regenerating maplist..."
			# sh ./remake_links.sh maps
			# NUMMAPS=`sh ./remake_links.sh maps $ARGA | tail -1`
			export NOPROGRESS=1
			export NOEXEC=1
			sh ./remake_links.sh maps $ARGA $ARGSREST 2>&1 | respond_stream
			# echo "noexec=$NOEXEC" | respond_stream
		;;

		soft-restart|restart)
			do_with_cancel_option "Soft restart" sh ./really_soft_restart.sh
		;;

		hard-restart|reboot)
			do_with_cancel_option "Hard reboot" sh ./joeys_start_server.sh start
			## I think the earlier bug of multiple njadmins running could be avoided if we exit here.
			# exit 0
			## But I think I already fixed it with dodgy mykill :P
		;;

		no|cancel)
			if [ -f /tmp/considering_start ]
			then
				touch /tmp/cancelled
				respond "Cancelled OK."
			else
				respond "There is nothing to cancel you n00b."
			fi
		;;

		screenshot)
			DISPLAY=:0 xsnapshot
		;;

		screenshots)
			for X in `seq 1 10`
			do
				DISPLAY=:0 xsnapshot
				sleep 5
			done &
		;;

		whereis)
			/home/utquery/utquery/whereis "$ARGA" $ARGSREST | randomorder | head -10 | sort | respond_stream
		;;

		# jsh|ls|killall)
			# "$COMMAND" $ARGA | head -20 | tr ' ' '_' | while read LINE; do respond "$LINE"; done
		jsh)
			eval "$*" | head -20 | tr ' ' '_' | while read LINE; do respond "$LINE"; done
		;;

		*)
			[ "$COMMAND" = help ] || respond "I don't understand: $COMMAND"
			respond 'Commands: !help, !status, !mutate [map|muts], !clearmuts, !newmap[s] [<regexp>]*, !restart, !reboot, !no' # , !whereis [<player>]'
			# respond "Usage: nj ( help | status | mutate [map|muts] | clearmuts | newmap[s] [<regexp>]* | restart | reboot | no )"
		;;

	esac

	echo "njadmin_watch: Done."

done
