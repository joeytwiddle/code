class NoobJuice expands Mutator;

var config bool bChangeLighting;
var config bool bRandomSpeed;
var config bool bRandomFriendlyFire;

var String knownPlayers;
// var float nextCheck;

defaultproperties {
  bChangeLighting=True // doesn't work
  bRandomSpeed=False
  bRandomFriendlyFire=False
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
simulated function PreBeginPlay() {
  // local Pawn p;
  local Actor p;
  local Light l;
  local int i;

  if (bChangeLighting) {

    // TESTING: change lighting (i don't think this is having any effect; maybe if we did it on the client?)
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

  }

  if (bRandomSpeed) {

    // Change gamespeed
    // Level.Game.GameSpeed = 0.85 + Int(RandRange(0,2)) * (0.15 + 0.25*Int(RandRange(0,4)));
    i = Rand(100);
    if (i<5) {
      Level.Game.GameSpeed = 0.85;
    } else
    if (i<10) {
      Level.Game.GameSpeed = 0.9;
    } else
    if (i<15) {
      Level.Game.GameSpeed = 1.4;
    } else
    if (i<20) {
      Level.Game.GameSpeed = 1.5;
    } else
    if (i<40) {
      Level.Game.GameSpeed = 1.25;
    } else {
      Level.Game.GameSpeed = 1.0;
    }

  }

  if (Level.Game.IsA('TeamGamePlus') && bRandomFriendlyFire) {

    i = Rand(100);
    if (i<50) {
      TeamGamePlus(Level.Game).FriendlyFireScale = 0.0;
    } else
    if (i<80) {
      TeamGamePlus(Level.Game).FriendlyFireScale = 0.1;
    } else
    if (i<90) {
      TeamGamePlus(Level.Game).FriendlyFireScale = 0.25;
    } else {
      TeamGamePlus(Level.Game).FriendlyFireScale = 0.5;
    }

  }

  // Level.Game.BaseMutator.AddMutator(Self);

  // Start checker for new players:
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
	local String extra;
	if (Level.Game.GameSpeed == 100) {
		extra = "";
	} else {
		extra = " at " $ Int(Level.Game.GameSpeed*100) $ " speed";
	}
	p.ClientMessage("Welcome to "$ Left(""$Level.Game,InStr(""$Level.Game,".")) $ extra $ " on noggin's noobJuice.");
	// p.ClientMessage( "[hwi.ath.cx] playing " $ Left(""$Level.Game,InStr(""$Level.Game,".")) $ extra );
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
*/

