# TARGET=/mnt/big/ut/ut_win/Maps/noggins*
# TARGET=/mnt/big/ut/ut_win/Maps/noggins*/*Pure*
TARGET=/mnt/big/ut/ut_win/Maps/noggins*/*Noggin*

dush $TARGET

verbosely java -Xmx"512m" -cp "$PWD/classes" org.neuralyte.binarydiff.BinaryDiff -q /tmp/BinaryDiff.bdiff $TARGET | tee bdiff_log
