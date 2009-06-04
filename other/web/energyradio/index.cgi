#!/bin/bash

export JPATH=/home/joey/j
. $JPATH/startj-simple

DB="/home/www-data/amarokradio/collection.db"

. importshfn memo rememo

if ! which sqlite3 >/dev/null
then echo "amarokradio cannot run - sqlite3 is not installed." ; exit 1
fi

# {{{ HTML Pre
cat << !
Content-type: text/html

<html>
<head>
<title>energy radio</title>
<style type="text/css">
a:link {
	text-decoration: none;
	color: #0000cc;
}
a:hover {
	text-decoration: underline;
	color: #0000cc;
}
a:visited {
	text-decoration: none;
	color: #aa00aa;
}
</style>
</head>
<body bgcolor="#ffffff">

<H1>eNerGy rAdIo</H1>

!
# }}}

# {{{ Functions

function sqlite3 () {
	# echo "Doing: sqlite3 $*" | tohtml >&2
	/usr/bin/sqlite3 "$@"
}

function sql_string_escape () {
	## I escaped ' but it failed!
	## TODO: I haven't tested whether " escaping works - we don't have any in the database!
	echo -n "$1" |
	# sed "s+'+\\\\'+g"
	sed 's+"+\\\\"+g'
}

function show_sql_table () {
	## TODO: The 2>&1 is for developer convenience - but may give away information - not safe for production!
	# sqlite3 "$DB" "SELECT $1" 2>&1 |
	# tohtml
	echo "<CENTER>"
	echo "<TABLE>"
	sqlite3 "$DB" "$1" 2>&1 |

	## Strips path to filename:
	afterlast / | beforelast "\.mp3" |

	tohtml |
	sed 's+|+</TD><TD>+g ; s+^+<TR bgcolor="#aacccc"><TD>+ ; s+$+</TD></TR>+'
	# sqlite3 -html "$DB" "$1" 2>&1
	echo "</TABLE>"
	echo "</CENTER>"
}

# }}}

# {{{ Exposed pages (how do we trust this fact - by the author?!):

index () {

	show_sql_table "SELECT * FROM 'Labels' SORTBY"
	# show_sql_table "SELECT * FROM 'tags' SORTBY"
	# show_sql_table "SELECT labels.id,name,tags.url FROM 'labels','tags' WHERE labels.id = "
	# show_sql_table "SELECT statistics.url"
	# show_sql_table "SELECT COUNT(

}

function show_label_cloud () {
	echo "<STYLE type='text/css'> .cloudBit { vertical-align:middle; } </STYLE>"
	echo "<P style='text-align:center; border: 0px solid black;'>"
	echo "Tags:<BR/>"
	# sqlite3 "$DB" "SELECT labels.name FROM labels" | sed 's+$+, +' | tr -d '\n' ; echo
	sqlite3 "$DB" "SELECT labels.name FROM labels" |
	# sqlite3 "$DB" "SELECT labels.name,COUNT(tags_labels.*) FROM labels,tags_labels WHERE labels.id=tags_labels.labelid" |
	# sqlite3 "$DB" "SELECT COUNT(*) as labelid,url FROM tags_labels group by url"
	sqlite3 "$DB" "SELECT COUNT(*) as url,labels.name FROM tags_labels,labels where labels.id=tags_labels.labelid group by labelid" |
	sort -k 2 -t '|' |
	# while read SHOWLABEL
	while IFS="|" read COUNT SHOWLABEL
	do
		if [ "$SHOWLABEL" = "$LABEL" ] && false ## Don't do this if we are memo-ing!
		then echo -n "<B class='cloudBit'>$SHOWLABEL</B>" ## No size/count/etc.?
		else
			SZ=$(echo "60+l(1+$COUNT)*30" | bc -l | beforefirst "\.")
			COL=$(echo "64+l(1+$COUNT)*32" | bc -l | beforefirst "\.")
			[ "$COL" -gt 255 ] && COL="255"
			COL=$((255-COL))
			echo -n "<A class='cloudBit' title='$COUNT' style='font-size:$SZ%; color:rgb($COL,$COL,255);' href='?label=$(tocgi "$SHOWLABEL")'>"
			echo -n "$SHOWLABEL" # | tohtml | sed 's+<BR>$++'
			echo -n "</A>"
		fi
		# echo -n " <FONT size='-2'>$COUNT</FONT>"
		echo -n "&nbsp;&nbsp; "
	done
	echo "</P>"
}

