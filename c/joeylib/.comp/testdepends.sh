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

ORIGMODULES="$MODULES"

stripline () { # 1=lineToDrop
	N=0
	while read Y; do
		N=`expr $N + 1`;
		if test ! "$N" = "$1"; then
			echo "$Y"
		fi
	done
}

testwith () {
	echo "Testing with: "`echo "$1" | tr "\n" " "`
	(
		echo "$1" |
		while read MOD; do
			echo "#include <$MOD.h>"
		done
		echo "#include <$MODULE.c>"
	) > test.c
	g++ -w -c -I . test.c > /dev/null 2>&1
}

testwith "$MODULES"

NOMORE=
while test ! $NOMORE; do
	NOMORE=true
	NUM=`echo "$MODULES" | countlines`
	if test "$NUM" -gt 1; then
		echo "$NUM"
		for NUM2DROP in `seq $NUM 1`; do
			TESTMODULES=`echo "$MODULES" | stripline "$NUM2DROP"`
			testwith "$TESTMODULES"
			if test "$?" = "0"; then
				echo "Succeeded!"
				MODULES="$TESTMODULES"
				NOMORE=
				break
			fi
		done
	fi
done

echo "Stripped down to: "`echo "$MODULES" | tr "\n" " "`
echo "            from: "`echo "$ORIGMODULES" | tr "\n" " "`

