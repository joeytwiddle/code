## TODO: Want a safe tar, that does not copy:
## higher directories (.*), symbolic links, executables, zips

export OLDDESTDIR=/mnt/stig/oldbackups
export DESTDIR=/mnt/stig/backups

error() {
	echo "Error: $*"
	echo "Error: $*" >&2
}

safetar() {
	DESTNAME="$1"
	shift
	echo "Creating $DESTNAME from $*"
	# cd $1
	# /bin/tar cfz "$DESTDIR"/$2.tgz *
	tar $TAROPTS --create -z --file "$DESTDIR/$DESTNAME.tgz" "$@" ||
	error "compressing $DESTDIR/$DESTNAME.tgz"
}

centralise "Starting routine backup"
## echo Hwi is performing a routine backup | wall
date

# echo $PATH

centralise "Before"
ls -l "$DESTDIR/"
# dush "$DESTDIR/"

centralise "Doing backup"

# Copy current backups into old and clear ready for new
mkdir -p "$DESTDIR"/
mkdir -p "$OLDDESTDIR"/
echo "Moving backups from $DESTDIR to $OLDDESTDIR"
mv -f "$DESTDIR"/* "$OLDDESTDIR"

# /etc
safetar etc /etc/
#safetar twiddle /home/joey/

# Joey's ~ directory (.* files and stuff...)
# We get a weird error if the zip goes at the bottom, after the
# slightly dodgy safetar etc ... !
# zip -q "$DESTDIR"/twiddle `find /home/joey/debian/ -type f -size 0 -o -size 1 -o -size 2 -o -size 3 -o -size 4 -o -size 5 -o -size 6 -o -size 7 -o -size 8 -o -size 9 -maxdepth 2`
# zip -q "$DESTDIR"/twiddle .* *
cd /home/joey/
cp private.tgz.encrypted "$DESTDIR" ||
cp private.tgz.encrypted.old "$DESTDIR" ||
cp private.tgz.encrypted.bak "$DESTDIR" ||
error "Error copying private"
# zip -q -r "$DESTDIR"/twiddle debian/.gnupg debian/.wine* debian/.mutt debian/Mail debian/.vmware
## I have no idea why I need to touch it!
touch "$DESTDIR"/twiddle.tgz
# tar cfz "$DESTDIR"/twiddle.tgz debian/.gnupg debian/.wine* debian/Mail debian/.vmware j/org j/music j/logs/debpkgs-list-today.log
export TAROPTS="--exclude=debian/Mail/Lists"
safetar twiddle \
        debian/.gnupg debian/.wine*/ debian/.vmware \
        debian/.*rc debian/.fetchmail* debian/.forward* \
        j/*.conf j/org j/music j/logs/debpkgs-list-today.log \
        j/*.dummy
        # debian/Mail 
        ## The dummy is to prove thar tar will still work if any of the other files are working.
        ## (Unless you want it to crash and inform you if they are missing!!)
unset TAROPTS

## Now all in CVS.
# Joeylib, JLib, C, Java sources
# makeport
# cp /home/joey/j/out/hwiport.tgz "$DESTDIR"/

# Website
# safetar hwihtml /var/www/
safetar portablelinux /stuff/portablelinux/
# safetar servlets /usr/share/java/servlets/ ## Used to be in there rather than java/servlets

# Organiser
safetar org /home/joey/j/org/
# Tools go with makeport
# safetar shellscript /home/joey/j/code/shellscript

# Submit revision changes
# revisionchanges

safetar cvsroot /stuff/cvsroot

centralise "After"
ls -l "$DESTDIR/"
# dush "$DESTDIR/"

crontab -l > "$DESTDIR"/root.crontab
su - joey -- crontab -l > "$DESTDIR"/joey.crontab

centralise Done
## echo Backup complete | wall
echo

# ( echo
	# date
	# cvsdiffs
# ) > $JPATH/logs/cvsdiffs.txt
