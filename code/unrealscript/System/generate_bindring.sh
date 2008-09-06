find . -name "*.bindring" |
while read RINGDATAFILE
do
	KEY=`echo "$RINGDATAFILE" | beforelast "\.bindring" | afterlast /`
	NUM=1
	# verbosely del "$KEY""Bind"*.exec
	MAX=` cat "$RINGDATAFILE" | grep -v "^;" | countlines `
	jshinfo "Generating $KEY""Bind[1-$MAX].exec ..."
	cat "$RINGDATAFILE" |
	grep -v "^;" | ## allow comments in bindring file
	sed 's+\\+\\\\+g' | ## double-escape \s so that they are not lost by shell =/
	while read LINE
	do
		OFILE="./$KEY""Bind$NUM.exec"
		# jshinfo "Generating $OFILE"
		NUM=$((NUM+1))
		NEWNUM="$NUM"
		[ "$NUM" -lt "$MAX" ] || NEWNUM=1
		echo "$LINE" > "$OFILE"
		echo "set input $KEY exec $KEY""Bind$NEWNUM.exec" >> "$OFILE"
	done

	## Check for the bind in User.ini:
	jshinfo "Checking for bind in User.ini:"
	cat User.ini | grep -i "^`toregexp "$KEY"`=.*"
	if cat User.ini | grep -i "`toregexp "$KEY"`=.*\<exec $KEY""Bind[0-9][0-9]*\.exec\>" >/dev/null
	then jshinfo "OK"
	else jshwarn "Missing line in User.ini: $KEY=exec $KEY""Bind1.exec"
	fi

done

