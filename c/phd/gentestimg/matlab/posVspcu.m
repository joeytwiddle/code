function ans=posVspcu(Input,U,V,W)
% ok so the X we see is (k1+A*x)/(k2+B*x)
% and the Y is (k1+A*x+A)/(k2+B*x+B)-(k1+A*x)/(k2+B*x)
% we rearrange the first to get x in terms of X (input)
% x=-(Input*k2-k1)./(Input*B-A);
x=(U-Input)./(Input*W-U*V);
ans=U*(1+V*(x+1))./(1+W*(x+1))-U*(1+V*x)./(1+W*x);
