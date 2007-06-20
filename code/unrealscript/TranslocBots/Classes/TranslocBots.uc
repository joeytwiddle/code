// TODO: hey look we can create new actors from only their class name! BaseMutator.AddMutator(Spawn(MutatorClass));
//       we can also use .Class to get the class of an actor

class TranslocBots expands Mutator;

var bool bBuildTranslocPaths; // Whether this Mutator will do anything.

defaultproperties {
	bBuildTranslocPaths=True
}

var name currentTag;

var int depth; // used to prevent infinite recursion when replacing actors

function PreBeginPlay() {
}

// Force game to always keep this actor, even if other mutators want to get rid of it.
function bool AlwaysKeep(Actor Other) {

	// Log("TranslocBots.AlwaysKeep("$Other$") called.");
	// Log("TranslocBots.AlwaysKeep("$Other$") called; currentTag="$currentTag$"");
	// if ( (""$Other.Class)=="TranslocStart" || (""$Other.Class)=="TranslocDest" || (""$Other.Class)=="LiftCenter" ) {
	// }
	// if (currentTag == None) {
	if ((""$currentTag) == "None") {
		// currentTag = "POST_GEN_TAG_" $ RandRange(1,100);
		// currentTag = 'POST_GEN_TAG_';
		// currentTag = 'POST_GEN_TAG_' $ RandRange(1,100);
		SetPropertyText("currentTag","POST_GEN_TAG_" $ RandRange(1,100)); // workaround to build a new name variable from a string
	}
	if (Other.IsA('LiftExit')) {
		Log("TranslocBots.AlwaysKeep(): LiftExit Tag = "$LiftExit(Other).LiftTag$" ("$Other$")");
		if ((""$LiftExit(Other).LiftTag) == "None") {
			Log("= LiftExit("$Other$").LiftTag = "$currentTag);
			LiftExit(Other).LiftTag = currentTag;
		} else {
			currentTag = LiftExit(Other).LiftTag;
		}
	}
	if (Other.IsA('LiftCenter')) {
		Log("TranslocBots.AlwaysKeep(): LiftCenter Tag = "$LiftExit(Other).LiftTag$" "$Other$")");
		if ((""$LiftCenter(Other).LiftTag) == "None") {
			Log("= LiftCenter("$Other$").LiftTag = "$currentTag);
			LiftCenter(Other).LiftTag = currentTag;
		} else {
			currentTag = LiftCenter(Other).LiftTag;
		}
	}

	// Pass to other mutators in the default way:
	return Super.AlwaysKeep(Other);

}

function MyReplaceWith(Actor Other,String str) {
	if (str=="None") {
		Log("["$depth$"] x skipping "$ Other $ " -> " $ str);
		return;
	}
	depth++;
	Log("["$depth$"] > replacing "$ Other $ " -> " $ str);
	ReplaceWith(Other,str);
	depth--;
}

// Allow mutators to remove actors (by returning false), or replace them.
function bool CheckReplacement(Actor Other, out byte bSuperRelevant) {
	local int i;

	if (depth>0) {
		Log("("$depth$") < todeep " $ Other $ " ("$bSuperRelevant$")");
		return True;
	}

	if (bBuildTranslocPaths) {

		if (Other.IsA('PathNode')) {
			/*
			i = RandRange(1,4);
			if (i == 1) {
				MyReplaceWith(Other,"Botpack.TranslocStart");
			} else if (i == 2) {
				MyReplaceWith(Other,"Botpack.TranslocDest");
			} else if (i == 3) {
				MyReplaceWith(Other,"Engine.LiftCenter");
			} else {
				Log("TranslocBots.CheckReplacement(): RandRange(1,4) returned "$i);
			}
			*/
			MyReplaceWith(Other,"Engine.LiftExit");
			return true;
		}

	}

	return Super.CheckReplacement( Other, bSuperRelevant );

}

