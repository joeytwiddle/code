# Does it find the lowest-level of required links?
# (eg. it could claim dependency on a big module, not for its functions, but for its includes!)
# Nah I think it's OK, because joeylib.c is ordered list
# and it tried omitting modules from the bottom.

MODULE="$1"
MODULES=`
	cat ../joeylib.c |
		grep -E "^[^/]*#include" |
		afterfirst "<" | beforefirst "\." |
	while read X; do
		if test "$X" = "$MODULE"; then
			break
		else
			echo "$X"
		fi
	done
	# encodeslashn |
	# beforefirst "$MODULE" |
	# decodeslashn
`
# echo "Depends on at most:
# $MODULES"

testwith () {
	# echo "Testing with: "`echo "$1" | tr "\n" " "`
	(
		echo "$1" |
		while read MOD; do
			echo "#include <$MOD.h>"
		done
		echo "#include <$MODULE.c>"
	) > test.c
	g++ -w -c -I . test.c > /dev/null 2>&1
}

ORIGMODULES="$MODULES"
echo "Requires at most: "`echo "$MODULES" | tr "\n" " "`
testwith "$MODULES"

NOMORE=
while test ! $NOMORE; do
	NOMORE=true
	if test `echo "$MODULES" | countlines` -gt 0; then
		for OMITMOD in `echo "$MODULES" | reverse`; do
			echo "Testing without: $OMITMOD"
			TESTMODULES=`echo "$MODULES" | grep -v "^$OMITMOD$"`
			testwith "$TESTMODULES"
			if test "$?" = "0"; then
				MODULES="$TESTMODULES"
				echo "Succeeded ommiting $OMITMOD."
				echo "Now using: "`echo "$MODULES" | tr "\n" " "`
				NOMORE=
				break
			fi
		done
	fi
done

echo "Stripped down to: "`echo "$MODULES" | tr "\n" " "`
echo "            from: "`echo "$ORIGMODULES" | tr "\n" " "`

