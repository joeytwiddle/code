top6to45(X) :- boy_names(X,1875,Y1), boy_names(X,1925,Y2),
	boy_names(X,1975,Y3), bound(Y1), bound(Y2), bound(Y3).
bound(Y) :- Y>5, Y=<45.