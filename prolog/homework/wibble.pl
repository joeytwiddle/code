isdog(lassie).

isgirl(anna).

isanimal(X):- isdog(X).

likes(anna,X):- isdog(X).

likes(chris,X):- isgirl(X).

likes(veronica,X):- likes(X,Y), isdog(Y).

likes(veronica,X):- likes(X,_).

likes(veronica,X):- isanimal(X).

likes(chris,X):- likes(anna,X); likes(X,anna).

likes(anna,veronica).

likes(tim,X):- likes(mark,X).

likes(mark,X):- likes(tim,X).
