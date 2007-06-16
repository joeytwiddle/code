## TODO: We haven't yet included data files from the UT game client directory.
##       We shouldn't really have to, but at least CTF-Chasm wanted one of these!

## OMG ln -sf overwrites files!  I didn't know that.  In this case, they are matching files, but...!
## OK well I have removed all occurrences, but this may cause problems!

export LINK_DIR="$PWD/restored_from_cache"

. ut_server.config

if [ ! "$MAP_DIRS" ]
then
	echo "Need MAP_DIRS"
	exit 1
fi

rmlinks () {
	DIR="$1"
	verbosely find "$DIR" -type l |
	# foreachdo verbosely rm
	withalldo verbosely rm
}

get_map_list () {
	# find /stuff/software/games/unreal/server/maps_defaults+some_i_found_on_web/ -type f
	# find /stuff/software/games/unreal/server/maps/unzipped/ -type f
	# find /stuff/software/games/unreal/server/maps_unzipped/ -type f -name "*.unr"
	verbosely find $MAP_DIRS -type f -name "*.unr"
	if [ -d "$LINK_DIR" ]
	then
		# find "$LINK_DIR" -type l |
		# while read LINK
		# do [ -e "$LINK" ] && verbosely ln -sf "$LINK" .
		# done
		verbosely find "$LINK_DIR" -name "*.unr"
	fi
}

TODO="$1"
shift

case "$TODO" in

	cache)
		mkdir -p "$LINK_DIR"
		BEFORE="$PWD"
		cd "$LINK_DIR"
		rmlinks .
		cd "$BEFORE"
		# sh ./restore_names_from_cache.sh /mnt/big/unreal_cache "$LINK_DIR"
		sh ./restore_names_from_cache.sh /home/joey/linux/.loki/ut/Cache "$LINK_DIR"

		## And now we also add any textures/sounds etc. from the map dirs.
		# verbosely find $MAP_DIRS -type f -name "*.u*" |
		verbosely find $MAP_DIRS -type f -name "*.u*" -not -name "*.unr" |
		verbosely withalldo ln -s --- "$LINK_DIR"
	;;

	maps)
		cd ut-server/Maps &&
		rmlinks .

		get_map_list | grep "\.unr$" | wc -l
		# get_map_list | grep "\.unr$" | randomorder | head -200 |
		(
			[ "$1" ] && get_map_list | grep "\.unr" | grep -i "$1"
			get_map_list | grep "\.unr$" | randomorder
		) | head -200 |
		withalldo verbosely ln -s --- .
		verbosely find . -name "*.unr" | wc -l
		cd ../..
	;;

	txandaudio)
		cd ut-server/Textures &&
		rmlinks .
		ln -s "$LINK_DIR"/*.utx .
		cd ../..

		cd ut-server/Sounds &&
		rmlinks .
		ln -s "$LINK_DIR"/*.umx .
		ln -s "$LINK_DIR"/*.uax .
		cd ../..

		cd ut-server/Music &&
		rmlinks .
		ln -s "$LINK_DIR"/*.umx .
		cd ../..
	;;

	system)
		cd ut-server/System &&
		rmlinks .
		ln -s "$LINK_DIR"/*.u .
		ln -s "$LINK_DIR"/*.unn . ## skeletalchars.unn!
		cd ../..
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
		verbosely find $MAP_DIRS "$LINK_DIR"/ "$PWD/ut-server"/ -name "*.u*" -not -type d |
		# withalldo verbosely ln -s --- /var/www/tmp/ut_files/
		# sort | ## That really mucks up order above!
		# verbosely foreachdo -x ln -s '$X' /var/www/tmp/ut_files/
		while read FILE
		do ln -s "$FILE" /var/www/tmp/ut_files/
		done
		# Argument list too long: verbosely ln -s ` find "$LINK_DIR"/ "$PWD/ut-server"/ -name "*.u*" ` /var/www/tmp/ut_files/
	;;

	mods)
		## Patches are not in ut-server; used by asu not ucc.
		cd Patches &&
		rmlinks .
		# verbosely ln -s /stuff/software/games/unreal/server/mods/* .
		verbosely find /stuff/software/games/unreal/server/mods/ -type f |
		# notindir added_ok windows_only might_work_not_tried | ## Strips these subdirs
		withalldo verbosely ln -s --- .
		cd ..
	;;

	all)
		for TODO in cache maps txandaudio system http mods
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

