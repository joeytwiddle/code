// TeamScreenMutator: Catches TeamSay messages, and adds them to a ScreenSlide, to create a visible log inside the map.
// Useful for referring back to long-term team-tactics.
// As an Actor embedded in the map, the relevant slides may be defined in the Actor's properties.
// Otherwise TeamScreenMutator will try to find the relevant ScreenSlides at startup: the red team ScreenSlide should contain the text "red team", and likewise for the blue team.

class TeamScreenMutator extends Mutator;

var() ScreenSlidePage RedSlide;
var() ScreenSlidePage BlueSlide;
var() ScreenSlidePage GreenSlide;
var() ScreenSlidePage GoldSlide;

var int MaxChars; // Should be <512

defaultproperties {
	MaxChars=500
}

function PreBeginPlay() {
	// TODO: check not already added
	Level.Game.BaseMutator.AddMutator(Self);
	Level.Game.RegisterMessageMutator(Self);

	if (redSlide == None && blueSlide == None) {
		GuessSlides();
	}
}

function GuessSlides() {
	local ScreenSlidePage S;
	foreach AllActors(class 'ScreenSlidePage', S) {
		if (InStr(Caps(S.Text),"RED TEAM")>=0) {
			redSlide = S;
		} else
		if (InStr(Caps(S.Text),"BLUE TEAM")>=0) {
			blueSlide = S;
		} else
		if (InStr(Caps(S.Text),"GREEN TEAM")>=0) {
			greenSlide = S;
		} else
		if (InStr(Caps(S.Text),"GOLD TEAM")>=0) {
			goldSlide = S;
		}
	}
}

function AddMutator(Mutator M) {
	if (M==Self) {
		Log("TeamScreenMutator: Refusing to add Self again.");
	} else {
		Super.AddMutator(M);
	}
}

function bool MutatorTeamMessage (Actor Sender, Pawn Receiver, PlayerReplicationInfo PRI, coerce string S, name Type, optional bool bBeep) {
	if (Sender == Receiver) { // only process each message once
		// Log("Super.MutatorTeamMessage("$Sender$", "$Receiver$", "$PRI$", "$S$", "$Type$", "$bBeep$")");
		if (Sender.IsA('PlayerPawn') && Type=='TeamSay') {
			switch (PlayerPawn(Sender).PlayerReplicationInfo.Team) {
				case 0: UpdateSlide(redSlide,"red",PlayerPawn(Sender),S); break;
				case 1: UpdateSlide(blueSlide,"blue",PlayerPawn(Sender),S); break;
				case 2: UpdateSlide(greenSlide,"green",PlayerPawn(Sender),S); break;
				case 3: UpdateSlide(goldSlide,"gold",PlayerPawn(Sender),S); break;
			}
		}
	}
	return Super.MutatorTeamMessage(Sender, Receiver, PRI, S, Type, bBeep);
}

function UpdateSlide(ScreenSlidePage Slide, String color, PlayerPawn Sender, String Msg) {
	local int i;
	// local String LocationName;
	if (Slide != None) {
		// if (Sender.PlayerReplicationInfo.PlayerLocation != None) {
			// LocationName = Sender.PlayerReplicationInfo.PlayerLocation.LocationName;
		// } else if (Sender.PlayerReplicationInfo.PlayerZone != None) {
			// LocationName = Sender.PlayerReplicationInfo.PlayerZone.ZoneName;
		// }
		Msg = ToHTML(Msg);
		// Slide.Text = Slide.Text $ "<BR>" $ "<FONT color='" $ color $ "'>" $ Sender.GetHumanName() $ "</FONT> (" $ LocationName $ "): " $ Msg;
		Slide.Text = Slide.Text $ "<FONT color='" $ color $ "'>" $ Sender.GetHumanName() $ "</FONT>: " $ Msg $ "<BR>";
		while (Len(Slide.Text) >= MaxChars) {
			// text in this slide has exceeded max limit; remove everything before the first <BR>
			i = InStr(Slide.Text,"<BR>");
			if (i<0) break;
			Slide.Text = Mid(Slide.Text,i+4);
		}
	}
}

function String ToHTML(String str) {
	str = StrReplaceAll(str,"<","&lt;");
	str = StrReplaceAll(str,">","&gt;");
	return str;
}

function String StrReplaceAll(String input, String find, String replace) {
	local String output;
	local int i;
	output = "";
	while (true) {
		i = InStr(input,find);
		if (i<0) {
			output = output $ input;
			return output;
		} else {
			output = output $ Left(input,i) $ replace;
			input = Mid(input,i+Len(find));
		}
	}
	return output;
}

