#ifndef error_C
  #define error_C

  #include <error.h>

/* Skipping declared method char *Sformat(char *, ... ) */


void error(char *s, ... ) {

// #ifdef ALLEGRO
    // if (allegrostarted>0)
      // allegroexit();
  // #endif


/*  va_list vas;
  va_start(vas,s);
  //char * t=Sformat(s,vas);
  //printf("%s\n",t);
  //printf(s,vas);
  printf("ERROR! %s",s);
  exit(1);*/
  
  va_list vas;
  va_start(vas,s); // Scount(f,"%")
  char * t=new char[256];
  vsprintf(t,s,vas);
  printf("ERROR! %s\n",t);
  #ifdef FORCESEGFAULT
    int i=*(int *)NULL;
    printf("%i\n",i);
  #endif
  exit(1);

}

void myerror(char *s) {
  error(s);
}




#endif
