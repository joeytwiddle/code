export REMOTE_DBDUMP_PATH=/home/mike/db-backups
/www/scripts/mirror_imc_site.sh bristol mike@tronic.southspace.net

# export MYSQLDESTDB="imc_bristol"
# export LOOPBACKHACK=buggyloopbackhack
/www/scripts/post-pull.sh bristol
