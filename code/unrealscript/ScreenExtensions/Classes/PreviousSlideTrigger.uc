class PreviousSlideTrigger extends ReplicatedTrigger;

simulated function ReplicatedTrigger() {
	local Screen ThisScreen;
	local ScreenSlide PreviousSlide;

	// Role < ROLE_Authority works for online play, but for offline play, we need Role == ROLE_Authority
	if (Role < ROLE_Authority) { // or singleplayer
		foreach AllActors(class 'Screen', ThisScreen, Event) {
			Log(Self$" ["$Role$"]: checking "$ThisScreen);
			if (ThisScreen.SlideCurrent != None) {
				PreviousSlide = FindSlideBefore(ThisScreen.SlideCurrent);
				if (PreviousSlide != None) {
					Log(Self$" ["$Role$"]: Switching from "$ThisScreen.SlideCurrent$" to "$PreviousSlide);
					ThisScreen.SwitchTriggered.SlideSwitch = PreviousSlide;
					ThisScreen.SwitchTriggered.Update++;
					ThisScreen.Action = SlideAction_Exiting;
				}
			}
		}
	}

}

simulated function ScreenSlide FindSlideBefore(ScreenSlide Slide) {
	local ScreenSlide OtherSlide;
	foreach AllActors(class 'ScreenSlide', OtherSlide) {
		if (OtherSlide.SlideNext == Slide) {
			return OtherSlide;
		}
	}
	Log(Self$" ["$Role$"]: Failed to find slide before "$Slide);
	return None;
}

