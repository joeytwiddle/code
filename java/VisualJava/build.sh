del classes

cd src

find . -name "*.java" | notindir CVS | withalldo javac
# find . -name "*.java" | notindir CVS | withalldo jikes

find . -name "*.class" |
while read CLASSFILE
do
	mkdir -p ../classes/"`dirname \"$CLASSFILE\"`" || exit
	mv "$CLASSFILE" ../classes/"$CLASSFILE"
done

cp -a META-INF ../classes

cd ..
