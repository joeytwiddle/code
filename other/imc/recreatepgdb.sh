# Note: can't adjust db dump here since not owner of file.

PSQLDB="active_bristol"
BACKUPDIR="/www/db-backups"

echo "######## POSTGRES:"
echo "Destroying $PSQLDB database (doesn't matter if it fails)."
destroydb $PSQLDB
echo "Recreating $PSQLDB database (takes a while!)"
createdb $PSQLDB
# Only needed once per initdb:
# psql -d $PSQLDB -c 'create user php with password fr0gg3r'
psql -d $PSQLDB -f $BACKUPDIR/$PSQLDB.psql > /tmp/db.out 2>&1
echo
