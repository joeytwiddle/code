getparam () {
	grep "^$1 *= *" "$2" | sed "s/^$1 *= *//"
}

GTU=`getparam "U" gplsolve-sim.txt`
GTV=`getparam "V" gplsolve-sim.txt`
GTW=`getparam "W" gplsolve-sim.txt`

U=`getparam "u" gplsolve-sim.txt`
V=`getparam "gotV" simgen.out`
W=`getparam "gotW" simgen.out`

# PPU=`getparam "guessU" pp.out`
PPU=`getparam "u" gplsolve-pp.txt`
PPV=`getparam "gotV" pp.out`
PPW=`getparam "gotW" pp.out`

# echo "param	grndtrth	simgot		ppgot"
# echo "  U	$GTU	$U	$PPU"
# echo "  V	$GTV	$V	$PPV"
# echo "  W	$GTW	$W	$PPW"

echo "param	U		V		W"
echo "grndtth	$GTU  	$GTV  	$GTW"
echo "simgot	$U  	$V  	$W"
echo "ppgot	$PPU  	$PPV  	$PPW"
