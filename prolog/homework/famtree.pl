/* The family program from Bratko's book, page 19
 */ 

parent(pam,bob).                      %Pam is a parent of Bob
parent(tom,bob).
parent(tom,liz).
parent(bob,ann).
parent(bob,pat).
parent(pat,jim).

female(pam).                          %Pam is female
male(tom).                            %Tom is male
male(bob).
female(liz).
female(ann).
female(pat).
male(jim).

offspring(Y,X):-                      %Y is an offspring of X if
                 parent(X,Y).         %X is a parent of Y

mother(X,Y):-                         %X is the mother of Y if
                 parent(X,Y),         %X is a parent of Y and
                 female(X).           %X is female

grandparent(X,Z):-
                 parent(X,Y),
                 parent(Y,Z).

sister(X,Y):-
                 parent(Z,X),
                 parent(Z,Y),
                 female(X),
                 X \== Y.

predecessor(X,Z):-
                 parent(X,Z).

predecessor(X,Z):-
                 parent(X,Y),
                 predecessor(Y,Z).
