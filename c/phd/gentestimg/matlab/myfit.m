function sse=myfit(params,Input,Actual_Output)
                      A=params(1);
                      lamda=params(2);

                      Fitted_Curve=A.*exp(-lamda*Input);
                      Error_Vector=Fitted_Curve - Actual_Output;
                      %       When curvefitting, a typical quantity to 
                      %       minimize is the sum of squares error
                      sse=sum(Error_Vector.^2);
                      %       You could also write sse as
                      %       sse=Error_Vector(:)'*Error_Vector(:);
