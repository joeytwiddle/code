function sse=jfit2u(params,Input,Actual_Output)
                      U=params(1);
                      V=params(2);
                      W=params(3);

                      % Fitted_Curve=A.*exp(-B*Input);
			% This ./ matrix division ais prime suspect - ah no it was the initial variables that were the problem
%                      Fitted_Curve=((k1+A*Input)./(k2+B*Input));
% ok so the X we see is (k1+A*x)/(k2+B*x)
% and the Y is (k1+A*x+A)/(k2+B*x+B)-(k1+A*x)/(k2+B*x)
% we rearrange the first to get x in terms of X (input)
x=(U-Input)./(W*Input-U*V);
x=0:1:max(size(x))-1;
% x=1:1:max(size(x));
x=x(:);
Fitted_Curve=U*(1+V*(x+1))./(1+W*(x+1))-U*(1+V*x)./(1+W*x);
% Fitted_Curve=posVspcu(Input,U,V,W);
% where the last clause shoudl = Input anyway
                      Error_Vector=Fitted_Curve - Actual_Output;
                      %       When curvefitting, a typical quantity to 
                      %       minimize is the sum of squares error
                      sse=sum(Error_Vector.^2);
                      %       You could also write sse as
                      %       sse=Error_Vector(:)'*Error_Vector(:);
