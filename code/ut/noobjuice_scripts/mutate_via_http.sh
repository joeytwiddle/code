## TODO: We should memo re-used calls to the UT HTTP server per call to this script.
##       We could do this by touching an existing or creating a new memofile whenever we want to clear memo's cache.

BASEURL="http://`ppp-getip`:5080/ServerAdmin"

. ut_server.config

if [ "$ALWAYS_MUTATORS_REGEXP" ] && [ "$PROBLEMATIC_MUTATORS_REGEXP" ]
then :
else
	echo "Need ALWAYS_MUTATORS_REGEXP and PROBLEMATIC_MUTATORS_REGEXP"
	exit 1
fi

# echo "$PAGE"
# cursered
# echo "$PAGE" | grep -A1 "ExcludeMutatorsSelect" | extractregex -atom 'value="([^"]*)"'
# cursegreen
# echo "$PAGE" | grep -A1 "IncludeMutatorsSelect" | extractregex -atom 'value="([^"]*)"'
# cursenorm

geturl () {
	# wget --http-user=nogginbasher --http-passwd=wibble -nv --tries=2 --timeout=15 --connect-timeout=15 -O - "$BASEURL/$1" 2>/dev/null || error "wget failed"
	## Removed / due to problems with getmaplist and changeMap, but will this cause problems elsewhere?!
	wget --http-user=nogginbasher --http-passwd=wibble -nv --tries=2 --timeout=15 --connect-timeout=15 -O - "$BASEURL$1" 2>/dev/null || error "wget failed"
	## My guess is that I changed to a version of wget which does not automatically squeeze duplicate //s into /.
}

extractParam () {
	RELURL="$1"
	PARAM="$2"
	geturl "$RELURL" |
	grep -A1 "name=\"$PARAM\"" | extractregex -atom 'value="([^"]*)"'
}

getmaplist_real () {
	# # geturl "/defaults_maps?GameType=Botpack.CTFGame"
	# extractParam "/defaults_maps?GameType=Botpack.CTFGame" "ExcludeMapsSelect"
	# extractParam "/defaults_maps?GameType=Botpack.CTFGame" "IncludeMapsSelect"
	## For some reason I was only getting DM maps in that list :S
	# find ./ut-server/Maps -name "CTF-*.unr" | afterlast / | beforelast "\."
	## Doh, that doesn't solve the problem, because the WebAdmin still can't select the CTF map!
	## But I did fix the no CTF maps in list problem, by "switching" to CTF game-mode manually through WebAdmin.
	## Then I was still getting no maps, until I discovered two //s in the URL were confusing WebAdmin.
	(
	extractParam "/defaults_maps?GameType=Botpack.CTFGame" "ExcludeMapsSelect"
	extractParam "/defaults_maps?GameType=Botpack.CTFGame" "IncludeMapsSelect"
	) | tee /tmp/1.debug |
	## What we do here is extract from this list only those which are still in /Maps/
	## This *might* produce no matching maps, in which case it seems to restart the current map :o
	while read MAPNAME
	do
		find ut-server/Maps -name "$MAPNAME" |
		afterlast / | beforelast ".unr$"
		# grep . > /dev/null && echo "$MAPNAME"
	done | tee /tmp/2.debug
}

. importshfn memo rememo
getmaplist () {
	IKNOWIDONTHAVEATTY=1 memo -t "2 minutes" getmaplist_real "$@"
}

addMap () {
	MAP="$1"
	geturl "/defaults_maps?GameType=Botpack.CTFGame&MapListType=Custom&MapListSelect=Custom&ExcludeMapsSelect=$MAP&AddMap=++%3E+&MoveMapCount=1" >/dev/null
}

changeMap () {
	MAP="$1"
	geturl "/current_game?GameTypeSelect=BotPack.CTFGame&MapSelect=$MAP&SwitchMap=Switch" >/dev/null
}

extractMutatorParam () {
	extractParam /current_mutators "$1"
}

clear_all_muts () {
	sh "$0" listactive | grep -v "$ALWAYS_MUTATORS_REGEXP" |
	withalldo sh "$0" delmut |
	cat > /dev/null ## This works so who cares?!
}

postMutatorParam () {
	MODE="$1"
	shift
	if [ "$MODE" = -add ]
	then
		PARAM="ExcludeMutatorsSelect"
		URL="/current_mutators?AddMutator=++%3E+"
	elif [ "$MODE" = -del ]
	then
		PARAM="IncludeMutatorsSelect"
		URL="/current_mutators?DelMutator=++%3C+"
	else
		echo "Must -add or -del"
		return 1
	fi
	for MUT
	do URL="$URL""&""$PARAM=$MUT"
	done
	geturl "$URL" >/dev/null
}

case "$1" in
	listmaps)
		getmaplist
	;;
	randommap)
		# NEWMAP=`getmaplist | randomorder | head -1`
		shift
		# ( getmaplist | grep -i "$1" ; getmaplist | randomorder ) | countlines >> ./ut-server/Logs/ucc.init.log
		## Debugging:
		# # echo "DEBUG: $1" >> ./ut-server/Logs/ucc.init.log
		# if [ "$1" ]
		# then
			# echo "Matches with $1" >> ./ut-server/Logs/ucc.init.log
			# getmaplist | grep -i "$1" | randomorder >> ./ut-server/Logs/ucc.init.log
		# fi
		## End Debugging.
		NEWMAP=`
			(
				[ "$1" ] && getmaplist | grep -i "$1" | randomorder
				getmaplist | randomorder
			) | head -1
		`
		echo "Changing to map: $NEWMAP"
		addMap "$NEWMAP"
		changeMap "$NEWMAP"
		# echo "Selected $NEWMAP => $?"
	;;
	listactive)
		extractMutatorParam "IncludeMutatorsSelect"
	;;
	listinactive)
		extractMutatorParam "ExcludeMutatorsSelect"
	;;
	addmut)
		shift
		echo "Adding mutators: $*"
		postMutatorParam -add "$@"
		# echo "Added mutators: $* => $?"
	;;
	delmut)
		shift
		echo "Removing mutators: $*"
		postMutatorParam -del "$@"
		# echo "Removed mutators: $* => $?"
	;;
	clearmuts)
		echo "Clearing all mutators... (next u must restart)"
		clear_all_muts
		echo "Done."
	;;
	randommuts)
		echo "Randomising mutators..."
		clear_all_muts
		# while read MUT
		# do sh "$0" delmut "$MUT"
		# done
		sh "$0" listinactive | grep "$ALWAYS_MUTATORS_REGEXP" |
		pipeboth | ## Hmm not many left!
		withalldo sh "$0" addmut

		(
			shift
			if [ "$*" ]
			then
				for EXPR
				do
					echo "Mutators matching \"$EXPR\":" >&2
					sh "$0" listinactive | grep -i "$EXPR" | pipeboth
				done
			fi
			sh "$0" listinactive | grep -v "$PROBLEMATIC_MUTATORS_REGEXP" |
			pipeboth | ## Hmm not many left!
			randomorder
		) | head -5 |
		withalldo sh "$0" addmut
		# while read MUT
		# do sh "$0" addmut "$MUT"
		# done
	;;
	*)
		echo "$1 ?"
	;;
esac

