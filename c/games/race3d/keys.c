// Variables
bool UPpressed = 0;
bool DOWNpressed = 0;
bool LEFTpressed = 0;
bool RIGHTpressed = 0;
bool F10pressed = 0;
bool SPACEpressed = 0;
bool qpressed = 0;
bool wpressed = 0;
bool epressed = 0;
bool rpressed = 0;
bool tpressed = 0;
bool ypressed = 0;
bool upressed = 0;
bool ipressed = 0;
bool opressed = 0;
bool ppressed = 0;
bool apressed = 0;
bool spressed = 0;
bool dpressed = 0;
bool fpressed = 0;
bool gpressed = 0;
bool hpressed = 0;
bool jpressed = 0;
bool kpressed = 0;
bool lpressed = 0;
bool zpressed = 0;
bool xpressed = 0;
bool cpressed = 0;
bool vpressed = 0;
bool bpressed = 0;
bool npressed = 0;
bool mpressed = 0;
bool LCTRLpressed = 0;
bool ALTpressed = 0;

// Checking
bool observeEvent(SDL_Event event) {
	if ( event.type == SDL_KEYDOWN || event.type == SDL_KEYUP ) {
		bool position = ( event.type == SDL_KEYDOWN );
		if ( event.key.keysym.sym == SDLK_UP ) {
			UPpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_DOWN ) {
			DOWNpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_LEFT ) {
			LEFTpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_RIGHT ) {
			RIGHTpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_F10 ) {
			F10pressed = position;
		}
		if ( event.key.keysym.sym == SDLK_SPACE ) {
			SPACEpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_q ) {
			qpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_w ) {
			wpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_e ) {
			epressed = position;
		}
		if ( event.key.keysym.sym == SDLK_r ) {
			rpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_t ) {
			tpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_y ) {
			ypressed = position;
		}
		if ( event.key.keysym.sym == SDLK_u ) {
			upressed = position;
		}
		if ( event.key.keysym.sym == SDLK_i ) {
			ipressed = position;
		}
		if ( event.key.keysym.sym == SDLK_o ) {
			opressed = position;
		}
		if ( event.key.keysym.sym == SDLK_p ) {
			ppressed = position;
		}
		if ( event.key.keysym.sym == SDLK_a ) {
			apressed = position;
		}
		if ( event.key.keysym.sym == SDLK_s ) {
			spressed = position;
		}
		if ( event.key.keysym.sym == SDLK_d ) {
			dpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_f ) {
			fpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_g ) {
			gpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_h ) {
			hpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_j ) {
			jpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_k ) {
			kpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_l ) {
			lpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_z ) {
			zpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_x ) {
			xpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_c ) {
			cpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_v ) {
			vpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_b ) {
			bpressed = position;
		}
		if ( event.key.keysym.sym == SDLK_n ) {
			npressed = position;
		}
		if ( event.key.keysym.sym == SDLK_m ) {
			mpressed = position;
		}
		if ( event.key.keysym.mod == KMOD_LCTRL ) {
			LCTRLpressed = position;
		}
		if ( event.key.keysym.mod == KMOD_ALT ) {
			ALTpressed = position;
		}
		return true;
	}
	return false;
}
