#define QUIET

#include <joeylib.h>

main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  String filename=a.argor("results file","results.txt");
  a.done();

	float realrightangerrsum=0;
	float realdownangerrsum=0;
	int var769=0;
	int var770=0;
	float hvpreldistsum=0;
	float vvpreldistsum=0;
	int hvpreldistcnt=0;
	int vvpreldistcnt=0;

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

			float realrightangerr=180.0-180.0*V3d::angBetween(gtright,ppright)/pi;
			float realdownangerr=180.0-180.0*V3d::angBetween(gtdown,ppdown)/pi;
			float realrightangerrzerone=1.0-realrightangerr/180.0;
			float realdownangerrzerone=1.0-realdownangerr/180.0;

			// Bad:
			float focal=2560; // Just seems!
			// float focal=0; // Just seems!
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

			float hvpdist=gthvp.dist(pphvp);
			float vvpdist=gtvvp.dist(ppvvp);
			float hvpreldist=hvpdist/gthvp.mag();
			float vvpreldist=vvpdist/gtvvp.mag();
			
		  if (yaw>5 && yaw<85 && pitch>5 && pitch<85) {	
			  if (myabs(realrightangerr)<123456789.0) {
			    realrightangerrsum+=myabs(realrightangerr);
				  var769++;
			  }
			  if (myabs(realdownangerr)<123456789.0) {
			    realdownangerrsum+=myabs(realdownangerr);
				  var770++;
			  }
			  if (hvpreldist<3.0) {
				  hvpreldistcnt++;
				  hvpreldistsum+=hvpreldist;
			  } else
					hvpreldist=0;
			  if (vvpreldist<3.0) {
				  vvpreldistcnt++;
				  vvpreldistsum+=vvpreldist;
			  } else
					vvpreldist=0;
		  }
			
		  printf("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
				yaw,pitch,acc,corra,corrb,
				gtright.x,gtright.y,gtright.z,ppright.x,ppright.y,ppright.z,
				gtdown.x,gtdown.y,gtdown.z,ppdown.x,ppdown.y,ppdown.z,
				altacc,faceonness,maxangle,rightdiff,rightang,
				hvpcorr,vvpcorr, // 23,24
				hvpdist,vvpdist, // 25,26
				hvpreldist,vvpreldist, // 27,28
				realrightangerr,realdownangerr, // 29,30
				realrightangerrzerone,realdownangerrzerone // 31,32
			);
		  
		  // if (!Seq(l,""))
			  // fprintf(stderr,"Error C line %i\n",i);
		  // i++;
		}
	}
	printf("\n# 1 yaw\n# 2 pitch\n# 3 acc\n# 4 racc\n# 5 dacc\n# 6 gtrx\n# 7 gtry\n# 8 gtrz\n# 9 pprx\n# 10 ppry\n# 11 pprz\n# 12 gtdx\n# 13 gtdy\n# 14 gtdz\n# 15 ppdx\n# 16 ppdy\n# 17 ppdz\n# 18 altacc\n# 19 faceonness 20 maxang\n# 21 rightdiff\n# 22 rightang\n# 23 hvpcorr\n# 24 vvpcorr\n# 25 hvpdist\n# 26 vvpdist\n# 27 relhvpdist\n# 28 relvvpdist\n");
	printf("# 29 realrightangerr\n");
	printf("# 30 realdownangerr\n");
	printf("# 31 realdownangacc0-1\n");
	printf("# 32 realdownangacc0-1\n");

	fprintf(stderr,"realrightangerrave = %f over %i\n",realrightangerrsum/(float)var769,var769);
	fprintf(stderr,"realdownangerrave = %f over %i\n",realdownangerrsum/(float)var770,var770);
	fprintf(stderr,"hvpreldistave = %f over %i\n",hvpreldistsum/(float)hvpreldistcnt,hvpreldistcnt);
	fprintf(stderr,"vvpreldistave = %f over %i\n",vvpreldistsum/(float)vvpreldistcnt,vvpreldistcnt);
	
}
