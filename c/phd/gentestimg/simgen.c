// I think simgen is a combination of the other two (gentestimg and simulatecorrel)

#include <joeylib.h>

int imgheight,imgwidth;
float focallength,scale;
Plane imageplane;
V3d eye=V3d(0,0,0);

V3d imgplaneFromPixel(Pixel p) {
  return V3d(((float)p.x-(float)imgwidth/2.0)*scale,((float)p.y-(float)imgheight/2.0)*scale,focallength);
}

Pixel pixelFromImagePlane(V3d v) {
  return Pixel(v.x/scale+(float)imgwidth/2.0,imgheight-(v.y/scale+(float)imgheight/2.0));
}

Pixel proj(V3d v) {
	return pixelFromImagePlane(imageplane.intersect(Line3d(eye,v)));
}

int main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  bool domatlab=a.argexists("-matlab","vvp estimation with Matlab");
  bool genimage=a.argexists("-image","generate simulated image");
  bool overlay=a.argexists("-overlay","overlay info on simulated image");
  bool centralise=a.argexists("-centralise","centralise the document in the image");
  focallength=a.floatafter("-focal","focal length (prop to img width)",50.0);
  a.comment("Orientation of plane in degrees:");
  float roll=deg2rad(a.floatafter("-roll","roll",0.0));
  float yaw=deg2rad(a.floatafter("-yaw","yaw",20.0));
  float pitch=deg2rad(a.floatafter("-pitch","pitch",-20.0));
  float depth=a.floatafter("-depth","depth",100.0);
  scale=a.floatafter("-scale","scale",0.1);
  imgwidth=a.intafter("-width",640);
  imgheight=imgwidth*3/4;
	float xoff=a.floatafter("-xoff","x offset",0.0);
	float yoff=a.floatafter("-yoff","y offset",0.0);
  int numlines=a.intafter("-lines","number of lines in paragraph",10);
  float noise=a.floatafter("-noise","noise",0.0*640/imgwidth);
  String inname=a.argor("document image file","in.bmp");
  String outname=a.argor("output filename","out.bmp");
  a.done();
  float width=70;
  float height=100;

  RGBmp *inputimg;
  RGBmp outputimg;

  // Set up page
  
  float size=scale*imgwidth;
  // V3d right=V3d(scale,0,0)*imgwidth;
  // V3d down=V3d(0,-scale*(float)inputimg->height/(float)inputimg->width,0)*imgwidth;
  V3d right=V3d(width,0,0);
  V3d down=V3d(0,height,0); // Note not -height as in Maple
  right=right.rotated(V3d::k,roll);
  right=right.rotated(V3d::i,pitch);
  right=right.rotated(V3d::j,-yaw);
  down=down.rotated(V3d::k,roll);
  down=down.rotated(V3d::i,pitch);
  down=down.rotated(V3d::j,-yaw);
  down.y=-down.y; // Hack to sync with Maple
  // V3d worldA=V3d(imgwidth/2,imgheight/2,depth);
  V3d worldA=V3d(0,0,depth);
	worldA=worldA-V3d(xoff,yoff,0);
	if (centralise)
		worldA=worldA-down/2.0-right/2.0;
  V3d worldB=worldA+down;
  V3d worldC=worldA+right;
  V3d worldD=worldA+right+down;
  V3d cen=(worldA+worldD)/2.0;
  printf("%s %s\n",right.toString(),down.toString());

  printf("right = %s\n",right.toString());
  printf("down = %s\n",down.toString());
  printf("A = %s\n",worldA.toString());
  printf("B = %s\n",worldB.toString());
  printf("C = %s\n",worldC.toString());
  printf("D = %s\n",worldD.toString());

  if (genimage) {
  
    // RGBmp *inputimg=RGBmp::toRGBmp(Map2d<float>::readfile(inname));
    inputimg=RGBmp::readfile(inname);
    outputimg=RGBmp(imgwidth,imgheight,myRGB::white);
  
    inputimg->writefile("test.bmp");
    
    TexturedRectangle3d rec=TexturedRectangle3d(worldA,right,down,inputimg);
    
    // Generate image
  
    for (int i=0;i<imgwidth;i++) {
      if ( (i % 30) == 0 )
        printf("%i/%i\n",i,imgwidth);
      for (int j=0;j<imgheight;j++) {
      Pixel p=Pixel(i,imgheight-1-j);
        V3d pixel=imgplaneFromPixel(p);
        Line3d l=Line3d(eye,pixel);
        V3d intersect=rec.intersect(l);
      // printf("%f\n",intersect.z);
        if (!rec.onplane(intersect))
          printf("Error here %f\n",rec.distto(intersect));
        // printf("%s %s %s %s\n",eye.toString(),pixel.toString(),l.toString(),intersect.toString());
        if (rec.inimage(intersect)) {
          // printf("*\n");
          outputimg.setpos(i,j,rec.colAt(intersect));   
        }
      }
    }

		if (overlay) {
			// outputimg.
		}

  }
  
  // Do simulation
    
    imageplane=Plane(V3d(0,0,focallength),V3d(0,0,1));
    List<Line2d> lines;
    List<Line2d> noisylines;
    for (int i=0;i<=numlines;i++) {
      float thru=(float)i/(float)numlines;
      Line3d line=Line3d(worldA+thru*down,worldA+right+thru*down);
      // Line3d line=Line3d(rec.pos+rec.down*thru,
                      // rec.pos+rec.right+rec.down*thru);
      Line3d la=Line3d(eye,line.a);
      V3d inta=imageplane.intersect(la);
      printf("%s -> %s\n",line.a.toString(),inta.toString());
      Line3d lb=Line3d(eye,line.b);
      V3d intb=imageplane.intersect(lb);
      printf("%s -> %s\n",line.b.toString(),intb.toString());
      Line2d line2d=Line2d(V2d(inta.x,inta.y),V2d(intb.x,intb.y));
      Line2d noisyline2d=Line2d(V2d(inta.x,inta.y)+V2d::randomcircle()*noise,V2d(intb.x,intb.y)+V2d::randomcircle()*noise);
      lines.add(line2d);
      noisylines.add(noisyline2d);
      if (genimage && overlay) {
				Pixel pa=proj(line.a);
				Pixel pb=proj(line.b);
        outputimg.thickline(pa,pb,myRGB::darkgreen,4);
				printf("  %s - %s\n",pa.toString(),pb.toString());
			}
      printf("  %s\n",line2d.toString());
      printf("  %f\n",inta.y);
    }
    V2d hvp=lines.num(1).intersect(lines.num(2));
    Line2d baseline=Line2d(lines.num(1).a,lines.num(numlines).a);
    V2d vvpdir=(baseline.b-baseline.a).norm();
    Line2d otherline=Line2d(imageplane.intersect(Line3d(eye,worldC)).dropz(),imageplane.intersect(Line3d(eye,worldD)).dropz());
    V2d vvp=baseline.intersect(otherline);
    printf("VVP = %s\n",vvp.toString());
    printf("vvp dist = %f\n",(vvp-lines.num(1).a).mag());
    // printf("down = %s\n",down);
				
		// scry = ( k1 + l * A ) / ( l2 + l * B )
		//   where k1 is y init, A is down.y, k2 is z init, B is down z

		float groundK1 = worldA.y;
		float groundK2 = worldA.z;
		float groundA = down.y;
		float groundB = down.z;

		float groundU = groundK1/groundK2;
		float groundV = groundA/groundK1;
		float groundW = groundB/groundK2;
		
		printf("Ground truth:\n");
		printf("  U = %f\n",groundU);
		printf("  V = %f\n",groundV);
		printf("  W = %f\n",groundW);
		printf("  U*V/W = %f\n",groundU*groundV/groundW);
		
    if (domatlab) {
			lines=noisylines;
			FILE *sout=fopen("data.txt","w");
      fprintf(sout,"Data=[");
      for (int i=0;i<lines.len;i++) {
        Line2d l=lines.get(i);
        // fprintf(sout,"%f",l.a.y);
        fprintf(sout,"%f",V2d::dist(l.a,lines.get(0).a));
        if (i<lines.len-1)
          fprintf(sout,", ");
      }
      fprintf(sout,"];\n");
			fclose(sout);

			system("cat data.txt matlab/sol1u.txt > solve.txt");
			system("cat solve.txt | matlab > matlab.out");
			system("cat matlab.out | tail -2 | head -1 > matlab.ans");
			
			FILE *sin=fopen("matlab.ans","r");
			float matlabAns;
			fscanf(sin,"%f",&matlabAns);
			fclose(sin);
			printf("matlab answer = %f\n",matlabAns);
    }
    
		printf("ans just y = %f\n",focallength*down.y/down.z);
		float realA=focallength*down.dropz().mag()/down.z;
		printf("realAns = %f\n",realA);

    /*
    // From pp.c
  
    List<float> cds;
    for (int i=1;i<=numlines;i++) {
      V2d hit=baseline.intersect(Line2d(hvp,lines.num(i).b));
      printf("%s ~ %s ?\n",lines.num(i).a.toString(),hit.toString());
      cds.add((hit-baseline.a).dot(vvpdir));
    }

    // We now do simultaneous calculation of
    // various correlation methods for comparison

    // Accumulate correlator
    Correlator2d csspacing;
    Correlator2d cslength;
    // Why not do line-spacing with gaps?
    // (seems equivalent so no worry)
    for (int i=1;i<numlines;i++) {
      float a=cds.num(i);
      // if (correlator==LineSpacing) {
        // x = average of two cds, y = dist between
        float b=cds.num(i+1);
        // float pos=(a+b)/2.0;
        float pos=a;
        csspacing.add(V2d(pos,myabs(b-a)),1.0);
        // printf("Correlating using lines =)\n");
      // } else if (correlator==LineWidth) {
        // y = width of line
        // float pos=a;
        cslength.add(V2d(pos,lines.num(i).length),1.0);
      // } else {
        // error("Non existent correlation method");
      // }
    }

    // One of the methods is chosen for final VVP.
    float vpd=csspacing.crossesy();
    printf("VP is %f along baseline\n",vpd);

    V2d vvp=baseline.a+vvpdir*vpd;
    V2d vvpfromdata=baseline.intersect(Line2d(lines.num(1).b,lines.num(numlines).b));

    printf("Found %s ~ Source %s ?\n",vvp.toString(),vvpfromdata.toString());

    drawCorrelator2dInColour(csspacing).writefile("spacings.bmp");
    drawCorrelator2dInColour(cslength).writefile("lengths.bmp");
  
  //// Overlay simulation results

    outputimg.line(hvp,lines.num(1).a,myRGB::red);  
    outputimg.line(hvp,lines.num(numlines).a,myRGB::red);  
    outputimg.line(vvp,lines.num(1).a,myRGB::red);  
    outputimg.line(vvp,lines.num(1).b,myRGB::red);  
    outputimg.line(vvpfromdata,lines.num(1).a,myRGB::red);  
    outputimg.line(vvpfromdata,lines.num(1).b,myRGB::red);  

  */

  if (genimage)
    outputimg.writefile(outname);

  // Output data to accompany
//   List<String> data;
//   data.add(Sformat("Eye:%s",eye.toString());
//   data.add(Sformat("Eye:%s",eye.toString());
//   data.add(Sformat("Eye:%s",eye.toString());
//   data.add(Sformat("Eye:%s",eye.toString());
//   data.add(Sformat("Eye:%s",eye.toString());
//   data.add(Sformat("Eye:%s",eye.toString());
//   data.add(Sformat("Eye:%s",eye.toString());
  
//   Map2d<bool> *in=Map2d<float>::readfile(ifname)->threshold(0.5);
// //  in->display();
//   Map2d<bool> *n=new Map2d<bool>(in->width,in->height);
// 
//   for (int i=0;i<n->width;i++)
//     for (int j=0;j<=n->height;j++) {
//       if (npixelwithinrad(in,i,j,r))
//         n->pos[i][j]=true;
//       else
//         n->pos[i][j]=false;
//     }
// 
//   n->writefile(ofname);
// //  n->display();
//   printf("expand: written file %s radius %i\n",ofname,r);

}
