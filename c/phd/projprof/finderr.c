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
  // showbadx=a.intafter("-badx","x pos of bad PP to show",-1);
  // showbady=a.intafter("-bady","y pos of bad PP to show",-1);
  // String bname=a.getarg("binary image");
  // String oname=a.argor("original image","*none*");
  // String datafile=a.argor("crop data file","*none*");
  String filename=a.argor("results file","results.txt");
  a.done();

	List<String> lines=readlinesfromfile(filename);
	int i=0;
	String l=lines.get(i);
	if (!Seq(l,"START"))
		printf("Error 0 line %i\n",i);
	else
		i++;
	while (i<lines.len) {
		l=lines.get(i);
		if (!Seq(l,"SIM:")) {
			// printf("Error A line %i\n",i);
			break;
		}	else {
		  float roll,yaw,pitch;
		  V3d gtright,gtdown;
		  V3d ppright,ppdown;
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
		  if (!Seq(l,"PP:"))
			  printf("Error B line %i\n",i);
		  i++; l=lines.get(i);
		  if (Seq(l,"")) {
			  // printf("Error D line %i\n",i);
		  } else {
			  sscanf(l,"right = (%f,%f,%f)",&ppright.x,&ppright.y,&ppright.z);
			  // printf("Got %s for GTr\n",ppright.toString());
			  i++; l=lines.get(i);
			  sscanf(l,"down = (%f,%f,%f)",&ppdown.x,&ppdown.y,&ppdown.z);
			  // printf("Got %s for GTr\n",ppdown.toString());
			  float corra=V3d::normdot(gtright,ppright);
			  float corrb=V3d::normdot(gtdown,ppdown);
			  // printf("Correlation: %f %f\n",corra,corrb);
			  // printf("%f %f %f\n",yaw,pitch,myabs(corra*corrb));
			  // printf("%f %f %f\n",yaw,pitch,corra*corrb);
			  // printf("%f %f %f %f\n",yaw,pitch,corra,corrb);
			  // printf("%f %f %f %f %f\n",yaw,pitch,(myabs(corra*corrb)+1.0)/2.0,corra,corrb);
			  printf("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
					yaw,pitch,corra*corrb,corra,corrb,
					gtright.x,gtright.y,gtright.z,ppright.x,ppright.y,ppright.z,
					gtdown.x,gtdown.y,gtdown.z,ppdown.x,ppdown.y,ppdown.z
				);
			  i++; l=lines.get(i);
		  }
		  if (!Seq(l,""))
			  printf("Error C line %i\n",i);
		  i++;
		}
	}
	printf("1 yaw     2 roll    3 acc     4 racc   5 dacc     6 gtrx    7 gtry    8 gtrz    9 pprx    10 ppry    11 pprz    12 gtdx    13 gtdy   14 gtdz   15 ppdx   16 ppdy   17 ppdz\n");
	
}
