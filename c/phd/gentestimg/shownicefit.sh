# killall ghostview

rm gplfit-*.eps

RND=$RANDOM && test "$RND" || RND=`getrandom` || RND=20
# RND=16632

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
-ins 2
-rnd $RND
$@";
# -imgnoise 0.0
# -noise 0.2
# -rems 2
# -focal 0.1

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
	# rm title.dogpl

	echo `curseyellow`$SIMCOM $X $Y -focal $F`cursegrey`
	$SIMCOM $X $Y -focal $F | tee simgen.out | grep RANSAC

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
echo `curseyellow`./plot-ranvreal$X.sh gpldata-$F$X-sim.dat gpldata-$F$X-noransac-sim.dat`cursegrey`
./plot-ranvreal$X.sh gpldata-$F$X-sim.dat gpldata-$F$X-noransac-sim.dat > /dev/null 2>&1
mv gplfit.ps gplfit-ranvreal-$F$X.eps
done
done

# convert -loop 0 -delay 100 -geometry 400 gplfit*.eps anim.gif

echo "RND=$RND"
