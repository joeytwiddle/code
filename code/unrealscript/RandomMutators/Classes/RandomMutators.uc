class RandomMutators expands Mutator;

var config String mutatorList;
var config int numToAdd;

defaultproperties {
	mutatorList="Botpack.LowGrav,FastCap.FC_Mutator,NevPack.AutoCannon,Radiated.Radiated,BotPack.FatBoy,CrouchBlocksDamage.CrouchBlocksDamage,WhoPushedMe.WhoPushedMe,BatThwap.BatThwap,ArenaFallback.ArenaFallback"
	// mutatorList="Botpack.LowGrav,NevPack.AutoCannon,Radiated.Radiated,BotPack.FatBoy"
	numToAdd=3
	chosenMutators=
	knownPlayers=
	// nextCheck=0
}

var String chosenMutators;
var String knownPlayers;
// var float nextCheck;

function PreBeginPlay() {
	local int i;
	local String mutName;
	local Mutator mut;
	for (i=0;i<numToAdd;i++) {
		mutName = getRandomMutator();
		Log("RandomMutators: Spawning and adding a new "$mutName);
		mut = Spawn( class<mutator>(DynamicLoadObject(mutName, class'Class')) );
		Level.Game.BaseMutator.AddMutator(mut);
		if (chosenMutators != "") chosenMutators = chosenMutators $ ", ";
		// chosenMutators = chosenMutators $ Mid(""$mut,InStr(""$mut,".")+1);
		// chosenMutators = chosenMutators $ Mid(""$mut,InStr(""$mut,".")+1,Len(""$mut)-Instr(""$mut,".")-2);
		chosenMutators = chosenMutators $ mutName;
	}
	SetTimer(51,True);
}

// function PostBeginPlay() {
	// Level.Game.BaseMutator.AddMutator(Self);
  // SetTimer(55,True);
// }

function String getRandomMutator() {
	// return class'Botpack.SuperShockRifle';
	local String mutators[255];
	local int mutatorsCount;
   local String mut;
	// local class<weapon> w;
	mutatorsCount = SplitString(mutatorList,",",mutators);
	// C = class<ChallengeBotInfo>(DynamicLoadObject("Botpack.ChallengeBotInfo", class'Class'));
	// return Class(DynamicLoadObject(weapons[ RandRange(0,weaponsCount) ], class'Class'));
	// w = class<weapon>(DynamicLoadObject(weapons[ RandRange(0,weaponsCount) ], class'Class'));
	// Log("ArenaFallback: getRandomWeaponClass() returning: "$w);
	// Log("["$depth$"] 2 ! choosing "$w);
	// return w;
	// C = class<ChallengeBotInfo>(DynamicLoadObject("Botpack.ChallengeBotInfo", class'Class'));
	mut = mutators[ RandRange(0,mutatorsCount) ];
   // mut = Left(mut,InStr(mut,"_")) $ "." $ Mid(mut,Instr(mut,"_")+1);
   return mut;
}

/*
function int SplitString(String str, String divider, out String parts[255]) {
   // local String parts[255];
   // local array<String> parts;
   local String tmp;
   local int i,nextSplit;
   tmp = str;
   i=0;
   while (true) {
      nextSplit = InStr(str,divider);
      if (nextSplit >= 0) {
         // parts.insert(i,1);
         parts[i] = Left(str,nextSplit);
         Log("parts["$i$"] = "$parts[i]);
         str = Mid(str,nextSplit+1);
         i++;
      } else {
         // parts.insert(i,1);
         parts[i] = str;
         Log("parts["$i$"] = "$parts[i]);
         i++;
         break;
      }
   }
   return i;
}
*/

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



event Timer() {
   CheckPlayerList();
}

function CheckPlayerList() {
	local Pawn p;
	for (p=Level.PawnList; p!=None; p=p.NextPawn) {
		if (p.bIsPlayer && !p.IsA('Bot')) {
			if (InStr(knownPlayers," "$p.getHumanName()$" ")<0) {
				knownPlayers = " " $ p.getHumanName() $ " " $ knownPlayers;
				HandleNewPlayer(PlayerPawn(p));
			}
		}
	}
}

function HandleNewPlayer(PlayerPawn p) {
	// p.ClientMessage("Welcome to "$Level.Game$" on noggin's noobJuice.");
	p.ClientMessage("Current mutators are: " $ chosenMutators);
}
