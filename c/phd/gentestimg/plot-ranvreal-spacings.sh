#!/bin/bash

RANSACDATA="$1"
REALDATA="$2"

/usr/local/gnu/bin/gnuplot << !

u = -18.429321
v = -20.000000
w = -0.000010
U=0.005000
V=-2382.549805
W=-0.021493
# e(x) is expected = ground truth
# f(x) is fitted

# u must be provided by the program.
# u=0.001
# This "default" work better for some images!
# v=2
# w=0.1
# These are the defaults for all images:
# v=-20
# w=0.00001
# Now also provided by calling C program.

# u=U
# v=V
# w=W

u2=u
v2=v
w2=w


# The advanced fitting method (using spacings) --------------------

set xlabel "line position y(n)" "Times-Roman,26"
set ylabel "line spacing Y(n)" "Times-Roman,26"

# Expected, with U,V,W known:
# They must be provided for this to work.
# U=0.001667
# V=-2401.85
# W=-0.0359646
e(x)=U*(1+V*(((U-x)/(W*x-U*V)-0.5)+1))/(1+W*(((U-x)/(W*x-U*V)-0.5)+1))-U*(1+V*((U-x)/(W*x-U*V)-0.5))/(1+W*((U-x)/(W*x-U*V)-0.5))

# The actual fitting:
# This is an estimate of the real eqn, but more likely to converge because less complicated!
# x=(u-y)/(w*y-u*v)
# f(x)=u*v/(1+x*w)
# it gives rubbish results but good as a more stable initial fit
f(y)=u*v/(1+w*(u-y)/(w*y-u*v))
fit f(x) "$RANSACDATA" via v,w
f(x)=u*(1+v*(((u-x)/(w*x-u*v)-0.5)+1))/(1+w*(((u-x)/(w*x-u*v)-0.5)+1))-u*(1+v*((u-x)/(w*x-u*v)-0.5))/(1+w*((u-x)/(w*x-u*v)-0.5))
fit f(x) "$RANSACDATA" via v,w

g(y)=u2*v2/(1+w2*(u2-y)/(w2*y-u2*v2))
fit g(x) "$REALDATA" via v2,w2
g(x)=u2*(1+v2*(((u2-x)/(w2*x-u2*v2)-0.5)+1))/(1+w2*(((u2-x)/(w2*x-u2*v2)-0.5)+1))-u2*(1+v2*((u2-x)/(w2*x-u2*v2)-0.5))/(1+w2*((u2-x)/(w2*x-u2*v2)-0.5))
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
# plot "$REALDATA" w p 7, f(x), g(x)
# plot "$REALDATA" w p 7, f(x) w l 8 0, g(x) w l 1 0
plot "$REALDATA" w p 7, f(x) w l 8 0, g(x) w l 1 0

!

# sedreplace "/LT7 { .* } def" "/LT7 { PL [] 0 0 0.7 DL } def" gplfit.ps
sedreplace "/LT7 { .* } def" "/LT7 { PL [] 0 0 0.6 DL } def" gplfit.ps
# sedreplace "/LT0 { .* } def" "/LT0 { PL [8 dl 6 dl] 0.8 0 0 DL } def" gplfit.ps
# sedreplace "/LT0 { .* } def" "/LT0 { PL [8 dl 6 dl] 0 0.6 0 DL } def" gplfit.ps
sedreplace "/LT0 { .* } def" "/LT0 { PL [8 dl 6 dl] 0.6 0 0 DL } def" gplfit.ps

