% Assignment 2
% Paul Clark (pc5065).


% Question 1

% Naive algorithm
reverse([],[]).
reverse([X|R],L):- reverse(R,P), concat(P,[X],L).

% Efficient algorithm
reverse2(L1,L2):- reverse3(L1,[],L2).
reverse3([],L,L).
reverse3([X|L1],L2,L3):- reverse3(L1,[X|L2],L3).

concat([],L,L).
concat([X|L1],L2,[X|L3]):- concat(L1,L2,L3).

reverse_test:- reverse([1,2,3,4],L), reverse2([1,2,3,4],L), L=[4,3,2,1].


% Question 2

gets(state(_,_,_,has),[]).
gets(S1,L):- move(S1,_,S2,X), gets(S2,L1), concat([X],L1,L).
move(state(middle,onBox,X,hasnt),grasp,state(middle,onBox,X,has),grasp).
move(state(P,onFloor,P,H),climb,state(P,onBox,P,H),climb).
move(state(P,onFloor,P,H),push(P,Q),state(Q,onFloor,Q,H),push(P,Q)).
move(state(P,onFloor,X,H),walk(P,Q),state(Q,onFloor,X,H),walk(P,Q)).

/* monkey(L) will give a list of the actions taken by the monkey.
   If such a list is returned then the monkey has succeeded.
   If the monkey cannot succeed (or the statements are in the wrong
   order), Prolog will keep going for ever, probably letting the
   monkey walk or push back and forth for ever. To make this happen
   we can put either the push or the walk statement before the climb
   or grasp statements. This will cause the monkey to try walking or
   pushing before climbing or grasping, so he will keep going back
   and forth without every trying to grasp, so he will never "have"
   the banana, so the program will never finish. */

monkey(L):- gets(state(atDoor,onFloor,atWindow,hasnt),L), !.


% Question 3


% Provided relations:

% win(Player,Board) means Player has won the game on Board; lose(P,B) is the
% opposite.

win(P,[[P,P,P],_,_]).
win(P,[_,[P,P,P],_]).
win(P,[_,_,[P,P,P]]).
win(P,[[P,_,_],[P,_,_],[P,_,_]]).
win(P,[[_,P,_],[_,P,_],[_,P,_]]).
win(P,[[_,_,P],[_,_,P],[_,_,P]]).
win(P,[[P,_,_],[_,P,_],[_,_,P]]).
win(P,[[_,_,P],[_,P,_],[P,_,_]]).

lose(P,B) :- other(P,Q),win(Q,B).

% put(Player,Position,Board1,Board2) means that it is legal for Player to play
% at Position on Board1, and the result is Board2.

put(P,[1,C],[R1,R2,R3],[NEWR1,R2,R3]) :- putrow(P,C,R1,NEWR1).
put(P,[2,C],[R1,R2,R3],[R1,NEWR2,R3]) :- putrow(P,C,R2,NEWR2).
put(P,[3,C],[R1,R2,R3],[R1,R2,NEWR3]) :- putrow(P,C,R3,NEWR3).

putrow(P,1,[u,P2,P3],[P,P2,P3]).
putrow(P,2,[P1,u,P3],[P1,P,P3]).
putrow(P,3,[P1,P2,u],[P1,P2,P]).

% other(Player1,Player2) means Player2 is the other player to Player1.

other(o,x).
other(x,o).


% My relations

% The user decides who starts: x for human, o for computer.
main(P):- write('\n'), start(B), play(P,B).
mefirst:- main(x).
himfirst:- main(o).

start([R1,R2,R3]):- startrow(R1), startrow(R2), startrow(R3).
startrow([u,u,u]).

% human player is always x
play(x,B):- writeifwon(o,B); finished(B); (writenice(B), readiffree(B,Pos), put(x,Pos,B,B2), play(o,B2)).
% computer player is always o
play(o,B):- writeifwon(x,B); finished(B); (choose(o,B,Pos), put(o,Pos,B,B2), play(x,B2)).

writeifwon(P,B):- win(P,B), writenice(B), write('Player '), writenicechar(P), write(' wins.\n').

finished([R1,R2,R3]):- finishedrow(R1), finishedrow(R2), finishedrow(R3),
		       writenice([R1,R2,R3]), write('Board full.\n').
finishedrow([A,B,C]):- not(A=u), not(B=u), not(C=u).

not(P):- \+ P.

writenice([R1,R2,R3]):- writenicerow(R1), writeline, writenicerow(R2), writeline, writenicerow(R3), write('\n').
writenicerow([R1,R2,R3]):- writenicechar(R1), write('|'), writenicechar(R2), write('|'), writenicechar(R3), write('\n').
writenicechar(o):- write('O').
writenicechar(x):- write('X').
writenicechar(u):- write(' ').
writeline:- write('-+-+-\n').

readiffree(B,Pos):- (read(Pos), free(B,Pos), write('\n')); (write('Square not free.\n'), readiffree(B,Pos)).

free([R1,_,_],[1,C]):- freerow(R1,C).
free([_,R2,_],[2,C]):- freerow(R2,C).
free([_,_,R3],[3,C]):- freerow(R3,C).
freerow([u,_,_],1).
freerow([_,u,_],2).
freerow([_,_,u],3).

/* Attempt 1: First I dreamed up a player that looked forwards recursively and only
   played if it was a sure win.  Of course, that means he would never play a first move!
   (Hindsight: Could have done that and than put an arbitrary choose if there was no
   certainty of success, but didn't think of that!)
   
   Attempt 2: Thought instead the player should compare relative assets of each move
   (again recursively).  This require a complicated scoring system and got too confusing
   so I gave up.
   
   Attempt 3 (final): This player isn't so clever and can be beaten if human takes first
   move.  However, if he can see a move that will guarantee a win on his next move, he
   will play it.  You can see an example of this by letting him start and then choosing
   a square on an edge (as opposed to corner).  He will beat you! */

% Tries to choose winning square.
choose(o,B,Pos):- free(B,Pos), put(o,Pos,B,B2), win(o,B2).
% Tries to block opponent.
choose(o,B,Pos):- free(B,Pos), put(x,Pos,B,B2), win(x,B2).
% Tries to force win next go.
choose(o,B,Pos):- free(B,Pos), put(o,Pos,B,B2), forces(B2,B3),
		  free(B3,Pos2), put(o,Pos2,B3,B4), win(o,B4).
% Tries to choose middle.
choose(o,B,[2,2]):- free(B,[2,2]).
% Chooses arbitrarily.
choose(o,B,Pos):- free(B,Pos).
% Note: If finished then choose will fail, but that should never happen anyway.

% forces(B,B3): Does B force player x to play Pos, resulting in B3?
forces(B,B3):- free(B,Pos), put(o,Pos,B,B2), win(o,B2), put(x,Pos,B,B3).
