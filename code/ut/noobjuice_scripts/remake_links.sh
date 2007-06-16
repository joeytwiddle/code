## TODO: We haven't yet included data files from the UT game client directory.
##       We shouldn't really have to, but at least CTF-Chasm wanted one of these!

## OMG ln -sf overwrites files!  I didn't know that.  In this case, they are matching files, but...!
## OK well I have removed all occurrences, but this may cause problems!

export LINK_DIR="$PWD/restored_from_cache"

. ut_server.config

. importshfn verbosely

if [ ! "$MAP_DIRS" ] || [ ! "$FILE_DIRS" ]
then
	echo "Need MAP_DIRS and FILE_DIRS"
	exit 1
fi

rmlinks () {
	DIR="$1"
	verbosely find "$DIR" -type l |
	# foreachdo verbosely rm
	# withalldo verbosely rm
	# foreachdo verbosely rmlink
	catwithprogress |
	# foreachdo rmlink
	foreachdo rm
}

get_map_list () {
	verbosely find $MAP_DIRS -type f -name "*.unr"
	# if [ -d "$LINK_DIR" ]
	# then
		# # find "$LINK_DIR" -type l |
		# # while read LINK
		# # do [ -e "$LINK" ] && verbosely ln -sf "$LINK" .
		# # done
		# verbosely find "$LINK_DIR" -name "*.unr"
	# fi
}

TODO="$1"
shift

