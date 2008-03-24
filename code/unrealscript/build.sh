## DONE: Maybe the .uc.jpp file has not change, but one of the included files HAS changed, hence a reparse IS required although it doesn't look like it.
##       OK for now, it will reparse the file, if ANY file ending ".jpp" in that folder is newer than it.  This still doesn't resolve #included files from *other* folders.

TOPDIR="$PWD"
# find */Classes -maxdepth 2 -name "*.jpp" |
find */Classes/*.uc.jpp -type f |
while read JPPFILE
do
	cd "$TOPDIR"
	cd "`dirname "$JPPFILE"`"
	JPPFILE="`basename "$JPPFILE"`"
	TARGETFILE="`echo "$JPPFILE" | sed 's+\.jpp$++'`"
	# if [ ! -e "$TARGETFILE" ] || ( [ -f "$TARGETFILE" ] && newer "$JPPFILE" "$TARGETFILE" )
	if [ ! -e "$TARGETFILE" ] || find . -name "*.jpp" -newer "$TARGETFILE" | grep . >/dev/null
	then
		[ "$JPP_MAKE_BACKUPS" ] && [ -f "$TARGETFILE" ] && mv "$TARGETFILE" "$TARGETFILE".`geekdate -fine`
		# verbosely jpp -- "$JPPFILE" | verbosely dog "$TARGETFILE"
		echo "jpp: $JPPFILE -> $TARGETFILE" >&2
		jpp -- "$JPPFILE" > "$TARGETFILE"
	fi
done
cd "$TOPDIR"



## Problem: comments out required packages!  also adds some packages (e.g. Screen) which we don't actually want to recompile
# cat compiling.ini | grep EditPackages= |
# afterlast = |
# while read PKG
# do
	# if [[ -d "$PKG/Classes" ]]
	# then
		# if [[ ! -f "System/$PKG.u" ]] || find "$PKG/Classes" -newer "System/$PKG.u" | grep . >/dev/null
		# then
			# echo "Needs rebuild: $PKG"
			# cat compiling.ini | sed "s+^[; ]*EditPackages=$PKG$+EditPackages=$PKG+" | dog compiling.ini
		# else
			# echo "Nothing new: $PKG"
			# cat compiling.ini | sed "s+^[; ]*EditPackages=$PKG$+; EditPackages=$PKG+" | dog compiling.ini
		# fi
	# fi
# done



cmd /c make

