./listmodules.sh |
while read X; do
	if test -f "$X.c"; then
		./testdepends.sh $X
	else
		echo "$X does not appear to be local."
	fi
done
