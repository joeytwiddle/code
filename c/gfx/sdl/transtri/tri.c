// Triangle plotting using Bresenham

// Provide:
// TR_x1,TR_y1,TR_x2,TR_y2,TR_x3,TR_y3
// TR_todo(x,y)

#define NOEDGES

#define min(x,y) (x<y?x:y)
#define max(x,y) (x>y?x:y)
// #define swap(t,a,b); { t tmp; tmp=a; a=b; b=tmp; }
#define swap(t,a,b); { t tmp; tmp=*(a); *(a)=*(b); *(b)=tmp; }
#ifndef forminmax
    #ifdef NOEDGES
        #define forminmax(forminmax_i1,forminmax_i2,forminmax_do); { register Sint16 forminmax_a,forminmax_b,forminmax_i; if (forminmax_i1<forminmax_i2) { forminmax_a=forminmax_i1; forminmax_b=forminmax_i2; } else { forminmax_a=forminmax_i2; forminmax_b=forminmax_i1; } for (forminmax_i=forminmax_a;forminmax_i<forminmax_b;forminmax_i++) { forminmax_do; } }
    #else
        #define forminmax(forminmax_i1,forminmax_i2,forminmax_do); { register Sint16 forminmax_a,forminmax_b,forminmax_i; if (forminmax_i1<forminmax_i2) { forminmax_a=forminmax_i1; forminmax_b=forminmax_i2; } else { forminmax_a=forminmax_i2; forminmax_b=forminmax_i1; } for (forminmax_i=forminmax_a;forminmax_i<=forminmax_b;forminmax_i++) { forminmax_do; } }
    #endif
#endif

	{
		Sint16
			x1 = TR_x1,
			y1 = TR_y1,
			x2 = TR_x2,
			y2 = TR_y2,
			x3 = TR_x3,
			y3 = TR_y3;
			
		// Order points in ascending x
		// Hence edge 1-3 is longest
		if (y1>y2) {
			swap(Sint16,&x1,&x2); swap(Sint16,&y1,&y2);
		}
		if (y2>y3) {
			swap(Sint16,&x2,&x3); swap(Sint16,&y2,&y3);
			if (y1>y2) {
				swap(Sint16,&x1,&x2); swap(Sint16,&y1,&y2);
			}
		}

		{
			
		// 1-2
		Sint16
			dx12  = abs(x2 - x1),
			dy12  = abs(y2 - y1);
        Sint8
			ddx12 = sgn(x2-x1);
		// 1-3
		Sint16
			dx13  = abs(x3 - x1),
			dy13  = abs(y3 - y1);
        Sint8
			ddx13 = sgn(x3-x1);
		// 2-3
		Sint16
			dx23  = abs(x3 - x2),
			dy23  = abs(y3 - y2);
        Sint8
			ddx23 = sgn(x3-x2);
		register Sint16
			y   = y1,
			xb  = x1,
			xt  = x1,
            et  = 0,
			eb  = 0;

			if (y1==y2) {
                #ifndef NOEDGES
				    forminmax(x1,x2,TR_todo(forminmax_i,y));
                #endif
			} else {
			for ( ; y < y2; y++ )  {
				forminmax(xt,xb,TR_todo(forminmax_i,y));
				et += dx12;
				eb += dx13;
				while ( (et << 1) >= dy12 )  {
					xt+=ddx12;  et -= dy12;
				}
				while ( (eb << 1) >= dy13 )  {
					xb+=ddx13;  eb -= dy13;
				}
			}
			}
			if (y2==y3) {
                #ifndef NOEDGES
				    forminmax(x2,x3,TR_todo(forminmax_i,y));
                #endif
			} else {
			et=0; // Assertion?
			xt=x2; // True eycept in case y1==y2
			for ( ; y <= y3; y++ )  {
				forminmax(xt,xb,TR_todo(forminmax_i,y));
				et += dx23;
				eb += dx13;
				while ( (et << 1) >= dy23 )  {
					xt+=ddx23;  et -= dy23;
				}
				while ( (eb << 1) >= dy13 )  {
					xb+=ddx13;  eb -= dy13;
				}
			}
			}
		}
		
	}
