% Turing machine simulation.
% Paul Clark (PC5065).

% Transitions for top program in booklet.
% trans(current state, current symbol, new state, new symbol, change position)
trans(s,b,q0,b,r).
%
trans(q0,0,q0,0,r).
trans(q0,3,q0,3,r).
trans(q0,6,q0,6,r).
trans(q0,9,q0,9,r).
trans(q0,1,q1,1,r).
trans(q0,4,q1,4,r).
trans(q0,7,q1,7,r).
trans(q0,2,q2,2,r).
trans(q0,5,q2,5,r).
trans(q0,8,q2,8,r).
%
trans(q1,0,q1,0,r).
trans(q1,3,q1,3,r).
trans(q1,6,q1,6,r).
trans(q1,9,q1,9,r).
trans(q1,2,q0,2,r).
trans(q1,5,q0,5,r).
trans(q1,8,q0,8,r).
trans(q1,1,q2,1,r).
trans(q1,4,q2,4,r).
trans(q1,7,q2,7,r).
%
trans(q2,0,q2,0,r).
trans(q2,3,q2,3,r).
trans(q2,6,q2,6,r).
trans(q2,9,q2,9,r).
trans(q2,1,q0,1,r).
trans(q2,4,q0,4,r).
trans(q2,7,q0,7,r).
trans(q2,2,q1,2,r).
trans(q2,5,q1,5,r).
trans(q2,8,q1,8,r).

% Initial state and tape.
startstate(s).
lefttape([]).
midtape(b).
righttape([2,7,1,8,b,b]).


% turing - initialises state and tape, and calls recurse.
turing:- startstate(S1), lefttape(TL), righttape(TR), midtape(TM),
	      recurse(S1,TL,TM,TR,_,_,_,_).


% recurse(initial state, initial tape left, mid, right,
%	  new state, new tape left, mid, right).
% recurse - makes next move for machine, then calls itself on new state and tape.
recurse(S,TL,TM,TR,S3,TL3,TM3,TR3):-
	printtape(S,TL,TM,TR),  % Print current state and tape for user.
	trans(S,TM,NS,NTM,CP),  % Check that a transition goes from current state.
	move(TL,NTM,TR,TL2,TM2,TR2,CP),  % Print symbol and move.
	recurse(NS,TL2,TM2,TR2,S3,TL3,TM3,TR3).  % Do the next transition.
recurse(S,TL,TM,TR,S,TL,TM,TR).  % If no transition, return what was given
				 % (no change).


% move(tape left, mid, right, new tape left, mid, right,
%      direction (l=left,n=none,r=right)) moves tape along to left or right, or
% doesn't move tape at all.
move(TL,TM,TR,TL,TM,TR,n).
move(TL,TM,TR,NTL,NTM,NTR,l):- tail(TL,NTL), head(TL,NTM), concat([TM],TR,NTR).
move(TL,TM,TR,NTL,NTM,NTR,r):- concat(TL,[TM],NTL), head(TR,NTM), tail(TR,NTR).

% concat(X,Y,Z) concatenates lists X and Y to make Z.
concat([],L,L).
concat([X|L1],L2,[X|L3]):- concat(L1,L2,L3).


% head(X,Y) returns Y as head of X or blank if X is empty.
head([],' ').
head([X|_],X).

% tail(X,Y) returns Y as tail of X or empty if X empty.
tail([],[]).
tail([_|R],R).


% printtape(tape left, mid, right) prints current state and the contents of
% the tape to the screen, including machine's position, followed by a carriage
% return.
printtape(S,TL,TM,TR):- write(S), write(' - '), p(TL), write('>'), write(TM),
			write('<'), p(TR), nl.

% p(X) helps printtape by printing all the characters in the list X.
p([]).
p([X|R]):- write(X), p(R).