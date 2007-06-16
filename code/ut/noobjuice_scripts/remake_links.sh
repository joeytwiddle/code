sh ./restore_names_from_cache.sh /mnt/big/unreal_cache

cd Patches
# ln -s /stuff/software/games/unreal/server/mods/* .
find /stuff/software/games/unreal/server/mods/ -type f |
withalldo ln -s --- .
cd ..

cd ut-server/Maps
ln -s /stuff/software/games/unreal/server/maps/unzipped/* .
cd ../..

cd ut-server/Maps
if [ -d /tmp/links ]
then
	# find /tmp/links -type l |
	# while read LINK
	# do [ -e "$LINK" ] && ln -s "$LINK" .
	# done
	ln -s /tmp/links/*.unr .
fi
cd ../..

cd ut-server/Maps
TOTAL=`find . -type f -or -type l | wc -l`
LINKS=`find . -type l | wc -l`
WANTED=255
OVER=`expr $TOTAL - 255`
if [ "$OVER" -gt "$LINKS" ]
then OVER="$LINKS"
fi
jshinfo "Trimming $OVER maps to reach $WANTED from $TOTAL"
find . -type l | randomorder | head -n $OVER | foreachdo del
cd ../..

cd ut-server/Textures
ln -s /tmp/links/*.utx .
cd ../..

cd ut-server/System
ln -s /tmp/links/*.u .
cd ../..

cd ut-server/Sounds
ln -s /tmp/links/*.umx .
ln -s /tmp/links/*.uax .
cd ../..

cd ut-server/Music
ln -s /tmp/links/*.umx .
cd ../..
