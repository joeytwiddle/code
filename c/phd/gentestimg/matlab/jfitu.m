function sse=jfitu(params,Input,Actual_Output)
                      U=params(1);
                      V=params(2);
                      W=params(3);

                      % Fitted_Curve=A.*exp(-B*Input);
			% This ./ matrix division ais prime suspect
                      Fitted_Curve=U*(1+V*Input)./(1+W*Input)
			Actual_Output
			% Input
			% Fitted_Curve
			% Actual_Output
                      Error_Vector=Fitted_Curve - Actual_Output;
                      %       When curvefitting, a typical quantity to 
                      %       minimize is the sum of squares error
                      sse=sum(Error_Vector.^2);
                      %       You could also write sse as
                      %       sse=Error_Vector(:)'*Error_Vector(:);
