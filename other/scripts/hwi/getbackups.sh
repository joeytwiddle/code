## Note: if script outputs anything then cron will mail it: check same address!
## Maybe we could ask wget to be silent.
BACKDIR=/home/joey/.hwibackups
mkdir -p $BACKDIR
(
	cd $BACKDIR &&
	rm -f problems &&
	wget --no-parent -mirror "http://hwi.ath.cx/.backups" 2>&1 ||
	touch problems
) > $BACKDIR/getbackup.log
if test ! -d $BACKDIR || test -f $BACKDIR/problems
then cat $BACKDIR/getbackup.log | mail joey@hwi.ath.cx
fi
