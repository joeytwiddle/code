#!/usr/bin/env bash
set -e

rm -rf ./classes/

. get_classpath.shlib

cd src/

#find . -name "*.java" | notindir CVS | withalldo jikes
find . -name "*.java" | xargs -d '\n' javac

find . -name "*.class" |
while read classfile
do
	mkdir -p ../classes/"$(dirname "$classfile")"
	mv "$classfile" ../classes/"$classfile"
done

find . -name "*.png" |
while read resourcefile
do
	mkdir -p ../classes/"$(dirname "$resourcefile")"
	cp -a "$resourcefile" ../classes/"$resourcefile"
done

cp -a META-INF ../classes/

cd ..
