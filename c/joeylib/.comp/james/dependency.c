#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "ccsParser.h"
#include "dependency.h"

/* Basically a binary tree
 * ADT ! (to start with)
 */

dTree *mkDTnode(char *, int, int, int);
int insDTnode(dTree *, char *, int);

dTree *mkDTnode(char *s, int t, int p, int f)
{
    dTree *ret=malloc(sizeof(dTree));
    ret->left=NULL;
    ret->right=NULL;
    ret->symbol=strdup(s);
    ret->paired=p;
    ret->type=t;
    ret->flags=f;
    return ret;

}

int insDTnode(dTree *this, char *s, int t)
{

    /* Does not handle rates *yet*
     */

    if (strcmp(this->symbol,s)==0)
    {
        if (this->type==1 && t==1)
        {
            fprintf(stderr,"Suspect duplicate labelling of agents.\n");
            exit(7);
        }

        if (this->type==1 && t==4)
        {
            if (this->paired==0)
                this->paired=1;
            else
            {
                fprintf(stderr,"Suspect duplicate use of an agent in a composition.\n");
                exit(8);
            }
        }


    }
}

/* 0=Bad
 * 1=Good
 */
int checkCCS(ccsParseTree *cpt)
{
    dTree *dt=mkDTnode("",0,0,1);
    ccsAgents *at=cpt->agents;
    ccsComposition *ct=cpt->composition;
    ccsAgent *agt=NULL;
    ccsActions *act=NULL;
    ccsAction *actt=NULL;

    return 1;    
}

