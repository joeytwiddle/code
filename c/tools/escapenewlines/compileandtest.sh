TESTSRC=encodeslashn.c
gcc encodeslashn.c -o encodeslashn &&
gcc decodeslashn.c -o decodeslashn &&
./encodeslashn $TESTSRC > test1.enc &&
./decodeslashn test1.enc > test1.dec &&
cat $TESTSRC | ./encodeslashn > test2.enc &&
cat test2.enc | ./decodeslashn > test2.dec &&
cmp $TESTSRC test1.dec &&
cmp $TESTSRC test2.dec &&
! cmp $TESTSRC test1.enc > /dev/null &&
! cmp $TESTSRC test2.enc > /dev/null &&
test `countlines test1.enc` = 1 &&
test `countlines test2.enc` = 1 &&
(
	echo "Compilation and tests went fine =) .  You may want to:"
	echo ln -sf $PWD/encodeslashn $HOME/bin/encodeslashn
	echo ln -sf $PWD/decodeslashn $HOME/bin/decodeslashn

)
