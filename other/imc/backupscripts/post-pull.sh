if test ! $IMC_CONFIG_LOADED
then
	echo "No config loaded.  Please source your imc config file first."
	exit 1
fi

echo
echo "######## Post-pull operations"
echo

## Set to true if you want to dev on this box and want to be able to check your changes
DOBACKUPSB4DEV=

# More notes:
# The SQL log on tronic is linked outside the tree.  I couldn't be bothered to download it.  I created a similar directory+file outside the tree on buggy so the link works.

export CITYPATH="/www/active-cvs/bristol"
export NEARCITY="/www/" ## on same fs please, used temporarily

## This next line was left out but I suspect cron had it.  But check if a lone source works sometime...
export JPATH=/home/joey/j
source /home/joey/j/startj-simple

echo 'Putting "Warning: mirror" notice on the site.'
sedreplace -changes '.*Please refrain' '<font color="#ff8080" size="+2">Please note: You are viewing a mirror / backup of the main site.  <b>Any submissions made here WILL be LOST!</b></font><p><font color="#ffffff">Please refrain' "$CITYPATH/local/include/imcfront-header.inc"
echo

echo "Giving www-data owner privilege on all bristol imc files"
chown www-data:imc $CITYPATH/ /www/uploads/bristol/ -R
echo

if test "$DOBACKUPB4DEV"
then
	echo "Making a backup of the live site before making development changes."
	rm -rf /www/active-cvs/bristol-b4dev
	# Hide heavy directories
	mv $CITYPATH/webcast/logs $NEARCITY/logs.hiding
	mv $CITYPATH/local/webcast/cache $NEARCITY/cache.hiding
	cp -a $CITYPATH /www/active-cvs/bristol-b4dev
	# Restore the heavy directories
	mv $NEARCITY/logs.hiding $CITYPATH/webcast/logs
	mv $NEARCITY/cache.hiding $CITYPATH/local/webcast/cache
fi

echo "Moving bristol_dev/.htaccess to /tmp cos it causes problems."
mv $CITYPATH/webcast/.htaccess /tmp
echo

# echo "Pointing towards active_bristoldev instead of active_bristol PG DB."
# sedreplace -changes \
	# '$db_setup\["database"\] = "active_bristol";' \
	# '$db_setup["database"] = "active_bristoldev";' \
	# $CITYPATH/local/db-setup.php3
# echo

# added imc_user now
# sedreplace -changes \
	# 'my $default_usr = "imc_user"' \
	# 'my $default_usr = "root"' \
		# $CITYPATH/shared/modules/IMC/Database.pm

if test $LOOPBACK_HACK
then
	echo "Refresh http gets: Can't see $HOST but can see $LOOPBACK_HACK fix"
	WC="$CITYPATH/webcast"
	sedreplace -changes \
		'getenv("HTTP_HOST")' \
		"'$LOOPBACK_HACK'" \
			`greplist "refresh=y" \`find $WC -name "*.php3"\``
			# $WC/led-process.php3 $WC/front.php3 $WC/new_data-process.php3
			# `find $CITYPATH/webcast/ -name *.php3`
	# echo
fi

echo "Removing incompatible php"
sedreplace -changes '<!-- <?=\$summary_file?> -->' ' ' "$CITYPATH/webcast/index_imc.php3"
echo

## Patches were on the old buggy  :=(
# echo "Applying other patches:"
# cd $CITYPATH/
# for X in /www/tronic-to-buggy-patches/*
# do patch -p0 < "$X"
# done
# cd /www/
# echo

if test "$DOBACKUPB4DEV"
then
echo "Finally checksumming the result so you can easily see what files you have changed."
cksum `find $CITYPATH/ -type f | notindir CVS cache log logs | grep -v '\.b4sr$'` > /www/bristolb4local.cksum
echo
fi




if test $1
then echo "Skipping DB creation cos argument passed." ; exit 0
fi




echo "Local PGDB does not recognise user bristol, so must connect as postgres."
sedreplace \
	'connect - bristol' \
	'connect - postgres' \
		/www/db-backups/active_bristol.psql
echo

# Re-create the MYSQL DB:
/www/recreatemydb.sh

# Ensure postgres is not busy so that we may destroy the DB:
# Note: "su - root" used in case cron does not have full PATH setup.
su - root /etc/init.d/postgresql stop
sleep 15
su - root /etc/init.d/postgresql start
sleep 15
# Replace the DB:
su - postgres /www/recreatepgdb.sh

echo "All done."
