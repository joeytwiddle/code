#!/bin/bash

RANSACDATA="$1"
REALDATA="$2"
PLOTDATA="$3"
shift

GPLEXTRAS=""
if test ! "$3" = ""; then
GPLEXTRAS="
u=$3
v=$4
w=$5
"
fi

/usr/local/gnu/bin/gnuplot << !

u = 0.005005
v = 20.000000
w = 0.000010
U=0.005000
V=-2382.549805
W=-0.021493
# e(x) is expected = ground truth
# f(x) is fitted

# u must be provided.
# u=0.001
# These are the defaults for all images.
# v=-20
# w=0.00001
v=-2000
w=-0.005
# u=U
# v=V
# w=W

u2=u
v2=v
w2=w


# The simple fitting method ---------------------------------------

set xlabel "observed line number n" "Times-Roman,26"
set ylabel "line position y(n)" "Times-Roman,26"

# Expected, with U,V,W known:
# They must be provided for this to work.
# U=0.001667
# V = -2396.83 # -2237.489990
# W = -0.0360439 # -0.033641
e(x)=U*(1+V*x)/(1+W*x)

# The actual fitting:
f(x)=u*(1+v*x)/(1+w*x)
fit f(x) "$RANSACDATA" via v,w

$GPLEXTRAS

g(x)=u2*(1+v2*x)/(1+w2*x)
fit g(x) "$REALDATA" via v2,w2
# Plotting --------------------------------------------------------

set noclabel
set nokey
set nolabel
set term post eps color "Times-Roman,18"
set pointsize 1.3

set output "gplfit.ps"
# plot "$REALDATA" w p 7,e(x)
# plot "$REALDATA" w p 7,f(x),e(x)

# set output "origdata.ps"
# plot "whatever.dat" w p 6,f(x), "$REALDATA" w p 7,f(x)
# plot "$PLOTDATA" w p 0 7, f(x) w l 84 0, g(x) w l 1234 0
# Solid:
# plot "$PLOTDATA" w p 0 7, f(x) w l 7 0, g(x) w l 1 0
# Dotted:
plot "$PLOTDATA" w p 0 7, f(x) w l 7 0, g(x) w l 1 0

!

# sedreplace "| /LT3 { PL [1 dl 1.5 dl] 1 0 1 DL } def" "| /LT3 { PL [1 dl 1.5 dl] 0 0 1 DL } def" gplfit.ps
# sedreplace "^LT6$" "LT0" gplfit.ps
# sedreplace "/LT6 { PL \[2 dl 2 dl 2 dl 4 dl\] 0 0 0 DL } def" "/LT6 { PL [] 0 0.5 0 DL } def" gplfit.ps
sedreplace "/LT6 { PL \[2 dl 2 dl 2 dl 4 dl\] 0 0 0 DL } def" "/LT6 { PL [8 dl 6 dl] 0 0.5 0 DL } def" gplfit.ps

