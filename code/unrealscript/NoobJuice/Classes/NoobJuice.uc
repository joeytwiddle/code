class NoobJuice expands Mutator;

var String knownPlayers;
var float nextCheck;

defaultproperties {
	knownPlayers=
	nextCheck=0
}

function PreBeginPlay() {
	Level.Game.BaseMutator.AddMutator(Self);
}

function void Tick() {
	if (nextCheck > Level.TimeSeconds)
		return;
	CheckPlayerList();
	nextCheck = Level.TimeSeconds + 10;
}

function void CheckPlayerList() {
	Pawn p;
	for (p=Level.PawnList; p!=None; p=p.NextPawn) {
		if (p.bIsPlayer && !p.IsA('Bot')) {
			if (InStr(knownPlayers," "$p.getHumanName()$" ")<0) {
				knownPlayers = " " $ p.getHumanName() $ " " $ knownPlayers;
				HandleNewPlayer(PlayerPawn(p));
			}
		}
	}
}

function void HandleNewPlayer(PlayerPawn p) {
	p.SendClientMessage("Welcome to "$Level.Game$" on noggin's noobJuice.");
}

function int SplitString(String str, String divider, out String parts[255]) {
   // local String parts[255];
   // local array<String> parts;
   local int i,nextSplit;
   i=0;
   while (true) {
      nextSplit = InStr(str,divider);
      if (nextSplit >= 0) {
         // parts.insert(i,1);
         parts[i] = Left(str,nextSplit);
         str = Mid(str,nextSplit+1);
         i++;
      } else {
         // parts.insert(i,1);
         parts[i] = str;
         i++;
         break;
      }
   }
   // return parts;
   return i;
}

