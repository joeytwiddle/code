#!/bin/bash
# (we need bash in order to check UID)

# Ensures the local /www/active-cvs/bristol is an exact copy of tronic:/www/active-cvs/bristol
# Does the same for /www/uploads/bristol
# Also puts tronic:/home/mike/db-backups into localhost:/www/db-backups

if test ! "$UID" = "0"; then
	echo "If you do not run as root, I may not be able to allocate correct owner:groups, or overwrite local files not owned by you = $USER."
fi
# Note: as root, rsync will try to preserve user/group-name if it exists, otherwise it will preserve user-ID.
# Note: since we login as mike, remote files/directories not visible to mike will be skipped :-(

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
# COMMONPARAMS="$COMMONPARAMS --size-only"
# COMMONPARAMS="$COMMONPARAMS -n"

cd /www

echo
echo "--- 1/4 --- Backup DBs on tronic -------------------------------------"
echo

# Sometimes these sudo's work automatically.  Maybe for a limited time after authentication by password?

# Not really needed so much now that these backups are being dumped automatically.

$RSYNC_RSH mike@bristol.indymedia.org sudo su postgres -c /home/mike/bin/dumpdbs.sh

echo
echo "--- 2/4 --- Get local copy of DBs ------------------------------------"
echo

rsync $COMMONPARAMS mike@bristol.indymedia.org:/home/mike/db-backups .

echo
echo "--- 3/4 --- Get local copy of code and generated pages ---------------"
echo

rsync $COMMONPARAMS mike@bristol.indymedia.org:/www/active-cvs/bristol /www/active-cvs/

echo
echo "--- 4/4 --- Get local copy of site's uploads ---------------"
echo

rsync $COMMONPARAMS mike@bristol.indymedia.org:/www/uploads/bristol /www/uploads

date > /www/last-got-bristol.at
