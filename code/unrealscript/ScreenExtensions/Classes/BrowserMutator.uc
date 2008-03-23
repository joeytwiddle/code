class BrowserMutator extends Mutator;

// defaultproperties {
// }

function PreBeginPlay() {
	// TODO: check not already added
	Log("[BrowserMutator] Adding Self to Mutator Chain.");
	Level.Game.BaseMutator.AddMutator(Self);
	Level.Game.RegisterMessageMutator(Self);
}

function AddMutator(Mutator M) {
	if (M==Self) {
		Log("[BrowserMutator] Refusing to add Self again.");
	} else {
		Super.AddMutator(M);
	}
}

/*
// Catch messages from spectators:
function bool MutatorBroadcastMessage(Actor Sender, Pawn Receiver, out coerce string Msg, optional bool bBeep, out optional name Type) {
 local bool hideMessage;
 hideMessage = SuperCheckMessage(Sender,Receiver,Mid(Msg,InStr(Msg,":")+1));
 return Super.MutatorBroadcastMessage(Sender,Receiver,Msg,bBeep,Type) && (!hideMessage); // || !bSwallowSpokenCommands);
}

// Catch messages from players:
function bool MutatorTeamMessage(Actor Sender, Pawn Receiver, PlayerReplicationInfo PRI, coerce string Msg, name Type, optional bool bBeep) {
 local bool hideMessage;
 hideMessage = SuperCheckMessage(Sender,Receiver,Msg);
 return Super.MutatorTeamMessage(Sender,Receiver,PRI,Msg,Type,bBeep) && (!hideMessage); // || !bSwallowSpokenCommands);
}

function Mutate(String str, PlayerPawn Sender) {
 if (bAcceptMutateCommands) {
  CheckMessage(str, Sender);
 }
 Super.Mutate(str, Sender);
}
*/

// Returns True if the command was recognised (and therefore the player's message could optionally be swallowed).
function bool CheckMessage(String line, PlayerPawn Sender) {
 local int argCount;
 local String args[256];
 local Actor A;
 local String result;
 local int i,j;
 local String squishedName;
 local String url;
 local String rebuilt_string; // CONSIDER: instead of rebuilding the string, we could just use StrAfter(line," ") one or more times.
 local String command;

 // Log("ActorEditor.CheckMessage() ("$Sender$"): "$Msg$"");
 argcount = SplitString(line," ",args);

 command = args[0];

  if (command ~= "GOOGLE" || command ~= "G") {
       A = FindClosestActorMatching(Sender,"ScreenSlidePageWeb");
       WebBrowse(ScreenSlidePageWeb(A),"http://www.google.com/search?q="$args[1]);
  }

}

function WebBrowse(ScreenSlidePageWeb sspw, String url) {
	if (InStr(url,"://")<0) {
		url="http://"$url;
	}
}

function PostBeginPlay() {
 local Actor A;
 local int i;

 // If we were not added as a mutator, but run in some other way (e.g. as a ServerActor), then we need to register as a mutator:
   // Level.Game.BaseMutator.AddMutator(Self);

 // Register to receive spoken messages in MutatorTeamMessage() below:
 // if (bAcceptSpokenCommands) {
  Level.Game.RegisterMessageMutator(Self);
 // }

 // Update all edits from config:
 /*
 foreach AllActors(class'Actor', A) {
  for (i=0;i<1024;i++) {
   if (UpdateActor[i] == "") {
    break; // TODO: this makes the updating faster, BUT we must that lower lines are moved up if any lines are cleared
   }
   if ( Caps(UpdateActor[i]) == Caps(""$A) ) {
    Log("ActorEditor: Updating "$A$"."$UpdateProperty[i]$" -> "$UpdateValue[i]);
    A.SetPropertyText(UpdateProperty[i],UpdateValue[i]);
   }
  }
 }
 */

}

// Catch messages from players:
function bool MutatorTeamMessage(Actor Sender, Pawn Receiver, PlayerReplicationInfo PRI, coerce string Msg, name Type, optional bool bBeep) {
 local bool hideMessage;
 hideMessage = False;
 if (Sender == Receiver && Sender.IsA('PlayerPawn')) { // Only process each message once.
  if (StrStartsWith(Msg,"!")) {
   hideMessage = CheckMessage(Mid(Msg,1), PlayerPawn(Sender));
  }
 }
 return Super.MutatorTeamMessage(Sender,Receiver,PRI,Msg,Type,bBeep) && (!hideMessage);
}

function Mutate(String str, PlayerPawn Sender) {
 // if (bAcceptMutateCommands) {
  CheckMessage(str, Sender);
 // }
 Super.Mutate(str, Sender);
}


