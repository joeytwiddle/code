% Assignment 3
% Paul Clark (PC5065)


% Data for problem.

people([tom, rose, bob, lee, ann, mary, sue, john, amy, mike]).
quiet(tom).
quiet(mary).
quiet(john).
quiet(mike).
couple(tom,rose).
couple(bob,sue).
couple(john,ann).
smokes(bob).
smokes(tom).
hatessmoke(mary).
hatessmoke(ann).
hatessmoke(john).
hatessmoke(amy).
hatessmoke(lee).
hatessmoke(mike).


% Finds solution for problem, by trying all possible
% permutations.

solution(S) :- people(P), perm(P,S), acceptable(S).


% Adds first person to end of list before checking, since
% table is round, so first next to last must be checked.

acceptable([X|R]) :- concat([X|R],[X],P), acc2(P).

concat([],L,L).
concat([X|L1],L2,[X|L3]) :- concat(L1,L2,L3).

acc2([_]).
acc2([X,Y|R]) :- acc2([Y|R]), cansit(X,Y), cansit(Y,X).

cansit(X,Y) :-  \+ couple(X,Y),
		\+ (smokes(X), hatessmoke(Y)),
		\+ (quiet(X), quiet(Y)).


% Gets possible permutations of seating arrangments.

perm([],[]).
perm(L,[X|R]) :- remove(L,X,P), perm(P,R).

remove([X|R],X,R).
remove([Y|R],X,[Y|N]) :- remove(R,X,N).


%  The first 4 solutions my program finds:
%  S = [tom,bob,rose,lee,mary,ann,mike,amy,john,sue]
%  S = [tom,bob,rose,lee,john,amy,mary,ann,mike,sue]
%  S = [tom,bob,rose,lee,john,amy,mike,ann,mary,sue]
%  S = [tom,bob,rose,lee,mike,ann,mary,amy,john,sue]
