//15 multi-res
//
// Projection profiler, with vanishing points
// Measures: entropy, square sum, derivative square sum,
// variance
// Proportional mode optional

// Also finds vertical projection profile, and recovers
// fronto-parallel image

// The slowest part is due to the neccessary comparison
// of every +ve pixel position and every vanishing point
// to get the angle between them, to generate the
// projection profiles.

/* All my source code is freely distributable under the GNU public licence.
	I would be delighted to hear if have made use of this code.
	If you make money with this code, please give me some!
	If you find this code useful, or have any queries, please feel free to
	contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
	Paul "Joey" Clark, hacking for humanity, Feb 99
	www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk */

// #define USINGJFONT
// No printing of file input/output
#define QUIET

// Fix PP resolution relative to input image
// or allow user to specify?
#define FIXPPRES

// Do we run in proportional mode or not?
// Proportional mode uses floats so is slower
// but produces smoother projection profiles by
// distributing accumulations evenly over accumulators
#define PROPORTIONAL

// PPunit determines whether we should use floats or ints for
// accumulator, and for accumulator's index
#ifdef PROPORTIONAL
#define PPunit float
#else
#define PPunit int
#endif

#include <joeylib.h>

#include "../gentestimg/linespacings.c"

bool uselinespacings;

int LineSpacing=1;
int LineWidth=2;
// int correlator=LineSpacing;
// int correlator=LineWidth;
bool useoldspacingsmethod;
bool usetwolines;

RGBmp info;

int outcount=0;

int showbadx=-1;
int showbady=-1;
int maxpixelsused;
int recwid;

Line2d summarybaseline;
Line2d summarysecondline;
Line2d summaryleftline;
Line2d summarycenline;
Line2d summaryrightline;
List<V2d> summaryleftused;
List<V2d> summaryleftunused;
List<V2d> summaryrightused;
List<V2d> summaryrightunused;
List<V2d> summarycenused;
List<V2d> summarycenunused;
List<V2d> summarybaseused;
List<V2d> summarybaseunused;

float (*measure)(Map1d<PPunit>);

float interestingWidth(Map1d<PPunit> m) {
	return (float)(m.lastnonzero()-m.firstnonzero());
}

float measureentropy(Map1d<PPunit> m) {
	return m.entropy();
}
float measurederiv(Map1d<PPunit> m) {
	return -m.totalderiv();
}
float measurevariance(Map1d<PPunit> m) {
	return -m.variance();
}
float measuresquare(Map1d<PPunit> m) {
	// return measurevariance(m);
	return -m.totalsquare();
	// Does not help!
	// Favours narrowness, ie. x=inf on noisy images
	// return -m.totalsquare()/mysquare(interestingWidth(m));
	// Crazy
	// return -m.totalsquare()*mysquare(interestingWidth(m));
}

void map1ddotwritedata(Map1d<float> map,String fileName) {
	FILE *fp=fopen(fileName,"wa");
	for (int i=0;i<map.width;i++) {
		fprintf(fp,"%f\n",map.getpos(i));
	}
	fclose(fp);
}

// A vanishing point with its projection profile
// which will be accumulated using the add() function
class VP {
	public:
		V2d pos;
		Map1d<PPunit> pp;
		float left,right; // relative to vector to centre
		V2d tocentre;
		float realleft; // absolute
		PPunit q;

		// Set up the window of angles over which the VP can
		// see the text region.
		// Set up optimization variables to make the add()
		// function efficient.
		VP(V2d p,int res,int width,V2d imgcen) {
			pos=p;
			tocentre=(imgcen-p).norm();
			pp=Map1d<PPunit>(res); // => pp.width==res
			left=V2d::angbetween(imgcen+width*tocentre.perp()-pos,tocentre);
			right=V2d::angbetween(imgcen-width*tocentre.perp()-pos,tocentre);
			if (right<left)
				right=right+2.0*pi;
			q=(float)res/(right-left);
			realleft=modtwopi(tocentre.angle()+left);
			//    printf("VP %s l=%.2f r=%.2f\n",pos.toString(),left,right);
		}

		// Add to the projection profile a pixel at point v
		void add(V2d v) { // accepts a +- vector relative to centre of image, with mag<=width above
			//    printf("%s %s\n",v.toString(),pos.toString());
			// Fastest method
			float ang=modtwopi((v-pos).angle()-realleft);
			PPunit i=ang*q;
			// Slower method
			/* float ang=V2d::angbetween(v-pos,tocentre);
				if (ang<left)
				ang=ang+2.0*pi;
				PPunit i=(ang-left)*q; */
			// Even slower method
			/* float ang=V2d::angbetween(v-pos,tocentre);
				PPunit i=modtwopi(ang-left)*(float)pp.width/(right-left); */
#ifdef PROPORTIONAL
			if (i>=0 && i<pp.width-1)
				pp.incposprop(i);
#else
			if (i>=0 && i<pp.width)
				pp.incpos(i);
#endif
			else {
				outcount++;
				//      printf("%s: %s (i%i) out of range %i (l%f,%f,r%f),%i\n",pos.toString(),v.toString(),l,pp.width,left,ang,right,q);
			}
		}

};

float tofloat(VP *v) {
	return 0.0;
}

class ProjectionProfiler {
	public:
		int res,ppres;
		Map2d<VP *> vps;
		List<VP *> vpslist; // sometimes nicer to access
		Map2d<bool> binimg;
		Pixel best;
		bool writepps,writebestpp,doinfo;
		List<float> gaps;
		V2d imgcen;
		int scale;
		Line2d baseline;

		ProjectionProfiler() { }

		ProjectionProfiler(int c,int p,Map2d<bool> b,bool wpps,bool wbpp,bool doi) {
			res=c;
			ppres=p;
			binimg=b;
			imgcen=V2d((float)binimg.width/2.0,(float)binimg.height/2.0);
			writepps=wpps;
			writebestpp=wbpp;
			doinfo=doi;
		}

		// From vps map to infinity circle
		V2d maptocircle(int i,int j) {
			return (V2d(i,j)-V2d((float)res/2.0,(float)res/2.0))/((float)res/2.0);
		}

		// From infinity circle to vps map
		Pixel mapfromcircle(V2d v) {
			return (float)res/2.0*(v+V2d(1.0,1.0))+V2d(.5,.5);
		}

