# killall ghostview

rm *.eps

COM="./simgen
-yoff 0.01 -depth 0.5
-lines 14 -roll 0 -yaw 0 -pitch 30 -size 0.4
-qnd -image -overlay
simgs/dar-crpage2.bmp tmp.bmp
-imgnoise 0.0
-noise 0.2
-gnuplot
-showexp
-nornd
$@";
# -ins 1
# -rems 1
# -focal 0.1

getparam () {
	grep "^$1 = " simgen.out | sed "s/^$1 = //"
}

for F in `seq -w 0.5 0.3 4.0`; do
# for X in "" "-spacings"; do
# for Y in "" "-noransac"; do
for X in ""; do
for Y in ""; do
	echo `curseyellow`$COM`cursegrey`
	echo "set title \"$X $Y, f.p. $F\" \"Times-Roman,26\"" > title.dogpl
	$COM $X $Y -focal $F > simgen.out
	U=`getparam U`
	V=`getparam U`
	W=`getparam U`
	echo "u=$U"
	echo "v=$V"
	echo "w=$W"
	cp gplfit.ps gplfit$X$Y$F.eps
	# gv gplfit$X$Y.eps
done
done
done

convert -loop 0 -delay 100 -geom 400 gplfit*.eps anim.gif
