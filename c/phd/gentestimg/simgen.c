// simgen is a combination of the other two (gentestimg and simulatecorrel)

#include <joeylib.h>

#include "linespacings.c"

int imgheight,imgwidth;
float focallength,scale;
Plane imageplane;
V3d eye=V3d(0,0,0);

float rotang;

V2d unrotabout(V2d v) {
	return v.rotateabout(rotang,V2d((float)imgwidth/2.0,(float)imgheight/2.0));
}

V2d unrot(V2d v) {
	// return v.rotateabout(-rotang,V2d((float)imgheight/2.0,(float)imgwidth/2.0));
	return v.rotated(rotang);
}

V3d imgplaneFromPixel(V2d p) {
	return V3d(((float)p.x-(float)imgwidth/2.0),-((float)p.y-(float)imgheight/2.0),focallength);
}

// Pixel pixelFromImagePlane(V3d v) {
	// return Pixel(v.x/scale+(float)imgwidth/2.0,imgheight-(v.y/scale+(float)imgheight/2.0));
// }

// Pixel proj(V3d v) {
	// return pixelFromImagePlane(imageplane.intersect(Line3d(eye,v)));
// }

V2d proj(V3d v) {
	return V2d(focallength*v.x/v.z+(float)imgwidth/2.0,imgheight/2.0-focallength*v.y/v.z); // +(float)imgheight/2.0);
}

