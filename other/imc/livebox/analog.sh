#!/bin/sh
#
# Script to create run analog and make output available from the web.
#

LOGDIR="/www/bristol/logs"
PAGE="/home/bristol/imc_bristol/webcast/wwwlogs4us.html"

cd /home/mike/bin

date > analog-started
 
analog +ganalog.cfg > "$PAGE" 2> analog.error

printf "" > /www/bristol/logs/access.log

## Keep a record of analog's output
NICEDATE=`date +"%Y%M%d"`
cp "$PAGE" "$LOGDIR"/wwwlog-"$NICEDATE".html
gzip -f "$LOGDIR"/wwwlog-"$NICEDATE".html

date > analog-ended
 
