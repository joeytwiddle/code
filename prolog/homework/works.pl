%% Eulerian graph program.
%% Paul Clark (PC5065).


%% makee - compiles a list of the edges.
%% makee(E1,E2) - makes finished list E2 from list so far E1.

makee(E1,E3):- edge(A,B), \+in((A,B),E1),
	       conc(E1,[(A,B)],E2), makee(E2,E3).
makee(E1,E1):- edge(A,B), in((A,B),E1).


%% makev - compiles a list of the vertices.
%% makev(E1,E2) - makes finished list E2 from list so far E1.

makev(E1,E3):- edge(A,_), \+in(A,E1), conc(E1,[A],E2), makev(E2,E3).
makev(E1,E3):- edge(_,B), \+in(B,E1), conc(E1,[B],E2), makev(E2,E3).
makev(E1,E1):- edge(A,_), in(A,E1).
makev(E1,E1):- edge(_,B), in(B,E1).


%% makel(VL,XL) - compiles adjacency list XL from vertices VL.

makel([],[]).
makel([V|VS],[(V,L)|XS]):- makea([],V,L), makel(VS,XS).

%% makea(L1,V,L2) - makes finished list L2 of vertices adjacent to V from
%% 		    list so far L1.

makea(L1,V,L3):- edge(V,A), \+in(A,L1), conc(L1,[A],L2), makea(L2,V,L3).
makea(L1,V,L3):- edge(A,V), \+in(A,L1), conc(L1,[A],L2), makea(L2,V,L3).
makea(L1,V,L1):- edge(V,A), in(A,L1).
makea(L1,V,L1):- edge(A,V), in(A,L1).


%% run - Main routine.
%%	 Reads in graph.
%%	 Compiles list of vertices.
%%	 Compiles adjacency list.
%%	 Displays incidence matrix.
%%	 Asks user for first vertex of path.
%%	 Asks user for second vertex of path.
%%	 Compiles list of edges, removing the user's first edge.
%%	 Constructs the rest of the path, and displays it.

run:- [graph2],
      makev([],VS), write('Vertices: '), write(VS), nl,
      makel(VS,AL), write('Adjacency list: '), write(AL), nl, !,
      check(VS,AL),
      write('Incidence matrix:'), nl, write('  '), dwos(VS), nl, imatrix(VS,AL),
      write('Enter initial vertex followed by ".": '), read(V1),
      write('Enter next vertex followed by ".": '), read(V2),
      makee([],ES), del((V1,V2),ES,ES2), del((V2,V1),ES2,ES3),
      path(VS,ES3,AL,[(V1,V2)],V2,P), write('Path: '), write(P), nl.


%% path(VS,ES,AL,P1,V,P2) - given vertices, edges, adjacency list, path so far P1
%% 			    and current vertex, returns whole path P2.

path(_,[],_,P,_,P).
path(VS,ES,AL,P1,V,P3):- get(V,AL,POSS), in(TRY,VS), in(TRY,POSS),
	( in((V,TRY),ES); in((TRY,V),ES)), del((V,TRY),ES,ES2), del((TRY,V),ES2,ES3),
	conc(P1,[(V,TRY)],P2), path(VS,ES3,AL,P2,TRY,P3).


%% get(V,AL,L) - given vertex V and adjacency list AL, returns list of adjacent vertices L.

get(V,[],_):- err1(V).
get(V,[(V,L)|_],L):- !.
get(V,[_|R],L):- get(V,R,L).


%% check(VS,AL) - given list of vertices and adjacency list, checks graph is Eulerian.

check(VS,AL):- ( ( in(A,VS), in(B,VS), A\==B, odd(AL,A), odd(AL,B),
		   rem(A,AL,AL2), rem(B,AL2,AL3), resteven(AL3), write('2 odd, rest even') );
	         ( resteven(AL), write('All even') ) ),
	       write(' - is Euclidean.'), nl, !.
check(_,_):- write('Is not Euclidean.'), nl.


%% imatrix(VS,AL) - given vertices VL and adjacency list AL, displays incidence matrix.

imatrix(_,[]).
imatrix(VS,[(V,L)|R]):- write(V), write(' '), im2(VS,L), nl, imatrix(VS,R).

im2([],_).
im2([V|R],L):- in(V,L), write('1'), im2(R,L), !.
im2([_|R],L):- write('0'), im2(R,L).


%% dwos(L) - displays list L without spaces.

dwos([]).
dwos([E|R]):- write(E), dwos(R).


%% rem(V,L1,L2) - given vertex V and adjacency list L1, removes vertex and its list to produce L2.

rem(V,[],[]):- err1(V).
rem(V,[(V,_)|R],R):- !.
rem(V,[E|R1],R3):- rem(V,R1,R2), conc([E],R2,R3).


% del(E,L1,L2) - removes element E from list L1 to produce list L2.

del(_,[],[]).
del(E,[E|R],R):- !.
del(E,[X|R1],R3):- del(E,R1,R2), conc([X],R2,R3).


% odd(AL,V) - given adjacency list AL, determines if odd number of vertices adjacent to V.

odd([],V):- err1(V).
odd([(V,L)|_],V):- !, oddlength(L).
odd([(_,_)|R],V):- odd(R,V).


% oddlength(L) - returns true if list L is of odd length.

oddlength(L):- \+evenlength(L).


% even(AL,V) - given adjacency list AL, determines if even number of vertices adjacent to V.

even([],V):- err1(V).
even([(V,L)|_],V):- !, evenlength(L).
even([(_,_)|R],V):- even(R,V).


% evenlength(L) - returns true if list L is of even length.

evenlength(L):- len(L,LEN), (LEN/2)=:=float(LEN//2).


%% err1(V) - displays occurence of error (never happens if graph is Eulerian).

err1(V):- write('Error: '), write(V), write(' not found in adjacency matrix during odd/even search or remove.'), nl.


%% resteven(AL) - returns true if all vertices in AL have an even number of adjacent vertices.

resteven([]).
resteven([(_,L)|R]):- evenlength(L), resteven(R).


%% conc(L1,L2,L3) - concatenates lists L1 and L2 to produce list L3.

conc([],L,L).
conc([X|L1],L2,[X|L3]):- conc(L1,L2,L3).


%% len(L,X) - X is length of list L.

len([],0).
len([_|R],X+1):- len(R,X).


%% in(E,L) - returns true if element E is in list L.

in(X,[X|_]).
in(X,[_|R]):- in(X,R).