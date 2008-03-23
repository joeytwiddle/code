class NextSlideTrigger extends ReplicatedTrigger;

simulated function ReplicatedTrigger() {
	local Screen ThisScreen;

	// Role < ROLE_Authority works for online play, but for offline play, we need Role == ROLE_Authority
	if (Role < ROLE_Authority) { // or singleplayer
		foreach AllActors(class 'Screen', ThisScreen, Event) {
			if (ThisScreen.SlideCurrent != None) {
				Log(Self$" ["$Role$"]: Switching from "$ThisScreen.SlideCurrent$" to "$ThisScreen.SlideCurrent.SlideNext);
				// During online play, the server's SlideCurrent never actually switches,
				// so we can only trust the client's.
				// But during offine play, we must switch the authority copy of the Slide.
				// This does actually get called on the server, but the server's screen never changes. 
				ThisScreen.SwitchTriggered.SlideSwitch = ThisScreen.SlideCurrent.SlideNext;
				ThisScreen.SwitchTriggered.Update++;
				ThisScreen.Action = SlideAction_Exiting;
			}
		}
	}

}

