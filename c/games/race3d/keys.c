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
			printf("UPpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_DOWN ) {
			DOWNpressed = position;
			printf("DOWNpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_LEFT ) {
			LEFTpressed = position;
			printf("LEFTpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_RIGHT ) {
			RIGHTpressed = position;
			printf("RIGHTpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_F10 ) {
			F10pressed = position;
			printf("F10pressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_SPACE ) {
			SPACEpressed = position;
			printf("SPACEpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_q ) {
			qpressed = position;
			printf("qpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_w ) {
			wpressed = position;
			printf("wpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_e ) {
			epressed = position;
			printf("epressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_r ) {
			rpressed = position;
			printf("rpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_t ) {
			tpressed = position;
			printf("tpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_y ) {
			ypressed = position;
			printf("ypressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_u ) {
			upressed = position;
			printf("upressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_i ) {
			ipressed = position;
			printf("ipressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_o ) {
			opressed = position;
			printf("opressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_p ) {
			ppressed = position;
			printf("ppressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_a ) {
			apressed = position;
			printf("apressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_s ) {
			spressed = position;
			printf("spressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_d ) {
			dpressed = position;
			printf("dpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_f ) {
			fpressed = position;
			printf("fpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_g ) {
			gpressed = position;
			printf("gpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_h ) {
			hpressed = position;
			printf("hpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_j ) {
			jpressed = position;
			printf("jpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_k ) {
			kpressed = position;
			printf("kpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_l ) {
			lpressed = position;
			printf("lpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_z ) {
			zpressed = position;
			printf("zpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_x ) {
			xpressed = position;
			printf("xpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_c ) {
			cpressed = position;
			printf("cpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_v ) {
			vpressed = position;
			printf("vpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_b ) {
			bpressed = position;
			printf("bpressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_n ) {
			npressed = position;
			printf("npressed = %i\n",position);
		}
		if ( event.key.keysym.sym == SDLK_m ) {
			mpressed = position;
			printf("mpressed = %i\n",position);
		}
		if ( event.key.keysym.mod == KMOD_LCTRL ) {
			LCTRLpressed = position;
			printf("LCTRLpressed = %i\n",position);
		}
		if ( event.key.keysym.mod == KMOD_ALT ) {
			ALTpressed = position;
			printf("ALTpressed = %i\n",position);
		}
		return true;
	}
	return false;
}
