for X in *.jpp
do
	Y="$(echo "$X" | sed 's+\.jpp$++')"
	jshinfo "jpp '$X' -> '$Y'"
	jpp -- "$X" | dog "$Y"
done
