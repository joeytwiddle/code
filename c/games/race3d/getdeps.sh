# Nah needs to do recursive search,
# because these includes might have their
# own dependencies on other .o's.

SRCDIR="$JPATH/code/c/joeylib/.comp"

getmods () {
	MSF="$2"
	if test -f "$1"; then
		cat "$1" |
		grep "include.*\.h" |
		after "<" | before "\.h>" |
		grep -v '^joeylib$'
	fi
}

adddeps () {
	MSF="$1"
	getmods "$SRCDIR/$2.h" |
	while read X
	do
		if ! echo "$MSF" | grep "$X" > /dev/null; then
			# echo "  Adding $X ("`echo "$MSF" | tr "\n" " "`")" > /dev/stderr
			echo "$X"
			MSF="$MSF
$X"
			MTA=`adddeps "$MSF" "$X"`
			echo "$MTA"
			MSF="$MSF
$MTA"
		fi
	done
}

MODULES=`getmods "$1"`

# cursecyan; echo "Top header includes:"
# echo $MODULES
# cursegrey

export MSF="$MODULES";
MODULES="$MODULES
"`
	echo "$MSF" |
	while read MOD
	do
		# echo > /dev/stderr
		# echo "Finding modules for $MOD:" > /dev/stderr
		TOADD=\`adddeps "$MSF" $MOD\`
		# echo "toadd got:" > /dev/stderr
		# echo "$TOADD" | tr "\n" " " > /dev/stderr
		# echo > /dev/stderr
		MSF="$MSF
$TOADD"
		echo "$TOADD"
		# echo "$MSF" | tr "\n" " " > /dev/stderr
		# echo > /dev/stderr
	done
`

# cursecyan; echo "All includes:"
echo "$MODULES" |
# cursegrey

# trimempty |
# sed "s+^+$SRCDIR/+;s/$/\.o/"
while read MOD; do
	if test -f "$SRCDIR/$MOD.o"; then
		# echo "$SRCDIR/$MOD.o"
		'ls' "$SRCDIR/$MOD"*.o 2> /dev/null
	# else
		# echo "$SRCDIR/$MOD.o does not exist!" > /dev/stderr
	fi
done
