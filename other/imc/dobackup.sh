# MAILONERROR="joey@hwi.ath.cx"
MAILONERROR="cronjobs@hwi.ath.cx"

SRCURL="http://bristol.indymedia.org/db-backups"

FILES="imc.mysql active_bristol.psql err.txt"

TOPDIR="/www/db-cron-backups"

WGETOPTS="-N"
# WGETOPTS="-r -L -np -A sql,txt -nd"

WHICH="$1"

LOGFILE="$TOPDIR/dobackup-$WHICH.log"

case "$WHICH" in
	daily)
		DIR="$TOPDIR/daily/"`date | sed "s/ .*//"`
		;;
	monthly)
		DIR="$TOPDIR/monthly/"`date | sed "s/[^ ]* //;s/ .*//"`
		;;
	*)
		echo "dobackup.sh daily | monthly"
		echo "  will copy $FILES from $SRCURL into a suitable subdir of $TOPDIR"
		exit 1
		;;
esac

mkdir -p "$DIR" &&

echo "Changing to directory $DIR" &&

cd "$DIR" &&

DBKERROR="0" &&

# Retrieve each file individually - hopefully they are there!
for FILE in $FILES; do
	echo "Getting $FILE ..."

	# wget $WGETOPTS -q -O "$FILE" "$SRCURL/$FILE"
	# For Debug:
	wget $WGETOPTS -O "$FILE" "$SRCURL/$FILE"

	if test "$?" = 0; then
		echo "Got OK"
	else
		echo "DBKERROR getting file"
		DBKERROR="1"
	fi
done > "$LOGFILE"

if test "$DBKERROR" != "0"; then
	cat "$LOGFILE" | mail -s "$HOST:[$0 $*] wget had exit code $DBKERROR" "$MAILONERROR"
	cat "$LOGFILE"
	exit "$DBKERROR"
fi

# Recursive retrieval yuk
# wget -r -L -np -A sql,txt -nd "$SRCURL/" ||

# One file at a time - watch out for renaming!
# wget $WGETOPTS "$SRCURL/active_bristol.psql" &&
# wget $WGETOPTS "$SRCURL/imc.mysql" &&
