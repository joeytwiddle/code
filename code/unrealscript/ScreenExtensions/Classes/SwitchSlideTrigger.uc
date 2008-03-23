// This works like the original ScreenTrigger, except it forces the current slide into Exit mode, like NextSlideTrigger and PreviousSlideTrigger.

class SwitchSlideTrigger extends ReplicatedTrigger;

var() ScreenSlide SlideSwitch;

simulated function ReplicatedTrigger() {
	local Screen ThisScreen;
	local ScreenSlide PreviousSlide;

	foreach AllActors(class 'Screen', ThisScreen, Event) {
		if (ThisScreen.SlideCurrent != None) {
			ThisScreen.SwitchTriggered.SlideSwitch = SlideSwitch;
			ThisScreen.SwitchTriggered.Update++;
			ThisScreen.Action = SlideAction_Exiting;
		}
	}
}

