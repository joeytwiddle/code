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
	int lastpitch=-1;
	int i=0;
	String l=lines.get(i);
	if (!Seq(l,"START"))
		fprintf(stderr,"Error 0 line %i\n",i);
	else
		i++;
	while (i<lines.len) {
		if (i<lines.len) l=lines.get(i);
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
			float gtU,gtV,gtW;
			float ppU,ppV,ppW;
		  i++; if (i<lines.len) l=lines.get(i);
		  sscanf(l,"roll = %f",&roll);
		  // printf("Got %f for roll\n",roll);
		  i++; if (i<lines.len) l=lines.get(i);
		  sscanf(l,"yaw = %f",&yaw);
		  // printf("Got %f for yaw\n",yaw);
		  i++; if (i<lines.len) l=lines.get(i);
		  sscanf(l,"pitch = %f",&pitch);
		  // printf("Got %f for pitch\n",pitch);
		  i++; if (i<lines.len) l=lines.get(i);
			if (Sstarts(l,"right = ")) {
		  	sscanf(l,"right = (%f,%f,%f)",&gtright.x,&gtright.y,&gtright.z);
		    // printf("Got %s for GTr\n",gtright.toString());
		    i++; if (i<lines.len) l=lines.get(i);
			} else {
			  fprintf(stderr,"Bad gen right %i\n",i);
			}
			if (Sstarts(l,"down = ")) {
		  	sscanf(l,"down = (%f,%f,%f)",&gtdown.x,&gtdown.y,&gtdown.z);
		    // printf("Got %s for GTd\n",gtdown.toString());
		    i++; if (i<lines.len) l=lines.get(i);
			} else {
			  fprintf(stderr,"Bad gen down %i\n",i);
			}
			if (Sstarts(l,"HVP = ")) {
		    sscanf(l,"HVP = (%f,%f)",&gthvp.x,&gthvp.y);
		    i++; if (i<lines.len) l=lines.get(i);
			} else {
			  fprintf(stderr,"Bad gen HVP %i\n",i);
			}
			if (Sstarts(l,"VVP = ")) {
		    sscanf(l,"VVP = (%f,%f)",&gtvvp.x,&gtvvp.y);
		    i++; if (i<lines.len) l=lines.get(i);
			} else {
			  fprintf(stderr,"Bad gen VVP %i\n",i);
			}
				if (Sstarts(l,"U = ")) {
		      sscanf(l,"U = %f",&gtU);
		      i++; if (i<lines.len) l=lines.get(i);
				} else {
					// fprintf(stderr,"no gt U line %i r%f y%f p%f\n",i,roll,yaw,pitch);
				}
				if (Sstarts(l,"V = ")) {
		      sscanf(l,"V = %f",&gtV);
		      i++; if (i<lines.len) l=lines.get(i);
				} else {
					// fprintf(stderr,"no gt V line %i r%f y%f p%f\n",i,roll,yaw,pitch);
				}
				if (Sstarts(l,"W = ")) {
		      sscanf(l,"W = %f",&gtW);
		      i++; if (i<lines.len) l=lines.get(i);
				} else {
					// fprintf(stderr,"no gt W line %i r%f y%f p%f\n",i,roll,yaw,pitch);
				}
		  if (!Seq(l,"PP:"))
			  fprintf(stderr,"Error B line %i\n",i);
		  i++; if (i<lines.len) l=lines.get(i);
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
			    i++; if (i<lines.len) l=lines.get(i);
				} else {
					badgen=true;
				}
			  if (sscanf(l,"down = (%f,%f,%f)",&ppdown.x,&ppdown.y,&ppdown.z)) {
			    // printf("Got %s for GTr\n",ppdown.toString());
		      i++; if (i<lines.len) l=lines.get(i);
				} else {
					badgen=true;
				}
		    if (sscanf(l,"HVP = (%f,%f)",&pphvp.x,&pphvp.y)) {
		    	i++; if (i<lines.len) l=lines.get(i);
				} else {
					fprintf(stderr,"no hvp line %i\n",i);
					badhvp=true;
				}
				if (Sstarts(l,"VVP = ")) {
		      sscanf(l,"VVP = (%f,%f)",&ppvvp.x,&ppvvp.y);
		      i++; if (i<lines.len) l=lines.get(i);
				} else {
					fprintf(stderr,"no vvp line %i r%f y%f p%f\n",i,roll,yaw,pitch);
					badvvp=true;
				}
				if (Sstarts(l,"U = ")) {
		      sscanf(l,"U = %f",&ppU);
		      i++; if (i<lines.len) l=lines.get(i);
				} else {
					// fprintf(stderr,"no pp U line %i r%f y%f p%f\n",i,roll,yaw,pitch);
				}
				if (Sstarts(l,"V = ")) {
		      sscanf(l,"V = %f",&ppV);
		      i++; if (i<lines.len) l=lines.get(i);
				} else {
					// fprintf(stderr,"no pp V line %i r%f y%f p%f\n",i,roll,yaw,pitch);
				}
				if (Sstarts(l,"W = ")) {
		      sscanf(l,"W = %f",&ppW);
		      i++; if (i<lines.len) l=lines.get(i);
				} else {
					// fprintf(stderr,"no pp W line %i r%f y%f p%f\n",i,roll,yaw,pitch);
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
				  fprintf(stderr,"  single -ve r%i-y%i-p%i\n",(int)roll,(int)yaw,(int)pitch);
				  altacc=-3;
				}
			}
		  
			// printf("Correlation: %f %f\n",corra,corrb);
		  // printf("%f %f %f\n",yaw,pitch,myabs(corra*corrb));
		  // printf("%f %f %f\n",yaw,pitch,corra*corrb);
		  // printf("%f %f %f %f\n",yaw,pitch,corra,corrb);
		  // printf("%f %f %f %f %f\n",yaw,pitch,(myabs(corra*corrb)+1.0)/2.0,corra,corrb);
			
			if (yaw<lastyaw || pitch<lastpitch)
				printf("\n");
			lastyaw=yaw;
			lastpitch=pitch;
			
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
			
		  if (yaw>10 && yaw<80 && pitch>10 && pitch<80) {	
			  if (myabs(realrightangerr)<123456789.0) {
			    realrightangerrsum+=myabs(realrightangerr);
				  var769++;
			  }
			  if (myabs(realdownangerr)<123456789.0) {
			    realdownangerrsum+=myabs(realdownangerr);
				  var770++;
			  }
			  float maxreldisterr=1.0;
			  if (hvpreldist<maxreldisterr) {
				  hvpreldistcnt++;
				  hvpreldistsum+=hvpreldist;
			  } // else
					// hvpreldist=0;
			  if (vvpreldist<maxreldisterr) {
				  vvpreldistcnt++;
				  vvpreldistsum+=vvpreldist;
			  } // else
					// vvpreldist=0;
		  }

			if (realrightangerr>45)
							realrightangerr=45;
			if (realdownangerr>45)
							realdownangerr=45;

			gtU=myabs(gtU);
			gtV=myabs(gtV);
			gtW=myabs(gtW);
			ppU=myabs(ppU);
			ppV=myabs(ppV);
			ppW=myabs(ppW);

			float Udiff=ppU-gtU;
			float Vdiff=ppV-gtV;
			float Wdiff=ppW-gtW;
			
		  printf("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
				yaw,pitch,acc,corra,corrb,
				gtright.x,gtright.y,gtright.z,ppright.x,ppright.y,ppright.z,
				gtdown.x,gtdown.y,gtdown.z,ppdown.x,ppdown.y,ppdown.z,
				altacc,faceonness,maxangle,rightdiff,rightang,
				hvpcorr,vvpcorr, // 23,24
				hvpdist,vvpdist, // 25,26
				-hvpreldist,-vvpreldist, // 27,28
				-realrightangerr,-realdownangerr, // 29,30
				realrightangerrzerone,realdownangerrzerone, // 31,32
				gtU,gtV,gtW, // 33,34,35
				ppU,ppV,ppW, // 36,37,38
				Udiff,Vdiff,Wdiff // 39,40,41
			);
		  
		  // if (!Seq(l,""))
			  // fprintf(stderr,"Error C line %i\n",i);
		  // i++;
		}
	}
	printf("\n# 1 yaw\n# 2 pitch\n# 3 acc\n# 4 racc\n# 5 dacc\n# 6 gtrx\n# 7 gtry\n# 8 gtrz\n# 9 pprx\n# 10 ppry\n# 11 pprz\n# 12 gtdx\n# 13 gtdy\n# 14 gtdz\n# 15 ppdx\n# 16 ppdy\n# 17 ppdz\n# 18 altacc\n# 19 faceonness 20 maxang\n# 21 rightdiff\n# 22 rightang\n# 23 hvpcorr\n# 24 vvpcorr\n# 25 hvpdist\n# 26 vvpdist\n# 27 relhvpdist\n# 28 relvvpdist\n");
	printf("# 29 realrightangerr\n");
	printf("# 30 realdownangerr\n");
	printf("# 31 realrightangacc0-1\n");
	printf("# 32 realdownangacc0-1\n");
	printf("# 33,34,35 gtU gtV gtW\n");
	printf("# 36,37,38 ppU ppV ppW\n");
	printf("# 39,40,40 diffU diffV diffW\n");

	fprintf(stderr,"realrightangerrave = %f over %i\n",realrightangerrsum/(float)var769,var769);
	fprintf(stderr,"realdownangerrave = %f over %i\n",realdownangerrsum/(float)var770,var770);
	fprintf(stderr,"hvpreldistave = %f over %i\n",hvpreldistsum/(float)hvpreldistcnt,hvpreldistcnt);
	fprintf(stderr,"vvpreldistave = %f over %i\n",vvpreldistsum/(float)vvpreldistcnt,vvpreldistcnt);
	
}
