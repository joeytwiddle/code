if test "$1" = ""; then
	echo "dogplfitting simple | spacings" > /dev/stderr
	exit 1
fi

METHOD="$1"

SRCDIR="../gentestimg"

# Prepate gnuplot file
cat gpldo.txt "$SRCDIR/fitgnuplot2-$METHOD.txt" "$SRCDIR/fitgnuplot2-plot.txt" > gplsolve.txt

# Run gnuplot
cat gplsolve.txt | gnuplot > gplans.txt 2>&1

# Collect data
grep '^v' gplans.txt | grep '=' | tail -1 | after '= ' | before ' ' > v.txt
grep '^w' gplans.txt | grep '=' | tail -1 | after '= ' | before ' ' > w.txt
grep '^WSSR' gplans.txt | tail -1 | between ':' | before 'delta' | sed 's/ //g' > wssr.txt
# cat wssr.txt
