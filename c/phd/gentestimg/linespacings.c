V2d vvpFromPoints(Line2d baseline,List<V2d> endpoints,int imgwidth,int imgheight,bool usingspacings) {

	// Project the points down onto the line
	for (int i=0;i<endpoints.len;i++) {
		V2d v=endpoints.get(i);
		endpoints.put(i,baseline.perpproject(v));
	}
				
	float angle=(baseline.b-baseline.a).angle();
	// Rotate baseline and endpoints
	for (int i=0;i<endpoints.len;i++) {
		V2d v=endpoints.get(i);
		v=v.rotateabout(-angle,V2d((float)imgwidth/2.0,(float)imgheight/2.0));
		endpoints.put(i,v);
	}
	baseline.a=baseline.a.rotateabout(-angle,V2d((float)imgwidth/2.0,(float)imgheight/2.0));
	baseline.b=baseline.b.rotateabout(-angle,V2d((float)imgwidth/2.0,(float)imgheight/2.0));
				
	FILE *dataout=fopen("gpldata.txt","w");
	
	if (usingspacings) {
					
	  for (int i=0;i<endpoints.len-1;i++) {
			V2d u=endpoints.get(i);
			V2d v=endpoints.get(i+1);
			fprintf(dataout,"%f %f\n",(float)imgheight/2.0-(u.y+v.y)/2.0,u.y-v.y);
		}

	} else {

		for (int i=0;i<endpoints.len;i++) {
			fprintf(dataout,"%i %f\n",i,(float)imgheight/2.0-endpoints.get(i).y);
		}

	}

	fclose(dataout);

	FILE *gplout=fopen("gpldo.txt","w");

	float guessU=(float)imgheight/2.0-endpoints.get(0).y;

	fprintf(gplout,"#!/usr/local/gnu/bin/gnuplot\n");
	fprintf(gplout,"u = %f\n",guessU);
	
	// fflush(gplout);
	fclose(gplout);

	system("cat gpldo.txt fitgnuplot2.txt > gplsolve.txt");
	system("cat gplsolve.txt | gnuplot > gplans.txt 2>&1");
	system("grep '^v' gplans.txt | grep '=' | tail -1 | after '= ' | before ' ' > v.txt");
	system("grep '^w' gplans.txt | grep '=' | tail -1 | after '= ' | before ' ' > w.txt");
	float V=readfloatfromfile("v.txt");
	float W=readfloatfromfile("w.txt");

	printf("got V = %f\n",V);
	printf("got W = %f\n",W);

	float vvpdist=guessU*V/W;
	V2d vvp=baseline.a-(baseline.b-baseline.a).norm()*vvpdist;
	vvp=vvp.rotateabout(angle,V2d((float)imgwidth/2.0,(float)imgheight/2.0));
	
	return vvp;

}
