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

