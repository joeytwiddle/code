MYSQLDB="imc"
BACKUPDIR="/www/db-backups"

echo "######## MYSQL:"
echo "Destroying and creating fresh $MYSQLDB database"
mysql << !
drop database imc;
!
mysql << !
create database imc;
!
echo "Repopulating $MYSQLDB database"
mysql imc < "$BACKUPDIR/$MYSQLDB.mysql"
mysql imc << !
grant SELECT,INSERT,UPDATE,DELETE,CREATE,DROP on * to imc_user@localhost IDENTIFIED BY 'lkjc925r42';
INSERT INTO IMC_SITE_INFO  (NAME, NUMBER_TO_LIST) VALUES( 'bristol', 5 );
!
echo
