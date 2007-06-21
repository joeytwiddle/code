# LOGFILE=`ls -rt ./ut-server/Logs/Unreal.ngLog.*.tmp | tail -n 1`
## Check the files last-modified date to see when server was last active
# if [ "$1" = -f ]
# then tail -n 2000 -f "$LOGFILE"
# else cat "$LOGFILE"
# fi |
# head -200 |
# strings |

# cat ./ut-server/Logs/Unreal.ngLog.*.log |
# striptermchars |
# cat

GOOD="
^ChatLog (
^Level is
"
GOODRE=`echo "$GOOD" | grep -v "^$" | list2regexp`

# cd ./ut-server/Logs
# higrep "$GOODRE" ucc.init.log*

jzcat ./ut-server/Logs/ucc.init.log* |
# grep "ChatLog ("
grep "$GOODRE" |
grep -v "): nj " |

## TODO: Catch mutators from Browse line or summat

while read WHAT TWO THREE REST
do
	case "$WHAT" in
		Level)
			LEVEL="$REST"
		;;
		ChatLog)
			echo "$LEVEL $TWO $THREE $REST"
		;;
	esac
done
