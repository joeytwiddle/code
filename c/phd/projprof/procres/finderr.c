#define QUIET

#include <joeylib.h>

main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  // bool recoverquad=true;
  // int cirres=a.intafter("-res","resolution of final circle",120);
  // int lowcirres=a.intafter("-lowres","resolution of initial circle",60);
  // maxpixelsused=a.intafter("-maxpixels","max # pixels to collect",5000);
  // recwid=a.intafter("-recwid","Width of recovered image",500);
  // float adapthresh=a.floatafter("-adapt","adaptive threshold for clipping",0.1);
  // int ppres=-1;
  // #ifndef FIXPPRES
    // ppres=a.intafter("-ppres","resolution of each projection profile",200);
  // #endif
  // bool writepps=a.argexists("-writepps","write the potential PPs");
  // bool writebestpp=!a.argexists("-dontwritebestpp","don't write the best PP");
  // bool doinfo=true; // Needed for summaries!  a.argexists("-doinfo","draw info image");
  // bool usebaselineonly=!a.argexists("-usetwolines","use two best correlated lines instead of just baseline");
  // bool usetwolines=!usebaselineonly;
  // float gamma=a.floatafter("-gamma","take original image this far to white before summary",0.4);
  // correlator=( a.argexists("-spacings","use line spacing not width")
    // ? LineSpacing
    // : LineWidth );
  // if (a.argexists("-corinc","use over-inclusive RANSAC"))
    // Correlator2dMethod=2;
  // bool overlayquadonly=a.argexists("-quadonly","overlay quadrilateral only on summary image");
  // int overlaythickness=a.intafter("-oth","overlay thickness",7);
  // bool overlayinfquadonly=a.argexists("-infquadonly","overlay extended quadrilateral only on summary image");
  // bool lightoverlay=a.argexists("-light","light overlay for dark image");
// //  a.comment("Info image options:");
  // a.comment("PP measure of correctness: (default = derivative)");
  // measure=&measurederiv;
  // if (a.argexists("-deriv","use derivative measure"))
    // measure=&measurederiv;
  // if (a.argexists("-square","use square measure"))
    // measure=&measuresquare;
  // if (a.argexists("-entropy","use entropy measure"))
    // measure=&measureentropy;
  // if (a.argexists("-variance","use variance measure"))
    // measure=&measurevariance;
