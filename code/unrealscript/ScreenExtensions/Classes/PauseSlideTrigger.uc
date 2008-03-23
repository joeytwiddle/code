class PauseSlideTrigger extends ReplicatedTrigger;

simulated function ReplicatedTrigger() {
	local Screen ThisScreen;
	local ScreenSlide PreviousSlide;

	// Role < ROLE_Authority works for online play, but for offline play, we need Role == ROLE_Authority
	if (Role < ROLE_Authority) { // or singleplayer
		foreach AllActors(class 'Screen', ThisScreen, Event) {
			Log(Self$" ["$Role$"]: checking "$ThisScreen);
			if (ThisScreen.SlideCurrent != None) {
				if (ThisScreen.Action != SlideAction_None) {
					ThisScreen.Action = SlideAction_None;
				} else if (ThisScreen.Action == SlideAction_None) {
					ThisScreen.Action = SlideAction_Scrolling;
				}
			}
		}
	}

}

