/Paul Clark PC5065

/Answer to question 1

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


/Answer to question 2

% distance(C1, C2, D) says that
% the distance between C1 and C2 is D

/* 
 * Be careful that, unlike the next(X,Y) relation in Assignment 1,
 * distance(Place1, Place2, Distance) is a bi-directional relation.
 */

distance(bristol, exeter,    77).
distance(bristol, bath,      13).
distance(bristol, salisbury, 69).

distance(exeter,  bath,      72).
distance(exeter,  salisbury, 45).

distance(bath,    salisbury, 40).

test_route([bristol, exeter, bath, salisbury, bristol]).

total_distance([X],0).
total_distance([X,Y|L],D):- dist(X,Y,D1), total_distance([Y|L],D2), D is
D1+D2.

dist(X,Y,D):- distance(X,Y,D); distance(Y,X,D).
