for X in `listnums 0 90 by 1`; do
	echo "set view $X,$X,1,1"
	echo "replot"
done

