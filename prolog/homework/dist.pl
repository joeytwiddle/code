
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
