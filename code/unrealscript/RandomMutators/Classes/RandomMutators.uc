class RandomMutators expands Mutator config(RandomMutators);

var config String mutatorList;
var config int numToAdd;

defaultproperties {
	mutatorList="Botpack.LowGrav,Botpack.JumpMatch,Botpack.FlakArena,Botpack.BioArena,Botpack.MinigunArena"
	// mutatorList="Botpack.LowGrav,FastCap.FC_Mutator,NevPack.AutoCannon,Radiated.Radiated,BotPack.FatBoy,CrouchBlocksDamage.CrouchBlocksDamage,WhoPushedMe.WhoPushedMe,BatThwap.BatThwap,ArenaFallback.ArenaFallback"
	numToAdd=2
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
		if (CheckForMut1(mutName)) {
			Log("RandomMutators: Refusing to add "$mutName);
			continue;
		}
		Log("RandomMutators: Spawning and adding a new "$mutName);
		mut = Spawn( class<mutator>(DynamicLoadObject(mutName, class'Class')) );
		if (mut == None) {
			Log("RandomMutators:   x Failed.");
			continue;
		}
		if (CheckForMut2(mut)) {
			Log("RandomMutators: Refusing to add "$mut);
			mut.Destroy();
			continue;
		}

		// This will usually add the mutator at the end of the chain:
		// Level.Game.BaseMutator.AddMutator(mut);

		// Add at start (dirty hack!):
		// mut.NextMutator = Level.Game.BaseMutator.NextMutator;
		// Level.Game.BaseMutator.NextMutator = mut;

		// Add after this mutator:
		mut.NextMutator = Self.NextMutator;
		Self.NextMutator = mut;

		if (chosenMutators != "") chosenMutators = chosenMutators $ ", ";
		// chosenMutators = chosenMutators $ Mid(""$mut,InStr(""$mut,".")+1);
		chosenMutators = chosenMutators $ Mid(""$mut,InStr(""$mut,".")+1,Len(""$mut)-Instr(""$mut,".")-2);
		// chosenMutators = chosenMutators $ mutName;
		// TODO CONSIDER: mut.PreBeginPlay();
	}
	SetTimer(51,True);
	Super.PreBeginPlay();
}

function bool CheckForMut1(String mutName) {
	local Mutator m;
	for (m=Level.Game.BaseMutator; m!=None; m=m.NextMutator) {
		if ((""$mutName) == (""$m.Class)) {
			Log("RandomMutators.CheckForMut1(): Found already existing "$mutName$": "$m);
			return True;
		}
		// Log("RandomMutators.CheckForMut1(): "$mutName$" != "$m.Class);
	}
	return False;
}

function bool CheckForMut2(Mutator mut) {
	local Mutator m;
	for (m=Level.Game.BaseMutator; m!=None; m=m.NextMutator) {
		if (mut.Class.Name == m.Class.Name) {
			Log("RandomMutators.CheckForMut2(): Found duplicate of "$mut$": "$m);
			return True;
		}
	}
	return False;
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
	// return Class(DynamicLoadObject(weapons[ Rand(weaponsCount) ], class'Class'));
	// w = class<weapon>(DynamicLoadObject(weapons[ Rand(weaponsCount) ], class'Class'));
	// Log("ArenaFallback: getRandomWeaponClass() returning: "$w);
	// Log("["$depth$"] 2 ! choosing "$w);
	// return w;
	// C = class<ChallengeBotInfo>(DynamicLoadObject("Botpack.ChallengeBotInfo", class'Class'));
	mut = mutators[ Rand(mutatorsCount) ];
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
         str = Mid(str,nextSplit+Len(divider));
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
         str = Mid(str,nextSplit+Len(divider));
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
	local Mutator mut;
	local String mutators;
	// p.ClientMessage("Welcome to "$Level.Game$" on noggin's noobJuice.");
	// p.ClientMessage("Welcome to "$ Left(""$Level.Game,InStr(""$Level.Game,".")) $" on noggin's noobJuice.");
	// p.ClientMessage("Current mutators are: " $ chosenMutators);
	// p.ClientMessage("Welcome to "$ Left(""$Level.Game,InStr(""$Level.Game,".")) $" on noggin's noobJuice.");
	// p.ClientMessage("Currently playing " $ Left(""$Level.Game,InStr(""$Level.Game,".")) $ " with " $ chosenMutators $ ".");
	local String extra;
	if (Level.Game.GameSpeed == 100) {
		extra = "";
	} else {
		extra = " at " $ Int(Level.Game.GameSpeed*100) $ " speed";
	}
	// p.ClientMessage("Welcome to "$ Left(""$Level.Game,InStr(""$Level.Game,".")) $ extra $ " on noggin's noobJuice.");
	// p.ClientMessage( "[hwi.ath.cx] playing " $ Left(""$Level.Game,InStr(""$Level.Game,".")) $ extra );
	// p.ClientMessage( "with: " $ chosenMutators $ "" );

	p.ClientMessage( "Mutators are: " $ chosenMutators $ "" );

	/*
	// all mutators:
	// It's just too many!
	mutators = "";
	for (mut=Level.Game.BaseMutator; mut!=None; mut=mut.NextMutator) {
		if (mutators != "")
			mutators = mutators $ ", ";
		mutators = mutators $ mut.Name;
	}
	p.ClientMessage( "Mutators are: " $ mutators $ "" );
	*/

}

