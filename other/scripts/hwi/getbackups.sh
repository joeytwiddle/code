BACKDIR=/home/joey/.hwibackups
mkdir -p $BACKDIR
(
	cd $BACKDIR &&
	rm -f problems &&
	wget -r --no-parent "http://hwi.ath.cx/.backups" ||
	touch problems
) > $BACKDIR/getbackup.log
if test ! -d $BACKDIR || test -f $BACKDIR/problems
then cat $BACKDIR/getbackup.log | mail joey@hwi.ath.cx
fi
