#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ccsParser.h"
#include "fileReader.h"
#include "globals.h"


#define MAXX_LABEL 128

ccsAgents *ccsParseAgents(fileReader *);
ccsAgent *ccsParseAgent(fileReader *);
ccsActions *ccsParseActions(fileReader *);
ccsAction *ccsParseAction(fileReader *);
ccsComposition *ccsParseComposition(fileReader *);

void ccsPrintAgents(ccsAgents *);
void ccsPrintActions(ccsActions *);
void ccsPrintComposition(ccsComposition *);

ccsParseTree *ccsParseFile(char *filename)
{
    fileReader *fp=frOpen(filename,"r");
    ccsParseTree *ret=NULL;

    /* Just to double check ! */
    if (fp==NULL)
    {
        fprintf(stderr,"Specified filename doesn't seem to exist.\nThis is a problem. Try again.\n");
        return NULL;
    }

    /* OK, so now we start ... */
    ret=malloc(sizeof(ccsParseTree));

    /* Parse agents (as if) */
    ret->agents=ccsParseAgents(fp);

    /* Parse Composition */
    ret->composition=ccsParseComposition(fp);

    return ret;
}

ccsAgents *ccsParseAgents(fileReader *fp)
{
    ccsAgents *ret = malloc(sizeof(ccsAgents));
    ccsAgents *tmp = ret;
    ret->next=NULL;
    ret->agent=NULL;

    /* Iteratevely call until we run out of hashes
     */
    while (lookahead(fp)=='#')
    {
        if (ret->agent==NULL)
            ret->agent=ccsParseAgent(fp);
        else
        {
            tmp->next=malloc(sizeof(ccsAgents));
            tmp=tmp->next;
            tmp->next=NULL;
            tmp->agent=ccsParseAgent(fp);
        }
    }
    return ret;
}

ccsAgent *ccsParseAgent(fileReader *fp)
{
    int c;
    char *label=malloc(sizeof(char)*MAXX_LABEL);
    ccsAgent *ret;
    int t1,t2;

    for (t1=0;t1<MAXX_LABEL;t1++)
    {
        label[t1]=0;
    }

    
    c=frGetchar(fp);
    if (c=='#')
    {
        c=frGetchar(fp);
        t1=0;
        while (c!='=' && c!='\n' && c!=EOF && c!=' ' && t1<MAXX_LABEL)
        {
            label[t1++]=c;
            c=frGetchar(fp);
        }
        if (t1==MAXX_LABEL) exit(5);

        ret=malloc(sizeof(ccsAgent));
        ret->label=strdup(label);
        ret->actions=ccsParseActions(fp);
        free(label);

        return ret;
    }
    else return NULL;
}

ccsActions *ccsParseActions(fileReader *fp)
{
    ccsActions *ret=malloc(sizeof(ccsActions));
    ccsActions *tmp=ret;
    ccsAction *a=NULL;
    char *label=malloc(sizeof(char)*MAXX_LABEL);
    int i;
    int c=frGetchar(fp);

    tmp->action=NULL;
    tmp->next=NULL;

    while (c!='=' && c!='\n' && c!=EOF)
        c=frGetchar(fp);

    if (c=='\n')
        return NULL;
    if (c==EOF)
        exit(5);
    if (c=='=')
    {
        c=frGetchar(fp);
        while (c==' ')
            c=frGetchar(fp);
        if (c=='(')
        {
            frUngetchar(fp,c);
            /* Action */
            if (tmp->action==NULL)
                tmp->action=ccsParseAction(fp);
            else
            {
                tmp->next=malloc(sizeof(ccsActions));
                tmp->next->next=NULL;
                tmp=tmp->next;
                tmp->action=ccsParseAction(fp);
            }
        }
        else 
        {
            /* Agent Label */
            for (i=0; i<MAXX_LABEL; i++)
                label[i]=0;

            i=1;
            label[0]=c;
            c=frGetchar(fp);
            while (c!=' ' && c!=')' && c!=EOF && c!='\n' && c!=',' && c!='.' && c!=';')
            {
                label[i++]=c;
                c=frGetchar(fp); 
            }
            if (c=='\n' || c==EOF || c==',')
                exit(5);
            if (c=='.' || c==';')
                frUngetchar(fp,c);
            if (c==' ' || c=='.' || c==';')
            {
                a=malloc(sizeof(ccsAction));
                a->type=0;
                a->label=strdup(label);

                if (tmp->action==NULL)
                    tmp->action=a;
                else
                {
                   tmp->next=malloc(sizeof(ccsActions));
                   tmp=tmp->next;
                   tmp->next=NULL;
                   tmp->seperator=0;
                   tmp->action=a;
                }
            }
        } 
        c=frGetchar(fp);
        while (c==' ')
            c=frGetchar(fp);
        while (c=='.' || c=='+')
        {
            tmp->seperator=c;
            c=frGetchar(fp);
            while(c==' ')
                c=frGetchar(fp);
         


        if (c=='(')
        {
            frUngetchar(fp,c);
            /* Action */
            if (tmp->action==NULL)
                tmp->action=ccsParseAction(fp);
            else
            {
                tmp->next=malloc(sizeof(ccsActions));
                tmp->next->next=NULL;
                tmp=tmp->next;
                tmp->action=ccsParseAction(fp);
            }
        }
        else
        {
            /* Agent Label */
            for (i=0; i<MAXX_LABEL; i++)
                label[i]=0;

            i=1;
            label[0]=c;
            c=frGetchar(fp);
            while (c!=' ' && c!=')' && c!=EOF && c!='\n' && c!=',' && c!='.' && c!=';')
            {
                label[i++]=c;
                c=frGetchar(fp);
            }
            if (c=='\n' || c==EOF || c==',')
                exit(5);
            if (c=='.' || c==';')
                frUngetchar(fp,c);
            if (c==' ' || c=='.' || c==';')
            {
                a=malloc(sizeof(ccsAction));
                a->type=0;
                a->label=strdup(label);

                if (tmp->action==NULL)
                    tmp->action=a;
                else
                {
                   tmp->next=malloc(sizeof(ccsActions));
                   tmp=tmp->next;
                   tmp->next=NULL;
                   tmp->seperator=0;
                   tmp->action=a;
                }
            }
        }
        c=frGetchar(fp);
        while (c==' ')
            c=frGetchar(fp);

       

        }
        if (c==';')
            while(c!='\n' && c!=EOF)
                c=frGetchar(fp);

/****/
    i=0;
    while (c!='\n' && c!=EOF)
    {
        printf("%c",c);
        c=frGetchar(fp);
        i=1;
    }
    if (i) printf("\n");
/****/
        return ret;
    }    
}

