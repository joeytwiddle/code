#ifndef __parser_h
#define __parser_h

typedef struct Rate
{
    char *label;
    double value;
} Rate;


/* Don't know why I had to seperate these out -- it wouldn't
 * compile otherwise. No good explanation as far as I can tell
 */
typedef struct Rates Rates;

struct Rates
{
    Rate *rate;
    Rates *next;
} ;

typedef struct Action Action;

struct Action
{
    /* 1 = Action 
     * 0 = Agent Label 
     */
    int type;

    char *label;
    char *rate;

    /* To be handled properly later */
    char *message;
} ;


typedef struct Actions Actions;

struct Actions
{
    Action *action;
    char seperator;
    Actions *next;
} ;

typedef struct Agent
{
    char *label;
    Actions *actions;
} Agent;

typedef struct Agents Agents;

struct Agents
{
    Agent *agent;
    Agents *next;
} ;

typedef struct CElement
{
     char *label;
} CElement;

typedef struct Composition Composition;

struct Composition
{
    CElement *ce;
    
    /* 0 = Unknown or none
     * 1 = Parallel/unspecified
     * 2 = Parallel/explicit labels
     */
    int seperator;
    Composition *next;
} ;

typedef struct ParseTree
{
    Rates *rates;
    Agents *agents;
    Composition *composition;
} ParseTree;


ParseTree *parseFile(char *);
void printParseTree(ParseTree *);

#endif
