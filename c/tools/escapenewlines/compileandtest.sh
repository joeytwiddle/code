TESTSRC=escapenewlines.c
gcc escapenewlines.c -o escapenewlines &&
gcc unescapenewlines.c -o unescapenewlines &&
./escapenewlines $TESTSRC > test1.escaped &&
./unescapenewlines test1.escaped > test1.dec &&
cat $TESTSRC | ./escapenewlines > test2.escaped &&
cat test2.escaped | ./unescapenewlines > test2.dec &&
cmp $TESTSRC test1.dec &&
cmp $TESTSRC test2.dec &&
! cmp $TESTSRC test1.escaped > /dev/null &&
! cmp $TESTSRC test2.escaped > /dev/null &&
test `countlines test1.escaped` = 1 &&
test `countlines test2.escaped` = 1 &&
(
	echo "Compilation and tests went fine =) .  You may want to:"
	echo ln -sf $PWD/escapenewlines $HOME/bin/escapenewlines
	echo ln -sf $PWD/unescapenewlines $HOME/bin/unescapenewlines

)
