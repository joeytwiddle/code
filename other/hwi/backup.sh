## TODO: Want a safe tar, that does not copy:
## higher directories (.*), symbolic links, executables, zips

## TODO: rotate all files in backup dir, not just safetars
## safetars should be makebackup'ed.

export OLDDESTDIR=/mnt/stig/oldbackups
export DESTDIR=/mnt/stig/backups

error() {
	echo "Error: $*"
	echo "Error: $*" >&2
}

safetar() {
	DESTNAME="$1"
	shift
	# echo "Creating $DESTNAME from $*"
	# cd $1
	# /bin/tar cfz "$DESTDIR"/$2.tgz *
	# tar $TAROPTS --create --gzip --file "$DESTDIR/$DESTNAME.tgz" "$@" ||
	verbosely tar $TAROPTS --create --gzip --file "$DESTDIR/$DESTNAME.tgz" "$@" ||
	error "compressing $DESTDIR/$DESTNAME.tgz"
	# rotate -nozip "$DESTDIR/$DESTNAME.tgz"
}

function encrypt_file() {
	FILETOENCRYPT="$1"
	## Use joey's key, not root's:
	export GNUPGHOME=/home/joey/debian/.gnupg
	## Because even joey had trouble trusting the key I was using, we make it trust it anyway:
	export ENCDIR_GPGOPTS="--trust-model always"
	## Encrypt the backup of etc:
	NOBACKUP=true encryptdir -notar "$FILETOENCRYPT" &&
	rm "$FILETOENCRYPT"
}

centralise "Starting routine backup"
## echo Hwi is performing a routine backup | wall
date

# echo $PATH

centralise "Before"
dsr "$DESTDIR/"
# dush "$DESTDIR/"

centralise "Moving current $DESTDIR/* to $OLDDESTDIR/"

# Copy current backups into old and clear ready for new
mkdir -p "$DESTDIR"/ || exit 2
mkdir -p "$OLDDESTDIR"/ || exit 3
# echo "Moving backups from $DESTDIR to $OLDDESTDIR"
# mv -f "$DESTDIR"/* "$OLDDESTDIR"
echo "Copying backups from $DESTDIR to $OLDDESTDIR"
cp -f "$DESTDIR"/* "$OLDDESTDIR"
cd "$OLDDESTDIR" || exit 4
find . -type f | egrep -v "\.[[:digit:]]+" |
while read FILE
do rotate -nozip -max 4 "$FILE"
done

centralise "Doing backup"

## TODO: the encryptdir should be run in-stream
##       otherwise they could read etc.tgz in the brief moment before it's removed
# /etc
safetar etc /etc/
encrypt_file "$DESTDIR"/etc.tgz
#safetar twiddle /home/joey/

## If these files get to big due to /(xchat|)logs then just go for /plugin
verbosely ssh joey@neuralyte.org tar cj ./ircbot/ .xchat2.utb0t/ .ircbot/ .pugbot/ > "$DESTDIR"/utb0t-neuralyte.tar.bz2
verbosely ssh joey@hwi.ath.cx tar cj ./ircbot/ .xchat2.utb0t/ .ircbot/ .pugbot/ > "$DESTDIR"/utb0t-hwi.tar.bz2

# Joey's ~ directory (.* files and stuff...)
# We get a weird error if the zip goes at the bottom, after the
# slightly dodgy safetar etc ... !
# zip -q "$DESTDIR"/twiddle `find /home/joey/debian/ -type f -size 0 -o -size 1 -o -size 2 -o -size 3 -o -size 4 -o -size 5 -o -size 6 -o -size 7 -o -size 8 -o -size 9 -maxdepth 2`
# zip -q "$DESTDIR"/twiddle .* *
cd /home/joey/linux/.private/
cp private.tgz.encrypted "$DESTDIR" ||
cp private.tgz.encrypted.old "$DESTDIR" ||
cp private.tgz.encrypted.bak "$DESTDIR" ||
error "Error copying private"
cd /home/joey/
# zip -q -r "$DESTDIR"/twiddle debian/.gnupg debian/.wine* debian/.mutt debian/Mail debian/.vmware
## I have no idea why I need to touch it!
touch "$DESTDIR"/twiddle.tgz
# tar cfz "$DESTDIR"/twiddle.tgz debian/.gnupg debian/.wine* debian/Mail debian/.vmware j/org j/music j/logs/debpkgs-list-today.log
export TAROPTS="--exclude=debian/Mail/Lists --exclude=j/music/streamed --exclude=debian/.kde/share/apps/kmail/imap --exclude=debian/.kde/cache-hwi"
safetar twiddle \
				debian/Mail/account* \
				debian/Mail/banking \
        *.txt \
        linux/.loki/*/*.ini* \
        debian/.gnupg debian/.wine*/ debian/.vmware \
        debian/.*rc debian/.fetchmail* debian/.forward* \
        debian/.gaim debian/.ymessenger \
        debian/.kde \
        debian/*cron* \
        debian/.gnupg \
        j/*.conf j/org j/config j/music j/logs/debpkgs-list-today.log \
        j/code/shellscript \
				# kinda want j/* \
				# j/Help \
        j/*.dummy ## doesn't exist; just an end line not requiring \
        # debian/Mail
        # debian/.ssh ## don't really want to back these up (unless we encrypt them, but this same archive contains the keys for unencrypting!)
        ## The dummy is to prove thar tar will still work if any of the other files are working.
        ## (Unless you want it to crash and inform you that they are missing!!)
				# debian/Mail/friends \
				# debian/Mail/incoming \
				# debian/Mail/Drafts \
				# debian/Mail/postponed \
				# debian/Mail/*sent* \
				# debian/Mail/*Sent* \
				# debian/Mail/*tribe* \
				# debian/Mail/tosort \
				# debian/Mail/news* \
				# debian/Mail/ \
unset TAROPTS
encrypt_file "$DESTDIR"/twiddle.tgz

## We really need a secure copy of the key which we use to encrypt these backups!
## Well since we don't plan to make these backups publically readable anyway, we take an open copy of the key, unmasked.
safetar gnu $HOME/.gnupg

        # debian/Mail/(friends*|incoming*|accounts*|news*|tosort|*tribe*|postponed|Drafts|*Sent*|*sent*|banking) \

# zip -r "$DESTDIR"/Mail.zip Mail/(friends*|incoming*|accounts*|news*|tosort*/*|*tribe*|postponed|Drafts|*Sent*|*sent*|banking)

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
dsr "$DESTDIR/"
# dush "$DESTDIR/"

crontab -l > "$DESTDIR"/root.crontab
su joey -- crontab -l > "$DESTDIR"/joey.crontab

centralise Done
## echo Backup complete | wall
echo

# ( echo
	# date
	# cvsdiffs
# ) > $JPATH/logs/cvsdiffs.txt
