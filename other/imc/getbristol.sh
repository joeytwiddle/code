#!/bin/bash
# (we need bash in order to check UID)
# Joey (pclark@cs.bris.ac.uk)
# Ensures the local /www/active-cvs/bristol is an exact copy of tronic:/www/active-cvs/bristol
# Does the same for /www/uploads/bristol
# Also puts tronic:/home/mike/db-backups into localhost:/www/db-backups
# (To do this it forces a backup on tronic, which may require sudo auth.)

# There are still some errors caused by mike@tronic not being able to read some
# files on tronic.  Not currently a problem though.  The errors are visible in
# the output of this script (in the form "send_files failed to open "...).

if test ! -f /www/getbris-auto && test ! "$1" = "-force"; then
	echo "/www/getbris-auto not present, aborting."
	echo "Use -force from the command line to override."
	exit 1
fi

# Commented out because it's difficult to do anyway!
# echo "Note: please do not run this script automatically, eg. in a regular cron job."
# echo "See the code for an explanation."
# echo
# Automatic backups are being taken automatically on Hwi.
# However, automatic backups run the risk of backing up the wrong site if we
# ever redirect bristol.indymedia.org.
# Or maybe that is exactly what we want the backup to do, backup the redirect
# (which presumably people will be submitting to).
# But if we want to avoid the redirected bristol.indymedia.org, we should
# use tronic.southspace.net instead throughout this script.
# Latter now implemented.

if test ! "$UID" = "0"; then
	echo "ERROR: If you do not run as root, I may not be able to allocate the same owner:groups as on tronic, or overwrite local files not owned by you ($USER)."
	exit 1
fi
# Note: as root, rsync will try to preserve user/group-name if it exists, otherwise it will preserve user-ID.
# Note: since we login as mike, remote files/directories not visible to mike will be skipped :-(

cd /www

export RSYNC_RSH="ssh-agent ssh"

# rsync options:
# -a means preserve all information (times, permissions) (requires root).
# --delete means delete local files which have been deleted from remote.
# -n means don't copy or delete any files, but show what would be done.
# -vv means verbose (informative) output.  To see errors only, remove -vv.
# -P means show progress, and continue partial downloads.
# --size-only is pretty speedy but skips some checksums.
# -z is for compression of data transfer.  =)

COMMONPARAMS="-a --delete -z -P -vv"
# COMMONPARAMS="-a --delete -z -P -vv"
# COMMONPARAMS="$COMMONPARAMS --size-only"
# COMMONPARAMS="$COMMONPARAMS -n"

cd /www

echo
echo "--- 1/4 --- Backup DBs on tronic -------------------------------------"
echo

# Sometimes these sudo's work automatically.  Maybe for a limited time after authentication by password?

# Not really needed so much now that we are taking backups automatically elsewhere.

echo "Skipping: cannot sudo without user passwd, but cron@tronic should have dumped the DBs recently anyway."
# $RSYNC_RSH mike@tronic.southspace.net sudo su postgres -c /home/mike/bin/dumpdbs.sh

echo
echo "--- 2/4 --- Get local copy of DBs ------------------------------------"
echo

rsync $COMMONPARAMS mike@tronic.southspace.net:/home/mike/db-backups .

echo
echo "--- 3/4 --- Get local copy of code and generated pages ---------------"
echo

rsync $COMMONPARAMS mike@tronic.southspace.net:/www/active-cvs/bristol /www/active-cvs/

echo
echo "--- 4/4 --- Get local copy of site's uploads ---------------"
echo

rsync $COMMONPARAMS mike@tronic.southspace.net:/www/uploads/bristol /www/uploads

date > /www/last-got-bristol.at

./post-pull.sh
