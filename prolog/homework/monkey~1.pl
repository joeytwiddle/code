/* A program for the monkey and banana problem
 * from Bratko's book page 55
 */

% move(State1, Move, State2):	makeing Move in State1 results State2
%	a state is represented by a term:
%	state(MonkeyHorizontal, MonkeyVertical, BoxPosition, HasBanana)

move(state(middle, onbox, middle, hasnot),
     grasp,
     state(middle, onbox, middle, has)).

move(state(P, onfloor, P, H),
     climb,
     state(P, onbox, P, H)).

move(state(P1, onfloor, P1, H),
     push(P1,P2),
     state(P2, onfloor, P2, H)).

move(state(P1, onfloor, B, H),
     walk(P1,P2),
     state(P2, onfloor, B, H)).

% canget(Stete):	monkey can get banana in State

canget(state(_,_,_,has)).		% can 1: Monkey already has it

canget(State1):-
    move(State1, _, State2),		% do something
    canget(State2).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  top query
%
%  ?- canget(state(atdoor, onfloor, atwindow, hasnot)).
% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/* the follwoing is the trace of running the above query

[3:25pm] lapu-rong-101-ExFmBook>sicstus
SICStus 2.1 #8: Fri Jul 30 16:00:41 BST 1993
| ?- [monkey_banana].
{consulting /home/acrc/users/staff/rong/koogi/ExFmBook/monkey_banana.pl...}
{/home/acrc/users/staff/rong/koogi/ExFmBook/monkey_banana.pl consulted, 10 msec 1152 bytes}

yes
| ?- trace.
{The debugger will first creep -- showing everything (trace)}

yes
{trace}
| ?- canget(state(atdoor, onfloor, atwindow, hasnot)).
 + 1  1  Call: canget(state(atdoor,onfloor,atwindow,hasnot)) ? 
 + 2  2  Call: move(state(atdoor,onfloor,atwindow,hasnot),_354,_355) ? 
 + 2  2  Exit: move(state(atdoor,onfloor,atwindow,hasnot),walk(atdoor,_564),state(_564,onfloor,atwindow,hasnot)) ? 
 + 3  2  Call: canget(state(_564,onfloor,atwindow,hasnot)) ? 
 + 4  3  Call: move(state(_564,onfloor,atwindow,hasnot),_949,_950) ? 
 + 4  3  Exit: move(state(atwindow,onfloor,atwindow,hasnot),climb,state(atwindow,onbox,atwindow,hasnot)) ? 
 + 5  3  Call: canget(state(atwindow,onbox,atwindow,hasnot)) ? 
 + 6  4  Call: move(state(atwindow,onbox,atwindow,hasnot),_1536,_1537) ? g
Ancestors:
 + 1  1  canget(state(atdoor,onfloor,atwindow,hasnot))
 + 3  2  canget(state(atwindow,onfloor,atwindow,hasnot))
 + 5  3  canget(state(atwindow,onbox,atwindow,hasnot))
 + 6  4  Call: move(state(atwindow,onbox,atwindow,hasnot),_1536,_1537) ? 
 + 6  4  Fail: move(state(atwindow,onbox,atwindow,hasnot),_1536,_1537) ? 
 + 5  3  Fail: canget(state(atwindow,onbox,atwindow,hasnot)) ? 
 + 4  3  Redo: move(state(atwindow,onfloor,atwindow,hasnot),climb,state(atwindow,onbox,atwindow,hasnot)) ? 
 + 4  3  Exit: move(state(atwindow,onfloor,atwindow,hasnot),push(atwindow,_1159),state(_1159,onfloor,_1159,hasnot)) ? 
 + 5  3  Call: canget(state(_1159,onfloor,_1159,hasnot)) ? 
 + 6  4  Call: move(state(_1159,onfloor,_1159,hasnot),_1544,_1545) ? 
 + 6  4  Exit: move(state(_1159,onfloor,_1159,hasnot),climb,state(_1159,onbox,_1159,hasnot)) ? 
 + 7  4  Call: canget(state(_1159,onbox,_1159,hasnot)) ? 
 + 8  5  Call: move(state(_1159,onbox,_1159,hasnot),_2131,_2132) ? 
 + 8  5  Exit: move(state(middle,onbox,middle,hasnot),grasp,state(middle,onbox,middle,has)) ? 
 + 9  5  Call: canget(state(middle,onbox,middle,has)) ? 
 + 9  5  Exit: canget(state(middle,onbox,middle,has)) ? 
 + 7  4  Exit: canget(state(middle,onbox,middle,hasnot)) ? 
 + 5  3  Exit: canget(state(middle,onfloor,middle,hasnot)) ? 
 + 3  2  Exit: canget(state(atwindow,onfloor,atwindow,hasnot)) ? 
 + 1  1  Exit: canget(state(atdoor,onfloor,atwindow,hasnot)) ? 

yes
{trace}
| ?- 

*******************************************************************/
