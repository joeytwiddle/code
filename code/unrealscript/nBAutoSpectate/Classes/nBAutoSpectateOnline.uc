class nBAutoSpectateOnline extends nBAutoSpectate config(nBAutoSpectate);

function bool WeAreOnline() {
	return True;
}

function bool isCameraModeAllowed() {
	if (CameraMode == 0)
		return true;
	// if (CameraMode == 2)
		// return true;
	if (CameraMode == 5)
		return true;
	return false;
}

