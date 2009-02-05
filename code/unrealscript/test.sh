### Now done in 'ut' script
# ## Disable music if any is playing:
for PLAYAPP in mpg123 mplayer xmms nspluginviewer firefox-bin # ...
do
	if findjob "$PLAYAPP" | grep . >/dev/null
	then UT_OPTIONS="$UT_OPTIONS -nomusic"
	fi
done

UT_OPTIONS="$UT_OPTIONS -windowed"
# UT_OPTIONS="$UT_OPTIONS -pretty"

[ "$RUNNING_GENTOO" = 1 ] && DEBDO="debdo"

if $DEBDO sh ./build.sh
then

	cd /mnt/hdb6/ut_server
	# xterm -e sh ./run_server_once & # | higrep kx -C3 &
	# bigwin "sh ./run_server_once | higrep kx -C3"
	# xterm -geometry 170x50 -bg "#eeeeaa" -fg "#aa4400" -e "sh ./run_server_once | tee server.log ; read L" &
	xterm -geometry 170x50 -bg "#eeeeaa" -fg "#aa4400" -e "sh ./run_server_once | tee server.log | highlight kx magenta ; read L" &

	cd ~
	# ut $UT_OPTIONS -log 2>&1
	ut $UT_OPTIONS -log 2>&1
	# ut $UT_OPTIONS -log 2>&1 | higrep kx -C2

fi