function SaveUpdate(Actor A, String property, String newValue) {
 local int i;
 if (A == None) {
  Log("ActorEditor.SaveUpdate(None,\""$property$"\",\""$newValue$"\") was called.");
  return;
 }
 A.SetPropertyText(property,newValue);
 // for (i=0;i<1024;i++) {
  // Find first empty record, or matching record to replace:
  // if (UpdateActor[i] == "" || (UpdateActor[i]~=(""$A) && UpdateProperty[i]~=property)) {
   // UpdateActor[i] = ""$A;
   // UpdateProperty[i] = property;
   // UpdateValue[i] = newValue;
   // // SaveConfig();
   // break;
  // }
 // }
}


// Actually specific for my setup, which is 1 Teleporter, 1 ScreenSlidePageServer (which doesn't work) and 1 ScreenSlidePageWeb (which does work).
function SetServer(Actor Sender, String server_ip, String server_description) {
 local Actor A;
 A = FindClosestActorMatching(Sender,"Teleporter");
 SaveUpdate(A,"URL","unreal://"$server_ip);
 A = FindClosestActorMatching(Sender,"ScreenSlidePageServer");
 if (StrContains(server_ip,":")) {
  SaveUpdate(A,"AddressServer",StrBefore(server_ip,":"));
  SaveUpdate(A,"AddressPort",StrAfter(server_ip,":"));
 } else {
  SaveUpdate(A,"AddressServer",server_ip);
  SaveUpdate(A,"AddressPort","7777");
 }
 SaveUpdate(A,"Text","<p align=center><font color=gray size=+3>"$server_description$"</font></p>["$server_ip$"]");
 A = FindClosestActorMatching(Sender,"ScreenSlidePageWeb");
 SaveUpdate(A,"AddressHost","neuralyte.org");
 SaveUpdate(A,"AddressPort","80");
 // SaveUpdate(A,"AddressPath","/~joey/utportal/"$shortWebName$".html");
 // SaveUpdate(A,"Text","<p align=center><font color=gray>[No "$shortWebName$".html from website]</font></p>["$server_ip$"]");
 SaveUpdate(A,"AddressPath","/cgi-bin/utq?ip="$server_ip);
 SaveUpdate(A,"Text","<p align=center><font color=gray>[Waiting to access web CGI script...]</font></p>["$server_ip$"]");
}


function Actor FindClosestActor(Actor from) {
 local Actor A;
 local int distance;
 local int deltaRotation;
 local Actor bestActor;
 local int bestDistance;
 bestActor = None;
 foreach VisibleActors(class'Actor', A, 1024, from.Location) {
 // foreach AllActors(class'Actor', A) { // not using VisibleActors gets us more invisible actors like InventorySpot/Light/...
  if (A == from) { // don't find self!
   continue;
  }
  distance = VSize(A.Location - from.Location);
  deltaRotation = Abs( Rotator(A.Location - from.Location).Yaw - from.Rotation.Yaw ) % 65536;
  // if (deltaRotation < 8192 || deltaRotation > 8192*7) {
  if (deltaRotation > 8192*4) {
   deltaRotation = 8192*8 - deltaRotation;
  }
  if (deltaRotation < 8192 && deltaRotation > -8192) {
   if (bestActor == None || distance < bestDistance) {
    bestActor = A;
    bestDistance = distance;
    // PlayerPawn(from).ClientMessage("  " $ A $" (" $ deltaRotation $ ") -> " $ distance $ "");
   }
  }
 }
 return bestActor;
}

function Actor FindMatchingActor(string str) {
 local Actor A;
 foreach AllActors(class'Actor', A) {
  if (StrContains(Caps(""$A),Caps(str))) {
   return A;
  }
 }
 return None;
}

function Actor FindClosestActorMatching(Actor from, String str) {
 local Actor A;
 local int distance;
 local int deltaRotation;
 local Actor bestActor;
 local int bestDistance;
 bestActor = None;
 // foreach VisibleActors(class'Actor', A, 1024, from.Location) {
 foreach AllActors(class'Actor', A) { // not using VisibleActors gets us more invisible actors like InventorySpot/Light/...
  if (A == from) { // don't find self!
   continue;
  }
  if (!StrContains(Caps(""$A),Caps(str))) {
   continue;
  }
  distance = VSize(A.Location - from.Location);
  deltaRotation = Abs( Rotator(A.Location - from.Location).Yaw - from.Rotation.Yaw ) % 65536;
  // if (deltaRotation < 8192 || deltaRotation > 8192*7) {
  if (deltaRotation > 8192*4) {
   deltaRotation = 8192*8 - deltaRotation;
  }
  if (deltaRotation < 8192 && deltaRotation > -8192) {
   if (bestActor == None || distance < bestDistance) {
    bestActor = A;
    bestDistance = distance;
    // PlayerPawn(from).ClientMessage("  " $ A $" (" $ deltaRotation $ ") -> " $ distance $ "");
   }
  }
 }
 return bestActor;
}

