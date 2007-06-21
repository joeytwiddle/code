# . ASU/asu-main.sh

ulimit >&1

. ut_server.config

## I can only memo functions if the memo-ing scripts are functions:
. importshfn memo
. importshfn rememo

NL='\
'

list_installed_umods () {
(
sh asu.sh << !
u
l
x
x
!
) |
fromline "^> Installed Umods:$" |
toline "^$" |
drop 1 | chop 1 |
sed 's+^[ ]*++ ; s+ [^ ]*$++'
}


list_available_umods () {
(
sh asu.sh << !
u
i

x
x
!
) |
fromline "^Zip and Umod files in the Patches directory:$" |
drop 1 | toline "^$" |
sed "s+ *+$NL+g" | grep -v "^$"
}

uninstall_umod () {
UMOD="$1"
echo "Uninstalling umod \"$UMOD\"..." >&2
sh asu.sh >/dev/null << !
u
u
$UMOD
x
x
!
}

uninstall_all_umods () {
	list_installed_umods |
	while read UMOD
	do uninstall_umod "$UMOD"
	done
}

GAME_TYPE=2 ## CTF
# GAME_TYPE=4 ## AS
# GAME_TYPE=3 ## DOM (for FlagDOM but didn't work)

list_maps () {
install_init_script |
fromline "^Enter the game type" |
fromline "^>" | drop 1 | toline "^$" | chop 1 |
sed "s+, +$NL+g" |
grep -v "^$"
}

list_mutators () {
install_init_script |
fromline "^Select a map to start the game with" |
fromline "^>" | drop 1 | toline "^$" | chop 1 |
sed "s+ +$NL+g" |
grep -v "^$" |
tee "available_mutators.asu.txt" ## optional, just interesting
}

install_init_script () {
## Nothing worked: START_MAP=`echo "$START_MAP" | sed "s+'+\\\\\\\\'+g"`
sh asu.sh << !
i
oddjob2
$ADVERTISE_IP
$GAME_TYPE
$START_MAP
$MUTATORS
y
x
!
}

# echo "--- Maps ------------------------------------------------"
# list_maps
# echo "--- Mutators --------------------------------------------"
# list_mutators
# echo "--- Installed Umods -------------------------------------"
# list_installed_umods
# echo "--- Available Umods -------------------------------------"
# list_available_umods
# echo "---------------------------------------------------------"
# echo

speshl_mutator_filter () {
	grep -v "$PROBLEMATIC_MUTATORS_REGEXP"
}
# Radiated.Radiated
# Payne.PayneMutator
## Oh no Bunnies don't work
# BunnyBomber.BunnyBombArena
# FragGrab.FragGrab
# EIChallengeHUD.EIChallengeHUD
# Radar.Radar_Mutator
# FlagDom.FDGame
# MultiCTF.MultiCTFKeyBindings
# MultiCTF.MCTFServerTab
## Need ChaosUT:
# EIBotpackUpgrade.NewWeaponsMutator
# EIBotpackUpgrade.UpdateArenaMutator
# SLV2.StrangeMutator
## I think either seekersM.aliens or seekers.Mechmod gives us crazy monsters  .. former former!

export IKNOWIDONTHAVEATTY=true
## because I was getting NO_TTY warnings; either cos list_maps is a fn., but more likely cos ucc.init had bg'ed itself, and then called this.

# ADVERTISE_IP=`ppp-getip`
ADVERTISE_IP=192.168.11.2
# ADVERTISE_IP=127.0.0.1
ALL_MAPS=`memo -d "./restored_from_cache/" -d "./ut-server/Maps/" list_maps`
ALL_MUTATORS=`memo -t "10 minutes" list_mutators`

apply_a_random_configuration () {

	echo "Generating random config..."

	[ "$MAP_GREP" ] || MAP_GREP="."
	START_MAP=`echo "$ALL_MAPS" | grep -i "$MAP_GREP" | chooserandomline`
	# jshinfo "With MAP_GREP=$MAP_GREP, got START_MAP=$START_MAP"
	## Dodgy/nasty catch for if GREP failed to produce any maps:
	## Dodgy could inf recurse if we never find a START_MAP :P
	# if [ ! "$START_MAP" ] || echo "$START_MAP" | grep "[\`']" ## cos apostropes cause init to break
	## This should fix it:
	if [ "$MAP_GREP" ] && ( [ ! "$START_MAP" ] || echo "$START_MAP" | grep "[\`']" ) ## cos apostropes cause init to break
	then
		MAP_GREP=""
		apply_a_random_configuration
		return
	fi
	# MUTATORS="$ALWAYS_MUTATORS"
	AVAILABLE_MUTATORS=`echo "$ALL_MUTATORS" | speshl_mutator_filter`
	AVAILABLE_MUTATORS_REGEXP=`echo "$AVAILABLE_MUTATORS" | sed 's+^+^+ ; s+$+$+' | list2regexp`
	# MUTATORS=`echo "$ALWAYS_MUTATORS" | grep "$AVAILABLE_MUTATORS_REGEXP"`"
# "
	## For the sake of SmartCTF, we now add ALWAYS_MUTATORS at the end of the mutator list: (er lies!)
	MUTATORS="$ALWAYS_MUTATORS"
	# MUTATORS=""
	if [ "$ADD_RANDOM_MUTATORS" ]
	then
		for X in `seq 1 $NUM_MUTATORS`
		do
			MUTATOR=`echo "$AVAILABLE_MUTATORS" | grep -v "^$" | chooserandomline`
			## Next line prevents duplicates:
			echo "$MUTATORS" | grep "^$MUTATOR$" > /dev/null ||
			MUTATORS="$MUTATORS$MUTATOR
"
		done
	fi
	# MUTATORS="$MUTATORS$ALWAYS_MUTATORS"

	(
		echo "---- Random config generated:"
		echo "Start map = $START_MAP"
		echo "Mutators ="
		echo -n "$MUTATORS" | sed 's+^+  +'
	) | tee generated_config.last
	echo

	echo "Installing init script..."
	cp ./ut-server/ucc.init ./ut-server/ucc.init.`geekdate`
	install_init_script >/dev/null
	# cursecyan ; install_init_script ; cursenorm
	diff ./ut-server/ucc.init ./ut-server/ucc.init.`geekdate` | diffhighlight
	echo "Done."

}

