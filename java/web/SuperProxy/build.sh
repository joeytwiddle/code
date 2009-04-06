. addtoclasspath $HOME/j/code/javawebtools/CommonUtils/bin
. addtoclasspath $HOME/j/code/javawebtools/SimpleProxy/classes
for JAR in $PWD/lib/*.jar
do . addtoclasspath "$JAR"
done

del bin

cd src

find . -name "*.java" | notindir CVS | withalldo javac

find . -name "*.class" |
while read CLASSFILE
do
	mkdir -p ../bin/"`dirname \"$CLASSFILE\"`" || exit
	mv "$CLASSFILE" ../bin/"$CLASSFILE"
done

# cp -a META-INF ../bin

cd ..

