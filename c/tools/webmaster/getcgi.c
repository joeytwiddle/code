/*************************************************************************/
/**                                                                     **/
/**     getcgivars.c-- routine to read CGI input variables into an      **/
/**         array of strings.                                           **/
/**                                                                     **/
/**     Written in 1996 by James Marshall, james@jmarshall.com, except  **/
/**     that the x2c() and unescape_url() routines were lifted directly **/
/**     from NCSA's sample program util.c, packaged with their HTTPD.   **/
/**                                                                     **/
/**     For the latest, see http://www.jmarshall.com/easy/cgi/ .        **/
/**                                                                     **/
/*************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/** Convert a two-char hex string into the char it represents **/
char x2c(char *what) {
   register char digit;

   digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
   digit *= 16;
   digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));
   return(digit);
}

/** Reduce any %xx escape sequences to the characters they represent **/
void unescape_url(char *url) {
    register int i,j;

    for(i=0,j=0; url[j]; ++i,++j) {
        if((url[i] = url[j]) == '%') {
            url[i] = x2c(&url[j+1]) ;
            j+= 2 ;
        }
    }
    url[i] = '\0' ;
}


/** Read the CGI input and place all name/val pairs into list.        **/
/** Returns list containing name1, value1, name2, value2, ... , NULL  **/
char **getcgivars() {
    register int i ;
    char *request_method ;
    int content_length;
    char *cgiinput ;
    char **cgivars ;
    char **pairlist ;
    int paircount ;
    char *nvpair ;
    char *eqpos ;

    /** Depending on the request method, read all CGI input into cgiinput **/
    /** (really should produce HTML error messages, instead of exit()ing) **/
    request_method= getenv("REQUEST_METHOD") ;
    if (!strcmp(request_method, "GET") || !strcmp(request_method, "HEAD") ) {
        cgiinput= strdup(getenv("QUERY_STRING")) ;
    }
    else if (!strcmp(request_method, "POST")) {
        /* strcasecmp() is not supported in Windows-- use strcmpi() instead */
        if ( strcasecmp(getenv("CONTENT_TYPE"), "application/x-www-form-urlencoded")) {
            printf("getcgivars(): Unsupported Content-Type.\n") ;
            exit(1) ;
        }
        if ( !(content_length = atoi(getenv("CONTENT_LENGTH"))) ) {
            printf("getcgivars(): No Content-Length was sent with the POST request.\n") ;
            exit(1) ;
        }
        if ( !(cgiinput= (char *) malloc(content_length+1)) ) {
            printf("getcgivars(): Could not malloc for cgiinput.\n") ;
            exit(1) ;
        }
        if (!fread(cgiinput, content_length, 1, stdin)) {
            printf("Couldn't read CGI input from STDIN.\n") ;
            exit(1) ;
        }
        cgiinput[content_length]='\0' ;
    }
    else {
        printf("getcgivars(): unsupported REQUEST_METHOD\n") ;
        exit(1) ;
    }

    /** Change all plusses back to spaces **/
    for(i=0; cgiinput[i]; i++) if (cgiinput[i] == '+') cgiinput[i] = ' ' ;

    /** First, split on "&" to extract the name-value pairs into pairlist **/
    pairlist= (char **) malloc(256*sizeof(char **)) ;
    paircount= 0 ;
    nvpair= strtok(cgiinput, "&") ;
    while (nvpair) {
        pairlist[paircount++]= strdup(nvpair) ;
        if (!(paircount%256))
            pairlist= (char **) realloc(pairlist,(paircount+256)*sizeof(char **)) ;
        nvpair= strtok(NULL, "&") ;
    }
    pairlist[paircount]= 0 ;    /* terminate the list with NULL */

    /** Then, from the list of pairs, extract the names and values **/
    cgivars= (char **) malloc((paircount*2+1)*sizeof(char **)) ;
    for (i= 0; i<paircount; i++) {
        if (eqpos=strchr(pairlist[i], '=')) {
            *eqpos= '\0' ;
            unescape_url(cgivars[i*2+1]= strdup(eqpos+1)) ;
        } else {
            unescape_url(cgivars[i*2+1]= strdup("")) ;
        }
        unescape_url(cgivars[i*2]= strdup(pairlist[i])) ;
    }
    cgivars[paircount*2]= 0 ;   /* terminate the list with NULL */

    /** Free anything that needs to be freed **/
    free(cgiinput) ;
    for (i=0; pairlist[i]; i++) free(pairlist[i]) ;
    free(pairlist) ;

    /** Return the list of name-value strings **/
    return cgivars ;

}

/***************** end of the getcgivars() module ********************/
