# BDIFF_CLASSES="$PWD/classes"
BDIFF_CLASSES=`dirname "$0"`/classes

verbosely java -Xmx"512m" -cp "$BDIFF_CLASSES" org.neuralyte.binarydiff.BinaryDiff -q $OPTIONS "$@"
