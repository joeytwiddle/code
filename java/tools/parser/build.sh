javaTop="$JPATH/code/java"
[ -d "$javaTop" ] || javaTop="$HOME/j/code/java"
export CLASSPATH="$CLASSPATH:$javaTop/jlib/bin:$javaTop/web/CommonUtils/bin:$javaTop/tools/parser/bin"

set -e

rm -rf build/ bin/
mkdir -p build
mkdir -p bin
cp -a src/* build/ || true

cd build
find . -name "*.java" |
grep -v "/CVS/" |
withalldo javac

# mkdir -p ../bin
# find . -name "*.class" |
# while read C
# do mv "$C" ../bin/"$C"
# done
cd ..

cp -a build/* bin/

