%% hits(X,Y,Z,L):-
%% 	data(O1,O2,O3,D1,D2,D3),
%% 	X =:= O1+L*D1, Y =:= O2+L*D2, Z =:= O3+L*D3,
%% 	X*X+Y*Y+Z*Z =:= 1.

test:- trace, hits(0,0,-1,4).

data(0,0,-5,0,0,1).

eq1(add(A,B),X):- eq(X,add(A,B)); eq(X,add(B,A)).
eq1(A,B):- eq(A,B); eq(B,A).

eq(X,X).

eq(sub(A,B),X):- C is A+1, D is B+1, eq(sub(C,D),X).

eq(add(A,B),X):- eq1(B,sub(X,A)).

eq(add(0,X),Y):- eq1(X,Y).

eq(mult(_,0),0).
eq(mult(A,1),X):- eq1(X,A).

hits(X,Y,Z,L):-
	data(O1,O2,O3,D1,D2,D3),
%%	eq1(X,add(O1,mult(L,D1))),
%%	eq1(Y,add(O2,mult(L,D2))),
	eq1(Z,add(O3,mult(L,D3))),
	eq1(add(add(power(X,2),power(Y,2)),power(Z,2)),1).