ccsAction *ccsParseAction(fileReader *fp)
{
    ccsAction *ret=malloc(sizeof(ccsAction));
    char *label=malloc(sizeof(char)*MAXX_LABEL);
    int i,c=0;

    for (i=0; i<MAXX_LABEL; i++)
        label[i]=0;

    ret->type=1;

    c=frGetchar(fp);
    if (c=='(')
    {
        i=0;
        c=frGetchar(fp);
        if (c=='_')
        {
            ret->type=2;
            c=frGetchar(fp);
        }
        while (c!=' ' && c!=',' && c!='\n' && c!=')' && c!=EOF)
        {
            label[i++]=c;
            c=frGetchar(fp);
        }
        ret->label=strdup(label);
        while (c==' ')
            c=frGetchar(fp);

        if (c=='\n' || c==EOF)
            exit(5);
        ret->label=strdup(label);
        free(label);
        return ret;
    }
    else return NULL;
}

ccsComposition *ccsParseComposition(fileReader *fp)
{
    ccsComposition *ret=malloc(sizeof(ccsComposition));
    ccsComposition *tmp=ret;
    char *label=malloc(sizeof(char)*MAXX_LABEL);
    int i=0;
    int c=0;
 
    ret->ce=NULL;
    ret->seperator=0;
    ret->next=NULL;

    for (i=0; i<MAXX_LABEL; i++)
        label[i]=0;

    c=frGetchar(fp);

    while (c=='\n' || c==' ')
        c=frGetchar(fp);

    i=0;
    while (c!=' ' && c!='|' && c!='<' && c!='\n' && c!=EOF)
    {
        label[i++]=c;
        c=frGetchar(fp);
    }
    tmp->seperator=0;
    tmp->ce=malloc(sizeof(ccsCElement));
    tmp->ce->label=strdup(label);

    while (c==' ')
        c=frGetchar(fp);

    while (c=='|') 
    {
        if (lookahead(fp)=='|')
        {
            tmp->seperator=1;
            c=frGetchar(fp);
            c=frGetchar(fp);
            while (c==' ')
                c=frGetchar(fp);
            i=0;
            while (c!=' ' && c!='|' && c!='<' && c!='\n' && c!=EOF) 
            {
                label[i++]=c;
                c=frGetchar(fp);
            }
            label[i]=0;
            tmp->next=malloc(sizeof(ccsComposition));
            tmp=tmp->next;
            tmp->seperator=0;
            tmp->next=NULL;
            tmp->ce=malloc(sizeof(ccsCElement));
            tmp->ce->label=strdup(label);
            while (c==' ')
                c=frGetchar(fp);
        }
    }

    return ret;
}

void ccsPrintParseTree(ccsParseTree *pt)
{
    ccsPrintAgents(pt->agents);
    ccsPrintComposition(pt->composition);
    return;
}

void ccsPrintAgents(ccsAgents *as)
{
    ccsAgents *tmp=as;
    printf("----| Agents|--------------------------------\n");
    while (tmp!=NULL)
    {
        printf("%10s = ",tmp->agent->label);
        ccsPrintActions(tmp->agent->actions);
        tmp=tmp->next;
    }
}

void ccsPrintActions(ccsActions *as)
{
    ccsActions *tmp=as;
    while (tmp!=NULL)
    {
        if (tmp->action->type>=1)
        {
            printf("(%s%s",(tmp->action->type==2)?"~":"",tmp->action->label);
            printf(")");
        }
        if (tmp->action->type==0)
        {
            printf("%s",tmp->action->label);
        }
        if (tmp->next!=NULL && tmp->seperator!=0)
            printf("%c",tmp->seperator);
        tmp=tmp->next;
    }
    printf("\n");
}

void ccsPrintComposition(ccsComposition *c)
{
    ccsComposition *tmp=c;
    printf("----| Comp. |--------------------------------\n       ");
    while (tmp!=NULL)
    {
        printf("%s",tmp->ce->label);
        if (tmp->next!=NULL)
            printf(" %s ",(tmp->seperator==1)?"||":"|unknown|");
        tmp=tmp->next;
    }
    printf("\n");
}

