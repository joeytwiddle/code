(

if test ! $1
then
	echo "Usage: dumpdbs <city_name>"
	exit 1
fi

CITY_NAME="$1"

# DESTDIR="/home/mike/db-backups"
DESTDIR="/www/active-cvs/$CITY_NAME/webcast/db-backups"
# DESTDIR="/www/db-backups"

## If it's a symlink, remove it.
test -h "$DESTDIR" && rm -f "$DESTDIR"
mkdir -p "$DESTDIR"

PSQLDB="active_"$CITY_NAME
MYSQLDB="imc"
# MYSQLUSER="imc_backup"
# MYSQLPW="imc_backup"
MYSQLUSER="imc_user"
MYSQLPW="lkjc925r42"

PSQLDEST="$DESTDIR/$PSQLDB.psql"
MYSQLDEST="$DESTDIR/$MYSQLDB.mysql"

date
echo "Doing backups..."
echo
echo "Directory before:"
ls -lrtF "$DESTDIR/"

echo
echo "Postgresql before and after:"
cksum "$PSQLDEST"
su - postgres pg_dump -d "$PSQLDB" > "$PSQLDEST"
# Tronic's postgres was too old for this:
# /usr/local/pgsql/bin/pg_dump -F t "$PSQLDB" > "$PSQLDEST"
cksum "$PSQLDEST"

echo
echo "Mysql before and after:"
cksum "$MYSQLDEST"
mysqldump -u "$MYSQLUSER" -p$MYSQLPW "$MYSQLDB" > "$MYSQLDEST"
cksum "$MYSQLDEST"

echo
echo "Directory after:"
ls -lrtF "$DESTDIR/"

# gzipping does not help rsync!
# gzip -f "$PSQLDEST" "$MYSQLDEST"

) | tee /tmp/dumpdbs.err |
    mail cron@hwi.ath.cx
