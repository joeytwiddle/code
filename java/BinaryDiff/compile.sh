cd ./src &&
find . -name "*.java" | withalldo verbosely javac &&
find . -name "*.class" |
while read CLASSFILE
do mkdir -p "`dirname ../classes/"$CLASSFILE"`" && mv "$CLASSFILE" ../classes/"$CLASSFILE"
done
