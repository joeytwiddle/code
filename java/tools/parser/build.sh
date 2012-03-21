javaTop="$JPATH/code/java"
[ -d "$javaTop" ] || javaTop="$HOME/j/code/java"
export CLASSPATH="$CLASSPATH:$javaTop/jlib/bin:$javaTop/web/CommonUtils/bin:$javaTop/tools/parser/bin"

set -e

## We don't want to confuse the build with old classes
rm -rf build
## But until we start removing classes, it's handy to keep a few around for emergencies!
# rm -rf bin
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