wait_for_conclusion () {
	LOGFILE="ut-server/Logs/ucc.init.log"

	# /usr/bin/tail -n 1000 -F "$LOGFILE" &
	# OUT=`
		# /usr/bin/tail -n 1000 -F "$LOGFILE" |
		# # pipeboth |
		# # dos2unix |
		# grep -u -m 1 "^\(Game engine initialized\|Exiting\|Aborting\)" |
		# head -n 1
		# exit
	# `
	# echo "OUT=$OUT" >&2
	# if [ "$OUT" = "Game engine initialized" ]
	# then return 0
	# else return 1
	# fi

	## Old:
	while true
	do
		## Good:
		if grep '^Game engine initialized$' "$LOGFILE"
		then
			# break
			return 0
		fi
		## Bad:
		if grep '^\(Exiting\|Aborting\).$' "$LOGFILE"
		then
			cursered
			tail -10 "$LOGFILE" | sed "s+^Failed.*+`cursebold`\0`cursered`+"
			cursenorm
			# break
			return 1
		fi
		printf "." >&2
		sleep 1
	done
	# echo >&2 ## => by successful grep above
	return 3
}

try_random_configs_until_one_works () {

	## Disabled the loop cos ucc.init now call this in recursive loop.  Nice!

	for ATTEMPT_NUM in `seq 1 20`
	do

		apply_a_random_configuration

		## Restart:
		## This continue catches the case when ucc-bin does not start at all (because init script is broken, often due to funny unescaped map name).
		# /usr/local/install/games/ut_server/ut-server/ucc.init restart || continue
		/home/oddjob2/ut_server/ut-server/ucc.init restart || continue
		## Soft doesn't bring any advantages I can see, and doesn't start infinity :(
		# /usr/local/install/games/ut_server/ut-server/ucc.init soft-restart || continue
		## What was it before soft-restart?  I'm not sure that starts infinity :(
		## Doesn't change anything:
		# sh ./really_soft_restart.sh

		## Soft restart: (doesn't change startmap, probably doesn't change mutators, or anything!)
		# lynx "http://82.33.113.69:5080/ServerAdmin/defaults?GameType=Botpack.CTFGame&Page=defaults_restart"
		# wget --http-user="nogginBasher" --http-passwd="wibble" "http://82.33.113.69:5080/ServerAdmin/defaults?GameType=Botpack.CTFGame&Page=defaults_restart" -O -
		# wget -nv --tries=2 --timeout=2 --connect-timeout=2 --http-user="nogginBasher" --http-passwd="wibble" "http://82.33.113.69:5080/ServerAdmin/defaults_restart?GameType=Botpack.CTFGame" -O -
		# [ "$?" = 0 ] || /usr/local/install/games/ut_server/ut-server/ucc.init restart || continue

		# wait_for_conclusion
		## Last line of .../ut-server/Logs/ucc.init.log should be "Game engine initialized"
		# if ps -A | grep ucc-bin

		if wait_for_conclusion
		then
			echo "Attempt $ATTEMPT_NUM succeeded."
			break
		else
			echo "Attempt $ATTEMPT_NUM failed."
			echo
		fi

	done
	[ "$LOGFILE" ] && echo "Logfile is $LOGFILE"

}



# uninstall_all_umods

MAP_GREP="$1"
NUM_MUTATORS=10

# MAP_GREP=revenge-le
# MAP_GREP=incineratorle

try_random_configs_until_one_works

## Faster, use previous ucc.init: (been getting problems using this)
# /home/oddjob2/ut_server/ut-server/ucc.init restart
# wait_for_conclusion

## TODO: Apply (by export?) our map and mutator choice to ucc.init

## For NJWA's status:
sh ./mutate_via_http.sh listactive > active_mutators.lastboot
cat generated_config.last | grep "^ " | sed 's+^[ ]*++' > active_mutators.all.lastboot

