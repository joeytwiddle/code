source /home/joey/j/startj simple
cksum `find /www/active-cvs/bristol/ -type f | notindir CVS cache log logs | grep -v '\.b4sr$'` > /www/bristolnow.cksum
jfcsh -bothways /www/bristol*.cksum
