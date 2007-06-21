// TODO: hey look we can create new actors from only their class name! BaseMutator.AddMutator(Spawn(MutatorClass));
//       we can also use .Class to get the class of an actor

class TranslocBots expands Mutator;

var bool bBuildTranslocPaths; // Whether this Mutator will do anything.

var bool bWorking;

defaultproperties {
	bBuildTranslocPaths=True
	bWorking=False
}

var name currentTag;

var int depth; // used to prevent infinite recursion when replacing actors

function PreBeginPlay() {
}

// Force game to always keep this actor, even if other mutators want to get rid of it.
function bool AlwaysKeep(Actor Other) {

	// Log("TranslocBots.AlwaysKeep("$Other$") called.");
	// Log("TranslocBots.AlwaysKeep("$Other$") called; currentTag="$currentTag$"");
	// Log("TranslocBots.AlwaysKeep("$Other$") called; currentTag="$currentTag$" Location="$Other.Location$"");
	// Log("TranslocBots.AlwaysKeep("$Other$") at "$Other.Location$"");
	// if ( (""$Other.Class)=="TranslocStart" || (""$Other.Class)=="TranslocDest" || (""$Other.Class)=="LiftCenter" ) {
	// }
	// if (currentTag == None) {
	if (Other.IsA('LiftExit')) {
		// Log("TranslocBots.AlwaysKeep("$Other$"): LiftExit Tag = "$LiftExit(Other).LiftTag$"");
		if ((""$LiftExit(Other).LiftTag) == "None") {
			refreshCurrentTag();
			Log("= Setting "$Other$""$Other.Location$".LiftTag = "$currentTag);
			LiftExit(Other).LiftTag = currentTag;
		} else {
			currentTag = LiftExit(Other).LiftTag;
			// Log(". Leaving "$Other$"("$currentTag$")");
		}
	}
	if (Other.IsA('LiftCenter')) {
		// Log("TranslocBots.AlwaysKeep("$Other$"): LiftCenter Tag = "$LiftCenter(Other).LiftTag$"");
		if ((""$LiftCenter(Other).LiftTag) == "None") {
			refreshCurrentTag();
			Log("= Setting "$Other$""$Other.Location$".LiftTag = "$currentTag);
			LiftCenter(Other).LiftTag = currentTag;
		} else {
			currentTag = LiftCenter(Other).LiftTag;
			// Log(". Leaving "$Other$"("$currentTag$")");
		}
	}

	// Pass to other mutators in the default way:
	return Super.AlwaysKeep(Other);

}

function refreshCurrentTag() {
	if ((""$currentTag) == "None" || RandRange(1,7)<2) {
		// currentTag = "POST_GEN_TAG_" $ RandRange(1,100);
		// currentTag = 'POST_GEN_TAG_';
		// currentTag = 'POST_GEN_TAG_' $ RandRange(1,100);
		SetPropertyText("currentTag","Fresh_Tag" $ RandRange(1,100)); // workaround to build a new name variable from a string
		// Log("TranslocBots.refreshCurrentTag() NEW! SetPropertyText(\"currentTag\",\"" $ currentTag $ "\")");
	}
}

function MyReplaceWith(Actor Other,String str) {
	if (str=="None") {
		Log("["$depth$"] x skipping "$ Other $ " -> " $ str);
		return;
	}
	// Log("["$depth$"] > replacing "$ Other $ "" $ Other.Location $ " -> " $ str);
	// Log("["$depth$"] > replacing "$ Other $ "" $ Other.Location $ " -> " $ str);
	if ((""$Other.Class) != "PathNode") {
		Log("["$depth$"] > replacing "$ Other $ "" $ Other.Location $ " -> " $ str);
	}
	depth++;
	ReplaceWith(Other,str);
	depth--;
	if (!bWorking) {
		bWorking=True;
		Log("TranslocBots is attempting to create new transloc paths for bots on this map ("$Level$")");
	}
}

// Allow mutators to remove actors (by returning false), or replace them.
function bool CheckReplacement(Actor Other, out byte bSuperRelevant) {
	local int i;

	if (depth>0) {
		// Log("("$depth$") < checkreplacement ignoring " $ Other $ " ("$bSuperRelevant$")");
		return True;
	}

	if (bBuildTranslocPaths) {

		if (Other.IsA('PathNode')) {
			i = RandRange(1,5);
			if (i == 1) {
				MyReplaceWith(Other,"TranslocBots.FreshTranslocStart");
			} else if (i == 2) {
				MyReplaceWith(Other,"TranslocBots.FreshTranslocDest");
			} else if (i == 3) {
				MyReplaceWith(Other,"TranslocBots.FreshLiftCenter");
			} else if (i == 4) {
				MyReplaceWith(Other,"TranslocBots.FreshLiftExit");
			} else {
				Log("TranslocBots.CheckReplacement(): RandRange(1,4) returned "$i);
			}
			return true;
		}

	}

	return Super.CheckReplacement( Other, bSuperRelevant );

}

