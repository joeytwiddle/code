class TeamSwitcher expands Mutator;

// DONE: For SONY_sCarface, add "!vote"

function PostBeginPlay() {
  Level.Game.RegisterMessageMutator(Self);
  Super.PostBeginPlay();
}

// Catch messages from spectators:
function bool MutatorBroadcastMessage(Actor Sender, Pawn Receiver, out coerce string Msg, optional bool bBeep, out optional name Type) {
  if (Sender == Receiver && Sender.IsA('Spectator')) { // Only process the message once.
    CheckMessage(Mid(Msg,InStr(Msg,":")+1), PlayerPawn(Sender));
  }
  return Super.MutatorBroadcastMessage(Sender,Receiver,Msg,bBeep,Type);
}

// Catch messages from players:
function bool MutatorTeamMessage(Actor Sender, Pawn Receiver, PlayerReplicationInfo PRI, coerce string Msg, name Type, optional bool bBeep) {
  if (Sender == Receiver && Sender.IsA('PlayerPawn')) { // Only process each message once.
    if (Left(Msg,1)=="!") { // optional - TODO TEST: what if msg=""?
      CheckMessage(Msg, PlayerPawn(Sender));
    }
  }
  return Super.MutatorTeamMessage(Sender,Receiver,PRI,Msg,Type,bBeep);
}

function CheckMessage(String Msg, PlayerPawn Sender) {

  if (Msg ~= "!RED") {
    ChangePlayerToTeam(Sender,0);
  }

  if (Msg ~= "!BLUE") {
    ChangePlayerToTeam(Sender,1);
  }

  if (Msg ~= "!GREEN") {
    ChangePlayerToTeam(Sender,2);
  }

  if (Msg ~= "!GOLD" || Msg ~= "!YELLOW") {
    ChangePlayerToTeam(Sender,3);
  }

  if (Msg ~= "!SPEC" || Msg ~= "!SPECTATE") {
    if (!Sender.IsA('Spectator')) {
      Sender.PreClientTravel(); // not sure if this is actually needed
      Sender.ClientTravel("?OverrideClass=Botpack.CHSpectator",TRAVEL_Relative, False);
    }
  }

  if (Msg ~= "!PLAY") {
    if (Sender.IsA('Spectator')) {
      Sender.PreClientTravel(); // not sure if this is actually needed
      Sender.ClientTravel("?OverrideClass=",TRAVEL_Relative, False);
    }
  }

  if (Msg ~= "!VOTE" || Msg ~= "!MAPVOTE") {
    Level.Game.BaseMutator.Mutate("bdbmapvote votemenu",Sender);
  }

  if (Msg ~= "!STATS") {
      Level.Game.BaseMutator.Mutate("smartctf stats",Sender);
  }

}

function ChangePlayerToTeam(Pawn p, int teamnum) {
  if (teamnum == p.PlayerReplicationInfo.Team) {
    return;
  }
  Level.Game.ChangeTeam(p,teamnum);
  p.Died(None, '', p.Location); // Kills the player, forcing them to drop flag if they have it
}

