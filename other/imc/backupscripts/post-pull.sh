echo
echo "######## Post-pull operations"
echo

echo "Todo:"
echo "Put warning signs on the site if it is not a fully active mirror."
echo '(eg. "please hold your article / post until main site back up")'
echo
# More notes:
# The SQL log on tronic is linked outside the tree.  I couldn't be bothered to download it.  I created a similar directory+file outside the tree on buggy so the link works.

export CITYPATH="/www/active-cvs/bristol"

source /root/j/startj simple

echo "Giving www-data owner privilege on all bristol imc files"
chown www-data:imc $CITYPATH/ /www/uploads/bristol/ -R
echo

echo "Making a backup of the live site before making development changes."
rm -rf /www/active-cvs/bristol-b4dev
# Hide heavy directories
mv $CITYPATH/webcast/logs /tmp/logs.temp
mv $CITYPATH/local/webcast/cache /tmp/cache.temp
cp -a $CITYPATH /www/active-cvs/bristol-b4dev
# Restore the heavy directories
mv /tmp/logs.temp $CITYPATH/webcast/logs
mv /tmp/cache.temp $CITYPATH/local/webcast/cache

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

echo "Buggy see itself fix"
WC="$CITYPATH/webcast"
sedreplace -changes \
	'getenv("HTTP_HOST")' \
	'"localhost:81"' \
		`greplist "refresh=y" \`find $WC -name "*.php3"\``
		# $WC/led-process.php3 $WC/front.php3 $WC/new_data-process.php3
		# `find $CITYPATH/webcast/ -name *.php3`
echo

cd $CITYPATH/
echo "Applying other patches:"
for X in /www/tronic-to-buggy-patches/*; do
	patch -p0 < "$X"
done
cd /www/
echo

echo "Finally checksumming the result so you can easily see what files you have changed."
cksum `find $CITYPATH/ -type f | notindir CVS cache log logs | grep -v '\.b4sr$'` > /www/bristolb4local.cksum
echo




if test $1; then
	echo "Skipping rest cos argument passed."
	exit 0
fi




echo "Local PGDB does not recognise user bristol, so must connect as postgres."
sedreplace \
	'connect - bristol' \
	'connect - postgres' \
		/www/db-backups/active_bristol.psql
echo

/www/recreatemydb.sh
# Ensure postgres is not busy so that we may destroy the DB
/etc/init.d/postgresql stop
sleep 15 # 5 was not long enough!
/etc/init.d/postgresql start
sleep 15
su - postgres /www/recreatepgdb.sh

echo "All done."
