cd /stuff/mirrors/

# wget -q -mirror "http://generation-online.org/"
wget -nv -mirror "http://generation-online.org/"

makebackup -efficient generation-online.org /mnt/stig/makebaks
