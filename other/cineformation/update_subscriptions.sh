MAILTO=joey@hwi.ath.cx

TAB="	"
WEIRD_CHAR=" "
NL="\\
"

EXCEL_SHEEL_TAB_DELIMETED="/tmp/Contacts Database.txt"
MAILMAN_LIST=/tmp/from_mailman
EXCEL_LIST=/tmp/from_table
NEW_FROM_EXCEL=/tmp/new_from_excel
IN_MAILMAN_BUT_NOT_IN_DB=/tmp/in_mailman_but_not_in_db
SIDE_BY_SIDE_DIFF=/tmp/side_by_side_diff
REPORT=/tmp/cineformation_report

## TODO: fromline is not excluding Content-Disposition line as it should.

fromline () {
	if [ "$1" = -x ]
	then
		shift
		PAT="$1"
		awk ' BEGIN { X=0 } { if ( X ) { print $0'\n' } } /'"$PAT"'/ { X=1 } '
	else
		PAT="$1"
		awk ' BEGIN { X=0 } /'"$PAT"'/ { X=1 } { if ( X ) { print $0'\n' } } '
	fi
}

toline () {
	if [ "$1" = -x ]
	then
		shift
		PAT="$1"
		awk ' /'"$PAT"'/ { exit } { print $0'\n' } '
	else
		PAT="$1"
		awk ' { print $0'\n' } /'"$PAT"'/ { exit } '
	fi
}

grabAttachmentNamed () {
	NAME="$1"
	TMPFILE=/tmp/cine_update.tmp
	cat > $TMPFILE
	if ! cat $TMPFILE | grep "^Content-Disposition: attachment; filename=\"$NAME\"$"
	then
		echo "Sorry cannot process.  Could not find any attachment named \"$NAME\" in your email."
		exit 1
	fi
	cat $TMPFILE |
	fromline -x "^Content-Disposition: attachment; filename=\"$NAME\"$" |
	toline   -x "^--=====================.*==_--$" |
	# tr -d '\n' |
	sed "s+=A0+$WEIRD_CHAR+g" |
	sed "s+=09+$TAB+g" |
	# tr '\n' '=' | sed "s+==++g" | tr "=" '\n' |
	sed 's+=$+<JOINLINEHERE>+' | tr '\n' '=' | sed 's+<JOINLINEHERE>=++g' | tr '=' '\n' |
	# tr -d '=' |
	cat
}

# grabAttachmentNamed "Contacts Database.txt" > "$EXCEL_SHEEL_TAB_DELIMETED" || exit 1
grabAttachmentNamed ".*" > "$EXCEL_SHEEL_TAB_DELIMETED" || exit 1

/usr/lib/mailman/bin/list_members cineformation |
grep "@" |
sort -f > $MAILMAN_LIST

cat "$EXCEL_SHEEL_TAB_DELIMETED" |
cut -d "$TAB" -f 3 |
grep "@" |
sort -f > $EXCEL_LIST

diff -i $MAILMAN_LIST $EXCEL_LIST |
grep "^> " | sed 's+^..++' |
cat > $NEW_FROM_EXCEL

diff -i $EXCEL_LIST $MAILMAN_LIST |
grep "^> " | sed 's+^..++' |
cat > $IN_MAILMAN_BUT_NOT_IN_DB

# env COLUMNS=80 diff -i --side-by-side $MAILMAN_LIST $EXCEL_LIST |
diff -i --width=80 --side-by-side --expand-tabs $MAILMAN_LIST $EXCEL_LIST |
cat > $SIDE_BY_SIDE_DIFF

(

	echo "Hwi's report on recommended Mailman subscription updates."
	echo
	echo "Note: Hwi does not feel confident to make any Mailman changes herself."
	echo "      For the moment, you must make changes manually, but this may help..."
	echo
	echo "Also note: You need to maximise your window and use a fixed-width font to"
	echo "           view Part 3.  Copy-paste to Notepad if neccessary."
	echo "    Alternatively, visit http://hwi.ath.cx/cineformation-updates.html"
	echo "    for a fixed-width version."
	echo
	echo
	echo
	echo "-- Part 1 ------------------------------------------------------------"
	echo
	echo "  The following addresses are in the Excel sheet but not yet in Mailman."
	echo
	echo "  If all seems well, you may wish to visit"
	echo
	echo "      http://hwi.ath.cx/cgi-bin/mailman/admin/cineformation/members"
	echo
	echo "  and copy-paste to mass subscribe them."
	echo
	cat $NEW_FROM_EXCEL
	echo
	echo
	echo
	echo "-- Part 2 ------------------------------------------------------------"
	echo
	echo "  The following addresses are in Mailman but not in the Excel sheet!"
	echo
	echo "  Should they be added to the Excel sheet or should they be removed from Mailman?"
	echo
	cat $IN_MAILMAN_BUT_NOT_IN_DB
	echo
	echo
	echo
	echo "-- Part 3 ------------------------------------------------------------"
	echo
	echo "  Finally, for your convenience and comparison, the two lists side-by-side."
	echo "  On the left: the members of the Mailman list.  On the right: emails from Excel."
	echo "  '<' means only in Mailman, '>' means only in Excel, '|' means different."
	echo
	# echo "<TT>"
	cat $SIDE_BY_SIDE_DIFF
	# echo "</TT>"
	echo
) > $REPORT

cat $REPORT | mail "$MAILTO" -s "Hwi's cineformation Mailman update report"

(
	echo "<HTML><BODY><FONT size='-3'><TT>"
	cat $REPORT |
	sed 's+$+<BR>+ ; s+ +\&nbsp\;+g'
	echo "</TT></FONT></BODY></HTML>"
) > /var/www/cineformation-updates.html
