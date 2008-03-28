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
	if [ ! -e "$TARGETFILE" ] || find . -name "*.jpp" -newer "$TARGETFILE" | grep . >/dev/null ## ANY new jpp file in this folder.
	then
		[ "$JPP_MAKE_BACKUPS" ] && [ -f "$TARGETFILE" ] && mv "$TARGETFILE" "$TARGETFILE".`geekdate -fine`
		# verbosely jpp -- "$JPPFILE" | verbosely dog "$TARGETFILE"
		echo "jpp: $JPPFILE -> $TARGETFILE" >&2
		jpp -- "$JPPFILE" > "$TARGETFILE"
	fi
done
cd "$TOPDIR"





## TODO: What's really nasty is when we compile a script, then try to load it as a lib when recompiling other things.  On hwi atm, it's causing ucc to stick on that package.

## Problem: comments out required packages!  also adds some packages (e.g. Screen) which we don't actually want to recompile
##    TODO: But it doesn't have to.  As long as it checks that it is really a custom package.
cat compiling.ini | dos2unix | grep EditPackages= |
afterlast = |
while read PKG
do
	if [[ -d "$PKG/Classes" ]]
	then
		if [[ ! -f "System/$PKG.u" ]] || find "$PKG/Classes" -newer "System/$PKG.u" | grep . >/dev/null
		then
			echo "Needs rebuild: $PKG"
			cat compiling.ini | dos2unix | sed "s+^[; ]*EditPackages=$PKG$+EditPackages=$PKG+" | dog compiling.ini
			[[ -f "System/$PKG.u" ]] && verbosely mv "System/$PKG.u" "System/$PKG.u.last"
		else
			echo "Nothing new: $PKG"
			cat compiling.ini | dos2unix | sed "s+^[; ]*EditPackages=$PKG$+; EditPackages=$PKG+" | dog compiling.ini
		fi
	fi
done





if [ "$WINDIR" ] # cygwin
then
	cmd /c make
	exit
fi

# REBUILD=true

cat compiling.ini |
dos2unix |
grep "^EditPackages=" |
afterfirst = |
# pipeboth |

while read PKGNAME
do
	if [ ! -f "System/$PKGNAME.u" ]
	then jshinfo "$PKGNAME.u is pending..."
	elif [ ! -d "$PKGNAME" ]
	then : # error "Source folder $PKGNAME/ does not exist!"
	elif [ "$REBUILD" ] || find "$PKGNAME"/Classes -maxdepth 1 '(' -iname "*.uc" -or -iname "*.jpp" ')' -and -newer "System/$PKGNAME.u" | grep . >/dev/null
	then
		jshinfo "$PKGNAME.u needs a rebuild..."
		# ls -l "System/$PKGNAME.u"
		## verbosely del System/$PKGNAME.u
		verbosely mv -f System/$PKGNAME.u System/$PKGNAME.u.last
	fi
done 2>&1 | grep . || jshwarn "No .u files out-of-date or missing."

cd System
verbosely wine ucc make ini=../compiling.ini

