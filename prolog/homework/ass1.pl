% Assignment 1
% Paul Clark (PC5065)

% Part a

% Question 1

next(p1,p2).
next(p2,p4).
next(p2,p5).
next(p4,p3).
next(p4,p5).
next(p5,p7).
next(p7,p6).

% Question 2

route(X,Y) :- next(X,Y).
route(X,Y) :- next(X,P), route(P,Y).

% Part b

exam(josh,french,a).
exam(josh,english,a).
exam(josh,lakdjf,b).
exam(josh,alkj,b).

exam(rob,alkj,a).
exam(rob,french,a).
exam(rob,lfslkjf,b).

exam(claire,spanish,a).
exam(claire,sldfjs,a).
exam(claire,sljflsk,a).

% canenter(X) returns Yes if X meets conditions to enter school.

canenter(X) :- ( exam(X,E1,a), exam(X,E2,a), exam(X,E3,a),
		 \+ duplicate([E1,E2,E3]) )
	       ;
	       ( exam(X,E1,a), exam(X,E2,a), exam(X,E3,b), exam(X,E4,b)),
	         \+ duplicate([E1,E2,E3,E4] ).

duplicate([]) :- 1==2.
duplicate([X|L]) :- in(X,L); duplicate(L).

in(X,[X|_]).
in(X,[_|L]) :- in(X,L).