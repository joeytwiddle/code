if test "$*" = ""; then
	KEYS="UP DOWN LEFT RIGHT F10 SPACE q w e r t y u i o p a s d f g h j k l z x c v b n m"
	MODS="LCTRL ALT"
else
	while test "$1" != "" && test "$1" != "-m"; do
		KEYS="$KEYS$1 "
		shift
	done
	if test "$1" = "-m"; then
		shift
		while test "$1" != ""; do
			MODS="$MODS$1 "
			shift
		done
	fi
fi

(

	echo "// Variables"
	for KEY in $KEYS $MODS; do
		echo "bool $KEY""pressed = 0;"
	done

	echo

	echo "// Checking"
	echo "bool observeEvent(SDL_Event event) {"
	echo "	if ( event.type == SDL_KEYDOWN || event.type == SDL_KEYUP ) {"
	echo "		bool position = ( event.type == SDL_KEYDOWN );"
	for KEY in $KEYS; do
		echo "		if ( event.key.keysym.sym == SDLK_$KEY ) {"
		echo "			$KEY""pressed = position;"
		# echo "			printf(\"$KEY""pressed = %i\\n\",position);"
		echo "		}"
	done
	for MOD in $MODS; do
		echo "		if ( event.key.keysym.mod == KMOD_$MOD ) {"
		echo "			$MOD""pressed = position;"
		# echo "			printf(\"$MOD""pressed = %i\\n\",position);"
		echo "		}"
	done
	echo "		return true;"
	echo "	}"
	echo "	return false;"
	echo "}"

) > keys.c
