MAPLISTTEMP="ut-server/System/MapListTemp.ini"
cp "$MAPLISTTEMP" "$MAPLISTTEMP".`geekdate -fine`

cat "$MAPLISTTEMP" |
dos2unix |
# sed 's+^\(M\[[0-9]*\]=\).*+\1+' |
grep -v "^M\[[0-9]*]=" |
sed 's+^\(M\[[0-9]*\]=\).*+\1+' |
trimempty |
dog "$MAPLISTTEMP"

(

	find ./ut-server/Maps/ -name "*.unr" | afterlast / | beforelast .unr |
	grep -v "^CTF" |
	while read MAP
	do echo "$MAP 1"
	done |
	randomorder | head -n 128 | sort

	find ./ut-server/Maps/ -name "*.unr" | afterlast / | beforelast .unr |
	grep -i "^CTF-[A-Ma-m]" |
	while read MAP
	do echo "$MAP 2"
	done |
	randomorder | head -n 128 | sort

	find ./ut-server/Maps/ -name "*.unr" | afterlast / | beforelast .unr |
	grep -i "^CTF-[N-Z]" |
	while read MAP
	do echo "$MAP 3"
	done |
	randomorder | head -n 128 | sort

	./list_proposed_maps_for_xol.sh |
	removeduplicatelines |
	while read MAP
	do
		if [ -f ./ut-server/Maps/"$MAP".unr ]
		then echo "$MAP"
		else
			jshwarn "XOL map $MAP is not in Maps/"
			# locateonly "$MAP".unr | striptermchars | filesonly |
			locate /"$MAP".unr | grep "\.unr$" | striptermchars | filesonly |
			while read TARGET
			do jshinfo "Recommend: ln -s \"$TARGET\" \"$PWD/ut-server/Maps/$MAP.unr\""
			done
		fi
	done |
	sed "s+^CTF-+XOL-+" |
	while read MAP
	do echo "$MAP 4"
	done

	find ./ut-server/Maps/ -name "*.unr" | afterlast / | beforelast .unr |
	grep -i "^CTF-" |
	grep -i -v "^ctf-[A-Z]" |
	sort |
	while read MAP
	do echo "$MAP 4"
	done |
	randomorder | head -n 128 | sort

) |

(

	echo "M[0]=" >> "$MAPLISTTEMP"

	# NUM=`cat "$MAPLISTTEMP" | dos2unix | grep -v "^M\[0\]=" | grep -m 1 "^M\[[0-9]*\]=$" | grep -o -m 1 "[0-9][0-9]*"`
	NUM=1

	while read MAP LIST
	do
		# NUM=`cat "$MAPLISTTEMP" | dos2unix | grep -v "^M\[0\]=" | grep -m 1 "^M\[[0-9]*\]=$" | grep -o -m 1 "[0-9][0-9]*"`
		# cat "$MAPLISTTEMP" | dos2unix |
		# verbosely sed "s^M\[$NUM\]=$M[$NUM]=$MAP:$LIST" |
		# dog "$MAPLISTTEMP"
		echo "M[$NUM]=$MAP:$LIST"
		echo "M[$NUM]=$MAP:$LIST" >> "$MAPLISTTEMP"
		NUM="$((NUM+1))"
	done

	while [ "$NUM" -lt 1024 ]
	do
		echo "M[$NUM]=" >> "$MAPLISTTEMP"
		NUM="$((NUM+1))"
	done
		
	echo >> "$MAPLISTTEMP"

)

# NUM=`cat "$MAPLISTTEMP" | dos2unix | grep -v "^M\[0\]=" | grep -m 1 "^M\[[0-9]*\]=$" | grep -o -m 1 "[0-9][0-9]*"`
NUM=`cat "$MAPLISTTEMP" | dos2unix | grep -v "^M\[0\]=" | grep "^M\[[0-9]*\]=." | wc -l`
cat "$MAPLISTTEMP" |
sed "s+^MapCount=.*+MapCount=$NUM+" |
dog "$MAPLISTTEMP"

