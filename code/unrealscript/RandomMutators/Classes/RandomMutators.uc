class RandomMutators expands Mutator;

var config String mutatorList;
var config int numToAdd;

defaultproperties {
	mutatorList=Botpack.LowGrav,FastCap.FC_Mutator,NevPack.AutoCannon,Radiated.Radiated,BotPack.FatBoy
	numToAdd=3
}

function PreBeginPlay() {
	local int i;
	local String mutName;
	local Mutator mut;
	for (i=0;i<numToAdd;i++) {
		mutName = getRandomMutator();
		Log("RandomMutators: Spawning and adding a new "$mutName);
		mut = Spawn( class<mutator>(DynamicLoadObject(mutName, class'Class')) );
		Level.Game.BaseMutator.AddMutator(mut);
	}
}

function String getRandomMutator() {
	// return class'Botpack.SuperShockRifle';
	local String mutators[255];
	local int mutatorsCount;
	// local class<weapon> w;
	mutatorsCount = SplitString(mutatorList,",",mutators);
	// C = class<ChallengeBotInfo>(DynamicLoadObject("Botpack.ChallengeBotInfo", class'Class'));
	// return Class(DynamicLoadObject(weapons[ RandRange(0,weaponsCount) ], class'Class'));
	// w = class<weapon>(DynamicLoadObject(weapons[ RandRange(0,weaponsCount) ], class'Class'));
	// Log("ArenaFallback: getRandomWeaponClass() returning: "$w);
	// Log("["$depth$"] 2 ! choosing "$w);
	// return w;
	return mutators[ RandRange(0,mutatorsCount) ];
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