		// Initialises a set of vanishing points for which
		// projection profiles will be calculated
		// VPs are distributed "evenly" across R^2
		void setup() {
			setup(NULL);
		}
		void setup(Map2d<bool> *clipmap) {
			if (doinfo) {
				info=RGBmp(binimg.width,binimg.height,myRGB::white);
				/* for (int i=0;i<binimg.width;i++) // Get coloured in when seperate lines found
					for (int j=0;j<binimg.height;j++)
					if (binimg.getpos(i,j))
					info.setpos(i,j,myRGB::black); */
			}
			scale=(int)sqrt(mysquare(binimg.width)+mysquare(binimg.height))/2;
#ifdef FIXPPRES
			//      #ifdef PROPORTIONAL
			ppres=scale*2;
			ppres=scale;
			//      #else
			//        ppres=scale;
			//      #endif
#endif
			int hw=binimg.width/2;
			int hh=binimg.height/2;
			vps=Map2d<VP *>(res,res);
			int vpcount=0;
			for (int i=0;i<res;i++)
				for (int j=0;j<res;j++) {
					bool ok=false;
					if (clipmap==NULL)
						ok=true;
					else {
						int x=i*clipmap->width/res;
						int y=j*clipmap->height/res;
						if (clipmap->getpos(x,y)<0.5)
							ok=true;
					}
					if (ok) {
						V2d pos=maptocircle(i,j);
						//      printf("Circle=%s\n",pos.toString());
						vps.setpos(i,j,NULL);
						// Must lie inside infinity circle
						// if (pos.mag()<1.0) {
						if (pos.mag()<0.999) {
							// We offset the position of the VP slightly
							// so that the one at (0,0) is resolvable!
							V2d realpos=infcircletoplane(pos+V2d(0.0000001,0.0000001));
							// Must lie at some distance (PP boundary not accurate near or inside image)
							if (realpos.mag()>scale*0.4) { // 0.1 // 1.5) {
								realpos=realpos+V2d(hw,hh);
								//          printf("Plane=%s\n",realpos.toString());
								VP *vp=new VP(realpos,ppres,scale,imgcen);
								vps.setpos(i,j,vp);
								vpslist.add(vp);
								vpcount++;
							}
							}
						} else
							vps.setpos(i,j,NULL);
					}
					printf("Set up %i vanishing points with %i projection profiles\n",vpcount,vpcount);
					}

					/* void clipsetupby(Map2d<bool> *clipmap) {
						for (int i=0;i<res;i++)
						for (int j=0;j<res;j++) {
						int x=i*clipmap->width/res;
						int y=j*clipmap->height/res;
						if (clipmap->getpos(x,y)>0.5)
						vps.setpos(i,j,NULL);
						}
						} */

					// Accumulates projection profiles of the points
					// in the input image
					void calculate() {
						int w=binimg.width;
						int h=binimg.height;
						int vw=vps.width;
						int vh=vps.height;
						// Count number of pixels first
						int totalcount=0;
						for (int i=0;i<w;i++)
							for (int j=0;j<h;j++)
								if (binimg.getpos(i,j))
									totalcount++;
						printf("Total contributing pixels: %i\n",totalcount);
						// This bit speeds it up by using a maximum
						// number of pixels (ie. dropping some if there
						// are too many to process!)
						float prob=1.0;
						if (totalcount>maxpixelsused) {
							prob=(float)maxpixelsused/(float)totalcount;
							printf("Too many pixels! (>%i) .'. only using %.0f\%\n",maxpixelsused,prob*100);
						}
						// Now actually do the work
						for (int i=0;i<w;i++) {
							if (i%17==0) // Learn your 17 times table as the
								printf("%i/%i\n",i,w); // program runs!
							for (int j=0;j<h;j++) {
								if (binimg.getpos(i,j)) {
									if (myrnd()>prob) {
										// binimg.setpos(i,j,false); no longer removing so all pixels _can_ be used once pps over
									} else {
										V2d pos=V2d(i,j);
										VP *vp;
										// for (register int a=0;a<vw;a++)
										// for (register int b=0;b<vh;b++) {
										for (register int a=0;a<vpslist.len;a++) {
											// vp=vps.getpos(a,b);
											vp=vpslist.get(a);
											if (vp!=NULL)
												vp->add(pos);
										}
									}
									}
								}
							}
						}

						// Finds the entropy of all the projection profiles
						// and puts them in a map
						Map2d<float> getppmap() {
#define backgrounddummy -1234567.8e9
							Map2d<float> map=Map2d<float>(vps.width,vps.height,backgrounddummy);
							for (int i=0;i<vps.width;i++)
								for (int j=0;j<vps.height;j++)
									if (vps.getpos(i,j)!=NULL) {
										// float f=sqrt(mysquare((i-vps.width/2)/(float)vps.width)+mysquare((j-vps.height/2)/(float)vps.width));
										float f=0.0;
										map.setpos(i,j,(1.0-f)*measure(vps.getpos(i,j)->pp));
										//        map.setpos(i,j,-vps.getpos(i,j)->pp.totalsquare());
										//        map.setpos(i,j,-vps.getpos(i,j)->pp.totalderiv());
										//        map.setpos(i,j,vps.getpos(i,j)->pp.entropy());
									}
							map.searchreplace(backgrounddummy,map.largest());
							return map;
						}

						// Finds the best horizontal vanishing point by finding the
						// PP with lowest variance (or whatever measure)
						V2d getvp() {
							Map2d<float> map=getppmap();
							float lowest=0.0;
							V2d vp;
							for (int i=0;i<vps.width;i++)
								for (int j=0;j<vps.height;j++) {
									if (vps.getpos(i,j)!=NULL) {
										if (map.getpos(i,j)<lowest) {
											lowest=map.getpos(i,j);
											printf("Lowest so far (%i,%i) with %f\n",i,j,lowest);
											best=Pixel(i,j);
											vp=vps.getpos(i,j)->pos;
											// Draw this "good" pp
											if (writepps) {
												Map2d<bool> acc=vps.getpos(i,j)->pp.draw();
												acc.writefile(Sformat("pp%02ix%02i.bmp",i,j));
											}
										}
										if (showbadx==i && showbady==j) {
											Map2d<bool> acc=vps.getpos(i,j)->pp.draw();
											acc.writefile("badpp.bmp");
											map1ddotwritedata(vps.getpos(i,j)->pp,"badpp.data");
										}
									}
								}
							printf("Best projection profile: %s\n",best.toString());
							/* Plot the final PP */
							if (writebestpp) {
								Map2d<bool> acc=vps.getpos(best)->pp.draw();
								acc.writefile("bestpp.bmp");
								map1ddotwritedata(vps.getpos(best)->pp,"bestpp.data");
							}
							/* Enlarge the inf circle map */
#define crosssize 20.0
#define mapscale 5.0
							Map2d<float> map2=*map.scaledby(mapscale);
							/* Plot the VP point */
							V2d c=(V2d(best)+V2d(0.5,0.5))*mapscale;
							float col=map2.largest();
							float collow=map2.smallest();
							map2.thickcross(c,crosssize+2,crosssize/4.0+4,collow);
							map2.thickcross(c,crosssize,crosssize/4.0,col);
							if (showbadx>=0 && showbady>=0) {
								c=(V2d(showbadx,showbady)+V2d(0.5,0.5))*mapscale;
								map2.thickcross(c,crosssize+2,crosssize/4.0+4,col);
								map2.thickcross(c,crosssize,crosssize/4.0,collow);
							}
							map2.writefile("ppmap.bmp");
							return vp;
						}

