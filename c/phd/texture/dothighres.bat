rm *.bmp
rm *.pat
rm *.res
rm *.td
jnn trainset new
jnn trainset image 10 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image img08 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image image10 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image image11 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image image15 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image image27 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image image2 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image image17 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image image20 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image image28 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image image29 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image image31 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset image image32 -wskip 10 -show -nptes 200 -nntes 200
jnn trainset finish
jnn trainnn
cp traindata.pat testdata.pat
batchman -f testnet.bat
forall ../images/*.bmp do getres %s
