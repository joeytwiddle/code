cat System/UnrealTournament_compiling.ini |
dos2unix |
grep "^EditPackages=" |
afterfirst = |
# pipeboth |

while read PKGNAME
do
	if [ ! -f System/$PKGNAME.u ]
	then jshinfo "$PKGNAME.u is pending..."
	elif find . -name "$PKGNAME".uc -newer System/$PKGNAME.u | grep . >/dev/null
	then
		jshinfo "$PKGNAME.u needs a rebuild..."
		# verbosely del System/$PKGNAME.u
		verbosely mv -f System/$PKGNAME.u System/$PKGNAME.u.last
	fi
done 2>&1 | grep . || jshwarn "No .u files out-of-date or missing."

cd System
verbosely wine ucc make ini=UnrealTournament_compiling.ini

