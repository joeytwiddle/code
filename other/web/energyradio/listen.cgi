#!/bin/bash

ALWAYS_INSERT_ID3TAG=true

export JPATH=/home/joey/j
. $JPATH/startj-simple

DB="/home/www-data/amarokradio/collection.db"

# . importshfn memo rememo

if ! which sqlite3 >/dev/null
then echo "amarokradio cannot run - sqlite3 is not installed." ; exit 1
fi

function sql_string_escape () {
	## I escaped ' but it failed!
	## TODO: I haven't tested whether " escaping works - we don't have any in the database!
	echo -n "$1" |
	# sed "s+'+\\\\'+g"
	sed 's+"+\\\\"+g'
}

echo "Content-type: audio/mpeg"
echo

CGILIB_NO_CONTENT=true
. /usr/lib/cgi-bin/joey/cgilib

LABEL=$(getcgi "label")

pull_bit () {
	if echo "$TITLE" | grep -E " - " >/dev/null
	then
		BIT="`echo "$TITLE" | beforefirst " - "`"
		TITLE="`echo "$TITLE" | afterfirst " - "`"
		return 0
	else
		return 1
	fi
}

sqlite3 "$DB" "SELECT statistics.rating,tags_labels.url FROM statistics,tags_labels,labels WHERE labels.name=\"$(sql_string_escape "$LABEL")\" AND labels.id=tags_labels.labelid AND statistics.url=tags_labels.url" |
# sort -t '|' -k 1 -n -r |
randomorder | head -n 5 |
while IFS='|' read RATING URL
do
	FILE="/$URL"
	if [ ! -f "$FILE" ] || [ ! -r "$FILE" ]
	then
		jshwarn "Not available: $FILE"
		continue
	fi
	jshwarn "Sending $FILE (rating $RATING) ..."

	if [ "$ALWAYS_INSERT_ID3TAG" ]
	then
		TITLE="`echo "$URL" | afterlast / | beforelast "\.\(mp3\|ogg\)"` ($RATING)"
		cat "$FILE" > /tmp/next.mp3
		FILE="/tmp/next.mp3"

		## Seems mp3info inserts only at the end of the file =/
		# mp3info -d "$FILE"
		# mp3info -t "$TITLE" "$FILE"

		# id3v2 -D "$FILE" >/dev/null
		if id3v2 -l "$FILE" | grep "No ID3 tag$" >/dev/null
		then
			# id3v2 -t "$TITLE" "$FILE" >/dev/null
			## Attempt to split artist/album/title by the -s
			pull_bit && id3v2 -a "$BIT" "$FILE" >/dev/null
			pull_bit && id3v2 -A "$BIT" "$FILE" >/dev/null
			id3v2 -t "$TITLE" "$FILE" >/dev/null
		else
			id3v2 -C "$FILE" >/dev/null
			## Argh i can see the data at the front of the stream, but xmms does not update when song switches!
		fi

	fi

	if cat "$FILE"
	then :
	else
		jshinfo "Cat responded: $? - closing session."
		## cat produces "write error: Broken pipe" if the client dropped the connection
		## but the error code 1 is the same for this as for "no such file" :P
		break
	fi
done

