#ifndef __ccs_parser_h
#define __ccs_parser_h

typedef struct ccsAction ccsAction;

struct ccsAction
{
    /* 2 = Input Action 
     * 1 = Output or Regular Action
     * 0 = Agent Label 
     */
    int type;
    char *label;
} ;


typedef struct ccsActions ccsActions;

struct ccsActions
{
    ccsAction *action;
    char seperator;
    ccsActions *next;
} ;

typedef struct ccsAgent
{
    char *label;
    ccsActions *actions;
} ccsAgent;

typedef struct ccsAgents ccsAgents;

struct ccsAgents
{
    ccsAgent *agent;
    ccsAgents *next;
} ;

typedef struct ccsCElement
{
     char *label;
} ccsCElement;

typedef struct ccsComposition ccsComposition;

struct ccsComposition
{
    ccsCElement *ce;
    
    /* 0 = Unknown or none
     * 1 = Parallel/unspecified
     */
    int seperator;
    ccsComposition *next;
} ;

typedef struct ccsParseTree
{
    ccsAgents *agents;
    ccsComposition *composition;
} ccsParseTree;


ccsParseTree *ccsParseFile(char *);
void ccsPrintParseTree(ccsParseTree *);

#endif
