generate_dependencies_list () {

	find Maps/ -name "*.unr" |

	while read MAP
	do

		MAPNAME=`echo "$MAP" | afterlast /`
		utdep.pl "$MAP" |
		while read DEP
		do echo "$MAPNAME -> $DEP"
		done

	done > dependencies.list

}



# generate_dependencies_list

find Textures/ Music/ Sounds/ System/ -name "*.u*" |
while read DEP
do

	DEPNAME=`echo "$DEP" | afterlast / | toregexp`
	grep -i " -> $DEPNAME$" dependencies.list >/dev/null ||
	jshinfo "Did not find $DEPNAME in dependencies.list"

done

