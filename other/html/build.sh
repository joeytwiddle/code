for X in *.jpp
do
	Y="$(echo "$X" | sed 's+\.jpp$++')"
	## The checks are useless - we want to recompile all the .jpp files when one
	## of them changes.  Otherwise we must check dependencies like Makefile does.
	if [ ! -f "$Y" ] || newer "$X" "$Y" || true
	then
		jshinfo "[jpp] '$X' -> '$Y'"
		[ -f "$Y" ] && [ -f "$Y.backup" ] || verbosely cp "$Y" "$Y.backup"
		verbosely jpp -- "$X" | dog "$Y"
	fi
done
