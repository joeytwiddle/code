if test "$1" = ""; then
	echo "dogplfitting simple | spacings [ <u> [ <v> <w> ] ]" > /dev/stderr
	exit 1
fi

METHOD="$1"
shift
U=$1
V=$2
W=$3

SRCDIR="../gentestimg"

# Prepare gnuplot file
(

	cat gpldo.txt

	if test ! "$U" = ""; then
		# Actually the one provided in gpldo.txt is better!
		# to test
		# echo "u=$U"
		echo "U=$U"
	fi
	if test ! "$W" = ""; then
		echo "V=$V"
		echo "W=$W"
	fi

	cat "$SRCDIR/fitgnuplot2-$METHOD.txt"
	cat "$SRCDIR/fitgnuplot2-plot.txt" |
		if test ! "$W" = ""; then
			grep -v "^plot "
		else
			cat
		fi

	if test ! "$W" = ""; then
		echo 'plot "gpldata.txt" w p 7,f(x),e(x)'
	fi

)  >  gplsolve.txt

# Run gnuplot
cat gplsolve.txt | gnuplot > gplans.txt 2>&1

# Collect data
grep '^v' gplans.txt | grep '=' | tail -1 | after '= ' | before ' ' > v.txt
grep '^w' gplans.txt | grep '=' | tail -1 | after '= ' | before ' ' > w.txt
grep '^WSSR' gplans.txt | tail -1 | between ':' | before 'delta' | sed 's/ //g' > wssr.txt
# cat wssr.txt
