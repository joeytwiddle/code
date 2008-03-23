
// CameraMode Camera modes:
// 0 - default following ViewTarget, auto-panning
// 1 * custom following FollowActor
// 2 - free standing camera, looking at FollowActor
// 3 * floating camera, panning / looking at centre of mass)
// 4 * custom following FollowActor, but looking at LookActor
// 5 - default following ViewTarget, but looking at LookActor
//
// * = camera floats towards a target

class nBAutoSpectateOnline extends nBAutoSpectate;

function bool isCameraModeAllowed() {
	if (CameraMode == 0)
		return true;
	// if (CameraMode == 2)
		// return true;
	if (CameraMode == 5)
		return true;
	return false;
}

