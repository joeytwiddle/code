(

echo "#!/usr/bin/gnuplot"

grep "U = " genres.txt | head -1 | sed "s/U/u/"
grep "V = " genres.txt | head -1 | sed "s/V/v/"
grep "W = " genres.txt | head -1 | sed "s/W/w/"

cat showdesiredspacings.gpl

) > showdesiredspacings.tmp

chmod a+x showdesiredspacings.tmp
./showdesiredspacings.tmp

