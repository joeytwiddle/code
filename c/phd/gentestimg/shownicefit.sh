# killall ghostview

rm *.eps

# DOC="simgs/dar-crpage2.bmp"
# DOC="simgs/left.bmp"
DOC="simgs/test.bmp"

# +ve pitch requires smaller size!
COM="./simgen
-yoff 0.01 -depth 0.5
-lines 11 -roll 0 -yaw 0 -pitch -30 -size 1.2
-image -qnd -overlay
$DOC tmp.bmp
-nornd
-gnuplot
-showexp
$@";
# -imgnoise 0.0
# -noise 0.2
# -ins 1
# -rems 1
# -focal 0.1

getparam () {
	grep "^$1 = " simgen.out | sed "s/^$1 = //"
}

# for X in "" "-spacings"; do
# for Y in "" "-noransac"; do
# for F in `seq -w 0.5 0.3 4.0`; do
for X in "-spacings"; do
for Y in ""; do
for F in 0.5; do

	echo "set title \"$X $Y, f.p. $F\" \"Times-Roman,26\"" > title.dogpl

	echo `curseyellow`$COM`cursegrey`
	$COM $X $Y -focal $F > simgen.out

	U=`getparam U`
	V=`getparam V`
	W=`getparam W`
	echo "u=$U"
	echo "v=$V"
	echo "w=$W"
	cp gplfit.ps gplfit$X$Y$F.eps

	echo "Press a key"
	waitforkeypress

	# Test pp
	invert -i tmp.bmp -o tmp-inv.bmp
	../projprof/pp $X $Y -res 180 -adapt 0.03 -maxpixels 2000 -deriv -gamma 0.0 -recwid 200 -light -oth 12 -badx 35 -bady 21 -dolowresscan -lowres 60 -lowsmooth 1 tmp-inv.bmp tmp.bmp

	# gv gplfit$X$Y.eps
done
done
done

convert -loop 0 -delay 100 -geom 400 gplfit*.eps anim.gif
