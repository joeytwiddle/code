// Bresenham line plotting algorithm

// Must define or provide:
// BR_x1,BR_y1,BR_x2,BR_y2: start,end vectors
// BR_todo(x,y): action to take at each point x,y on line

    {
		Sint16
			BR_dx  = abs(BR_x2 - BR_x1),
			BR_dy  = abs(BR_y2 - BR_y1);
        Sint8
			BR_ddx = sgn(BR_x2-BR_x1),
			BR_ddy = sgn(BR_y2-BR_y1);
		register Sint16
			BR_x   = BR_x1,
			BR_y   = BR_y1,
			BR_eps = 0;

		if (BR_dx>BR_dy) {
			for ( ; BR_x != BR_x2+BR_ddx; BR_x+=BR_ddx )  {
				BR_todo(BR_x,BR_y);
				BR_eps += BR_dy;
				if ( (BR_eps << 1) >= BR_dx )  {
					BR_y+=BR_ddy;  BR_eps -= BR_dx;
				}
			}
		} else {
			for ( ; BR_y != BR_y2+BR_ddy; BR_y+=BR_ddy )  {
				BR_todo(BR_x,BR_y);
				BR_eps += BR_dx;
				if ( (BR_eps << 1) >= BR_dy )  {
					BR_x+=BR_ddx;  BR_eps -= BR_dy;
				}
			}
		}
	}
