TITLEA="Full PP scan"
COLLECTIONA="dar-crpage2-left30-full-deriv-spacings-l-n"
TITLEB="Hierarchical PP scan"
COLLECTIONB="dar-crpage2-left30-hier-deriv-spacings-l-n"

XAXIS="Percentage of noisy pixels in image"

./gettotals.sh "$COLLECTIONA"
./gettotals.sh "$COLLECTIONB"

gnuplot << !

set format x "%.0f%%"

set data style lines
set clabel
# set nokey
# set key bottom outside title "Legend:"
show key
set xlabel "$XAXIS" ,-1.2 "Times-Roman,28"
# set title "Plot of HVP accuracy against $HEAD"
set term post eps "Times-Roman,18"

# set ylabel "HVP accuracy" "Times-Roman,28"
set ylabel "Accuracy of recovery of HVP" "Times-Roman,28"
set output "$TITLEA Versus $TITLEB hvp.eps"
plot [] [-2.5:0] "$COLLECTIONA-hvpchange.data" t "$TITLEA","$COLLECTIONB-hvpchange.data" ti "$TITLEB"

!

