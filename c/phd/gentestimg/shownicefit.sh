#!/bin/bash
#just for the *.eps at the end!

# killall ghostview

rm fit.log gplfit-*.eps

RND=$RANDOM && test "$RND" || RND=`getrandom` || RND=20
# RND=16632
# RND=13771
RND=16293

# DOC="simgs/dar-crpage2.bmp"
# DOC="simgs/left.bmp"
DOC="simgs/test.bmp"

# +ve pitch requires smaller size!
# -centralise
SIMCOM="./simgen
-yoff -0.01 -depth 0.5
-lines 15 -roll 180 -yaw 0 -pitch 30 -size 0.5
-image -qnd -overlay
$DOC tmp.bmp
-gnuplot
-showexp
-rnd $RND
-noise 0.1
$@";
# -imgnoise 0.0
# -focal 0.1

SIMMODS="
-ins 1
"
# -rems 2

PPPARAMS="
-hvpcheat
-deriv
-res 180 -adapt 0.03 -maxpixels 2000
-dolowresscan -lowres 60 -lowsmooth 1
-recwid 200
-gamma 0.0 -light
-oth 6 -badx 35 -bady 21
tmp-inv.bmp tmp.bmp
"

# for F in `seq -w 0.5 0.3 4.0`; do
# for X in "" "-spacings"; do
# for Y in "" "-noransac"; do
# for X in "-spacings"; do
for F in 0.5; do
for X in "" "-spacings"; do
for Y in "-noransac" ""; do

	# echo "set title \"$X $Y, f.p. $F\" \"Times-Roman,26\"" > title.dogpl
	# echo "set title \"test\"" > title.dogpl
	rm -f title.dogpl

	echo `curseyellow`$SIMCOM $X $Y -focal $F`cursenorm`
	$SIMCOM $X $Y -focal $F | tee simgen.out | grep RANSAC

	mv gplfit.ps gplfit-$F$X$Y-sim-nomod.eps
	mv gplsolve.txt gplsolve-$F$X$Y-sim-nomod.txt
	mv gpldata.txt gpldata-$F$X$Y-sim-nomod.dat

	echo `curseyellow`$SIMCOM $SIMMODS $X $Y -focal $F`cursenorm`
	$SIMCOM $SIMMODS $X $Y -focal $F | tee simgen.out | grep RANSAC

	mv gplfit.ps gplfit-$F$X$Y-sim.eps
	mv gplsolve.txt gplsolve-$F$X$Y-sim.txt
	mv gpldata.txt gpldata-$F$X$Y-sim.dat

	# echo "Press a key"
	# waitforkeypress

	# # Test pp
	# invert -i tmp.bmp -o tmp-inv.bmp
	# ../projprof/pp $X $Y $PPPARAMS > pp.out

	# mv gplfit.ps gplfit-$F$X$Y-pp.eps
	# mv gplsolve.txt gplsolve-$F$X$Y-pp.txt
	# mv gpldata.txt gpldata-$F$X$Y-pp.dat

	./showparamresults.sh

	# gv gplfit$X$Y.eps

done
if test "$X" = "-spacings"; then
cursegreen
echo ./plot-ranvreal$X.sh gpldata-$F$X-sim-nomod.dat gpldata-$F$X-noransac-sim.dat
cursenorm
./plot-ranvreal$X.sh gpldata-$F$X-sim-nomod.dat gpldata-$F$X-noransac-sim.dat > /dev/null 2>&1
mv gplfit.ps gplfit-nomodvreal-$F$X.eps
else
# ./plot-ranvreal$X.sh gpldata-$F$X-sim.dat gpldata-$F$X-noransac-sim.dat > /dev/null 2>&1
cursegreen
echo ./plot-ranvreal$X.sh gpldata-$F$X-sim-nomod.dat gpldata-$F$X-sim.dat gpldata-$F$X-noransac-sim.dat
cursenorm
./plot-ranvreal$X.sh gpldata-$F$X-sim-nomod.dat gpldata-$F$X-sim.dat gpldata-$F$X-noransac-sim.dat > /dev/null 2>&1
mv gplfit.ps gplfit-ranvreal-$F$X.eps
fi
done
done

echo "gv" gplfit-ran*.eps gplfit-nomod*.eps
# convert -loop 0 -delay 100 -geometry 400 gplfit*.eps anim.gif

echo "RND=$RND"
