% Assignment 4 - Paul Clark (PC5065)


% file jobs.pl 


% MY PROGRAMS

% get(T,L,C,N) - gets a list of the jobs in L that T can do: C
% and returns the rest: N.
get(_,[],[],[]).
get(T,[J|R1],[J|R2],N) :- job_type(J,JT), can_do(T,JT),
			  get(T,R1,R2,N).
get(T,[J|R1],R2,[J|N]) :- job_type(J,JT), \+can_do(T,JT),
			  get(T,R1,R2,N).

% job_type(J,T) - returns type T of job J.
job_type(j(_,T),T).

% first_four(L1,L2,R) - returns the first four (or as many as
% possible) elements of L1 in L2, and the rest in R.
% The cut in line 2 ensures line 3 is not used if avoidable by line 2.
% Line 1 seems to be redundant.
first_four([],[],[]).
first_four([E1,E2,E3,E4|R],[E1,E2,E3,E4],R) :- !.
first_four(X,X,[]).

% jobs_assign(T,L,A,R) - returns up to four jobs of L for T to
% do in A, and returns rest in R.
jobs_assign(T,L,A,R) :- get(T,L,G,R1), first_four(G,A,R2),
			concat(R1,R2,R).

% concat(A,B,C) - concatenates A and B to make C.
concat([],L,L).
concat([X|R1],L,[X|R2]) :- concat(R1,L,R2).

% get_job_location(J,L) - takes a list of jobs and returns
% their locations, removing duplicates.
get_job_location([],[]) :- !.
get_job_location([j(L,_)|R1],[L|R2]) :- get_job_location(R1,R2),
					\+in(L,R2).
get_job_location([j(L,_)|R1],R2) :- get_job_location(R1,R2),
				    in(L,R2).

% in(E,L) - returns true if element E is in list L.
in(_,[]) :- 1==2.
in(X,[Y|R]) :- X==Y; in(X,R).

% END OF MY PROGRAMS


% a list of jobs 

jobs_of_today([j(bristol,   j_type1),
 	       j(bristol,   j_type3),
	       j(bristol,   j_type2),
	       j(bath,      j_type5),
	       j(bath,      j_type4),
	       j(exeter,    j_type5),
	       j(exeter,    j_type3),
	       j(exeter,    j_type2),
	       j(salisbury, j_type3),
	       j(salisbury, j_type5),
	       j(salisbury, j_type4),
	       j(salisbury, j_type1)]).

% database on technicians 

% can_do_list(Tech, WhatTypeJobsTechCanDo)
can_do_list(tech1, [j_type1, j_type2, j_type3]).
can_do_list(tech2, [j_type1, j_type3, j_type5]).
can_do_list(tech3, [j_type2, j_type4]).

can_do(T, JobType):- can_do_list(T, L), member(JobType, L).

% lib

% member(X, [X|_]).
% member(X, [_|Y]):- member(X,Y).

% top level ------- two test programs: go1/0 and go2/0

go1:- 
    jobs_of_today(J0),
    jobs_assign(tech1, J0, J1, R1), get_job_location(J1, L1),
    jobs_assign(tech2, R1, J2, R2), get_job_location(J2, L2),
    jobs_assign(tech3, R2, J3, R3), get_job_location(J3, L3),
    print_result(tech1, J1, L1),
    print_result(tech2, J2, L2),
    print_result(tech3, J3, L3),
    print_result(rest,  R3, []).

go2:- 
    jobs_of_today(J0),
    jobs_assign(tech3, J0, J1, R1), get_job_location(J1, L1),
    jobs_assign(tech1, R1, J2, R2), get_job_location(J2, L2),
    jobs_assign(tech2, R2, J3, R3), get_job_location(J3, L3),
    print_result(tech1, J2, L2),
    print_result(tech2, J3, L3),
    print_result(tech3, J1, L1),
    print_result(rest,  R3, _).

%%%%%%%%%%%%%%%%%% dealing with output %%%%%%%%%%%%%%%%%%%%
% builtin write(X) writes term X to screen
% builtin nl writes a new line to the screen
% print_result(Tech, JobList, PlaceTogo)

print_result(rest, JobList, _):-
    write('unallocated job(s): '),  write(JobList), nl, nl.

print_result(Tech, JobList, PlaceToGo):- Tech \== rest,
    write(Tech), write(' will do the following jobs '), nl,
    write(JobList), nl, 
    write('locations are: '), write(PlaceToGo), nl, nl.
