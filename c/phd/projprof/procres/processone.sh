PLOT=
if test "$1" = "-plot"; then
	PLOT=true
	shift
fi

WRITEDATA=
if test "$1" = "-data"; then
	WRITEDATA=-data
	shift
fi

WRITETOTALS=
if test "$1" = "-totals"; then
	WRITETOTALS=true
	shift
fi

X="$1"

TMP=`jgettmp $$`

./extracterror "$X" $WRITEDATA 2> "$TMP"

if test $WRITETOTALS; then
	cat "$TMP" | grep "ave = " > "$X.totals"
fi

sleep 1 && jdeltmp "$TMP"

if test $PLOT; then

gnuplot << !
# cat << !

# splot [0:90] [0:90] [0:1] 'errgrid.dat' using 1:2:4
# set data style points
# set data style lines
# set contour surface
# set view 70,340,1,1
# set view 70,135,1,1

# The ones to use are:
set view 45,45,1,1
set surface
set noclabel
set nokey
# set title "Accuracy of recovery of HVP" "Times-Roman,28"
# set zlabel "Accuracy of recovery of VVP" "Times-Roman,28"
set xlabel "yaw" "Times-Roman,28"
set ylabel "pitch" "Times-Roman,28"
# set term post "landscape,times"
set term post eps "Times-Roman,18"
# set output "fullbasedata.eps"
# splot [5:85] [5:85] [-5:0] "fullbasedata.txt" using 1:2:28 w l
# set output "fulltestdata.eps"
# splot [5:85] [5:85] [-5:0] "fulltestdata.txt" using 1:2:28 w l
# set output "morebasedata.eps"
# splot [5:85] [5:85] [-5:0] "morebasedata.txt" using 1:2:28 w l
# set output "moretestdata.eps"
# splot [5:85] [5:85] [-5:0] "moretestdata.txt" using 1:2:28 w l

# set title "Accuracy of recovery of HVP" "Times-Roman,28"
set zlabel "HVP\naccuracy" "Times-Roman,28"
set output "$X-hvp.eps"
splot [5:85] [5:85] [-5:0] "$X.data" using 1:2:27 w l

# set title "Accuracy of recovery of VVP" "Times-Roman,28"
set zlabel "VVP\naccuracy" "Times-Roman,28"
set output "$X-vvp.eps"
splot [5:85] [5:85] [-5:0] "$X.data" using 1:2:28 w l

!

fi
