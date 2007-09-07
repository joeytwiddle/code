sh ./preprocess.sh
cd ../../System
del AutoTeamBalance.u
# wine ucc make ini=UnrealTournament_compiling.ini
wine ucc make ini=../AutoTeamBalance/Classes/make.ini
