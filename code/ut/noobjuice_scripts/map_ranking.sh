getvotes () {
	# memo -t "1 day" eval 'jzcat ut-server/Logs/*.gz | grep \"voted for\"' |
	memo -t "1 hour" eval 'jzcat ut-server/Logs/*.gz | grep \"voted for\"' |
	takecols 5 |
	## Undo the digits which were added to the name:
	sed 's+\(CTF-\)\([[:digit:]]{3}[\._]\)\(.*\)+\1\3+' |
	sort | uniq -c | sort -r
}

getranking () {
	getvotes |
	dropcols 1 2 | numbereachline
}

case "$1" in

	getranking)
		getranking
	;;

	rename_mapfiles|*)

		getranking |
		## Add digits to the name:
		sed 's+\(.*\)	CTF-\(.*\)+CTF-\2 CTF-\1_\2+' |
		# while read RANKING MAP_NAME
		while read MAP_NAME NEW_NAME
		do
			if [ -L ./ut-server/Maps/"$MAP_NAME".unr ]
			then
				# mv ./ut-server/Maps/"$MAP_NAME".unr ./ut-server/Maps/"$RANKING$MAP_NAME".unr
				# echo mv ./ut-server/Maps/"$MAP_NAME".unr ./ut-server/Maps/"$NEW_NAME".unr
				## Oh dear this appeared to break the server; refused to travel to maps prepended by numbers:
				# verbosely mv ./ut-server/Maps/"$MAP_NAME".unr ./ut-server/Maps/"$NEW_NAME".unr
				verbosely echo mv ./ut-server/Maps/"$MAP_NAME".unr ./ut-server/Maps/"$NEW_NAME".unr
			fi
		done

	;;

esac
