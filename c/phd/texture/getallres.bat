# echo Warning: getres will crash on images without exp
# jnn test $1 -wskip 2
jnn scan $1 -wskip 2
mkdir results
mkdir results/$1
mv *.bmp results/$1
