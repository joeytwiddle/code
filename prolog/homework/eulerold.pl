%% Eulerian graph program.
%% Paul Clark (PC5065).

%% run - runs the program.

run:- check.

%% makee - compiles a list of the edges.

makee(E1,E3):- isedge(X), \+in(X,E1),
	       conc([X],E1,E2), makee(E2,E3).
makee(E1,E1):- isedge(X), in(X,E1).

isedge((A,B)):- edge(A,B).

rev((A,B),(B,A)).

%% makev - compiles a list of the vertices.

makev(E1,E3):- isedge((A,_)), \+in(A,E1), conc([A],E1,E2), makev(E2,E3).
makev(E1,E3):- isedge((_,B)), \+in(B,E1), conc([B],E1,E2), makev(E2,E3).
makev(E1,E1):- isedge((A,_)), in(A,E1).
makev(E1,E1):- isedge((_,B)), in(B,E1).


%% check - checks graph is Eulerian.

check:- makev([],V), ( in(A,V), in(B,V), odd(A), odd(B), \+(A==B),
	  rem(A,V,V2), rem(B,V2,V3), resteven(V3) );
	( resteven(V) ).

arevertices([]).
arevertices([X|R]):- isvertex(X), arevertices(R).

isvertex(X):- edge(X,_); edge(_,X).

nodupl([]).
nodupl([X|R]):- \+ in(X,R), nodupl(R).

in(X,[Y|R]):- X==Y; in(X,R).

odd(X):- \+ even(X).

even(X):- makee(E), filter(X,E,E2), len(E2,L), (L/2)=:=float(L//2).

filter(_,[],[]).
filter(X,[(A,B)|R],[(A,B)|R]):- A==X; B==X.
filter(X,[(A,B)|R],R):- A\==X; B\==X.

%%resteven(L):- isvertex(X), \+ in(X,L), even(X), concat(L,X,L2), resteven(L2).

conc([],L,L).
conc([X|L1],L2,[X|L3]):- conc(L1,L2,L3).