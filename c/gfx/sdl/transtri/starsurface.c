#define circ(ang) ( 2.0*M_PI*qsin(ang) )
#define focalLength 250
#define PREXAG 8
#define project(X,Y,Z,x,y) { x = PREXAG*STARX+PREXAG*SCALE*(X)/(focalLength+(Z)); y = PREXAG*STARY+PREXAG*SCALE*(Y)/(focalLength+(Z)); }
// #define project(X,Y,Z,x,y) { x = SCRWID/2+(X); y = SCRHEI/2+(Y); }
#define crossProd(xa,ya,za,xb,yb,zb,x,y,z) { x = (ya)*(zb) - (za*yb); y = (za*xb)-(xa*zb); z = (xa)*(yb)-(ya)*(xb); }
// 1 <= checkSize
#define checkSize 2

{
    // int rad[starRes][starRes];
    Uint16 countAround[starRes+1];
    Uint16 xs[starRes+1][starRes];
    Uint16 ys[starRes+1][starRes];
    float Xs[starRes+1][starRes];
    float Ys[starRes+1][starRes];
    float Zs[starRes+1][starRes];
    float alpha,beta;
        // deltaa = M_PI/(float)starRes,
        // deltab;
    int i,j;

    // for (i=0,alpha=0.0;alpha<M_PI;alpha+=deltaa,i++) {
    for (i=0;i<starRes+1;i++) {
        // alpha=deltaa*(float)i;
        alpha=M_PI*(float)i/(float)(starRes);
        // deltab=2.0*deltaa*circ(alpha)/circ(M_PI/2.0);
        // countAround[i]=starRes*circ(alpha)/circ(M_PI/2.0);
        countAround[i]=starRes;
        for (j=0;j<countAround[i];j++) {
            beta = 2.0*M_PI*(float)j/(float)countAround[i];
            {
            // float alpha2=alpha+0.01*framebits;
            // float beta2=beta-0.014*framebits;
            // float
                // X=rad*sin(alpha2)*cos(beta2),
                // Y=(float)rad*cos(alpha2-M_PI),
                // Z=-rad*sin(alpha2)*sin(beta2);
            float rad=starRad(alpha,beta);
				#define rotalpha alpha // (543.210-alpha+framebits*0.04)
				#define rotbeta (123.456+beta+framebits*0.01)
            float
                X=rad*sin(rotalpha)*cos(rotbeta),
                Y=(float)rad*cos(rotalpha-M_PI),
                Z=-rad*sin(rotalpha)*sin(rotbeta);
            project(X,Y,Z,xs[i][j],ys[i][j]);
            Xs[i][j]=X;
            Ys[i][j]=Y;
            Zs[i][j]=Z;
            // printf("%f %f %f\n",X,Y,Z);
            // printf(" -> %i %i (%i,%i)\n",i,j,xs[i][j],ys[i][j]);
            }
        }
        // countAround[i]=j;
        // printf("%f %i\n",deltab,j);
    }

    for (i=0;i<starRes;i++) {
        for (j=0;j<countAround[i];j++) {
            int nexti=(i+1)%(starRes+1);
            int nextj=(j+1)%(countAround[i]);
            // printf("%i %i / %i   %i %i / %i\n",i,nexti,starRes,j,nextj,countAround[i]);
            // printf("(%i,%i)  ",xs[i][j],ys[i][j]);
            // printf("(%i,%i)\n",xs[nexti][j],ys[nexti][j]);
            // printf(" (%i,%i)  ",xs[i][nextj],ys[i][nextj]);
            // printf("(%i,%i)\n",xs[nexti][nextj],ys[nexti][nextj]);
            // printf("%i %i - %i %i\n",xs[i][j],ys[i][j],xs[nexti][nextj],ys[nexti][nextj]);
            float X,Y,Z;
            crossProd(Xs[nexti][j]-Xs[i][j],Ys[nexti][j]-Xs[i][j],Zs[nexti][j]-Xs[i][j],Xs[i][nextj]-Xs[i][j],Ys[i][nextj]-Ys[i][j],Zs[i][nextj]-Zs[i][j],X,Y,Z);
            {
                // float bright=X*X+Y*Y;
                // float bright=sqrt(square(X)+square(Y))/sqrt(square(X)+square(Y)+square(Z));
                float bright=fabs(Y)/sqrt(square(X)+square(Y)+square(Z));
                // float bright=1.0;
                // float bright=fabs(Z)/sqrt(square(X)+square(Y)+square(Z));
                // float bright=4.0*(X*camX+Y*camY+Z*camZ)/(sqrt(square(X)+square(Y)+square(Z)))/10.0;
                // if (bright<0)
                    // bright=0;
                // float bright=pow(fabs(qcos((float)j*M_PI/(float)countAround[i])),50.0);
                // if (((i/checkSize)%2)!=((j/checkSize)%2))
                if ((((i/checkSize)%2)&&((j/checkSize)%3))||((j/checkSize%2)))
                    // bright=0;
                {
                starPlotTri(xs[i][j]/PREXAG,ys[i][j]/PREXAG,xs[nexti][j]/PREXAG,ys[nexti][j]/PREXAG,xs[i][nextj]/PREXAG,ys[i][nextj]/PREXAG,bright);
                starPlotTri(xs[i][nextj]/PREXAG,ys[i][nextj]/PREXAG,xs[nexti][j]/PREXAG,ys[nexti][j]/PREXAG,xs[nexti][nextj]/PREXAG,ys[nexti][nextj]/PREXAG,bright);
                }
                // plotLine(screen,xs[i][j],ys[i][j],xs[nexti][nextj],ys[nexti][nextj],0xffffff);
                // plotLine(screen,xs[i][j],ys[i][j],xs[nexti][j],ys[nexti][j],0xffffff);
                // plotLine(screen,xs[i][j],ys[i][j],xs[i][nextj],ys[i][nextj],0xffffff);
            }
        }
    }

}



