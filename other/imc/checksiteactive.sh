#!/bin/sh
# checksiteactive.sh
# Informs maintainer if a site is inactive/broken.
# Tries to download the front page and then check it contains a key string.

SITE="bristol.indymedia.org"
KEYSTRING="B R I S T O L indymedia"
MAILONERROR="pclark@cs.bris.ac.uk"

LOGFILE="/tmp/checksite.log"
DLFILE="/tmp/$SITE/index.html"
cd /tmp

# Download copy of the front page

(
	echo
	echo "----- wgetting $SITE"
	echo
	rm -rf "$DLFILE"
	wget -x "http://$SITE/"
) > "$LOGFILE" 2>&1

# Check it contains the key string

(
	echo
	echo "----- grepping for \"$KEYSTRING\""
	echo
) >> "$LOGFILE" 2>&1
export RESULT=`grep "$KEYSTRING" "$DLFILE"` >> "$LOGFILE" 2>&1

if test ! "$RESULT" = ""; then

	printf "" # Printing nothing prevents cron report
	# echo "Site $SITE appears OK. =)"

else

	echo "Problems: mailing error report to $MAILONERROR ..."

	(

		echo
		echo "Hi this is $0 running as $USER on $HOSTNAME (aka $HOST)."
		echo
		echo "I tried to download http://$SITE/ to check the active site is delivering the front page OK."
		echo "But either it didn't download, or it doesn't contain the key string \"$KEYSTRING\""
		echo
		echo "-------------------- Here is the log $LOGFILE:"
		cat "$LOGFILE"
		echo
		echo "-------------------- Here is the file I retrieved: $DLFILE"
		cat "$DLFILE"
		echo

	) 2>&1 | mail -s "Error retrieving $SITE" "$MAILONERROR"

fi
