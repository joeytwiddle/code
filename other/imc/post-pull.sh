if test ! $1
then
	echo "Usage: post-pull <imc_site_name>"
	exit 1
fi

export CITY_NAME="$1"
shift

export CITYPATH="/www/active-cvs/$CITY_NAME"
export NEARCITY="/www/" ## on same fs please, used temporarily

## Note sedreplace is currently non-reversible, but that could change ...
test -x "$JSH" || JSH=/home/joey/j/jsh
# JSH=sh
# export JPATH=/home/joey/j
# . $JPATH/startj

## Set to true if you want to dev on this box and want to be able to check your changes
DOBACKUPSB4DEV=



# TODO: Check if this is still an issue, and if so resolve it: The SQL log on tronic is linked outside the tree.  I couldn't be bothered to download it.  I created a similar directory+file outside the tree on buggy so the link works.



echo "Giving www-data owner privilege on all $CITY_NAME imc files"
chown www-data:imc $CITYPATH/ /www/uploads/$CITY_NAME/ -R
echo

if test "$DOBACKUPB4DEV"
then
	echo "Making a backup of /www/active-cvs/$CITY_NAME before running post-pull scripts."
	rm -rf /www/active-cvs/$CITY_NAME-b4dev
	# Hide heavy directories
	mv $CITYPATH/webcast/logs $NEARCITY/logs.hiding
	mv $CITYPATH/local/webcast/cache $NEARCITY/cache.hiding
	# Make the backup
	cp -a $CITYPATH /www/active-cvs/$CITY_NAME-b4dev
	# Restore the heavy directories
	mv $NEARCITY/logs.hiding $CITYPATH/webcast/logs
	mv $NEARCITY/cache.hiding $CITYPATH/local/webcast/cache
fi



echo
echo "######## Post-pull operations"
echo

echo 'Putting "Warning: mirror" notice on the site.'
$JSH sedreplace -changes '.*Please refrain' '<font color="#ff8080" size="+2">Please note: You are viewing a mirror / backup of the main site.  <b>Any submissions made here WILL be LOST!</b></font><p><font color="#ffffff">Please refrain' "$CITYPATH/local/include/imcfront-header.inc"
echo

echo "Moving $CITY_NAME/.htaccess to /tmp cos it causes problems."
mv $CITYPATH/webcast/.htaccess /tmp
echo

echo "Removing incompatible php"
$JSH sedreplace -changes '<!-- <?=\$summary_file?> -->' ' ' "$CITYPATH/webcast/index_imc.php3"
echo

if test $LOOPBACK_HACK
then
	echo "Refresh http gets: Can't see $HOST but can see $LOOPBACK_HACK fix"
	WC="$CITYPATH/webcast"
	$JSH sedreplace -changes \
		'getenv("HTTP_HOST")' \
		"'$LOOPBACK_HACK'" \
			`greplist "refresh=y" \`find $WC -name "*.php3"\``
			# $WC/led-process.php3 $WC/front.php3 $WC/new_data-process.php3
			# `find $CITYPATH/webcast/ -name *.php3`
	# echo
fi

## TODO: implement this for MYSQL too and make it an option

# echo "Point PG DB towards active_bristoldev instead of active_bristol."
# $JSH sedreplace -changes \
	# '$db_setup\["database"\] = "active_bristol";' \
	# '$db_setup["database"] = "active_bristoldev";' \
	# $CITYPATH/local/db-setup.php3
# echo

# added imc_user now
# $JSH sedreplace -changes \
	# 'my $default_usr = "imc_user"' \
	# 'my $default_usr = "root"' \
		# $CITYPATH/shared/modules/IMC/Database.pm

## TODO: why not grep Apache conf to check a suitable virtualhost exists for this site otherwise inform user.



if test "$DOBACKUPB4DEV"
then
echo "Finally checksumming the result so you can easily see what files you have changed."
cksum `find $CITYPATH/ -type f | $JSH notindir CVS cache log logs | grep -v '\.b4sr$'` > /www/"$CITY_NAME"b4local.cksum
echo
fi



if test $1
then echo "Skipping DB creation cos argument passed." ; exit 0
else

	echo "Preparing to replace local DBs."
	## Local PGDB does not recognise user bristol, so must connect as postgres.
	$JSH sedreplace \
		'connect - bristol' \
		'connect - postgres' \
			/www/db-backups/$CITY_NAME/active_$CITY_NAME.psql
	echo

	# Re-create the MYSQL DB:
	/www/scripts/recreatemydb.sh

	# Ensure postgres is not busy so that we may destroy the DB:
	# Note: "su - root" used in case cron does not have full PATH setup.
	su root /etc/init.d/postgresql stop
	sleep 15
	su root /etc/init.d/postgresql start
	sleep 15
	# Replace the DB:
	su postgres env CITY_NAME=$CITY_NAME /www/scripts/recreatepgdb.sh

fi



echo "All done."
