// Perform local thresholds on image regions
// Region by region (but overlap happens)

#include <joeylib.c>

bool flip;

Map2d<int> regmap;
List<Region> regs;

float virtualwidth(int x,int y) {
  int c=regmap.getpos(x,y);
  if (c>0)
    return sqrt(regs.p2num(c)->getlist()->len);
  else
    return -1;
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
//  float p=a.floatafter("-p","proportion",0.5);
//  int r=a.intafter("-r","radius",20);
  float rp=a.floatafter("-rp","window size proportional to width=sqrt(area of region)",0.5);
  int toosmall=a.intafter("-ts","a region area less than this is too small to make a decision",100);
  bool outputseperate=a.argexists("-all","output all regions seperately");
  int minimumarea=a.intafter("-min","discard incoming regions below this area",60*60);
  bool zoom=a.argexists("-zoom","change size of images");
  int zoomarea=a.intafter("-zoomarea","zoom all outputs to this area",256*128);
  flip=a.argexists("-f","flip region if mostly black");
  float usern=a.floatafter("-mod","modify threshold by scalar",1.0);
  String iname=a.getarg();
  String bname=a.argor("*NONE*");
  a.done();

  RGBmp orig=*RGBmp::readfile(iname);
//  Map2d<float> *map=Map2d<float>::readfile(iname);
  Map2d<float> *map=orig.getgreyscale();
  Map2d<bool> *bin=( Seq(bname,"*NONE*") ?
                     new Map2d<bool>(map->width,map->height,true) :
                     Map2d<bool>::readbinfile(bname)->binscaleto(map->width,map->height) );
	
  regmap=bin->getregionmap(true);
	regs=regmap.collectregions();
	for (int i=1;i<=regs.len;i++) {
	  Region *r=regs.p2num(i);
	  printf("Region size %i scans with window width %.1f\n",r->getlist()->len,rp*sqrt(r->getlist()->len));
	}

  // Create partial sums map
  printf("Creating partial sums\n");
  Map2d<float> *ps=new Map2d<float>(map->width,map->height);
  Map2d<int> *psa=new Map2d<int>(map->width,map->height);
  for (int y=0;y<ps->height;y++) {
  float psrow=0;
  int psrowa=0;
  for (int x=0;x<ps->width;x++) {
      if (bin->getpos(Pixel(x,y))) {
        psrow+=map->pos[x][y];
        psrowa++;
      }
    if (y==0) {
      ps->setpos(x,y,psrow);
      psa->setpos(x,y,psrowa);
    } else {
      ps->setpos(x,y,psrow+ps->getpos(x,y-1));
      psa->setpos(x,y,psrowa+psa->getpos(x,y-1));
    }
  }
  }

  Map2d<bool> *n=new Map2d<bool>(map->width,map->height,false);
  Map2d<float> *ratio=new Map2d<float>(map->width,map->height,0.0);

  // Threshold original
  printf("Thresholding\n");

  for (int i=1;i<=regs.len;i++) {
    Region reg=regs.num(i);
    List<Pixel> pixs=*reg.getlist();

    int left,right,top,bottom;
    left=reg.leftmost();
    right=reg.rightmost();
    top=reg.topmost();
    bottom=reg.bottommost();

    if ((right-left)*(bottom-top)>=minimumarea) {

      Map2d<bool> regimg;
      RGBmp cropimg;
      Pixel corner;
      if (outputseperate) {
        corner=Pixel(left,top);
        regimg=Map2d<bool>(right-left,bottom-top,false);
        cropimg=RGBmp(right-left,bottom-top);
        for (int x=0;x<cropimg.width;x++)
        for (int y=0;y<cropimg.height;y++)
          cropimg.setpos(x,y,orig.getpos(left+x,top+y));
      }

      int countblack=0;
      for (int j=1;j<=pixs.len;j++) {
        Pixel p=pixs.num(j);
        float r=rp*virtualwidth(p.x,p.y);
        float c=ps->getposclip(p.x+r/2,p.y+r/2)-ps->getposclip(p.x+r/2,p.y-r/2)-ps->getposclip(p.x-r/2,p.y+r/2)+ps->getposclip(p.x-r/2,p.y-r/2);
        int b=psa->getposclip(p.x+r/2,p.y+r/2)-psa->getposclip(p.x+r/2,p.y-r/2)-psa->getposclip(p.x-r/2,p.y+r/2)+psa->getposclip(p.x-r/2,p.y-r/2);
        c=c/(float)b; // c=Average
	c=c*usern;
        if (map->getpos(p)<c && b>=toosmall) {
          n->setpos(p,true);
          if (outputseperate)
            regimg.setpos(p-corner,true);
        } else {
          n->setpos(p,false);
          countblack++;
        }
        ratio->setpos(p,map->getpos(i,j)-c);
      }
      if (flip && countblack<0.5*pixs.len) {
        printf("Flipping region because more foreground than background!\n");
        for (int j=1;j<=pixs.len;j++) {
          Pixel p=pixs.num(j);
          float r=rp*virtualwidth(p.x,p.y);
          int b=psa->getposclip(p.x+r/2,p.y+r/2)-psa->getposclip(p.x+r/2,p.y-r/2)-psa->getposclip(p.x-r/2,p.y+r/2)+psa->getposclip(p.x-r/2,p.y-r/2);
          if (b>=toosmall) {
            n->setpos(p,!n->getpos(p));
            regimg.setpos(p-corner,!regimg.getpos(p-corner));
          }
          }
      }
      if (outputseperate) {
        int i=getnextfilenum();
        float scale=sqrt((float)zoomarea/(float)regimg.width/(float)regimg.height);
        regimg=regimg.binscaleby(scale);
        regimg.writefile(Sformat("reg%03i.bmp",i));
        // cropimg=*cropimg.scaledby(scale);
        cropimg.writefile(Sformat("crop%03i.bmp",i));
        regimg.freedom(); // yeah right!
        List<String> cropdata;
        cropdata.add(Sformat("%f",(float)left/(float)orig.width));
        cropdata.add(Sformat("%f",(float)right/(float)orig.width));
        cropdata.add(Sformat("%f",(float)top/(float)orig.height));
        cropdata.add(Sformat("%f",(float)bottom/(float)orig.height));
        writelinestofile(cropdata,Sformat("crop%03i.dat",i));
      }

    }
  }

  n->writefile("total.bmp");
  ratio->writefile("ratio.bmp");

}
