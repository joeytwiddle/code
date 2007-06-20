COM="$1"
shift

await_death () {
	for SECONDS in `seq 1 2 "$2"`
	do
		# findjob "$1" || return 0
		mykillps "$1" || return 0
		sleep 2
	done
	return 1
}

case "$COM" in

	start)

		# ### Added: pause at initial system boot (for Joey's own convenience, booting X sooner, etc.)
		# UPTIMES=`cat /proc/uptime | beforefirst "\." 2>/dev/null`
		# if [ "$UPTIMES" ] && [ "$UPTIMES" -lt 3000 ]
		# then
			# echo -n "backgrounding for execution later..." >&2
			# (
				# sleep $((60*10))
				# sh "$0" stop ## This is needed cos sometimes without it the server never reappears.  Sometimes we just gotts kill ngWorldStats!
				# sh "$0" restart
			# ) &
			# echo "backgrounded." >&2
		# else
			sh "$0" stop ## This is needed cos sometimes without it the server never reappears.  Sometimes we just gotts kill ngWorldStats!
			sh "$0" restart
		# fi

	;;

	restart)
		rm ./ut-server/Logs/Unreal.ngLog.*.log ./ut-server/Logs/Unreal.ngLog.*.tmp
		## This is now done with an option: :)
		# rm ./ut-server/System/MapVoteHistory1.ini ## Lets us choose any map, even most recently played
		sh ./auto_asu.sh "$@"
	;;

	stop)
		./ut-server/ucc.init stop
		killall ucc.init # previous stops server but not infinity init script

		## Introduced the || break because was incorrectly waiting on njla's "tail ucc.init.log"!
		## The philosophy is that if killall fails then there is no point trying anything else!
		## But todo: ideally it wouldn't catch the wrong processes!
		## mykillps does a good job.

		# for PROG_TO_STOP in "ucc.init infinity" ucc-bin
		for PROG_TO_STOP in ucc.init ucc-bin ngWorldStats
		do
			echo "Ensuring $PROG_TO_STOP has stopped..."
			await_death "oddjob2.*$PROG_TO_STOP" 1 ||
				verbosely killall $PROG_TO_STOP || break
			await_death "oddjob2.*$PROG_TO_STOP" 2 ||
				verbosely killall -KILL $PROG_TO_STOP || break
			await_death "oddjob2.*$PROG_TO_STOP" 3 &&
				echo "$PROG_TO_STOP is no longer running." ||
				error "$PROG_TO_STOP has failed to die :-/"
		done
	;;

	status)
		wget -nv --tries=4 --timeout=5 --connect-timeout=5 --http-user="nogginBasher" --http-passwd="wibble" "http://82.33.113.69:5080/ServerAdmin/defaults" -O - 2>/dev/null | grep frameset >/dev/null
		if [ "$?" = 0 ]
		then
			echo "Failed to attach to web interface"
			findjob ucc
		fi
	;;

esac

