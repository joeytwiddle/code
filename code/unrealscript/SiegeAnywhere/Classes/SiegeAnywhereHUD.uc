class SiegeAnywhereHUD extends sgHUD;


// #ifdef CLOCK
var AssaultHUD clock;
// #endif


// #ifdef CLOCK
simulated function PostRender( canvas Canvas )
{
	local int X;
	local float clockY;

	if (clock == None) {
		clock = spawn(class'AssaultHUD',PlayerOwner);
	}
	if ( bHideAllWeapons || (HudScale * WeaponScale * Canvas.ClipX <= Canvas.ClipX - 256 * Scale) )
		clockY = Canvas.ClipY - 128 * Scale;
	else
		clockY = Canvas.ClipY - 192 * Scale;
	clock.DrawTimeAt(Canvas, 2, clockY);

	Super.PostRender( Canvas );		
}
// #endif