						// Draw a Real^2 picture of the distribution of
						// the vanishing points
						Map2d<float> drawvps() {
							Map2d<float> b=Map2d<float>(300,300,0.0);
							//    float farthest=maxinfcir*1.1e2;
							float farthest=0.0;
							for (int i=0;i<vps.width;i++)
								for (int j=0;j<vps.height;j++)
									if (vps.getpos(i,j)!=NULL) {
										V2d v=vps.getpos(i,j)->pos;
										if (v.mod()>farthest)
											farthest=v.mod();
									}
							for (int i=0;i<vps.width;i++)
								for (int j=0;j<vps.height;j++)
									if (vps.getpos(i,j)!=NULL) {
										V2d v=vps.getpos(i,j)->pos;
										Pixel p=Pixel(b.width/2.0*((v.x-binimg.width/2)/farthest+1.0),
												b.height/2.0*((v.y-binimg.height/2)/farthest+1.0));
										float rad=5.0*v.mod()/farthest;
										b.circle(p,rad,vps.getpos(i,j)->pp.entropy());
										if (best==Pixel(i,j))
											b.circle(p,7,-1.0);
									}
							return b;
						}

						// Find the gaps between the lines (return as a list
						// of angles relative to the HVP )

						// Could find cuttoff value as:
						//   lowest value with highest number of transitions
						//   when moving horizontally across PP at that height
						// This should stop before (say) 1/2 because it could
						// be thrown off by spiked at top.

						void getgapangles() {
							VP v=*vps.getpos(best);
							Map1d<PPunit> pp=v.pp;
							/*pp.smoothed(2).draw().writefile("bestppsm2.bmp");
							  pp.smoothed(3).draw().writefile("bestppsm3.bmp");
							  pp.smoothed(4).draw().writefile("bestppsm4.bmp");
							  pp.smoothed(5).draw().writefile("bestppsm5.bmp");
							  pp.smoothed(6).draw().writefile("bestppsm6.bmp");
							  pp.smoothed(7).draw().writefile("bestppsm7.bmp");
							  pp.smoothed(8).draw().writefile("bestppsm8.bmp");*/
							PPunit top=pp.largest();

							// Possibly better but doesn't work so well on image29!
							PPunit slice=pp.averageCropped()*2.0;
							// PPunit slice=top;

							Map2d<bool> ppmap=pp.draw();
							ppmap.line(0,top-slice,ppmap.width-1,top-slice,0);

							// 3. Like 2 but \ defined as >1/2 -> <1/2 and / likewise
							int lastpeak=-1;
							int nextpeak=-1;
							for (int i=0;i<pp.width-1;i++) {
								if ( (pp.getpos(i) > slice/2) &&
										(pp.getpos(i+1) <= slice/2) )
									lastpeak=i;
								if ( (lastpeak!=-1) &&
										(pp.getpos(i) <= slice/2) &&
										(pp.getpos(i+1) > slice/2) )
									nextpeak=i+1;
								if ( (lastpeak!=-1) &&
										(nextpeak!=-1) ) {
									float cen=(float)(lastpeak+nextpeak)/2.0;
									float f=v.realleft+cen/(float)pp.width*(v.right-v.left);
									gaps.add(f);
									pp.setpos((int)cen,top-10);
									ppmap.line(cen,top/20.0,cen,top/5.0,0);
									//        pp.setpos(lastpeak,slice-20);
									//        pp.setpos(nextpeak,slice-20);
									lastpeak=-1;
									nextpeak=-1;
								}
							}
							/*  // 2. Look for \ followed by / .  Use centre.
								 int lastdown=-1;
								 for (int i=0;i<pp.width-1;i++) {
								 if ( pp.getpos(i) > pp.getpos(i+1) )
								 lastdown=i;
								 if ( pp.getpos(i+1) > pp.getpos(i)
								 && lastdown!=-1 ) {
								 int firstup=i+1;
								 float cen=(float)(lastdown+firstup)/2.0;
								 if ( pp.getpos(cen) < top/2 ) {
								 float f=v.realleft+(cen)/(float)(pp.width)*(v.right-v.left);
								 gaps.add(f);
							//          printf("Gap %i: %f\n",gaps.len,f);
							pp.setpos((int)cen,slice);
							}
							lastdown=-1;
							}
							}*/
							/* // 1. Find change in deriv
							// i<pp.width-2
							if ( pp.getpos(i+1) < slice/2.0 &&
							pp.getpos(i+1)-pp.getpos(i) < 0 &&
							pp.getpos(i+2)-pp.getpos(i+1) > 0 ) {
							float f=v.realleft+(float)(i+1)/(float)(pp.width)*(v.right-v.left);
							gaps.add(f);
							printf("Gap %i: %f\n",gaps.len,f);
							pp.setpos(i+1,slice);
							} */
							printf("Found %i gaps between lines\n",gaps.len);
							// pp.draw().writefile("bestppgaps.bmp");
							ppmap.writefile("bestppgaps.bmp");
						}

						int whichline(V2d p) { // between 1 and gaps.len+1
							VP v=*vps.getpos(best);
							float ang=(p-v.pos).angle();
							for (int i=1;i<=gaps.len;i++)
								if (anglebetween(gaps.num(i),ang)<0)
									return i;
							return gaps.len+1;
						}

