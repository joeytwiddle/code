HEAD="$1"
# N=2
for X in `'ls' $HEAD*.totals | sed "s/$HEAD//;s/\(.*\)\..*/\1/;s/[^0123456789\.].*//" | sort -n`; do
	N="$X"
	# echo "grep hvpreldistave" "$HEAD$X.totals"
	# grep hvpreldistave "$HEAD$X.totals"
	HVPERR=-`grep "hvpreldistave = " "$HEAD$X.totals" | sed "s/.*= //;s/ .*//"`
	# echo "$HVPERR"
	echo "$N	$HVPERR" >&3
	VVPERR=-`grep "vvpreldistave = " "$HEAD$X.totals" | sed "s/.*= //;s/ .*//"`
	echo "$N	$VVPERR" >&2
	# N=`expr $N + 1`
done 3> "$HEAD-hvpchange.data" 2> "$HEAD-vvpchange.data"

gnuplot << !

set data style lines
set noclabel
set nokey
set xlabel "lines" "Times-Roman,28"
# set title "Plot of HVP accuracy against $HEAD"
set term post eps "Times-Roman,18"

set ylabel "HVP accuracy" "Times-Roman,28"
set output "$HEAD-hvpchange.eps"
plot "$HEAD-hvpchange.data"

set ylabel "VVP accuracy" "Times-Roman,28"
set output "$HEAD-vvpchange.eps"
plot "$HEAD-vvpchange.data"

!

