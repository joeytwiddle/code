find ut-server/Maps/ -iname "*.unr*" |
while read FILE
do
	# echo "`basename "$FILE"`:"
	utdep.pl "$FILE" |
	while read TARGET
	do
		find ut-server/ -iname "$TARGET" | grep . > /dev/null ||
		echo "Missing: `basename "$FILE"` -> $TARGET"
	done
	# echo
done

