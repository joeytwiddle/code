(

CITY_NAME=bristol

# DESTDIR="/home/mike/db-backups"
DESTDIR="/www/active-cvs/$CITY_NAME/webcast/db-backups"

## If it's a symlink, remove it.
test -h "$DESTDIR" && rm -f "$DESTDIR"
mkdir -p "$DESTDIR"

PSQLDB="active_"$CITY_NAME
MYSQLDB="imc"
MYSQLUSER="imc_backup"
MYSQLPW="imc_backup"

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
/usr/local/pgsql/bin/pg_dump -d "$PSQLDB" > "$PSQLDEST"
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

) | tee /home/mike/db-backups/err.txt |
    mail pclark@cs.bris.ac.uk
