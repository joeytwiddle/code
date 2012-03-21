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

