rm *.bmp
rm *.pat
rm *.res
rm *.td
jnn trainset new
jnn trainset image 06 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image 11 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image 01 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image 02 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image img02 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image img05 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image img06 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image img16 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image img15 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image img17 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image img22 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image img21 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image img25 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image img24 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image img38 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image img04 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image image16 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image image19 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset finish
jnn trainnn
cp traindata.pat testdata.pat
batchman -f testnet.bat
