ERROUT=errgrid.dat

echo > $ERROUT

echo "# 1 yaw   2 pitch   3 maxang   4 error   5 righterror   6 downerror" > $ERROUT
for roll in 90; do
	for yaw in `listnums 5 80 by 5`; do
		for pitch in `listnums 5 80 by 5`; do
			line=`grep "^$yaw.000000 $pitch.000000" data.txt`
			acc=`echo $line | takecols 18`
			rerr=`echo $line | takecols 4`
			derr=`echo $line | takecols 5`
			maxang=`echo $line | takecols 20`
			# This is no longer cheating
			# but it lets us see the values of the real errors
			if test "$acc" = "" -o "$acc" = " "; then
				acc="-1"
			fi
			if test "$rerr" = "" -o "$rerr" = " "; then
				rerr="-1"
			fi
			if test "$derr" = "" -o "$derr" = " "; then
				derr="-1"
			fi
			echo "$yaw $pitch $maxang $acc $rerr $derr" | sed "s/nan/-1.2/" >> $ERROUT
		done
		echo >> $ERROUT
	done
done
