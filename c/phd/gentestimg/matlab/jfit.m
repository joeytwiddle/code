function sse=jfit(params,Input,Actual_Output)
                      k1=params(1);
                      A=params(2);
                      k2=params(3);
                      B=params(4);

                      % Fitted_Curve=A.*exp(-B*Input);
			% This ./ matrix division ais prime suspect
                      Fitted_Curve=((k1+A*Input)./(k2+B*Input));
			% Input
			% Fitted_Curve
			% Actual_Output
                      Error_Vector=Fitted_Curve - Actual_Output;
                      %       When curvefitting, a typical quantity to 
                      %       minimize is the sum of squares error
                      sse=sum(Error_Vector.^2);
                      %       You could also write sse as
                      %       sse=Error_Vector(:)'*Error_Vector(:);