						V2d getvvp() {
							getgapangles();
							V2d hvp=vps.getpos(best)->pos;

							printf("Collecting split lines into regions...\n");
							// Find centroid of each line
							List<V2d> cens;
							List<int> cnt;
							List<Region> liner;
							List<Line2d> lines;
							// Set up
							for (int i=0;i<=gaps.len;i++) {
								cens.add(V2d(0,0));
								cnt.add(0);
								liner.add(Region(binimg.width,binimg.height));
							}
							// Collect pixels into centroids
							for (int i=0;i<binimg.width;i++)
								for (int j=0;j<binimg.height;j++) {
									if (binimg.getpos(i,j)) {
										int k=whichline(V2d(i,j));
										//        printf("%i ",k);
										int n=cnt.num(k);
										V2d old=cens.num(k);
										V2d vn=(old*(float)n+V2d(i,j))/(float)(n+1);
										cens.setnum(k,vn);
										cnt.setnum(k,n+1);
										liner.num(k).add(Pixel(i,j));
										if (doinfo) {
											myRGB col = ( k%2 == 0
													? myRGB::blue
													: myRGB::green ).dark();
											info.setpos(i,j,col);
										}
									}
								}

							printf("Correlating ends of lines...\n");
							// Make three correlators to generate lines for
							// the left, center and right
							// Correlate the centroids to find a line
							// up the middle
							Correlator2d leftc,cenc,rightc;
							for (int i=1;i<=cens.len;i++) {
								Line2d l=liner.num(i).endtoendvp(hvp);
								lines.add(l);
								//      cens.setnum(i,(l.a+l.b)/2.0);
								leftc.add(l.a);
								rightc.add(l.b);
								cenc.add((l.a+l.b)/2.0);
							}
							float lefterr=leftc.error();
							float cenerr=cenc.error();
							float righterr=rightc.error();

							// Check for justified paragraph
							// return VVP;
							int chosenBaseline=0; // 0=centre, -1=left, +1=right
							if (cenerr<lefterr && cenerr<righterr) {
								baseline=cenc.line();
								chosenBaseline=0;
								printf("\nStrongest correlation along CENTRES of lines\n\n");
								if (lefterr<righterr)
									summarysecondline=leftc.line();
								else
									summarysecondline=rightc.line();
							} else {
								if (lefterr<righterr) {
									baseline=leftc.line();
									chosenBaseline=-1;
									printf("\nStrongest correlation along LEFT of lines\n\n");
									if (cenerr<righterr)
										summarysecondline=cenc.line();
									else
										summarysecondline=rightc.line();
								} else {
									baseline=rightc.line();
									chosenBaseline=+1;
									printf("\nStrongest correlation along RIGHT of lines\n\n");
									if (lefterr<cenerr)
										summarysecondline=leftc.line();
									else
										summarysecondline=cenc.line();
								}
							}
							summaryleftline=leftc.line();
							summaryrightline=rightc.line();

							// Find centre
							//    V2d ave=c.centroid();
							//    printf("Angle up middle %f with centre %s\n",ang,ave.toString());
							V2d vvpdir=(baseline.b-baseline.a).norm();

							if (doinfo) {
								// Show centres correlation
								drawCorrelator2d(cenc).writefile("corcentre.bmp");
								drawCorrelator2d(leftc).writefile("corleft.bmp");
								drawCorrelator2d(rightc).writefile("corright.bmp");
								// Draw line through each gap
								Line2d upmiddle=baseline; // Line2d(ave-vvpdir*50.0,ave+vvpdir*50.0);
								for (int i=1;i<=gaps.len;i++) {
									Line2d fromside=Line2d(hvp,hvp+V2d::angle(gaps.num(i)));
									V2d hit=upmiddle.intersect(fromside);
									V2d farside=hvp+2.0*(hit-hvp);
									info.line(hvp,farside,myRGB::black); // used to be yellow
								}
								// Draw central line
								V2d diff=vvpdir*(float)max(binimg.width,binimg.height)/2.0;
								summarybaseline=Line2d(baseline.a-diff,baseline.a+diff);
								printf("Created summary baseline %s with %s %s\n",summarybaseline.toString(),baseline.a.toString(),diff.toString());
								info.line(summarybaseline,myRGB::black);
								// Plot correlation points
								for (int i=1;i<=cens.len;i++) { 
									if (leftc.used(i)) {
										info.cross(leftc.points.num(i).x,leftc.points.num(i).y,5,myRGB::red);
										summaryleftused.add(new V2d(leftc.points.num(i).x,leftc.points.num(i).y));
									} else {
										info.opencircle(leftc.points.num(i).x,leftc.points.num(i).y,5,myRGB::red);
										summaryleftunused.add(new V2d(leftc.points.num(i).x,leftc.points.num(i).y));
									}
									if (cenc.used(i)) {
										info.cross(cenc.points.num(i).x,cenc.points.num(i).y,5,myRGB::red);
										summarycenused.add(new V2d(cenc.points.num(i).x,cenc.points.num(i).y));
									} else {
										info.opencircle(cenc.points.num(i).x,cenc.points.num(i).y,5,myRGB::red);
										summarycenunused.add(new V2d(cenc.points.num(i).x,cenc.points.num(i).y));
									}
									if (rightc.used(i)) {
										info.cross(rightc.points.num(i).x,rightc.points.num(i).y,5,myRGB::red);
										summaryrightused.add(new V2d(rightc.points.num(i).x,rightc.points.num(i).y));
									} else {
										info.opencircle(rightc.points.num(i).x,rightc.points.num(i).y,5,myRGB::red);
										summaryrightunused.add(new V2d(rightc.points.num(i).x,rightc.points.num(i).y));
									}
								}
								if (chosenBaseline==-1) {
									summarybaseused=summaryleftused;
									summarybaseunused=summaryleftunused;
								} else if (chosenBaseline==0) {
									summarybaseused=summarycenused;
									summarybaseunused=summarycenunused;
								} else if (chosenBaseline==+1) {
									summarybaseused=summaryrightused;
									summarybaseunused=summaryrightunused;
								} else
									error("Baseline != 0 -1 or +1");
							}

							V2d vvp;

							if (usetwolines) {
								printf("Intersecting two best lines to get VVP.\n");
								vvp=summarybaseline.intersect(summarysecondline);
								printf("%s x %s = %s\n",summarybaseline.toString(),summarysecondline.toString(),vvp.toString());
								return vvp;
							}
							// if (!useoldspacingsmethod) {

							// New vvp estimation from spacings
							List<V2d> endpoints;
							for (int i=0;i<cens.len;i++) {
								Line2d l=Line2d(hvp,cens.get(i));
								V2d v=baseline.intersect(l);
								endpoints.add(v);
							}
							vvp=vvpFromPoints(baseline,endpoints,binimg.width,binimg.height,uselinespacings);

							/* } else {

							// Now correlate line spacings along the central line
							// First, project all centres onto this line
							List<float> cds;
							for (int i=1;i<=cens.len;i++) {
							Line2d l=Line2d(hvp,cens.num(i));
							V2d v=baseline.intersect(l);
							// if (doinfo)
							//  info.cross(v,5,myRGB::cyan);
							float f=(v-baseline.a).dot(vvpdir);
							cds.add(f);
							}

							// We now do simultaneous calculation of
							// various correlation methods for comparison

							// Accumulate correlator
							Correlator2d csspacing;
							Correlator2d cslength;
							// Why not do line-spacing with gaps?
							// (seems equivalent so no worry)
							int cdsend=( correlator==LineSpacing
							? cds.len-1
							: cds.len );
							for (int i=1;i<=cdsend;i++) {
							float a=cds.num(i);
							float pos=a;
							int weight=cnt.num(i);
							if (i<cds.len) {
							// if (correlator==LineSpacing) {
							// x = average of two cds, y = dist between
							float b=cds.num(i+1);
							// float pos=(a+b)/2.0;
							csspacing.add(V2d(pos,myabs(b-a)),weight);
							// printf("Correlating using lines =)\n");
							}
							// } else if (correlator==LineWidth) {
							// y = width of line
							// float pos=a;
							cslength.add(V2d(pos,lines.num(i).length),weight);
							// } else {
							// error("Non existent correlation method");
							// }
							}

							// One of the methods is chosen for final VVP.
							float vpd=( correlator==LineSpacing ?
							csspacing.crossesy() :
							cslength.crossesy() );
							printf("VP is %f along baseline\n",vpd);
							// vvp=baseline.a+vvpdir*vpd;
							V2d oldvvp=baseline.a+vvpdir*vpd;
							printf("Old method got VVP = %s\n",oldvvp.toString());
							vvp=oldvvp;
							if (doinfo) {
							drawCorrelator2dInColour(csspacing).writefile("spacings.bmp");
							drawCorrelator2dInColour(cslength).writefile("lengths.bmp");
							// V2d left=imgcen+(hvp-imgcen).norm()*(float)scale/3.0;
							// V2d right=imgcen-(hvp-imgcen).norm()*(float)scale/3.0;
							// info.line(left,vvp,myRGB::cyan);
							// info.line(right,vvp,myRGB::cyan);
							}

							} */

							// printf("New method got VVP = %s\n",vvp.toString());

							return vvp;
						}

					};

					void main(int argc,String *argv) {

						ArgParser a=ArgParser(argc,argv);
						bool recoverquad=true;

						a.comment("Finding horizontal vanishing point using projection profiles:");
						int cirres=a.intafter("-res","resolution of final circle",120);
						bool dolowresscan=a.argexists("-dolowresscan","do an initial low res scan");
						bool domiddlescan=a.argexists("-domiddlescan","do a middle scan");
						int medcirres=a.intafter("-medres","resolution of initial circle",60);
						int lowcirres=a.intafter("-lowres","resolution of initial circle",30);
						float smoothlowres=a.floatafter("-lowsmooth","smooth lowres before derivatives",1);
						maxpixelsused=a.intafter("-maxpixels","max # pixels to collect",5000);
						recwid=a.intafter("-recwid","Width of recovered image",500);
						bool usezerocrossings=a.argexists("-usezerocross","use minima to guide second PPmap pass (otherwise use adaptive threshold)");
						float adapthresh=a.floatafter("-adapt","adaptive threshold for clipping",0.1);
						int ppres=-1;
#ifndef FIXPPRES
						ppres=a.intafter("-ppres","resolution of each projection profile",200);
#endif

						a.comment("Finding vertical vanishing point using margins or curve fitting:");
						bool usebaselineonly=!a.argexists("-usetwolines","use two best correlated lines instead of just baseline");
						usetwolines=!usebaselineonly;
						// correlator=( a.argexists("-spacings","use line spacing not width")
								// ? LineSpacing
								// : LineWidth );
						uselinespacings=a.argexists("-spacings","use spacings rather than simple pos for fitting");

						a.comment("Finding horizontal vanishing point using projection profiles:");
						bool writepps=a.argexists("-writepps","write the potential PPs");
						bool writebestpp=!a.argexists("-dontwritebestpp","don't write the best PP");
						bool doinfo=true; // Needed for summaries!  a.argexists("-doinfo","draw info image");
						float gamma=a.floatafter("-gamma","take original image this far to white before summary",0.4);
						useoldspacingsmethod=a.argexists("-oldspacings","use old spacings method");
						if (a.argexists("-corinc","use over-inclusive RANSAC"))
							Correlator2dMethod=2;
						bool overlayquadonly=a.argexists("-quadonly","overlay quadrilateral only on summary image");
						int overlaythickness=a.intafter("-oth","overlay thickness",7);
						bool overlayinfquadonly=a.argexists("-infquadonly","overlay extended quadrilateral only on summary image");
						bool lightoverlay=a.argexists("-light","light overlay for dark image");
						//  a.comment("Info image options:");
						a.comment("PP measure of correctness: (default = derivative)");
						measure=&measurederiv;
						if (a.argexists("-deriv","use derivative measure"))
							measure=&measurederiv;
						if (a.argexists("-square","use square measure"))
							measure=&measuresquare;
						if (a.argexists("-entropy","use entropy measure"))
							measure=&measureentropy;
						if (a.argexists("-variance","use variance measure"))
							measure=&measurevariance;
						//  bool invert=a.argexists("-inv","invert input image to look for line spaces");
						showbadx=a.intafter("-badx","x pos of bad PP to show",-1);
						showbady=a.intafter("-bady","y pos of bad PP to show",-1);
						String bname=a.getarg("binary image");
						String oname=a.argor("original image","*none*");
						String datafile=a.argor("crop data file","*none*");
						a.done();

						Map2d<bool> binimg=*Map2d<bool>::readfile(bname)->threshold(0.5);
						RGBmp origimage;
						if (!Seq(oname,"*none*"))
							origimage=*(RGBmp::readfile(oname));
						else
							origimage=*(RGBmp::toRGBmp(&binimg));
						// origimage.writefile("test2.bmp");

						/* Inverting to use gaps instead of lines
						 * is naff because it takes too long, due to
						 * have to project all of black pixels
						 * Need to be more cunning!
						 * if (invert)
						 * binimg.invert();
						 */

						ProjectionProfiler pplowres;
						Map2d<bool> *lowresthresh;

						if (dolowresscan) {
							// First scan
							Map2d<float> lowresmap;
							pplowres=ProjectionProfiler(lowcirres,ppres,binimg,writepps,writebestpp,doinfo);
							pplowres.setup();
							pplowres.calculate();
							lowresmap=pplowres.getppmap();
							lowresmap.writefile("lowresmap.bmp");
							// lowresmap=*lowresmap.smoothed(smoothlowres);
							lowresmap=*lowresmap.filterby(lowresmap.gaussian(smoothlowres));
							lowresmap.writefile("lowresmapsmoothed.bmp");
							if (usezerocrossings) {

								Map2d<float> *filter=Map2d<float>::sobel();
								// Map2d<float> *filter=Map2d<float>::simple();

								// // well just a little experiment...
								// Map2d<float> *edgemag,*edgeang;
								// Map2d<float> *deriv2mag,*deriv2ang;
								// lowresmap.edgedetection(Map2d<float>::sobel(),&edgemag,&edgeang);
								// edgemag->edgedetection(Map2d<float>::sobel(),&deriv2mag,&deriv2ang);
								// edgemag->writefile("5edgemag.bmp");
								// // edgeang->writefile("5edgeang.bmp");
								// deriv2mag->writefile("5deriv2mag.bmp");
								// // deriv2ang->writefile("5deriv2ang.bmp");

								Map2d<float> hderiv=*lowresmap.filterby(filter);
								Map2d<float> vderiv=*lowresmap.filterby(filter->transpose());
								hderiv.writefile("5hderiv.bmp");
								vderiv.writefile("5vderiv.bmp");
								float zero=0;
								Map2d<bool> zerocrossimg=Map2d<bool>(hderiv.width,hderiv.height,false);
								for (int i=0;i<hderiv.width-1;i++)
									for (int j=0;j<hderiv.height-1;j++) {
										if (hderiv.getpos(i,j)<zero && hderiv.getpos(i+1,j)>=zero) {
											zerocrossimg.setpos(i,j,true);
											zerocrossimg.setpos(i+1,j,true);
											zerocrossimg.setpos(i-1,j,true); // yuk
										}
										if (vderiv.getpos(i,j)<zero && vderiv.getpos(i,j+1)>=zero) {
											zerocrossimg.setpos(i,j,true);
											zerocrossimg.setpos(i,j+1,true);
											zerocrossimg.setpos(i,j-1,true); // yuk
										}
									}
								zerocrossimg.writefile("5zerocross.bmp");
								lowresthresh=&zerocrossimg;
								// Dirty hack to normalise:
								lowresthresh=lowresthresh->expand(0);
							} else { // Do adaptive thresholding
								lowresthresh=lowresmap.adaptivethreshold(adapthresh);
								lowresthresh->writefile("lowresthresh0.bmp");
								lowresthresh->invert();
								lowresthresh=lowresthresh->expand(1);
							}
							lowresthresh->invert();
							lowresthresh->writefile("lowresthresh1.bmp");

							if (domiddlescan) {
								// Second scan
								pplowres=ProjectionProfiler(medcirres,ppres,binimg,writepps,writebestpp,doinfo);
								pplowres.setup(lowresthresh);
								pplowres.calculate();
								lowresmap=pplowres.getppmap();
								lowresmap.writefile("lowres2map.bmp");
								// lowresmap=*lowresmap.smoothed(smoothlowres);
								lowresmap=*lowresmap.filterby(lowresmap.gaussian(smoothlowres));
								lowresmap.writefile("lowres2mapsmoothed.bmp");
								if (usezerocrossings) {

									Map2d<float> *filter=Map2d<float>::sobel();
									// Map2d<float> *filter=Map2d<float>::simple();

									// // well just a little experiment...
									// Map2d<float> *edgemag,*edgeang;
									// Map2d<float> *deriv2mag,*deriv2ang;
									// lowresmap.edgedetection(Map2d<float>::sobel(),&edgemag,&edgeang);
									// edgemag->edgedetection(Map2d<float>::sobel(),&deriv2mag,&deriv2ang);
									// edgemag->writefile("5edgemag.bmp");
									// // edgeang->writefile("5edgeang.bmp");
									// deriv2mag->writefile("5deriv2mag.bmp");
									// // deriv2ang->writefile("5deriv2ang.bmp");

									Map2d<float> hderiv=*lowresmap.filterby(filter);
									Map2d<float> vderiv=*lowresmap.filterby(filter->transpose());
									hderiv.writefile("5hderiv.bmp");
									vderiv.writefile("5vderiv.bmp");
									float zero=0;
									Map2d<bool> zerocrossimg=Map2d<bool>(hderiv.width,hderiv.height,false);
									for (int i=0;i<hderiv.width-1;i++)
										for (int j=0;j<hderiv.height-1;j++) {
											if (hderiv.getpos(i,j)<zero && hderiv.getpos(i+1,j)>=zero) {
												zerocrossimg.setpos(i,j,true);
												zerocrossimg.setpos(i+1,j,true);
												zerocrossimg.setpos(i-1,j,true); // yuk
											}
											if (vderiv.getpos(i,j)<zero && vderiv.getpos(i,j+1)>=zero) {
												zerocrossimg.setpos(i,j,true);
												zerocrossimg.setpos(i,j+1,true);
												zerocrossimg.setpos(i,j-1,true); // yuk
											}
										}
									zerocrossimg.writefile("5zerocross.bmp");
									lowresthresh=&zerocrossimg;
									// Dirty hack to normalise:
									lowresthresh=lowresthresh->expand(0);
								} else { // Do adaptive thresholding
									lowresthresh=lowresmap.adaptivethreshold(0.005);
									lowresthresh->writefile("lowres2thresh0.bmp");
									lowresthresh->invert();
									// lowresthresh=lowresthresh->expand(1);
								}
								lowresthresh->invert();
								lowresthresh->writefile("lowres2thresh1.bmp");
							}
						}

						ProjectionProfiler pp=ProjectionProfiler(cirres,ppres,binimg,writepps,writebestpp,doinfo);
						if (dolowresscan) {
							pp.setup(lowresthresh);
						} else {
							pp.setup();
						}
						// pp.clipsetupby(lowresthresh);
						pp.calculate();
						V2d hvp=pp.getvp();
						printf("HVP = %s *note* could wait for corner+diffscale*\n",hvp.toString());
						useoldspacingsmethod=false;
						V2d vvp=pp.getvvp();
						// useoldspacingsmethod=true;
						// V2d oldvvp=pp.getvvp();
						printf(" -> Old VVP estimate: %s\n",summarybaseline.intersect(summarysecondline).toString());
						printf(" -> New VVP estimate: %s\n",vvp.toString());

						VP vp=*pp.vps.getpos(pp.best);
						hvp=vp.pos;

						V3d eye=V3d(binimg.width/2,binimg.height/2,-binimg.width);
						if (usebaselineonly) {
							// printf("Guessing VVP from base-line.\n");
							// V2d newvvp=vvpfromhvpandbaseline(hvp,pp.baseline,eye,info);
							// printf("    Original vvp estimate: %s\n",vvp.toString());
							// printf("    Dodgy new vvp estimate: %s\n",newvvp.toString());
							// if (mysgn(newvvp.x)!=mysgn(vvp.x)) {
							// printf("Nert swerping.\n");      newvvp.x=-newvvp.x;
							// }
							// if (mysgn(newvvp.y)!=mysgn(vvp.y)) {
							// printf("Nert swerping.\n");      newvvp.y=-newvvp.y;
							// }
							// vvp=newvvp;
						} else {
						}
						// printf("    VVP estimate: %s\n",vvp.toString());
						printf("VVP = %s\n",vvp.toString());

						if (recoverquad) {

							// Find top,bottom,left,right angles
							// int penlarge=20;
							int penlarge=0;
							float henlarge=(float)penlarge/hvp.mag();
							float venlarge=(float)penlarge/vvp.mag();
							float ha=vp.realleft+(vp.right-vp.left)*(float)vp.pp.firstnonzero()/(float)vp.pp.width-henlarge;
							float hb=vp.realleft+(vp.right-vp.left)*(float)vp.pp.lastnonzero()/(float)vp.pp.width+henlarge;
							if (hvp.x<0) // Ensure top and bottom correct
								swap(&ha,&hb);
							float va=(pp.imgcen-vvp).angle();
							float vb=va;
							for (int i=0;i<binimg.width;i++)
								for (int j=0;j<binimg.height;j++)
									if (binimg.getpos(i,j)) {
										V2d v=V2d(i,j);
										float ang=(v-vvp).angle();
										float da=mymod(ang-va,-pi,+pi);
										if (da<0)
											va=ang;
										float db=mymod(ang-vb,-pi,+pi);
										if (db>0)
											vb=ang;
									}
							va=va-venlarge;
							vb=vb+venlarge;
							if (vvp.y>0) // Ensure left and right correct
								swap(&va,&vb);
							float bigtmp=max(vvp.mag(),hvp.mag())*1.5;
							Line2d top=Line2d(hvp,hvp+bigtmp*V2d::angle(ha));
							Line2d bottom=Line2d(hvp,hvp+bigtmp*V2d::angle(hb));
							Line2d left=Line2d(vvp,vvp+bigtmp*V2d::angle(va));
							Line2d right=Line2d(vvp,vvp+bigtmp*V2d::angle(vb));
							if (doinfo) {
								info.line(top,myRGB::magenta.dark());
								info.line(bottom,myRGB::magenta.dark());
								info.line(left,myRGB::magenta.dark());
								info.line(right,myRGB::magenta.dark());
							}
							if (doinfo) {
								pp.drawvps().writefile("pps.bmp");
							}
							V2d tl=top.intersect(left);
							V2d tr=top.intersect(right);
							V2d bl=bottom.intersect(left);
							V2d br=bottom.intersect(right);
							/*    printf("TL: %s\n",tl.toString());
									printf("TR: %s\n",tr.toString());
									printf("BR: %s\n",br.toString());
									printf("BL: %s\n",bl.toString());*/
							if (doinfo) {
								info.line(tl,tr,myRGB::grey);
								info.line(bl,br,myRGB::grey);
								info.line(tl,bl,myRGB::grey);
								info.line(tr,br,myRGB::grey);
							}

							// Recover quadrilateral

							/* // New method using angles
								float aspect=(tl.dist(bl)+tr.dist(br))/(tl.dist(tr)+bl.dist(br));
								RGBmp n=RGBmp(600,600*aspect);
								for (int i=0;i<n.width;i++)
								for (int j=0;j<n.height;j++) {
								float xang=va+(vb-va)*(float)i/(float)n.width;
								float yang=ha+(hb-ha)*(float)j/(float)n.height;
								Line2d xline=Line2d(hvp,hvp+bigtmp*V2d::angle(yang));
								Line2d yline=Line2d(vvp,vvp+bigtmp*V2d::angle(xang));
								V2d hit=xline.intersect(yline);
								n.setpos(i,j,origimage.getinter(hit.x,hit.y));
								} */

							/* // Linear
								RGBmp n=*origimage.recoverquad(tl,tr,br,bl,600); */

							/*    // 3D estimate (best so far)
							//    RGBmp n=*origimage.recoverquad(&qs,1,600);
							RGBmp n=*origimage.recoverquadrilateral(tl,tr,br,bl,600); */

							V3d RIGHT,DOWN;
							// New method using VPs to find focal length
							// RIGHT=V3d(hvp.x-eye.x,hvp.y-eye.y,-1234);
							// DOWN=V3d(vvp.x-eye.x,vvp.y-eye.y,-1234);
							// printf("Know XY of right %s and down %s\n",RIGHT.dropz().toString(),DOWN.dropz().toString());
							// float fls=(RIGHT.x)*(DOWN.x)+(RIGHT.y)*(DOWN.y);
							// if (fls>=0)
							// printf("\n*** failed estimation of focal length %f ***\n\n",fls);
							// float focallength=(
							// fls<0 ? sqrt(-fls) : 2560 // binimg.width
							// );
							// printf("Got focal length %f\n",focallength);
							// RIGHT.z=focallength;
							// DOWN.z=focallength;
							// eye.z=-focallength;
							// 	
							// V3d tmpRIGHT=RIGHT; tmpRIGHT.y=-tmpRIGHT.y;
							// printf("right = %s\n",tmpRIGHT.toString());
							// printf("down = %s\n",DOWN.toString());

							// V3d A=V3d(tl.x,tl.y,0);
							// V3d B=Line3d(eye,V3d(bl.x,bl.y,0)).intersect(Line3d(A,A+DOWN));
							// DOWN=B-A;
							// V3d C=Line3d(eye,V3d(tr.x,tr.y,0)).intersect(Line3d(A,A+RIGHT));
							// RIGHT=C-A;
							// V3d D=C+DOWN; // this goes wrong!

							// Back to crap recovery method
							List<V2d> qs;
							qs.add(tl); qs.add(tr); qs.add(br); qs.add(bl);
							List<V3d> ws=rectanglefromquadrilateral(qs,eye);
							V3d A=ws.num(1);
							V3d B=ws.num(2);
							V3d C=ws.num(3);
							V3d D=ws.num(4);

							// Multiple cross products!
							// (OA x OB) x (OC x OD)
							//   .
							// (OA x OC) x (OB x OD)
							// =
							// ( z(ax-bx)(cxdy-cydx)-z(cx-dx)(axby-aybx) )
							//   x ( z(ax-cx)(bxdy-bydx) - z(bx-dx)(axcy-aycx) )
							// +
							// ( z(cy-dy)(axby-aybx) - z(ay-by)(cxdy-cydx) )
							//   x ( z(by-dy)(axcy-aycx) - z(ay-cy)(bxdy-bydx) )
							// +
							// zz ( (ay-by)(cx-dx) - (ax-bx)(cy-dy) )
							//   x ( (ay-cy)(bx-dx) - (ax-cx)(by-dy) )
							// A = ( (ay-by)(cx-dx) - (ax-bx)(cy-dy) ) * ( (ay-cy)(bx-dx) - (ax-cx)(by-dy) )
							// B =  ( (ax-bx)(cxdy-cydx)-(cx-dx)(axby-aybx) ) * ( (ax-cx)(bxdy-bydx) - (bx-dx)(axcy-aycx) ) + ( (cy-dy)(axby-aybx) - (ay-by)(cxdy-cydx) ) *  ( (by-dy)(axcy-aycx) - (ay-cy)(bxdy-bydx) )
							tl=V2d(tl.x-(float)binimg.width/2.0,tl.y-(float)binimg.height/2.0);
							bl=V2d(bl.x-(float)binimg.width/2.0,bl.y-(float)binimg.height/2.0);
							tr=V2d(tr.x-(float)binimg.width/2.0,tr.y-(float)binimg.height/2.0);
							br=V2d(br.x-(float)binimg.width/2.0,br.y-(float)binimg.height/2.0);
							float solA = ( (tl.y-bl.y)*(tr.x-br.x) - (tl.x-bl.x)*(tr.y-br.y) ) * ( (tl.y-tr.y)*(bl.x-br.x) - (tl.x-tr.x)*(bl.y-br.y) );
							float solB =  ( (tl.x-bl.x)*(tr.x*br.y-tr.y*br.x)-(tr.x-br.x)*(tl.x*bl.y-tl.y*bl.x) ) * ( (tl.x-tr.x)*(bl.x*br.y-bl.y*br.x) - (bl.x-br.x)*(tl.x*tr.y-tl.y*tr.x) ) + ( (tr.y-br.y)*(tl.x*bl.y-tl.y*bl.x) - (tl.y-bl.y)*(tr.x*br.y-tr.y*br.x) ) *  ( (bl.y-br.y)*(tl.x*tr.y-tl.y*tr.x) - (tl.y-tr.y)*(bl.x*br.y-bl.y*br.x) );
							tl=V2d(tl.x+(float)binimg.width/2.0,tl.y+(float)binimg.height/2.0);
							bl=V2d(bl.x+(float)binimg.width/2.0,bl.y+(float)binimg.height/2.0);
							tr=V2d(tr.x+(float)binimg.width/2.0,tr.y+(float)binimg.height/2.0);
							br=V2d(br.x+(float)binimg.width/2.0,br.y+(float)binimg.height/2.0);
							float fsq=-solB/solA;
							printf("f^2 = %f\n",fsq);
							if (fsq>0) {
								float focal=sqrt(fsq);
								printf("Got focal length = %f\n",focal);
								printf("^^___ using new method\n");
								// A=V3d(tl.x,tl.y,focal);
								// B=V3d(tr.x,tr.y,focal);
								// C=V3d(br.x,br.y,focal);
								// D=V3d(bl.x,bl.y,focal);
							}

							float aspect=(V3d::dist(B,C)+V3d::dist(D,A))/(V3d::dist(A,B)+V3d::dist(C,D));

							// Expand quadrilateral by 10% of width on all sides
							RIGHT=(B-A)*0.1;
							DOWN=(C-B).normalised()*RIGHT.mag();
							A=A-RIGHT-DOWN;
							B=B+RIGHT-DOWN;
							C=C+RIGHT+DOWN;
							D=D-RIGHT+DOWN;

							// Everything up until now has been in binary image space

							// Work out mapping from binary image to original image
							V2d corner=V2d(0,0);
							float diffscale=(float)origimage.width/(float)binimg.width;
							if (!Seq(datafile,"*none*")) {
								List<String> cropdat=readlinesfromfile(datafile);
								float cropl=floatFromString(cropdat.num(1));
								float cropr=floatFromString(cropdat.num(2));
								float cropt=floatFromString(cropdat.num(3));
								float cropb=floatFromString(cropdat.num(4));
								corner.x=origimage.width*cropl;
								corner.y=origimage.height*cropt;
								diffscale=(float)origimage.width*(cropr-cropl)/(float)binimg.width;
							}

							// Move corners into origimage scale
							tl=corner+diffscale*tl;
							tr=corner+diffscale*tr;
							bl=corner+diffscale*bl;
							br=corner+diffscale*br;

							// Recover
							printf("Doing recovery...\n");
							// printf("This is rubbish:\n");
							// // printf("roll = %f\n",V3d::angle(
							// // Assuming roll is 90:
							// printf("pitch = %f\n",V3d::angle(DOWN,-V3d::i));
							// printf("yaw = %f\n",V3d::angle(RIGHT,-V3d::j));

							// if ( V2d::dist(tl,tr)/V2d::dist(bl,br) > 6.0 || V2d::dist(bl,br)/V2d::dist(tl,tr) > 6.0 ) {
							if ( aspect>6.0 || aspect<1.0/6.0 ) {
								printf("Aspect ratio %f (%ix%i) too much.  Skipping recovery.\n",V2d::dist(bl,br)/V2d::dist(tl,tr),V2d::dist(bl,br),V2d::dist(tl,tr));
							} else {
								int size=max(V2d::dist(tl,tr),V2d::dist(bl,br))*4;
								if (size>recwid)
									size=recwid;
								if (size*size*aspect>recwid*recwid) {
									size=recwid/aspect;
									printf("MODIFIED size because too large - probably bad!\n");
								}
								printf("About to declare rgbmp %ix%i\n",size,(int)(size*aspect));
								RGBmp n=RGBmp(size,size*aspect);
								printf("Done.\n");
								// RGBmp n=RGBmp(600,600*aspect);
								for (int i=0;i<n.width;i++)
									for (int j=0;j<n.height;j++) {
										float xthru=(float)i/(float)(n.width-1);
										float ythru=(float)j/(float)(n.height-1);
										V3d world=A+xthru*(B-A)+ythru*(D-A);
										V2d hit=corner+diffscale*rectangleproject(world,eye);
										n.setpos(i,j,origimage.getinter(hit.x,hit.y));
									}
								n.writefile("recover.bmp");
							}

							printf("Overlaying summary on original image\n");

							/* origimage.greyify(); */

							// Fade original to white
							for (int i=0;i<origimage.width;i++)
								for (int j=0;j<origimage.height;j++) {
									myRGB oc=origimage.getpos(i,j);
									origimage.setpos(i,j,oc*(1.0-gamma)+myRGB::white*gamma);
								}


							myRGB overlaycol=( lightoverlay ? myRGB::red.pastel() : myRGB::blue );

							// Overlay info on original image
							if (overlayinfquadonly) {

								origimage.thicklineinf(tl,tr,overlaythickness,overlaycol);
								origimage.thicklineinf(br,tr,overlaythickness,overlaycol);
								origimage.thicklineinf(br,bl,overlaythickness,overlaycol);
								origimage.thicklineinf(tl,bl,overlaythickness,overlaycol);

							} else if (overlayquadonly) {

								origimage.thickline(tl,tr,overlaythickness,overlaycol);
								origimage.thickline(br,tr,overlaythickness,overlaycol);
								origimage.thickline(br,bl,overlaythickness,overlaycol);
								origimage.thickline(tl,bl,overlaythickness,overlaycol);

							} else {

								// Draw line through each gap
								for (int i=1;i<=pp.gaps.len;i++) {
									Line2d fromside=Line2d(hvp,hvp+V2d::angle(pp.gaps.num(i)));
									// From binimg space to origimage space
									fromside=fromside*diffscale+corner;
									V2d hitleft=Line2d(tl,bl).intersect(fromside);
									V2d hitright=Line2d(tr,br).intersect(fromside);
									origimage.thickline(hitleft,hitright,myRGB::white,1); // used to be yellow
								}
								// VVP lines
								float diagonal=sqrt(origimage.width*origimage.width+origimage.height*origimage.height);
								// Plot baseline!
								origimage.thickline(((diffscale*summarybaseline+corner)).expandedTo(2.0*diagonal),myRGB::yellow,3);
								if (usetwolines)
									origimage.thickline(((diffscale*summarysecondline+corner)).expandedTo(2.0*diagonal),myRGB::white,3);
								// correlation points
								for (int i=0;i<summaryleftused.len;i++) {
									V2d pnt=corner+diffscale*summaryleftused.get(i);
									origimage.filledcircle(pnt,7,myRGB::darkgreen);
								}
								for (int i=0;i<summaryleftunused.len;i++) {
									V2d pnt=corner+diffscale*summaryleftunused.get(i);
									origimage.thickcross(pnt,7,2,myRGB::red);
								}
								for (int i=0;i<summaryrightused.len;i++) {
									V2d pnt=corner+diffscale*summaryrightused.get(i);
									origimage.filledcircle(pnt,7,myRGB::darkgreen);
								}
								for (int i=0;i<summaryrightunused.len;i++) {
									V2d pnt=corner+diffscale*summaryrightunused.get(i);
									origimage.thickcross(pnt,7,2,myRGB::red);
								}
								for (int i=0;i<summarycenused.len;i++) {
									V2d pnt=corner+diffscale*summarycenused.get(i);
									origimage.filledcircle(pnt,7,myRGB::darkgreen);
								}
								for (int i=0;i<summarycenunused.len;i++) {
									V2d pnt=corner+diffscale*summarycenunused.get(i);
									origimage.thickcross(pnt,7,2,myRGB::red);
								}

								hvp=corner+diffscale*hvp;
								vvp=corner+diffscale*vvp;
								// origimage.thickline(tl,hvp,1,myRGB::white);
								// origimage.thickline(bl,hvp,1,myRGB::white);

								// Quadrilateral frame
								origimage.thickline(tl,tr,overlaythickness,overlaycol);
								origimage.thickline(br,tr,overlaythickness,overlaycol);
								origimage.thickline(br,bl,overlaythickness,overlaycol);
								origimage.thickline(tl,bl,overlaythickness,overlaycol);

								//origimage.line(corner+diffscale*top.a,corner+diffscale*top.b,myRGB::red);
								//origimage.line(corner+diffscale*bottom.a,corner+diffscale*bottom.b,myRGB::red);
								//origimage.line(corner+diffscale*left.a,corner+diffscale*left.b,myRGB::red);
								//origimage.line(corner+diffscale*right.a,corner+diffscale*right.b,myRGB::red);

								// origimage.line(tl,vvp,myRGB::green);
								// origimage.line(tr,vvp,myRGB::green);

							}

							origimage.writefile("origover.bmp");

						}

						if (doinfo)
							info.writefile("info.bmp");

						if (outcount>0)
							printf("%i pixels fell outside their window!\n");

}
