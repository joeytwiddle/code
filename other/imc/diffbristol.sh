export JPATH=/home/joey/j
source /home/joey/j/startj simple

export DIFFCOM="diff"

echo
remotediff /www/active-cvs/bristol/ mike@bristol.indymedia.org:/www/active-cvs/bristol/
echo
remotediff /www/db-backups/ mike@bristol.indymedia.org:/home/mike/db-backups/
echo
