#!/bin/bash
# (we need bash in order to check UID)

# Ensures the local /www/active-cvs/bristol is an exact copy of tronic:/www/active-cvs/bristol

if test ! "$UID" = "0"; then
	echo "If you do not run as root, I may not be able to allocate correct owner:groups, or overwrite local files not owned by you = $USER."
	# Note: as root, rsync will try to preserve user/group-name if it exists, otherwise it will preserve user-ID.
fi

export RSYNC_RSH="ssh-agent ssh"

# rsync options:
#   -z is for compression
#   -a means preserve all information (times, permissions) (requires root)
#   --delete means delete local files which have been deleted from remote
#   -vv means show what it's doing
#   -P means show progress, and continue partial downloads
#   --size-only is pretty speedy but does not do checksum
#   -n lets you see what it would do without actually doing it

COMMONPARAMS="-a --delete
              -z -P -vv
"
#              -n
# COMMONPARAMS="$COMMONPARAMS --size-only"
# COMMONPARAMS="$COMMONPARAMS -n"

echo
echo "--- 1/3 --- Backup DBs on tronic -------------------------------------"
echo

# Sometimes these sudo's work automatically.  Maybe for a limited time after authentication by password?

$RSYNC_RSH mike@bristol.indymedia.org sudo su postgres -c /home/mike/bin/dumpdbs.sh

# This is not neccessary:
$RSYNC_RSH mike@bristol.indymedia.org sudo touch /home/mike/db-backups/waiting_for_cron.daily

echo
echo "--- 2/3 --- Get local copy of DBs ------------------------------------"
echo

rsync $COMMONPARAMS mike@bristol.indymedia.org:/home/mike/db-backups .

echo
echo "--- 3/3 --- Get local copy of code and generated pages ---------------"
echo

rsync $COMMONPARAMS mike@bristol.indymedia.org:/www/active-cvs/bristol /www/active-cvs

date > /www/last-got-bristol.at
