#!/bin/bash
# (we need bash in order to check UID)
# Ensures the local /www/active-cvs/$CITY_NAME is an exact copy of $REMOTE_HOST's /www/active-cvs/$CITY_NAME
# Does the same for /www/uploads/$CITY_NAME
# Also copies $REMOTE_HOST:/home/mike/db-backups into localhost:/www/db-backups
# (The forced backup is disabled, so $REMOTE_HOST should perform dump the DBs shortly before this script is run.)

# There are still some errors caused by mike@tronic not being able to read some
# files on tronic.  Not currently a problem though.  The errors are visible in
# the output of this script (in the form "send_files failed to open "...).
# I think these are zero-length non-read/writeable files, presumably an active error.

if test "$1" = -force
then FORCE=true; shift
fi

if test ! $2
then
	echo "Usage: mirror_imc_site [ -force ] <city_name> <user>@<livehostname>"
	exit 1
fi

CITY_NAME="$1"
REMOTE_HOST="$2"

## Check config:
# CITY_NAME=bristol
# REMOTE_HOST=mike@tronic.southspace.net
if test ! $REMOTE_DBDUMP_PATH
then REMOTE_DBDUMP_PATH=/www/active-cvs/$CITY_NAME/webcast/db-backups
fi

## Check external dependencies:
if test ! -x `which rsync`
then
	echo "Won't work without rsync!  Please install it."
	exit 1
fi

## Check lockfile
if test ! -f /www/get$CITY_NAME-auto && test ! $FORCE; then
	echo "/www/get$CITY_NAME-auto not present, aborting."
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

test "$RSYNC_RSH" || export RSYNC_RSH="ssh-agent ssh"

# rsync options:
# -a means preserve all information (times, permissions) (requires root).
# --delete means delete local files which have been deleted from remote.
# -n means don't copy or delete any files, but show what would be done.
# -vv means verbose (informative) output.  To see errors only, remove -vv.
# -P means show progress, and continue partial downloads.
# --size-only is pretty speedy but skips some checksums.
# -z is for compression of data transfer.  =)

# COMMONPARAMS="-a --delete -z -P -vv"
COMMONPARAMS="-a --delete -z --partial -v --stats"
# COMMONPARAMS="-a --delete -z -P -vv"
# COMMONPARAMS="$COMMONPARAMS --size-only"
# COMMONPARAMS="$COMMONPARAMS -n"

cd /www

## TODO: we really should get the up-to-date DBs, eg. by giving mike@tronic dump privileges

echo
echo "--- 1/4 --- Backup DBs on tronic -------------------------------------"
echo

# This sudo can be done manually, although the password appears onscreen!
# Tronic's does work automatically for a few minutes after a successful sudo authentication.
echo "Skipping: cannot sudo without user passwd, but cron@tronic should have dumped the DBs recently anyway."
# $RSYNC_RSH $REMOTE_HOST sudo su postgres -c /home/mike/bin/dumpdbs.sh

echo
echo "--- 2/4 --- Get local copy of DBs ------------------------------------"
echo

mkdir -p /www/db-backups/$CITY_NAME
rsync $COMMONPARAMS $REMOTE_HOST:$REMOTE_DBDUMP_PATH/ /www/db-backups/$CITY_NAME

echo
echo "--- 3/4 --- Get local copy of code and generated pages ---------------"
echo

rsync $COMMONPARAMS $REMOTE_HOST:/www/active-cvs/$CITY_NAME /www/active-cvs

echo
echo "--- 4/4 --- Get local copy of site's uploads ---------------"
echo

rsync $COMMONPARAMS $REMOTE_HOST:/www/uploads/$CITY_NAME /www/uploads

date > /www/last-got-$CITY_NAME.at

# no longer: /www/scripts/post-pull.sh
