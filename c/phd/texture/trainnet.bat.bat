loadNet("current.net")
loadPattern("current.pat")

setSeed()
setInitFunc("Randomize_Weights",1.0,-1.0)
# setLearnFunc("Std_Backpropagation",0.005)
setLearnFunc("Std_Backpropagation",0.001)
setUpdateFunc("Topological_Order")
setShuffle(TRUE)

initNet()

while SSE>1.1 and CYCLES<=300 do
  if CYCLES mod 25 == 0 then
    print("cycles=",CYCLES,"/300 SSE=",SSE," MSE=",MSE)
  endif
  trainNet()
endwhile

saveNet("trainednet.net")
saveResult("trainresults.res",1,PAT,TRUE,TRUE,"create")
