TARGET=test_set/

dush $TARGET

for HIST_SIZE in $((1024*1024*2)) 5242880 $((1024*1024*10))
do

for SCANSIZE in 32 64 256
do

	OUTPUT_FILE="/tmp/BinaryDiff.scan-$SCANSIZE.hist-$HIST_SIZE.bdiff"

	# time java -Xmx"512m" -cp "$PWD/classes" org.neuralyte.binarydiff.BinaryDiff -q "$OUTPUT_FILE" $TARGET 2>&1 | tee "$OUTPUT_FILE.log"
	time ./big_repetition_compress -scansize "$SCANSIZE" -history "$HIST_SIZE" "$OUTPUT_FILE" $TARGET 2>&1 | tee "$OUTPUT_FILE.log"

done

done