## TODO: The pipes to 'tohtml' are slowing things down.  Maybe we can make a
## faster method.  If it was a fn call which we pass the line to as a parameter,
## it should be must faster (no forking, no parsing sh).
## Interesting to test how much more costly is |fn vs fn "$n"

show_label () {
	LABELNAME="$1"
	# echo "<P align='right'><A href='listen.cgi?label=$LABELNAME'>Listen to $LABELNAME radio</A></P>"
	echo "<H2>Browsing label &quot;$(tohtml "$LABELNAME")&quot;</H2>"
	# show_sql_table "SELECT url FROM tags_labels WHERE labelid=$LABELNAME"
	# show_sql_table "SELECT labels.name,tags_labels.url FROM labels,tags_labels WHERE labels.id = tags_labels.labelid AND labels.name='dub'"
	# show_sql_table "SELECT tags_labels.url FROM tags_labels,labels WHERE labels.name='dub' AND labels.id=tags_labels.labelid"
	# show_sql_table "SELECT tags_labels.url FROM tags_labels,labels WHERE labels.name='wipeout_style' AND labels.id=tags_labels.labelid"
	# show_sql_table "SELECT tags_labels.url FROM tags_labels,labels WHERE labels.name='$LABELNAME' AND labels.id=tags_labels.labelid"
	echo "<CENTER>"
	echo "<TABLE cellpadding='4'>" # width='90%' 
	sqlite3 "$DB" "SELECT statistics.rating,tags_labels.url FROM statistics,tags_labels,labels WHERE labels.name=\"$(sql_string_escape "$LABELNAME")\" AND labels.id=tags_labels.labelid AND statistics.url=tags_labels.url" |
	sort -t '|' -k 1 -n -r |
	while IFS='|' read RATING URL
	do
		# echo "$URL"
		LABELS=$(sqlite3 "$DB" "SELECT labels.name FROM labels,tags_labels WHERE labels.id=tags_labels.labelid AND tags_labels.url=\"$(sql_string_escape "$URL")\"" | sort)
		# echo "$LABELS" | wc -l
		echo "<TR>"
		echo "<TD bgcolor='#aabbcc'>" # width='50%' 
		echo "$URL" | afterlast / | beforelast "\(\.mp3\|\.ogg\)" | tohtml
		echo "</TD>"
		echo "<TD bgcolor='#ccbbaa'>" # width='50%' 
		# echo "$LABELS"
		echo "$LABELS" |
		while read LABEL
		do
			echo -n "<A href='?label=$(tocgi "$LABEL")'>"
			echo -n "$LABEL" | tohtml | sed 's+<BR>$++'
			echo -n "</A>&nbsp; "
		done
		echo "</TD>"
		echo "<TD bgcolor='#ddddaa'>$RATING</TD>"
		echo "</TR>"
	done
	echo "</TABLE>"
	echo "</CENTER>"
}

# }}}


## {{ Main

(

# index "$@"

CGILIB_NO_CONTENT=true
. /usr/lib/cgi-bin/joey/cgilib

LABEL=$(getcgi "label")

## Argh we can't memo this!  It blocks the "currently viewing" link, which changes!
memo -t '10 minutes' show_label_cloud
# memo -t '1 second' show_label_cloud ## For development

# echo "<P>Browse by: artist / tag / date</P>"

# jshinfo "LABEL=$LABEL"
# echo "LABEL = $LABEL" | tohtml
if [ "$LABEL" ]
then show_label "$LABEL"
fi

) ## Developer only: 2>&1

## }}



# {{{ Post-HTML
cat << !

<P align="right"><FONT size="-1">Powered by amarok and energyradio</FONT></P>

</body>
</html>
!
# }}}


