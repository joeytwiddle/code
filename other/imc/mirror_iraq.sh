export RSYNC_RSH="ssh-agent ssh -p 2222"
/www/scripts/mirror_imc_site.sh iraq joey@iraqimc.dyndns.org

# export LOOPBACKHACK=
export MYSQLDESTDB="imc_iraq" ## Likely we are mirroring on a machine which already has a backup.
/www/scripts/post-pull.sh iraq
