# Ensures the local /www/active-cvs/bristol is an exact copy of tronic:/www/active-cvs/bristol
# With the -n argument below, rsync will only show what it would do without actually doing it.
export RSYNC_RSH=`which ssh`
rsync -n -a --delete -vv -P mike@bristol.indymedia.org:/www/active-cvs/bristol /www/active-cvs
date > /www/last-got-bristol.at
