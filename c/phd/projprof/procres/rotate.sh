(

echo "#!/usr/bin/gnuplot"

echo "$*"

for X in `listnums 3 87 by 3`; do
  echo "set view $X,$X,1,1"
  echo "replot"
done

for X in `listnums 90 0 by -3`; do
  echo "set view $X,$X,1,1"
  echo "replot"
done

) > dorotate.gpl

chmod a+x dorotate.gpl

./dorotate.gpl
