// Triangle plotting using Bresenham

// Provide:
// TR_x1,TR_y1,TR_x2,TR_y2,TR_x3,TR_y3
// TR_todo(x,y)

#define min(x,y) (x<y?x:y)
#define max(x,y) (x>y?x:y)
// #define swap(t,a,b); { t tmp; tmp=a; a=b; b=tmp; }
#define swap(t,a,b); { t tmp; tmp=*(a); *(a)=*(b); *(b)=tmp; }
#define swapint(a,b) swap(int,a,b)
// void swapint(int *a,int *b) { int tmp; tmp=*(a); *(a)=*(b); *(b)=tmp; }
#define forminmax(forminmax_i1,forminmax_i2,forminmax_do); { int forminmax_a,forminmax_b,forminmax_i; if (forminmax_i1<forminmax_i2) { forminmax_a=forminmax_i1; forminmax_b=forminmax_i2; } else { forminmax_a=forminmax_i2; forminmax_b=forminmax_i1; } for (forminmax_i=forminmax_a;forminmax_i<=forminmax_b;forminmax_i++) { forminmax_do; } }

	{
		int
			x1 = TR_x1,
			y1 = TR_y1,
			x2 = TR_x2,
			y2 = TR_y2,
			x3 = TR_x3,
			y3 = TR_y3;
			
		// Order points in ascending x
		// Hence edge 1-3 is longest
		if (x1>x2) {
			swapint(&x1,&x2); swapint(&y1,&y2);
		}
		if (x2>x3) {
			swapint(&x2,&x3); swapint(&y2,&y3);
			if (x1>x2) {
				swapint(&x1,&x2); swapint(&y1,&y2);
			}
		}

		{
			
		// 1-2
		int
			dx12  = abs(x2 - x1),
			dy12  = abs(y2 - y1),
			ddy12 = sgn(y2-y1);
		// 1-3
		int
			dx13  = abs(x3 - x1),
			dy13  = abs(y3 - y1),
			ddy13 = sgn(y3-y1);
		// 2-3
		int
			dx23  = abs(x3 - x2),
			dy23  = abs(y3 - y2),
			ddy23 = sgn(y3-y2);
		register int
			x   = x1,
			yb  = y1,
			yt  = y1,
			et  = 0,
			eb  = 0;

			if (x1==x2) {
				forminmax(y1,y2,TR_todo(x,forminmax_i));
			} else {
			for ( ; x < x2; x++ )  {
				forminmax(yt,yb,TR_todo(x,forminmax_i));
				et += dy12;
				eb += dy13;
				while ( (et << 1) >= dx12 )  {
					yt+=ddy12;  et -= dx12;
				}
				while ( (eb << 1) >= dx13 )  {
					yb+=ddy13;  eb -= dx13;
				}
			}
			}
			if (x2==x3) {
				forminmax(y2,y3,TR_todo(x,forminmax_i));
			} else {
			et=0; // Assertion?
			yt=y2; // True except in case x1==x2
			for ( ; x <= x3; x++ )  {
				forminmax(yt,yb,TR_todo(x,forminmax_i));
				et += dy23;
				eb += dy13;
				while ( (et << 1) >= dx23 )  {
					yt+=ddy23;  et -= dx23;
				}
				while ( (eb << 1) >= dx13 )  {
					yb+=ddy13;  eb -= dx13;
				}
			}
			}
		}
		
	}
