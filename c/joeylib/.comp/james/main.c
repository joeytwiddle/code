#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ccsParser.h"
#include "dependency.h"
#include "globals.h"


/* Exit codes
 *
 * 0 -- Normal
 * 1 -- Bad or missing arguments
 * 2 -- Empty, missing or improperly formatted file
 * 3 -- Buffer overrun in frUngetchar
 * 4 -- Parsing error, somewhere in rates
 * 5 -- Parsing error, somewhere in agents 
 * 6 -- Parsing error, somewhere in composition
 * 7 -- Dependency error, duplicate agent
 * 8 -- Dependency error, suspect duplicate inclusion of an agent in a composition
 *
 */

int main(int argc, char *argv[])
{
    ccsParseTree *ccspt;
  
    if (argc<2)
    {
        printf("usage: %s file\n\nTry again.\n",argv[0]);
        exit(1);
    }

    /* Process options     */
    /*                     */
    /* To be defined later */

    /* Parse file */
    /* Argument is static for now, but needs to be changed later */
    ccspt = ccsParseFile(argv[1]);

    if (ccspt==NULL)
        return 2;

    ccsPrintParseTree(ccspt);
    
    return 0;
}

