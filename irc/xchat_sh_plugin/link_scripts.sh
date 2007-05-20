# for TARGET in $HOME/.xchat2 $HOME/.xchat2.nogginBasher $HOME/.xchat2."{FuB}"karma
for TARGET in $HOME/.xchat2*
do
	cd "$TARGET" && rmlink *
	# find $HOME/j/code/other/irc/xchat/scripts -type f |
	find $HOME/j/code/other/irc/xchat -type f -or -type l -maxdepth 1 -name "*.pl" |
	while read FILE
	do ln -s "$FILE" "$TARGET"/
	done
done
