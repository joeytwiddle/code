loadNet("trainednet.net")
loadPattern("question.pat")
testNet()
saveResult("answer.res",1,PAT,TRUE,TRUE,"create")
