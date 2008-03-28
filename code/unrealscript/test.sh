cd "`dirname "$0"`"/System

# MAP=CTF-Noggins2ndMap-15
# MAP=CTF-CivilWar2
# MAP=CTF-PureAction-HalfSize
# MAP=CTF-Bleak-HalfSize
# MAP=CTF-Bleak-DoubleSize
# MAP=CTF-ThornsPortal-0801
# MAP=CTF-Bleak-CE100
MAP=AS-Mazon
# MAP=CTF-ButcheredNW-002
# MAP=CTF-Gazpacho
# MAP=CTF-AzcanizeBETA
# MAP=CTF-AzcanizeBETA125
# MAP=CTF-AzcanizeBETA150
# MAP=CTF-AzcanizeBETALLc
# MAP=CTF-2on2-Crates
# MUTATORS="AutoTeamBalance.AutoTeamBalance,MapVoteLA13.BDBMapVote,SiegeAnywhere.SiegeForever,FastBuildXXL2dRC1.FastBuildMut"
# MUTATORS="AutoTeamBalance.AutoTeamBalance,MapVoteLA13.BDBMapVote,SiegeAnywhere.SiegeForever"
MUTATORS="MapVoteLA13.BDBMapVote"
# SiegeAnywhere.SiegeAnywhere,
# MapVoteLA13.BDBMapVote,
# ,FastBuildXXL2dRC1.FastBuildMut
# ,Resize.ResizeTest
# SiegeAnywhere.SiegeAnywhere,
# ,FastBuildXXL2dRC1.FastBuildMut
# ,SiegeAS.SiegeASMut
# 
# ,ActorEditor.ActorEditor,XDScoringRules.XDScoringRules,Resize.MarioPups
# 
# SniperRealism.SniperRealism,
# GrapHookND8.GrapMut,
# Translocam.TranslocamM,
# PubliciseScore.PubliciseScore,
## XOL: MYMODS=Botpack.NoRedeemer,KickIdlersV2b.KickIdlers,AntiShockHoPlusBeta3.AntiShockHo,ARskIDC.AntiRespawnkilling,Fate.FateNoFloodChat,Fate.FateNoCows,TestPack.CatchAllMutates,XDScoringRules.XDScoringRules,Teamspeak03c.TeamspeakMutator,AutoTeamBalance.AutoTeamBalance
# GAMETYPE=Botpack.CTFGame
GAMETYPE=Botpack.Assault
# GAMETYPE=SiegeXXL2dRC1.SiegeGI
# GAMETYPE=SiegeAnywhere.SiegeCTF
# GAMETYPE=SiegeAnywhere.SiegeAssault
# GAMETYPE="FastBuildXXL2dRC1.FastBuildGI"

if [ "$WINDIR" ]
then TERMX="rxvt -geometry 160x10 -font lucidatypewriter-10 -sl 10 -e cmd /c" ; UT="UnrealTournament.exe"
else TERMX="xterm -e" ; UT="ut -pretty_soft"
fi

## Start server:
# # rxvt -e "cd System && cmd /c start_server.bat" &
# rxvt -sl 1000 -e cmd /c start_server.bat &
# rxvt -geometry 160x20 -font lucidatypewriter-10 -sl 1000 -e cmd /c ucc server "$MAP?game=Botpack.CTFGame?mutator=$MUTATORS" -log=server.log &
$TERMX ucc server "$MAP?game=$GAMETYPE?mutator=$MUTATORS" -log=server.log ini=UnrealTournament.ini &

# rxvt -sl 1000 -e unj tail -n 200 -f UNREALTOURNAMENT.log &

## Start client:
# cmd /c UnrealTournament.exe unreal://127.0.0.1/?password=wibble
# # rxvt -sl 1000 -e cmd /c UnrealTournament.exe unreal://127.0.0.1/?password=wibble
# rxvt -geometry 60x10 -sl 10 -e cmd /c UnrealTournament.exe unreal://127.0.0.1/?password=wibble &
# rxvt -geometry 60x10 -sl 10 -e cmd /c UnrealTournament.exe "unreal://127.0.0.1/?password=wibble" &
$TERMX $UT "unreal://127.0.0.1?password=wibble" &

# cmd /c UnrealTournament.exe "$MAP?game=$GAMETYPE?mutator=$MUTATORS" &
# unj tail -f UNREALTOURNAMENT.log

# sleep 2
# unj tail -n 20 server.log ; echo

