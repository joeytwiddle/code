
/***********************************************************

simp(Equ1, Equ2) takes Equ1 as input, remove redundant 0
from Equ1 then construct a new structure Equ2.

***********************************************************/


simp(X,X):- atomic(X).

simp(X+Y, NewXY):-
	simp(X, NewX),
	simp(Y, NewY),
	rules_for_plus(NewX, NewY, NewXY).

simp(X*Y, NewXY):-
	simp(X, NewX),
	simp(Y, NewY),
	rules_for_mult(NewX, NewY, NewXY).

rules_for_plus(0, 0, 0).
rules_for_plus(X, 0, X):- X \== 0.
rules_for_plus(0, X, X):- X \== 0.
rules_for_plus(X, Y, X+Y):- X \== 0, Y \== 0.

rules_for_mult(0, _, 0).
rules_for_mult(_, 0, 0).
rules_for_mult(1, X, X):- X \== 0.
rules_for_mult(X, 1, X):- X \== 0.
rules_for_mult(X, Y, X*Y):- X \== 0, X \== 1, Y \== 0, Y
\== 1.
