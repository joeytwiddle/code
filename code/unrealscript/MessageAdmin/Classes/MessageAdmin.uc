class MessageAdmin expands Mutator config(MessageAdmin);

var config String Message[256];

function PostBeginPlay() {
	Level.Game.RegisterMessageMutator(Self);
	Super.PostBeginPlay();
}

function ModifyPlayer(Pawn p) {
	Super.ModifyPlayer(p);
	if (PlayerPawn(p)!=None && PlayerPawn(p).PlayerReplicationInfo.Deaths == 0) {
		p.ClientMessage("You can send a message to the server admin by saying: !admin <your_message>");
	}
}

function Mutate(String msg, PlayerPawn Sender) {
	Super.Mutate(msg,Sender);
}

function bool MutatorBroadcastMessage(Actor Sender, Pawn Receiver, out coerce string Msg, optional bool bBeep, out optional name Type) {
	local bool hideMessage;
	hideMessage = SuperCheckMessage(Sender,Receiver,Mid(Msg,InStr(Msg,":")+1));
	return Super.MutatorBroadcastMessage(Sender,Receiver,Msg,bBeep,Type) && (!hideMessage);
}

function bool MutatorTeamMessage(Actor Sender, Pawn Receiver, PlayerReplicationInfo PRI, coerce string Msg, name Type, optional bool bBeep) {
	local bool hideMessage;
	hideMessage = SuperCheckMessage(Sender,Receiver,Msg);
	return Super.MutatorTeamMessage(Sender,Receiver,PRI,Msg,Type,bBeep) && (!hideMessage);
}

// Returns True if this message should be swallowed / hidden from other players.
function bool SuperCheckMessage(Actor Sender, Pawn Receiver, String Msg) {
	local int i;
	if (PlayerPawn(Sender)!=None && Left(Msg,7)~="!admin ") {
		if (Receiver==Sender) {
			for (i=0;i<256;i++) {
				if (Message[i]=="") {
					break;
				}
			}
			if (i>=256) {
				i = 256*FRand();
				Log("[MessageAdmin] Your MessageAdmin.ini file is full, please read and delete some messages.");
				Log("[MessageAdmin] Now overwriting old message #"$i$":");
				Log("[MessageAdmin]   "$Message[i]);
			}
			Message[i] = GetDate() $" ("$ Sender.getHumanName() $") "$ Mid(Msg,7);
			// Sender.ClientMessage("The admin's mailbox is full, your message may not be received.");
			SaveConfig();
			PlayerPawn(Sender).ClientMessage("Your message to the admin has been saved.");
		}
		return True; // hide message from others
	}
	return False;
}

function string GetDate() {
	local string Date, Time;
	Date = Level.Year$"/"$PrePad(Level.Month,"0",2)$"/"$PrePad(Level.Day,"0",2);
	Time = PrePad(Level.Hour,"0",2)$":"$PrePad(Level.Minute,"0",2)$":"$PrePad(Level.Second,"0",2);
	return Date$"-"$Time;
}
function string PrePad(coerce string s, string p, int i) {
	while (Len(s) < i)
		s = p$s;
	return s;
}


defaultproperties {
}

