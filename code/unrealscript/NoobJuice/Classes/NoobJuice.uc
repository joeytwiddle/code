class NoobJuice expands Mutator;

var String knownPlayers;
// var float nextCheck;

defaultproperties {
  knownPlayers=
  // nextCheck=0
}

function Mutate(String str, PlayerPawn Sender) {
  // a shortcut if your mapvote button is not working; just type: mutate mapvote
  if (str ~= "mapvote") {
    Level.Game.BaseMutator.Mutate("bdbmapvote votemenu",Sender);
  }
  Super.Mutate(str,Sender);
}

// function PostBeginPlay() {
function PreBeginPlay() {
  // local Pawn p;
  local Actor p;
  local Light l;
  // for (p=Level.PawnList; p!=None; p=p.NextPawn) {
  foreach AllActors(class'Actor', p) {
    if (p.IsA('Light')) {
      l = Light(p);
      l.LightHue = l.LightHue + 128;
      // Log("Changed "$l$".LightHue to "$l.LightHue);
      l.LightBrightness = l.LightBrightness * RandRange(25,175)/100;
      l.LightRadius = l.LightRadius * RandRange(25,175)/100;
    }
  }
  // Level.Game.BaseMutator.AddMutator(Self);
  SetTimer(50,True);
}

// event Tick(float DeltaTime) {
  // if (nextCheck > Level.TimeSeconds)
    // return;
  // CheckPlayerList();
  // nextCheck = Level.TimeSeconds + 10;
// }

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
  // p.ClientMessage("Welcome to "$ Left(""$Level.Game,InStr(""$Level.Game,".")) $" on noggin's noobJuice.");
}

/*
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
*/

