grep \.h\> joeylib-header.h | after "<" | before "\.h>" > modules.list
cat modules.list |
while read X
do
	if test -f "$X.c"; then
		./testdepends.sh $X
	fi
done
