# Does it find the lowest-level of required links?
# (eg. it could claim dependency on a big module, not for its functions, but for its includes!)
# Nah I think it's OK, because joeylib.c is ordered list
# and it tried omitting modules from the bottom.

if test "$1" = ""; then
	echo "testdepends <c_file_name> [ gcc options ]"
	exit 1
fi

MODULE="$1"
shift
export GCCARGS="$@"
MODULES=`
	cat "$JPATH/code/c/joeylib/joeylib.c" |
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

testwith () {
	(
		echo "$1" |
		while read MOD; do
			if test $MOD; then
				echo "#include <$MOD.h>"
			fi
		done
		echo "#include \"$MODULE.c\""
	) > test.c
	g++ $GCCARGS -w -I "$JPATH/code/c/joeylib/.comp/" test.c > gcc.out 2>&1
}

ORIGMODULES="$MODULES"
echo
cursecyan
echo "$MODULE:"
cursegrey
echo "Requires at most: "`echo "$MODULES" | tr "\n" " "`
testwith "$MODULES"
if test "$?" = "0"; then
	echo "Compiled OK."
else
	echo "Error compiling with everything!"
fi
echo

# This double loop is only needed if solving one problem might solve others.
# ( 4 top, 2 middle = "	 " )
 NOMORE=
 while test ! $NOMORE; do
	 NOMORE=true
	 if test `echo "$MODULES" | countlines` -gt 0; then
		for OMITMOD in `echo "$MODULES" | reverse`; do
			# echo "Testing without: $OMITMOD"
			TESTMODULES=`echo "$MODULES" | grep -v "^$OMITMOD$"`
			testwith "$TESTMODULES"
			if test "$?" = "0"; then
				MODULES="$TESTMODULES"
				echo "Succeeded ommiting "`curseyellow`$OMITMOD`cursegrey`"."
				echo "Now using: "`echo "$MODULES" | tr "\n" " "`
				echo
				 NOMORE=
				 # break
			else
				echo "Errors omitting "`cursegreen`$OMITMOD`cursegrey`":"
				cursered
				head -4 gcc.out
				cursegrey
				echo
			fi
		done
	 fi
 done

cursecyan
echo
echo "$MODULE:"
cursegrey
echo "Stripped down to: "`echo "$MODULES" | tr "\n" " "`
echo "            from: "`echo "$ORIGMODULES" | tr "\n" " "`

if test -d stubs; then

	mv "$MODULE-header.h" stubs/

	if test ! -f "$MODULE.h.bak"; then
		cp "$MODULE.h" "$MODULE.h.bak"
	fi
	(
		echo "#ifndef $MODULE""_H"
		echo "  #define $MODULE""_H"
		echo
		for INCMOD in $MODULES; do
			echo "  #include <$INCMOD.h>"
		done
		echo
		# echo "  #include <$MODULE-header.h>"
		cat "stubs/$MODULE-header.h"
		echo
		echo "#endif"
	) > "$MODULE.h"

fi

testwith "$MODULES"

mv test.o $MODULE.o
