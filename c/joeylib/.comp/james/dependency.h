#ifndef __dependency_h
#define __dependency_h

typedef struct dTree dTree;

struct dTree
{
    char *symbol;
    struct dTree *left;
    struct dTree *right;

    /* 0=Root or Unknown
     * 1=Agent
     * 2=Action
     * 3=Rate
     * 4=Agent, in Composition
     * 5=Rate, in Actions
     */
    int type;

    /* The obvious use 
     */
    int paired;

    /* It is written in my notes to include
     * this, but I have no idea what it is
     * for. Hopefully it will become apparant
     * later on.
     */
    int flags; 
} ;

int checkCCS(ccsParseTree *);

#endif

