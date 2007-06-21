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
		Log("RandomMutators: Spawning and adding a new "$mutName);
		if (CheckForMut1(mutName)) {
			Log("RandomMutators: Refusing to add "$mut);
			continue;
		}
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
		Level.Game.BaseMutator.AddMutator(mut);
		if (chosenMutators != "") chosenMutators = chosenMutators $ ", ";
		// chosenMutators = chosenMutators $ Mid(""$mut,InStr(""$mut,".")+1);
		chosenMutators = chosenMutators $ Mid(""$mut,InStr(""$mut,".")+1,Len(""$mut)-Instr(""$mut,".")-2);
		// chosenMutators = chosenMutators $ mutName;
		// TODO CONSIDER: mut.PreBeginPlay();
	}
	SetTimer(51,True);
}

function bool CheckForMut1(String mutName) {
	local Mutator m;
	for (m=Level.Game.BaseMutator; m!=None; m=m.NextMutator) {
		if ((""$mutName) == (""$m.Class.Name)) {
			Log("RandomMutators.CheckForMut1(): Found already existing "$mutName$": "$m);
			return True;
		}
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
	// p.ClientMessage("Current mutators are: " $ chosenMutators);
	// p.ClientMessage("Welcome to "$ Left(""$Level.Game,InStr(""$Level.Game,".")) $" on noggin's noobJuice.");
	// p.ClientMessage("Currently playing " $ Left(""$Level.Game,InStr(""$Level.Game,".")) $ " with " $ chosenMutators $ ".");
	p.ClientMessage( "(hwi.ath.cx) playing " $ Left(""$Level.Game,InStr(""$Level.Game,".")) );
	p.ClientMessage( "with " $ chosenMutators $ "" );
}