// //  bool invert=a.argexists("-inv","invert input image to look for line spaces");
  // showbadgenx=a.intafter("-badgenx","x pos of badgen PP to show",-1);
  // showbadgeny=a.intafter("-badgeny","y pos of badgen PP to show",-1);
  // String bname=a.getarg("binary image");
  // String oname=a.argor("original image","*none*");
  // String datafile=a.argor("crop data file","*none*");
  String filename=a.argor("results file","results.txt");
  a.done();

	List<String> lines=readlinesfromfile(filename);
	int lastyaw=-1;
	int i=0;
	String l=lines.get(i);
	if (!Seq(l,"START"))
		fprintf(stderr,"Error 0 line %i\n",i);
	else
		i++;
	while (i<lines.len) {
		l=lines.get(i);
		if (!Seq(l,"SIM:")) {
			fprintf(stderr,"Error A line %i - seeking\n",i);
			i++;
			// break;
		}	else {
		  float roll,yaw,pitch;
		  V3d gtright,gtdown;
		  V3d ppright,ppdown;
			V2d gthvp,gtvvp;
			V2d pphvp,ppvvp;
		  i++; l=lines.get(i);
		  sscanf(l,"roll = %f",&roll);
		  // printf("Got %f for roll\n",roll);
		  i++; l=lines.get(i);
		  sscanf(l,"yaw = %f",&yaw);
		  // printf("Got %f for yaw\n",yaw);
		  i++; l=lines.get(i);
		  sscanf(l,"pitch = %f",&pitch);
		  // printf("Got %f for pitch\n",pitch);
		  i++; l=lines.get(i);
		  sscanf(l,"right = (%f,%f,%f)",&gtright.x,&gtright.y,&gtright.z);
		  // printf("Got %s for GTr\n",gtright.toString());
		  i++; l=lines.get(i);
		  sscanf(l,"down = (%f,%f,%f)",&gtdown.x,&gtdown.y,&gtdown.z);
		  // printf("Got %s for GTd\n",gtdown.toString());
		  i++; l=lines.get(i);
		  sscanf(l,"HVP = (%f,%f)",&gthvp.x,&gthvp.y);
		  i++; l=lines.get(i);
		  sscanf(l,"VVP = (%f,%f)",&gtvvp.x,&gtvvp.y);
		  i++; l=lines.get(i);
		  if (!Seq(l,"PP:"))
			  fprintf(stderr,"Error B line %i\n",i);
		  i++; l=lines.get(i);
			bool badgen=false;
			bool badhvp=false;
			bool badvvp=false;
		  if (Seq(l,"")) {
				fprintf(stderr,"no pp res r%i-y%i-p%i\n",(int)roll,(int)yaw,(int)pitch);
			  fprintf(stderr,"  Error D line %i\n",i);
				ppright=gtright;
				ppdown=gtdown;
				badgen=true;
		  } else {
			  if (sscanf(l,"right = (%f,%f,%f)",&ppright.x,&ppright.y,&ppright.z)) {
			    // printf("Got %s for GTr\n",ppright.toString());
			    i++; l=lines.get(i);
				} else {
					badgen=true;
				}
			  if (sscanf(l,"down = (%f,%f,%f)",&ppdown.x,&ppdown.y,&ppdown.z)) {
			    // printf("Got %s for GTr\n",ppdown.toString());
		      i++; l=lines.get(i);
				} else {
					badgen=true;
				}
		    if (sscanf(l,"HVP = (%f,%f)",&pphvp.x,&pphvp.y)) {
		    	i++; l=lines.get(i);
				} else {
					fprintf(stderr,"no hvp line %i\n",i);
					badhvp=true;
				}
				if (Sstarts(l,"VVP = ")) {
		      sscanf(l,"VVP = (%f,%f)",&ppvvp.x,&ppvvp.y);
		      i++; l=lines.get(i);
				} else {
					fprintf(stderr,"no vvp line %i r%f y%f p%f\n",i,roll,yaw,pitch);
					badvvp=true;
				}
			}
			
			// float corra=myabs(-V3d::normdot(gtright,ppright));
			// float acc=myabs(corra*corrb);
			float corra=-V3d::normdot(gtright,ppright);
		  float corrb=-V3d::normdot(gtdown,ppdown);
			float acc=corra*corrb;
			V3d gtnor=V3d::normcross(gtright,gtdown);
			V3d ppnor=V3d::normcross(ppright,ppdown);
			float altacc=V3d::normdot(gtnor,ppnor);
			float faceonness=V3d::normdot(gtnor,V3d::k);
			float maxangle=180*acos(faceonness)/pi;
			float rightdiff=-V3d::normdot(gtright,V3d::k);
			float rightang=180*asin(rightdiff)/pi;

			// Bad:
			float focal=500; // Just seems!
			V3d eye=V3d(320,240,-focal);
			V3d gtrightfromhvp=V3d(gthvp.x,gthvp.y,0)-eye;
			V3d pprightfromhvp=V3d(pphvp.x,pphvp.y,0)-eye;
			V3d gtdownfromvvp=V3d(gtvvp.x,gtvvp.y,0)-eye;
			V3d ppdownfromvvp=V3d(ppvvp.x,ppvvp.y,0)-eye;
			float hvpcorr=V3d::normdot(gtrightfromhvp,pprightfromhvp);
			if (badhvp)
				hvpcorr=-1;
			float vvpcorr=V3d::normdot(gtdownfromvvp,ppdownfromvvp);
			if (badvvp)
				vvpcorr=-1;

			if (!badgen) {
				if (corra<0 && corrb<0) {
					fprintf(stderr,"  double negative correl r%i-y%i-p%i\n",(int)roll,(int)yaw,(int)pitch);
			  } else if (corra<0 || corrb<0) {
				  fprintf(stderr,"  single negative correl r%i-y%i-p%i\n",(int)roll,(int)yaw,(int)pitch);
				  altacc=-3;
				}
			}
		  
			// printf("Correlation: %f %f\n",corra,corrb);
		  // printf("%f %f %f\n",yaw,pitch,myabs(corra*corrb));
		  // printf("%f %f %f\n",yaw,pitch,corra*corrb);
		  // printf("%f %f %f %f\n",yaw,pitch,corra,corrb);
		  // printf("%f %f %f %f %f\n",yaw,pitch,(myabs(corra*corrb)+1.0)/2.0,corra,corrb);
			
			if (yaw<lastyaw)
				printf("\n"); // Seperate blocks for grid plot
			lastyaw=yaw;
			
			// This is no longer cheating but it lets us see real errors
			// if (acc<0)
				// acc=1;
			// if (altacc<0)
				// altacc=1;

			if (badgen) {
				acc=-2; altacc=-2;
			}
			
		  printf("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
				yaw,pitch,acc,corra,corrb,
				gtright.x,gtright.y,gtright.z,ppright.x,ppright.y,ppright.z,
				gtdown.x,gtdown.y,gtdown.z,ppdown.x,ppdown.y,ppdown.z,
				altacc,faceonness,maxangle,rightdiff,rightang,
				hvpcorr,vvpcorr
			);
		  
		  // if (!Seq(l,""))
			  // fprintf(stderr,"Error C line %i\n",i);
		  // i++;
		}
	}
	printf("\n# 1 yaw     2 pitch   3 acc     4 racc   5 dacc     6 gtrx    7 gtry    8 gtrz    9 pprx    10 ppry    11 pprz    12 gtdx    13 gtdy   14 gtdz   15 ppdx   16 ppdy   17 ppdz   18 altacc  19 faceonness 20 maxang   21 rightdiff  22 rightang  23 hvpcorr  24 vvpcorr\n");
	
}