case "$TODO" in

	cache)
		mkdir -p "$LINK_DIR"
		# BEFORE="$PWD"
		# cd "$LINK_DIR"
		# rmlinks .
		# cd "$BEFORE"
		rmlinks "$LINK_DIR"/

		jshinfo "Main files"
		## Since cache is not mostly symlinks, we add the direct files first:
		tty >/dev/null && TTY=true
		## And now we also add any textures/sounds etc. from the map dirs.
		## First incantation does maps too, but maybe second is better (less errorfull) if maps are there already!
		verbosely find $FILE_DIRS "$PWD"/ut-server/ -type f -name "*.u*" |
		if [ "$TTY" ]
		then catwithprogress
		else cat
		fi |
		# verbosely find $MAP_DIRS -type f -name "*.u*" -not -name "*.unr" |
		# verbosely withalldo ln -s --- "$LINK_DIR"
		while read FILE
		# do verbosely ln -s "$FILE" "$LINK_DIR"
		do [ ! -e "$LINK_DIR/`filename "$FILE"`" ] && ln -s "$FILE" "$LINK_DIR"
		done

		jshinfo "Cache files"
		## Finally we actually add files from the cache: (now comes last because cachefiles are symlinks these days!)
		# sh ./restore_names_from_cache.sh /mnt/big/unreal_cache "$LINK_DIR"
		sh ./restore_names_from_cache.sh /home/joey/linux/.loki/ut/Cache "$LINK_DIR"
	;;

	maps)
		cd ut-server/Maps &&
		rmlinks .

		echo "Total maps: "` get_map_list | grep "\.unr$" | wc -l `
		# get_map_list | grep "\.unr$" | randomorder | head -200 |
		(

			## Top 100 from the ranking:
			cd ../..
			memo sh ./map_ranking.sh getranking | dropcols 1 | head -100 |
			while read MAPNAME
			do IKNOWIDONTHAVEATTY=1 memo find $MAP_DIRS -type f -iname "$MAPNAME.unr" | head -1
			done # | pipeboth
			echo -n "Top 100: " >&2
			memo sh ./map_ranking.sh getranking | dropcols 1 | head -100 | wc -l >&2

			## Any selected by user regexp:
			# [ "$1" ] && get_map_list | grep "\.unr" | grep -i "$1"
			if [ "$*" ]
			then
				# REGEXP=`for X; do echo "$X"; done | list2regexp`
				# get_map_list | grep "\.unr" | grep -i "$REGEXP"
				for STRING
				do
					REGEXP=`toregexp "$STRING"`
					## NOTE we apply the regexp to the list of file *paths* because the directory may be useful.
					## BUG that means some queries will match all maps; we should only really grep the relative path from the maps topdir!
					# get_map_list | grep "\.unr" | grep -i "$REGEXP"
					MATCHING_MAPS=`get_map_list | grep "\.unr$" | grep -i "$REGEXP"`
					echo "$MATCHING_MAPS"
					# NUM_MAPS_MATCHING=`get_map_list | grep "\.unr" | grep -i "$REGEXP" | wc -l`
					NUM_MAPS_MATCHING=`echo "$MATCHING_MAPS" | wc -l`
					echo "Maps matching \"$STRING\": $NUM_MAPS_MATCHING" >&2
					if [ "$NUM_MAPS_MATCHING" -lt 10 ]
					then ( echo "$MATCHING_MAPS" | sed 's+.*/++;s+$+, +' | tr -d '\n' | sed 's+,$++' ; echo ) >&2
					fi
					# echo "\"$STRING\" maps: "`get_map_list | grep "\.unr" | grep -i "$REGEXP" | wc -l` >&2
				done | randomorder
			fi

			## All the rest:
			get_map_list | grep "\.unr$" | randomorder

		) | head -200 | # pipeboth |
		withalldo verbosely ln -s --- . 2>/dev/null
		echo "Loaded maps: "`verbosely find . -name "*.unr" | wc -l`
		cd ../..

		## Now do the ranking:
		## No cos the maps don't seem to load =/
		## OK now trying with _ instead of .:
		sh ./map_ranking.sh
		echo "Applied ranking"
	;;

	serverfiles)
		rmlinks ut-server/

		find $FILE_DIRS -type f -name "*.utx" |
		while read FILE
		do ln -s "$FILE" ut-server/Textures/
		done

		find $FILE_DIRS -type f -name "*.umx" -or -name "*.uax" |
		while read FILE
		do ln -s "$FILE" ut-server/Sounds/
		done

		find $FILE_DIRS -type f -name "*.umx" |
		while read FILE
		do ln -s "$FILE" ut-server/Music/
		done

		# find $FILE_DIRS -type f -name "*.u" -or -name "*.unn" |
		find $FILE_DIRS -type f -name "*.u" -or -name "*.unn" -or -name "*.int" -or -name "*.ini" |
		while read FILE
		do ln -s "$FILE" ut-server/System/
		done

		jshwarn "After serverfiles you should also do maps!!"
	;;

	http)
		## For HTTP redirection:
		# verbosely ln -s "$LINK_DIR"/* /var/www/tmp/ut_files/
		# verbosely ln -s $PWD/ut-server/Textures/* /var/www/tmp/ut_files/
		# verbosely ln -s $PWD/ut-server/Sounds/* /var/www/tmp/ut_files/
		# verbosely ln -s $PWD/ut-server/Music/* /var/www/tmp/ut_files/
		# verbosely ln -s $PWD/ut-server/Maps/* /var/www/tmp/ut_files/
		# verbosely ln -s $PWD/ut-server/System/*.u /var/www/tmp/ut_files/
		PREV="$PWD"
		cd /var/www/tmp/ut_files &&
		rmlinks .
		cd "$PREV"
		## MAP_DIRS unneccessary, but better to reduce symlinks chains
		# verbosely find $MAP_DIRS "$LINK_DIR"/ "$PWD/ut-server"/ -name "*.u*" -not -type d |
		# verbosely find $MAP_DIRS "$PWD/ut-server"/ -name "*.u*" -type f -not -type d |
		verbosely find "$PWD/ut-server"/ $FILE_DIRS -name "*.u*" -type f -not -type d |
		# withalldo verbosely ln -s --- /var/www/tmp/ut_files/
		# sort | ## That really mucks up order above!
		# verbosely foreachdo -x ln -s '$X' /var/www/tmp/ut_files/
		catwithprogress |
		while read FILE
		do ln -s "$FILE" /var/www/tmp/ut_files/
		done
		# Argument list too long: verbosely ln -s ` find "$LINK_DIR"/ "$PWD/ut-server"/ -name "*.u*" ` /var/www/tmp/ut_files/
	;;

	mods)
		## Patches are not in ut-server; used by asu not ucc.
		if cd Patches
		then
			rmlinks .
			verbosely find /stuff/software/games/unreal/server/mods/ -type f |
			# notindir added_ok windows_only might_work_not_tried | ## Strips these subdirs
			withalldo verbosely ln -s --- .
			cd ..
		fi
	;;

	all)
		for TODO in cache serverfiles maps http mods
		# for TODO in maps txandaudio system http mods cache
		do verbosely sh "$0" "$TODO"
		done
	;;

	clean)
		cd ut-server &&
		rmlinks .
		cd ..
		## Others...?!  Well ATM each other does it just before it relinks.
	;;

	*)
		echo "Don't know: $TODO"
		exit 1
	;;

esac

## Originally:
# cd ut-server/Maps
# TOTAL=`find . -type f -or -type l | wc -l`
# LINKS=`find . -type l | wc -l`
# WANTED=255
# OVER=`expr $TOTAL - 255`
# if [ "$OVER" -gt "$LINKS" ]
# then OVER="$LINKS"
# fi
# jshinfo "Trimming $OVER maps to reach $WANTED from $TOTAL"
# find . -type l | randomorder | head -n $OVER | foreachdo del
# cd ../..

