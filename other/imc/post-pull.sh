echo
echo "######## Post-pull operations"
echo

echo "Todo:"
echo "Put warning signs on the site if it is not a fully active mirror."
echo '(eg. "please hold your article / post until main site back up")'
echo
# More notes:
# The SQL log on tronic is linked outside the tree.  I couldn't be bothered to download it.  I created a similar directory+file outside the tree on buggy so the link works.

source /root/j/startj simple

echo "Moving bristol_dev/.htaccess to /tmp cos it causes problems."
mv /www/active-cvs/bristol/webcast/.htaccess /tmp
echo

# echo "Pointing towards active_bristoldev instead of active_bristol PG DB."
# sedreplace \
	# '$db_setup\["database"\] = "active_bristol";' \
	# '$db_setup["database"] = "active_bristoldev";' \
	# /www/active-cvs/bristol/local/db-setup.php3
# echo

# sedreplace \
	# 'my $default_usr = "imc_user"' \
	# 'my $default_usr = "root"' \
		# /www/active-cvs/bristol/shared/modules/IMC/Database.pm

echo "Buggy see itself fix"
WC="/www/active-cvs/bristol/webcast"
sedreplace \
	'getenv("HTTP_HOST")' \
	'"localhost:81"' \
		`greplist "refresh=y" \`find $WC -name "*.php3"\``
		# $WC/led-process.php3 $WC/front.php3 $WC/new_data-process.php3
		# `find /www/active-cvs/bristol/webcast/ -name *.php3`
echo

echo "Giving www-data owner privilege on all bristol imc files"
chown www-data /www/active-cvs/bristol/ /www/uploads/bristol/ -R
echo

if test $1; then
	echo "Skipping rest cos argument passed."
	exit 0
fi

sedreplace \
	'connect - bristol' \
	'connect - postgres' \
		/www/db-backups/active_bristol.psql

/www/recreatemydb.sh
su - postgres /www/recreatepgdb.sh

echo "Finally checksumming the result so you can easily see what files you have changed."
cksum `find /www/active-cvs/bristol/ -type f | notindir CVS cache log logs | grep -v '\.b4sr$'` > /www/bristolb4local.cksum

echo "All done."
