// A way of hierarchical interpolation, ineficcient if eval is not memo-ed

void quadify(int l,int t,int r,int b,float (*eval)(int,int),void (*fulfil)(int,int,int,int,float,float,float,float),float tol,int res) {
  float nw=eval(l,t);
  float ne=eval(r,t);
  float sw=eval(l,b);
  float se=eval(r,b);
  float mid=eval((l+r)/2,(t+b)/2);
  float ave=(nw+ne+sw+se+mid)/5.0;
  if (max(diff(ave,mid),max(max(diff(ave,nw),diff(ave,ne)),max(diff(ave,sw),diff(ave,se))))>tol && max(r-l,b-t)>res) {
    int mx=(l+r)/2;
    int my=(t+b)/2;
    quadify(l,t,mx,my,eval,fulfil,tol,res);
    quadify(mx,t,r,my,eval,fulfil,tol,res);
    quadify(l,my,mx,b,eval,fulfil,tol,res);
    quadify(mx,my,r,b,eval,fulfil,tol,res);
  } else
    fulfil(l,t,r,b,nw,ne,sw,se);
}

/*void quadify(int l,int t,int r,int b,float (*eval)(int,int),void (*fulfil)(int,int,int,int,float,float,float,float),float tol,int res) {
  float nw=eval(l,t);
  float ne=eval(r,t);
  float sw=eval(l,b);
  float se=eval(r,b);
  float ave=(nw+ne+sw+se)/4.0;
  if (max(max(diff(ave,nw),diff(ave,ne)),max(diff(ave,sw),diff(ave,se)))>tol && max(r-l,b-t)>res) {
    int mx=(l+r)/2;
    int my=(t+b)/2;
    quadify(l,t,mx,my,eval,fulfil,tol,res);
    quadify(mx,t,r,my,eval,fulfil,tol,res);
    quadify(l,my,mx,b,eval,fulfil,tol,res);
    quadify(mx,my,r,b,eval,fulfil,tol,res);
  } else
    fulfil(l,t,r,b,nw,ne,sw,se);
}*/

JBmp *Quadjbmp;
Map2d<int> *Quadmap;
int (*Quadfn)(int,int);

float Quadgetpos(int i,int j) {
  if (Quadmap->pos[i][j]==-1)
    Quadmap->pos[i][j]=Quadfn(i,j);
  return Quadmap->pos[i][j];
}

void Quadfulfil(int l,int t,int r,int b,float nw,float ne,float sw,float se) {
  Quadjbmp->interpolate(l,t,r,b,nw,ne,sw,se);
}

void quadify(JBmp *j,int (*colourat)(int,int),int tol,int res) {
  Quadjbmp=j;
  Quadmap=new Map2d<int>(j->width,j->height,-1);
  Quadfn=colourat;
  quadify(0,0,j->width-1,j->height-1,&Quadgetpos,&Quadfulfil,tol,(res<3?3:res));
}

void quadify(JBmp *j,int (*colourat)(int,int),int tol,int res,int size) {
  Quadjbmp=j;
  Quadmap=new Map2d<int>(j->width,j->height,-1);
  Quadfn=colourat;
  for (int x=0;x<=j->width-size;x+=size)
  for (int y=0;y<=j->height-size;y+=size)
    quadify(x,y,x+size-1,y+size-1,&Quadgetpos,&Quadfulfil,tol,(res<3?3:res));
}