int main(int argc,String *argv) {

	boolean dofitting=false;
	String fittingcommand="echo ERROR NO FITTING COMMAND PROVIDED";
	bool domatlab=false;
	bool dooctave=false;
	bool dognuplot=false;

	ArgParser a=ArgParser(argc,argv);
	if (domatlab=a.argexists("-matlab","vvp estimation with Matlab")) {
		dofitting=true;
		fittingcommand="matlab";
	}
	if (dooctave=a.argexists("-octave","vvp estimation with octave (not)")) {
		dofitting=true;
		fittingcommand="octave";
	}
	if (dognuplot=a.argexists("-gnuplot","vvp estimation with gnuplot")) {
		dofitting=true;
		fittingcommand="gnuplot";
		fittingcommand="less";
	}
	String whichmatlabfile=a.argafter("-mlfile","which matlab solution file?","sol1u.txt");
	bool showgraph=a.argexists("-graph","show graph in Matlab");
	bool centralise=a.argexists("-centralise","centralise the document in the image");
	bool usespacings=a.argexists("-spacings","use spacings (rather than pos)");
	a.comment("Orientation of plane in degrees:");
	float roll=deg2rad(a.floatafter("-roll","roll",0.0));
	float yaw=deg2rad(a.floatafter("-yaw","yaw",0.0));
	float pitch=deg2rad(a.floatafter("-pitch","pitch",-30.0));
	float propfocallength=a.floatafter("-focal","focal length (prop to img width)",4.0);
	float propdepth=a.floatafter("-depth","depth (prop to img width)",0);
	float propsize=a.floatafter("-size","size of page (prop to img width)",0.1);
	imgwidth=a.intafter("-width","output image width",640);
	imgheight=imgwidth*3/4;
	float xoff=a.floatafter("-xoff","x offset",0.0);
	float yoff=a.floatafter("-yoff","y offset",0.0);
	int numlines=a.intafter("-lines","number of lines in paragraph",10);
	float noise=a.floatafter("-noise","noise",0.0*640/imgwidth);
	bool genimage=a.argexists("-image","generate simulated image");
	bool overlay=a.argexists("-overlay","overlay info on simulated image");
	String overlayname="overlay.bmp"; // a.argafter("-overlayimage","name of overlay output file","overlay.bmp");
	String inname=a.argor("document image file","in.bmp");
	String outname=a.argor("output filename","out.bmp");
	a.done();
	focallength=propfocallength*imgwidth;
	float size=propsize*(float)imgwidth;
	float depth=propdepth*imgwidth;
	printf("focal length = %f\n",focallength);
	printf("size = %f\n",size);
	printf("depth = %f\n",depth);
	float width=size;
	float height;

	RGBmp *inputimg;
	RGBmp outputimg;
	if (genimage) {
		inputimg=RGBmp::readfile(inname);
		height=(float)inputimg->height*width/(float)inputimg->width;
	} else {
		height=width*10.0/7.0;
	}

	// Set up page
	
	V3d right=V3d(width,0,0);
	V3d down=V3d(0,height,0); // Note not -height as in Maple
	// Maple:
	// right=right.rotated(V3d::k,roll);
	// right=right.rotated(V3d::i,pitch);
	// right=right.rotated(V3d::j,-yaw);
	// down=down.rotated(V3d::k,roll);
	// down=down.rotated(V3d::i,pitch);
	// down=down.rotated(V3d::j,-yaw);
	right=right.rotated(V3d::j,-yaw);
	right=right.rotated(V3d::i,pitch);
	right=right.rotated(V3d::k,roll);
	down=down.rotated(V3d::j,-yaw);
	down=down.rotated(V3d::i,pitch);
	down=down.rotated(V3d::k,roll);
	down.y=-down.y; // Hack to sync with Maple
	// V3d worldA=V3d(imgwidth/2,imgheight/2,depth);
	V3d worldA=V3d(xoff,yoff,focallength+depth);
	if (centralise)
		worldA=worldA-down/2.0-right/2.0;
	V3d worldB=worldA+down;
	V3d worldC=worldA+right;
	V3d worldD=worldA+right+down;
	V3d cen=(worldA+worldD)/2.0;
	printf("%s %s\n",right.toString(),down.toString());

	printf("correlright = %s\n",right.toString());
	printf("correldown = %s\n",down.toString());
	// printf("right = %s\n",right.toString());
	// printf("down = %s\n",down.toString());
	printf("A = %s\n",worldA.toString());
	printf("B = %s\n",worldB.toString());
	printf("C = %s\n",worldC.toString());
	printf("D = %s\n",worldD.toString());

	imageplane=Plane(V3d(0,0,focallength),V3d(0,0,1));

	if (genimage) {
	
		outputimg=RGBmp(imgwidth,imgheight,myRGB::white);
	
		// Generate image
		
		ProgMon progmon;

		for (int i=0;i<inputimg->width;i++) {
			(progmon.*progmon.nowthrough)((float)i/(float)imgwidth);
			for (int j=0;j<inputimg->height;j++) {
				V3d v=worldA+right*(float)i/(float)inputimg->width+down*(float)j/(float)inputimg->height;
				Pixel p=proj(v);
				outputimg.setpos(p.x,p.y,inputimg->getpos(i,j));
			}
		}

		 TexturedRectangle3d rec=TexturedRectangle3d(worldA,right,down,inputimg);
		// for (int i=0;i<imgwidth;i++) {
			// (progmon.*progmon.nowthrough)((float)i/(float)imgwidth);
			// for (int j=0;j<imgheight;j++) {
				// Pixel p=Pixel(i,j);
				// V3d pixel=imgplaneFromPixel(p);
				// Line3d l=Line3d(eye,pixel);
				// V3d intersect=rec.intersect(l);
				// if (rec.inimage(intersect)) {
					// outputimg.setpos(i,j,rec.colAt(intersect));	 
				// }
			// }
		// }

		 printf("\nright = %s\n",rec.right.toString());
		 printf("down = %s\n",rec.down.toString());
		// printf("planeA = %s\n",rec.pos.toString());
		// printf("0 = %f %f ?\n",V3d::normdot(worldB-worldA,rec.nor),V3d::normdot(worldB-worldA,rec.nor));

		(progmon.*progmon.end)();

		if (genimage)
			outputimg.writefile(outname);

	}
	
	// Generate lines
	
	List<Line2d> lines;
	List<Line2d> noisylines;
	for (int i=0;i<=numlines;i++) {
		float thru=(float)i/(float)numlines;
		Line3d line=Line3d(worldA+thru*down,worldA+right+thru*down);

		V2d pla=proj(line.a);
		V2d plb=proj(line.b);
		Line2d line2d=Line2d(pla,plb);
		lines.add(line2d);
		Line2d noisyline2d=Line2d(line2d.a+V2d::randomcircle()*noise,line2d.b+V2d::randomcircle()*noise);
		noisylines.add(noisyline2d);

		if (genimage && overlay) {
			Pixel pa=proj(line.a);
			Pixel pb=proj(line.b);
			outputimg.thickline(pa,pb,myRGB::darkgreen,4);
			// printf("	%s - %s\n",pa.toString(),pb.toString());
		}
		// printf("	%s\n",line2d.toString());
		// printf("	%f\n",inta.y);
	}

	V2d hvp=lines.num(1).intersect(lines.num(lines.len));
	V2d vvp=Line2d(proj(worldA),proj(worldB)).intersect(Line2d(proj(worldC),proj(worldD)));

	V2d testvvp=Line2d(proj(worldA),proj(worldB)).intersection(Line2d(proj(worldC),proj(worldD)));
	V2d testhvp=Line2d(proj(worldA),proj(worldC)).intersection(Line2d(proj(worldB),proj(worldD)));
	V3d gotRight=imgplaneFromPixel(testhvp);
	gotRight.y=-gotRight.y;
	V3d gotDown=imgplaneFromPixel(testvvp);
	gotDown.y=-gotDown.y;

	printf("gotright = %s\ngotdown = %s\n",gotRight.toString(),gotDown.toString());
	printf("%f %f\n",V3d::normdot(right,gotRight),V3d::normdot(down,gotDown));

	printf("Testing vanishing point method:\n");
	printf("%f\n",V3d::normdot(right,down));
	printf("%f\n",V3d::normdot(gotRight,gotDown));
	printf("%f\n",V3d::normdot(right,gotRight));
	printf("%f\n",V3d::normdot(down,gotDown));
	// float fsq=(gotRight.x-imgwidth/2.0)*(gotDown.x-imgwidth/2.0)+(gotRight.y-imgheight/2.0)*(gotDown.y-imgheight/2.0);
	float fsq=(gotRight.x)*(gotDown.x)+(gotRight.y)*(gotDown.y);
	// printf("%f \n",fsq);
	if (fsq<0)
		printf("Estimate focal length from quad: %f\n",sqrt(-fsq));
	else
		printf("Error resolving focal length fsq>0 %f\n",sqrt(fsq));

	printf("HVP = %s\n",hvp.toString());
	printf("testHVP = %s\n",testhvp.toString());
	printf("VVP = %s\n",vvp.toString());
	printf("testVVP = %s\n",testvvp.toString());
	printf("vvp dist = %f\n",(vvp-lines.num(1).a).mag());
			
	// scry = ( k1 + l * A ) / ( l2 + l * B )
	//	 where k1 is y init, A is down.y, k2 is z init, B is down z
	{
					
	  printf("Ground truth (vertical only):\n");
  
	  float groundK1 = worldA.y;
	  float groundK2 = worldA.z;
	  float groundA = down.y/(float)numlines;
	  float groundB = down.z/(float)numlines;
	  printf("K1 = %f\nK2 = %f\nA = %f\nB = %f\n",groundK1,groundK2,groundA,groundB);
  
	  float groundU = focallength * groundK1/groundK2;
	  float groundV = groundA/groundK1;
	  float groundW = groundB/groundK2;
  
	  float guessU = (float)imgheight/2.0-lines.get(0).a.y;
  	
	  printf("      oldU = %f\n",groundU);
	  printf("      oldV = %f\n",groundV);
	  printf("      oldW = %f\n",groundW);
	  printf(" oldguessU = %f\n",guessU);

	}

	printf("Ground truth:\n");
		printf(" >> %s\n",unrot(worldA.dropz()).toString());

	Line2d baseline=Line2d(proj(worldA),proj(worldB));
	// rotang=(baseline.b-baseline.a).angle();
	rotang=pi-down.dropz().angle(); // rotang=-rotang;
	printf(">> rotang = %f\n",180.0*rotang/pi);
	
	printf("%f %f\n",(worldA.dropz()).x,(worldA.dropz()).y);
	printf("worldA %s -> %s\n",worldA.dropz().toString(),unrot(worldA.dropz()).toString());
	printf("%f %f\n",unrot(worldA.dropz()).x,unrot(worldA.dropz()).y);
	float groundK1 = unrot(worldA.dropz()).y;
	// float testgroundK1 = worldA.dropz().mag();
	// printf("k1 = %f\ntest = %f\n",groundK1,testgroundK1);
	// printf("%f\n",unrot(worldA.dropz()).mag());
	// groundK1=testgroundK1;
	float groundK2 = worldA.z;
	float groundA = unrot(down.dropz()).y/(float)numlines;
	float groundB = down.z/(float)numlines;
	printf("K1 = %f\nK2 = %f\nA = %f\nB = %f\n",groundK1,groundK2,groundA,groundB);

	// Seems ok here, but in vvpFromPoints, U sometimes estimated wrong
	// Not too much of problem because graph still fits visually

	float groundU = focallength * groundK1/groundK2;
	float groundV = groundA/groundK1;
	float groundW = groundB/groundK2;

	printf("!>> %s\n",unrotabout(lines.get(0).a).toString());
	// float guessU = (float)imgheight/2.0-unrotabout(lines.get(0).a).y;
	
	printf("U = %f\n",groundU);
	printf("V = %f\n",groundV);
	printf("W = %f\n",groundW);
	// printf("	newguessU = %f\n",guessU);

	if (genimage && overlay) {
		outputimg.line(hvp,lines.num(1).center(),myRGB::blue);
		outputimg.line(hvp,lines.num(lines.len).center(),myRGB::blue);
		outputimg.line(vvp,lines.num(lines.len/2+1).a,myRGB::blue);
		outputimg.line(vvp,lines.num(lines.len/2+1).b,myRGB::blue);
	}
	
	if (dofitting) {
		lines=noisylines;

		List<V2d> endpoints;
		for (int i=0;i<lines.len;i++)
			endpoints.add(lines.get(i).a);
		
		V2d v=vvpFromPoints(
			Line2d(lines.get(0).a,lines.get(lines.len-1).a),
			endpoints, imgwidth,imgheight, true
		);
		printf("Got VVP = %s\n",v.toString());

	  if (genimage && overlay) {
		  outputimg.line(v,lines.num(lines.len/2+1).a,myRGB::red);
		  outputimg.line(v,lines.num(lines.len/2+1).b,myRGB::red);
	  }

		if (false) {
		
		  FILE *sout=fopen("data.txt","w");
  
		  if (usespacings) {
					  	
			  if (domatlab || dooctave) {
						  	
				  fprintf(sout,"%% line position\n");
				  fprintf(sout,"t=[");
				  for (int i=0;i<lines.len-1;i++) {
					  Line2d l=lines.get(i);
					  Line2d nl=lines.get(i+1);
					  fprintf(sout,"%f",(l.a.y+nl.a.y)/2.0);
					  if (i<lines.len-2)
						  fprintf(sout,", ");
				  }
				  fprintf(sout,"];\n");
				  fprintf(sout,"%% line spacings\n");
				  fprintf(sout,"Data=[");
				  for (int i=0;i<lines.len-1;i++) {
					  Line2d l=lines.get(i);
					  Line2d nl=lines.get(i+1);
					  fprintf(sout,"%f",nl.a.y-l.a.y);
					  if (i<lines.len-2)
						  fprintf(sout,", ");
				  }
				  fprintf(sout,"];\n");
			  	
			  } else if (dognuplot) {
						  	
				  for (int i=0;i<lines.len-1;i++) {
					  Line2d l=lines.get(i);
					  Line2d nl=lines.get(i+1);
					  fprintf(sout,"%f %f\n",(float)imgheight/2.0-(l.a.y+nl.a.y)/2.0,l.a.y-nl.a.y);
				  }
			  	
			  }
		  	
		  } else { // Use endpoints not spacings
					  	
			  if (domatlab || dooctave) {
						  	
				  fprintf(sout,"Data=[");
				  for (int i=0;i<lines.len;i++) {
					  Line2d l=lines.get(i);
					  // fprintf(sout,"%f",l.a.y);
					  // fprintf(sout,"%f",V2d::dist(l.a,lines.get(0).a));
					  fprintf(sout,"%f",l.a.y);
					  if (i<lines.len-1)
						   fprintf(sout,", ");
				  }
				  fprintf(sout,"];\n");
			  	
			  } else if (dognuplot) {
						  	
				  for (int i=0;i<lines.len;i++) {
					  fprintf(sout,"%i %f\n",i,(float)imgheight/2.0-lines.get(i).a.y);
				  }
  
			  }
  
		  }
  
		  if (dognuplot) {
			  fclose(sout);
			  sout=fopen("moredata.txt","w");
		  }
  
		  fprintf(sout,"groundU = %f\n",groundU);
		  fprintf(sout,"groundV = %f\n",groundV);
		  fprintf(sout,"groundW = %f\n",groundW);
		  fprintf(sout,"groundK1 = %f\n",groundK1);
		  fprintf(sout,"groundK2 = %f\n",groundK2);
		  fprintf(sout,"groundA = %f\n",groundA);
		  fprintf(sout,"groundB = %f\n",groundB);
		  // fprintf(sout,"guessU = %f\n",guessU);
		  fclose(sout);
  
		  system(Sformat("cat data.txt matlab/%s > solve.txt",whichmatlabfile));
		  system(Sformat("cat solve.txt | %s > matlab.out",fittingcommand));
		  if (showgraph)
			  system("infcat matlab.out | tail -2 | head -1 > matlab.ans");
		  else
			  system("cat matlab.out | tail -2 | head -1 > matlab.ans");
	  	
		  FILE *sin=fopen("matlab.ans","r");
		  float matlabAns;
		  fscanf(sin,"%f",&matlabAns);
		  fclose(sin);
		  printf("matlab answer = %f\n",matlabAns);

		}

	}
	
	printf("ans just y = %f\n",focallength*down.y/down.z);
	float realA=focallength*down.dropz().mag()/down.z;
	printf("realAns = %f\n",realA);

	if (overlay)
		outputimg.writefile(overlayname);
	
}
