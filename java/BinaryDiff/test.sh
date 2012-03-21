# time java -Xmx700m -cp "$PWD/classes" org.neuralyte.binarydiff.BinaryDiff testdata2/14a/* 2>&1 | tee output.log
# time java -Xmx1280m -cp "$PWD/classes" org.neuralyte.binarydiff.BinaryDiff testdata2/*/* 2>&1 | tee output.log
# time java -Xmx512m -cp "$PWD/classes" org.neuralyte.binarydiff.BinaryDiff testdata2.tar 2>&1 | ztee output.bdiff_log.gz

# time java -Xmx512m -cp "$PWD/classes" org.neuralyte.binarydiff.BinaryDiff testdata2.tar 2>&1 | ztee bdiff_log.gz

# SRCFILES=`find testdata2 -type f` 
# SRCFILES="testdata2.tar"
# SRCFILES="testdata3.tar"
# SRCFILES="testdata4.zip"
# SRCFILES="testdata.tar"
# SRCFILES="testdata/*"
SRCFILES=srctest

MEM="512m"
# MEM="128m"

time java -Xmx"$MEM" -cp "$PWD/classes" org.neuralyte.binarydiff.BinaryDiff /tmp/BinaryDiff.bdiff $SRCFILES 2>&1 | tee bdiff_log
# gzip bdiff_log bdiff_output
