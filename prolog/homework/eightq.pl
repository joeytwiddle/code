/*
 * Three Versions of Eight Queens program
 */

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Version 1 		from page 117
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% top call and the first solustion
%
% ?- template(S), sol_1(S).
%
% S = [1/4,2/2,3/7,4/3,5/6,6/8,7/5,8/1] ?

sol_1([]).
sol_1([X/Y | Others]):-
    sol_1(Others),
    member(Y, [1,2,3,4,5,6,7,8]),
    noattack_1(X/Y, Others).

noattack_1(_, []).
noattack_1(X/Y, [X1/Y1 | Others]):-
    Y =\= Y1,
    Y1-Y =\= X1-X,
    Y1-Y =\= X-X1,
    noattack_1(X/Y, Others).

member(Item, [Item|_]).
member(Item, [_|Rest]):- member(Item, Rest).

% A solution template

template([1/Y1, 2/Y2, 3/Y3, 4/Y4, 5/Y5, 6/Y6, 7/Y7, 8/Y8]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Version 2		 from page 119
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% top call and the first solustion
%
% ?- sol_2(Q).
%
% Q = [5,2,6,1,7,4,8,3] ? 

sol_2(Q):-
    permutation([1,2,3,4,5,6,7,8], Q),
    safe(Q).

permutation([],[]).
permutation([H|T],PermList):-
    permutation(T,PermTail),
    del(H,PermList,PermTail).	% insert H in permuted T

% del(Item, List, NewList): deleting Item from LIst gives NewList

del(Item, [Item|L], L).
del(Item, [First|L], [First|L1]):-
    del(Item, L, L1).

safe([]).
safe([Q|Others]):-
    safe(Others),
    noattack_2(Q,Others,1).

noattack_2(_,[],_).
noattack_2(Y,[Y1|Ylist],Xdist):-
    Y1-Y =\= Xdist,
    Y-Y1 =\= Xdist,
    Dist1 is Xdist + 1,
    noattack_2(Y, Ylist, Dist1).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Version 3		from page 121
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% top call and the first solustion
%
% | ?- sol_3(Q).
%
% Q = [1,5,8,6,3,7,2,4] ? 

sol_3(Ylist):-
    sol(Ylist,
      [1,2,3,4,5,6,7,8],
      [1,2,3,4,5,6,7,8],
      [-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7],
      [2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]).

sol([],[],Dy,Du,Dv).
sol([Y|Ylist],[X|Dx1],Dy,Du,Dv):-
    del(Y,Dy,Dy1),
    U is X-Y,
    del(U,Du,Du1),
    V is X+Y,
    del(V,Dv,Dv1),
    sol(Ylist, Dx1,Dy1,Du1,Dv1).

