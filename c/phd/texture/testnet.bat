loadNet("trainednet.net")
loadPattern("testdata.pat")
testNet()
saveResult("testresults.res",1,PAT,TRUE,TRUE,"create")
