function sse=jfit2(params,Input,Actual_Output)
                      k1=params(1);
                      A=params(2);
                      k2=params(3);
                      B=params(4);

                      % Fitted_Curve=A.*exp(-B*Input);
			% This ./ matrix division ais prime suspect - ah no it was the initial variables that were the problem
%                      Fitted_Curve=((k1+A*Input)./(k2+B*Input));
% ok so the X we see is (k1+A*x)/(k2+B*x)
% and the Y is (k1+A*x+A)/(k2+B*x+B)-(k1+A*x)/(k2+B*x)
x=-(Input*k2-k1)./(Input*B-A);
FittedCurve=(k1+A*x+a)/(k2+B*x+b)-(k1+A*x)/(k2+B*x);
% where the last clause shoudl = Input anyway
                      Error_Vector=Fitted_Curve - Actual_Output;
                      %       When curvefitting, a typical quantity to 
                      %       minimize is the sum of squares error
                      sse=sum(Error_Vector.^2);
                      %       You could also write sse as
                      %       sse=Error_Vector(:)'*Error_Vector(:);
