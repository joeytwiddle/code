# time java -Xmx512m -cp "$PWD/classes" org.neuralyte.binarydiff.Expander 2>&1 | tee bdiff_expansion.log
time java -Xmx512m -cp "$PWD/classes" org.neuralyte.binarydiff.BinaryDiff -expand /tmp/test.bdiff 2>&1 | tee bdiff_expansion.log
