for LOGFILE in *.bdiff_log
do
	echo -n "$LOGFILE:"
	echo -n " Matches=`grep MATCH_FOUND "$LOGFILE" | wc -l`"
	## Note MATCH_NOT_FOUND was only being printed if #searched was > 1000.
	echo -n " FailedMatches=`grep "\(MATCH_NOT_FOUND\|NO_MATCH_FOUND\)" "$LOGFILE" | wc -l`"
	# echo -n " FailedMatches=`grep MATCH_NOT_FOUND "$LOGFILE" | wc -l`"
	echo -n " UniqueChecksums=`grep "^Checksum.*has.*occurrences.$" "$LOGFILE" | wc -l`"
	echo -n " MostHits=`grep "^Checksum.*has.*occurrences.$" "$LOGFILE" | sort -n -k 4 | tail -n 1 | takecols 2 4`"
	echo
done
