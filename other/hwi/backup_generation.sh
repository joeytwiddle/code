cd /stuff/mirrors/

# wget -q -mirror "http://generation-online.org/"
wget -nv -mirror "http://generation-online.org/"

[ -d generation-online.org/analog-images ] &&
mv generation-online.org/analog-images ./tmp-analog-images

makebackup -efficient generation-online.org /mnt/stig/makebaks

[ -d ./tmp-analog-images ] &&
mv ./tmp-analog-images generation-online.org/analog-images


###############

makebackup -efficient /stuff/install/TWiki /mnt/stig/makebaks
