% Assignment 5
% Paul Clark (PC5065).

dist(p1, p2, 3).
dist(p1, p3, 4).
dist(p1, p4, 25).
dist(p2, p3, 20).
dist(p2, p4, 5).
dist(p3, p4, 6).
dist(X,Y,D):- dist(Y,X,D).





best_route(Start, ListPlaces, _):- 
	assert(route(_,9999)),		%fixes best route at arbritraily large value
	find_new(Start,ListPlaces,route([Start],0),R1), %finds new route section
	is_best(R1),
	fail.
	
best_route(_,_,route(R,D)):-		%returns current best route.
	retract(route(R,D)).
	

	
find_new(Start,P,route([Y|R],D),R1):-
	remove(X,P,T),
	dist(X,Y,D1),
	Rest is D1+D,
	check(route(_,Rest)),		
	find_new(Start,T,route([X,Y|R],Rest),R1).

find_new(Start,[],route([Y|R],D),R1):- % generates new branches.
	dist(Y,Start,D1),
	Rest is D1+D,
	R1 = route([Start,Y|R],Rest).

check(route(_,D)):-			
	route(_,Best),	      %Compares current with best..		
	D < Best.	

is_best(route(_,D)):-
	route(_,Best),			%gets current best route
	D >= Best,!.			%checks to see if best

is_best(route(Route,D)):-
	retract(route(_,_)).
	assert(route(Route,D)).


remove(X,[X|L],L).
remove(X,[Y|T],[Y|R]):- remove(X,T,R).