function Actor FindActorWithMatchingProperty(string prop, string val) {
 local Actor A;
 foreach AllActors(class'Actor', A) {
  if ( StrContains(Caps(A.GetPropertyText(prop)),Caps(val)) ) {
   return A;
  }
 }
 return None;
}

function bool isURL(String str) {
 return (InStr(str,"://")>=0 && InStr(str,"://")<50);
}














//===============//
//               //
//  JLib.uc.jpp  //
//               //
//===============//
function int SplitString(String str, String divider, out String parts[256]) {
 // local String parts[256];
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
function string GetDate() {
    local string Date, Time;
    Date = Level.Year$"-"$PrePad(Level.Month,"0",2)$"-"$PrePad(Level.Day,"0",2);
    Time = PrePad(Level.Hour,"0",2)$":"$PrePad(Level.Minute,"0",2)$"."$PrePad(Level.Second,"0",2);
    return Date$"-"$Time;
}
function string PrePad(coerce string s, string p, int i) {
    while (Len(s) < i)
        s = p$s;
    return s;
}
function bool StrStartsWith(string s, string x) {
 return (InStr(s,x) == 0);
 // return (Left(s,Len(x)) ~= x);
}
// function bool StrEndsWith(string s, string x) {
 // return (Right(s,Len(x)) ~= x);
// }
function bool StrContains(String s, String x) {
 return (InStr(s,x) > -1);
}
function String StrAfter(String s, String x) {
 local int i;
 i = Instr(s,x);
 return Mid(s,i+Len(x));
}
function string StrAfterLast(string s, string x) {
 local int i;
 i = InStr(s,x);
 if (i == -1) {
  return s;
 }
 while (i != -1) {
  s = Mid(s,i+Len(x));
  i = InStr(s,x);
 }
 return s;
}
function string StrBefore(string s, string x) {
 local int i;
 i = InStr(s,x);
 if (i == -1) {
  return s;
 } else {
  return Left(s,i);
 }
}
function string StrBeforeLast(string s, string x) {
 local int i;
 i = InStrLast(s,x);
 if (i == -1) {
  return s;
 } else {
  return Left(s,i);
 }
}
function int InStrOff(string haystack, string needle, int offset) {
 local int instrRest;
 instrRest = InStr(Mid(haystack,offset),needle);
 if (instrRest == -1) {
  return instrRest;
 } else {
  return offset + instrRest;
 }
}
function int InStrLast(string haystack, string needle) {
 local int pos;
 local int posRest;
 pos = InStr(haystack,needle);
 if (pos == -1) {
  return -1;
 } else {
  posRest = InStrLast(Mid(haystack,pos+Len(needle)),needle);
  if (posRest == -1) {
   return pos;
  } else {
   return pos + Len(needle) + posRest;
  }
 }
}
// Converts a string to lower-case.
function String Locs(String in) {
 local String out;
 local int i;
 local int c;
 out = "";
 for (i=0;i<Len(in);i++) {
  c = Asc(Mid(in,i,1));
  if (c>=65 && c<=90) {
   c = c + 32;
  }
  out = out $ Chr(c);
 }
 return out;
}

/*

 if (bLetPlayersSetScreens || Sender.bAdmin || (!bOnlyAdmin)) {

  // Specifically for Screen actors + teleporters.
  if (command ~= "SETSERVER") {
   rebuilt_string = ""; for (i=2;i<argCount;i++) { rebuilt_string = rebuilt_string $ args[i]; if (i<argCount-1) { rebuilt_string = rebuilt_string $ " "; } }
   if (args[1] == "" || rebuilt_string == "") {
    Sender.ClientMessage("Usage: !setserver <server_ip> <server_title/description>");
   } else {
    SetServer(Sender,args[1],rebuilt_string);
    Sender.ClientMessage("Screen server updated.");
   }
   return True;
  }

  if (command ~= "SETTEXT") {
   rebuilt_string = ""; for (i=1;i<argCount;i++) { rebuilt_string = rebuilt_string $ args[i]; if (i<argCount-1) { rebuilt_string = rebuilt_string $ " "; } }
   if (args[1] == "") {
    Sender.ClientMessage("Usage: !settext <new_text>");
   } else {
    A = FindClosestActorMatching(Sender,"ScreenSlidePage");
    SaveUpdate(A,"Text",rebuilt_string);
    Sender.ClientMessage("Screen text updated.");
   }
   return True;
  }

  if (command ~= "SETURL") {
   rebuilt_string = ""; for (i=1;i<argCount;i++) { rebuilt_string = rebuilt_string $ args[i]; if (i<argCount-1) { rebuilt_string = rebuilt_string $ " "; } }
   if (!isURL(args[1])) {
    Sender.ClientMessage("Usage: !seturl <url>");
   } else {
    A = FindClosestActorMatching(Sender,"Teleporter");
    if (!StrContains(Teleporter(A).URL,"://")) {
     Sender.ClientMessage("This local Teleporter may not be modified.");
    } else {
     SaveUpdate(A,"URL",rebuilt_string);
     Sender.ClientMessage("Teleporter URL updated.");
    }
   }
   return True;
  }

 }


 if (bOnlyAdmin && !Sender.bAdmin) {
  return False;
 }

 if (command ~= "HELP") {
  Sender.ClientMessage("ActorEditor commands:");
  Sender.ClientMessage("  help | look | search/find/seek <actor_name_part> | findclosest <actor_name_part> | searchprop/findprop/seekprop <property> <value_part> | ");
  // Sender.ClientMessage("  get/check <property> | set/put <property> <value> | store/save <property> <value> | grab <property>");
  Sender.ClientMessage("  get/check <property> | set/put/save/store <property> <value> | grab <property>");
  Sender.ClientMessage("  setserver <server_address> <server_title/description> | settext <new_text_for_screen> | seturl <new_url_for_teleporter>");
  Sender.ClientMessage("  saveall");
  return True;
 }

 if (command ~= "LOOK") {
  workingActor = FindClosestActor(Sender);
  Sender.ClientMessage("Viewing: "$workingActor);
  return True;
 }

 if (command ~= "SEARCH" || command ~= "FIND" || command ~= "SEEK") {
  workingActor = FindMatchingActor(args[1]);
  Sender.ClientMessage("Found: "$workingActor);
  return True;
 }

 if (command ~= "FINDCLOSEST") {
  workingActor = FindClosestActorMatching(Sender,args[1]);
  Sender.ClientMessage("Found: "$workingActor);
  return True;
 }

 if (command ~= "SEARCHPROP" || command ~= "FINDPROP" || command ~= "SEEKPROP") {
  workingActor = FindActorWithMatchingProperty(args[1],args[2]);
  Sender.ClientMessage("Found: "$workingActor$" with "$args[1]$": "$workingActor.GetPropertyText(args[1]));
  return True;
 }

 if (command ~= "GET" || command ~= "CHECK" || command ~= "SHOW") {
  result = workingActor.GetPropertyText(args[1]);
  Sender.ClientMessage(workingActor $ "." $ args[1] $ ": " $ result);
  return True;
 }

 if (command ~= "SET" || command ~= "PUT" || command ~= "STORE" || command ~= "SAVE") {
  rebuilt_string = ""; for (i=2;i<argCount;i++) { rebuilt_string = rebuilt_string $ args[i]; if (i<argCount-1) { rebuilt_string = rebuilt_string $ " "; } }
  SaveUpdate(workingActor,args[1],rebuilt_string);
  result = workingActor.GetPropertyText(args[1]);
  Sender.ClientMessage(workingActor $ "." $ args[1] $ " => " $ result);
  // If STORE or SAVE, then we could: SaveConfig();
  return True;
 }

 if (command ~= "GRAB") {
  // Copies the current value of the property into the config file, so it can be edited there.
  result = workingActor.GetPropertyText(args[1]);
  SaveUpdate(workingActor,args[1],result);
  // We could: SaveConfig();
  Sender.ClientMessage(workingActor $ "." $ args[1] $ " << " $ result);
  return True;
 }

 if (command ~= "SAVEALL") {
  SaveConfig();
  return True;
 }

 return False;

*/

/*

function UpdateSlide(ScreenSlidePage Slide, String color, PlayerPawn Sender, String Msg) {
	local int i;
	local String LocationName;
	if (Slide != None) {
		while (Len(Slide.Text) >= MaxChars) {
			// text in this slide has exceeded max limit; remove everything before the first <BR>
			i = InStr(Slide.Text,"<BR>");
			if (i<0) break;
			Slide.Text = Mid(Slide.Text,i+4);
		}
		if (Sender.PlayerReplicationInfo.PlayerLocation != None) {
			LocationName = Sender.PlayerReplicationInfo.PlayerLocation.LocationName;
		} else if (Sender.PlayerReplicationInfo.PlayerZone != None) {
			LocationName = Sender.PlayerReplicationInfo.PlayerZone.ZoneName;
		}
		Msg = StrReplaceAll(Msg,"<","&lt;");
		Msg = StrReplaceAll(Msg,">","&gt;");
		Slide.Text = Slide.Text $ "<BR>" $ "<FONT color='" $ color $ "'>" $ Sender.GetHumanName() $ "</FONT> (" $ LocationName $ "): " $ Msg;
	}
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
}

*/


