bool linespacings_do_ransac=true;

float testASubSet(List<V2d> ps);

bool equal(V2d u,V2d v) {
	return V2d::equal(u,v);
}


float guessU;

float lastV,lastW; // modified by testASubSet, read by doRansac and vvpFromPoints


float doRansac(List<V2d> ps) { // returns error

	float currentErr=testASubSet(ps);
	system("cp gplfit.ps gplfit1.ps");
	float bestV=lastV;
	float bestW=lastW;
	List<V2d> currentps;
	for (int i=0;i<ps.len;i++)
		currentps.add(ps.get(i));

	boolean loop=true;
	while (loop) {
		// See if we can improve currentErr by removing a single point
		float bestErr=currentErr;
		List<V2d> bestps;  bestps.add(currentps);
		if (linespacings_do_ransac) {
			// Try the current list with point number i removed, and keep the best one
			for (int i=0;i<currentps.len;i++) {
				// printf("Removing %i / %i\n",i,currentps.len);
				List<V2d> testps;
				testps.add(currentps);
				testps.removenum(i+1);
				// printf("Testing...\n");
				float newErr=testASubSet(testps);
				if (newErr<bestErr) {
					// printf("=)\n");
					bestErr=newErr;
					bestps=List<V2d>();
					bestps.add(testps);
					// printf("=)\n");
				}
			}
		}
		if (bestErr<currentErr*0.5) {
			// printf("Hello\n");
			printf("Improvement: %i %e\n",currentps.len-1,currentErr);
			currentErr=bestErr;
			currentps=bestps;
			// printf("Improvement: %f %s\n",currentErr,currentps.toString());
		} else {
			loop=false; // no improvements found, drop out of loop
		}
	}

	printf("RANSAC concluded with %i / %i\n",currentps.len,ps.len);
	// Redo current to get lastV and lastW correct
	return testASubSet(currentps);

}


float testASubSet(List<V2d> ps) { // returns error

	FILE *dataout=fopen("gpldata.txt","w");
	for (int i=0;i<ps.len;i++) {
		fprintf(dataout,"%f %f\n",ps.get(i).x,ps.get(i).y);
	}
	fclose(dataout);

	FILE *gplout=fopen("gpldo.txt","w");

	fprintf(gplout,"#!/usr/local/gnu/bin/gnuplot\n");
	fprintf(gplout,"u = %f\n",guessU);
	
	fflush(gplout);
	fclose(gplout);

	// system("cat gpldo.txt fitgnuplot2.txt > gplsolve.txt");
	// system("cat gplsolve.txt | gnuplot > gplans.txt 2>&1");
	// system("grep '^v' gplans.txt | grep '=' | tail -1 | after '= ' | before ' ' > v.txt");
	// system("grep '^w' gplans.txt | grep '=' | tail -1 | after '= ' | before ' ' > w.txt");
	// system("grep '^WSSR' gplans.txt | tail -1 | between ':' | before 'delta' | sed 's/ //g' > wssr.txt");
	system("./dogplfitting.sh");
	// system("cat wssr.txt");
	float V=readfloatfromfile("v.txt");
	float W=readfloatfromfile("w.txt");

	float Wssr;
	List<String> file=readlinesfromfile("wssr.txt");
	String s=file.get(0);
	if (Sinstr(s,"e")>0) {
		String pre=Sbefore(s,"e");
		String exp=Safter(s,"e");
		float fpre=Stofloat(pre);
		float fexp=Stofloat(exp);
		float num=fpre*pow(10.0,fexp);
		Wssr=num;
	} else {
		Wssr=Stofloat(s);
	}

	lastV=V;
	lastW=W;

	// printf("Done testing.\n");
 	
	return sqrt(Wssr)/(float)ps.len/(float)ps.len/(float)ps.len/(float)ps.len/(float)ps.len/(float)ps.len/(float)ps.len;
	
}



V2d vvpFromPoints(Line2d bl,List<V2d> eps,int imgwidth,int imgheight,bool usingspacings) {

	FILE *log=fopen("vvpFromPoints.txt","w");

	Line2d baseline=Line2d(bl.a,bl.b);
	// if (baseline.a.y<baseline.b.y)
		// baseline=Line2d(bl.b,bl.a);
	fprintf(log,"baseline = %s\n",baseline.toString());
	List<V2d> endpoints;
	endpoints.add(eps);
	fprintf(log,"first %s\nlast  %s\n",endpoints.num(1).toString(),endpoints.num(endpoints.len).toString());

	// Project the points down onto the line
	for (int i=0;i<endpoints.len;i++) {
		V2d v=endpoints.get(i);
		endpoints.put(i,baseline.perpproject(v));
	}
				
	float angle=(baseline.b-baseline.a).angle();
	printf(">> rotang = %f\n",angle*180.0/pi);
	// Rotate baseline and endpoints
	for (int i=0;i<endpoints.len;i++) {
		V2d v=endpoints.get(i);
		v=v.rotateabout(-angle,V2d((float)imgwidth/2.0,(float)imgheight/2.0));
		endpoints.put(i,v);
	}
	baseline.a=baseline.a.rotateabout(-angle,V2d((float)imgwidth/2.0,(float)imgheight/2.0));
	baseline.b=baseline.b.rotateabout(-angle,V2d((float)imgwidth/2.0,(float)imgheight/2.0));

	List<V2d> ps;
				
	if (usingspacings) {
					
	  for (int i=0;i<endpoints.len-1;i++) {
			V2d u=endpoints.get(i);
			V2d v=endpoints.get(i+1);
			ps.add(V2d((float)imgheight/2.0-(u.y+v.y)/2.0,u.y-v.y));
			// fprintf(dataout,"%f %f\n",(float)imgheight/2.0-(u.y+v.y)/2.0,u.y-v.y);
		}

	} else {

		for (int i=0;i<endpoints.len;i++) {
			ps.add(V2d(i,(float)imgheight/2.0-endpoints.get(i).y));
			// fprintf(dataout,"%i %f\n",i,(float)imgheight/2.0-endpoints.get(i).y);
		}

	}

	guessU=(float)imgheight/2.0-endpoints.get(0).y;
	printf("Using guessU = %f\n",guessU);
	printf("  from %s\n",endpoints.get(0).toString());

	float finalErr=doRansac(ps);
	printf("final error = %f\n",finalErr);

	printf("guessU = %f\n",guessU);
	printf("gotV = %f\n",lastV);
	printf("gotW = %f\n",lastW);

	float vvpdist=guessU*lastV/lastW;
	V2d vvp=baseline.a-(baseline.b-baseline.a).norm()*vvpdist;
	vvp=vvp.rotateabout(angle,V2d((float)imgwidth/2.0,(float)imgheight/2.0));

	fclose(log);
	
	return vvp;

}
