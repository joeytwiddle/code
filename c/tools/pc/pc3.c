/* Partial Concordance
   Paul Clark PC5065   */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define TRUE 1
#define FALSE 0
#define CR 10

typedef int Bool;
typedef int LineNo;
typedef int Count;
typedef struct WordNode {
	char		ch;
	struct WordNode	*next;
	} WordNode;
typedef struct WordNode *Word;
typedef struct RetRec {
	Word		bef;
	Word		w;
	Word		aft;
	LineNo		ln;
	int		result;
	} RetRec;
typedef struct ListNode {
	LineNo		lineno;
	struct ListNode	*next;
	} ListNode;
typedef struct ListNode *List;
typedef struct WRec {
	Word		word;
	List		list;
	Count		count;
	} WRec;
typedef struct TreeNode {
	WRec		*record;
	struct TreeNode	*less;
	struct TreeNode	*more;
	} TreeNode;
typedef struct TreeNode *Tree;

typedef Tree Array[];
LineNo l;
int x;
FILE* fp;
WRec results[20];

struct TreeNode *tnalloc(void) {
	return (struct TreeNode*)malloc(sizeof(struct TreeNode*));
	}

struct ListNode *lnalloc(void) {
	return (struct ListNode*)malloc(sizeof(struct ListNode*));
	}

struct WordNode *wnalloc(void) {
	return (struct WordNode*)malloc(sizeof(struct WordNode*));
	}

struct WRec *wralloc(void) {
	return (struct WRec*)malloc(sizeof(struct WRecmama*));
	}

Tree tnew() {
	return NULL;
	}

List lnew() {
	return NULL;
	}

List ladd(List l, LineNo n) {
	List new;
	/* If dealing with last pointer in list, allocate some space for the next node and point to it. */
	if (l==NULL) {
		l=(List)lnalloc();
		l->lineno=n;
		l->next=NULL;
		return l;
		}
	/* If not dealing with last pointer, recurse ladd to next node. */
	else {
		new=ladd(l->next,n);
		l->next=new;
		return l;
		}
	}

Word wnew() {
	return NULL;
	}

Word wadd(Word w, char c) {
	Word new;
	/* First, convert to lower case if not already. */
	if (c>=65 && c<=90) {
		c=c+32;
		}
	/* Then, if dealing with last pointer in list, allocate some space for the next node and point to it. */
	if (w==NULL) {
		w=(Word)wnalloc();
		w->ch=c;
		w->next=NULL;
		return w;
		}
	/* If not dealing with last pointer, recurse wadd to next node. */
	else {
		new=wadd(w->next,c);
		w->next=new;
		return w;
		}
	}

wprint(Word w) {
	if (w!=NULL) {
		printf("%c",w->ch);
		wprint(w->next);
		}
	}

sprint(Word w) {
	/* The same as wprint, but changes line no accordingly */
	if (w!=NULL) {
		printf("%c",w->ch);
		/*printf("[%i]",w->ch);*/
		x++;
		if ((x==40) || (w->ch==CR)) {
			x==0; l++;
			if (w->ch != CR) {
				printf("\n");
				}
			printf("%i - ",l);
			}
		sprint(w->next);
		}
	}

listing(char c) {
	if (c==EOF) {
		printf("<END>\n");
		}
	else {
		printf("%c",c);
		x++;
		if ((x==40) || (c==CR)) {
			x==0;  l++;
			if (c!=CR) {
				printf("_");  /* This prints before Bus Error. */
				printf("\n");  /* Crash occurs. */
				printf("*");  /* This does not print. */
				}
			printf("%i - ",l);
			}
		printf("/");
		}
	}

char whead(Word w) {
	if (w != NULL) {
		return w->ch;
		}
	else {
		return NULL;
		}
	}

Bool lessthan(Word a, Word b) {
	/* Returns true if a is less than b. */
	if (a==NULL && b==NULL) return FALSE;
	if (a==NULL && !b==NULL) return TRUE;
	if (!a==NULL && b==NULL) return FALSE;
	if (!a==NULL && !b==NULL) {
		if (a->ch < b->ch) return TRUE;
		if (a->ch > b->ch) return FALSE;
		if (a->ch == b->ch) return lessthan(a->next,b->next);
		}
	}

Bool morethan(Word a, Word b) {
	/* Returns true if a is greater than b. */
	if (a==NULL && b==NULL) return FALSE;
	if (a==NULL && !b==NULL) return FALSE;
	if (!a==NULL && b==NULL) return TRUE;
	if (!a==NULL && !b==NULL) {
		if (a->ch < b->ch) return FALSE;
		if (a->ch > b->ch) return TRUE;
		if (a->ch == b->ch) return morethan(a->next,b->next);
		}
	}

Tree addocct(Word w, LineNo l, Tree t) {
	printf("<");  wprint(w);  printf(">");
	if (t==NULL) {
		/*printf("New tree/node for ");
		wprint(w);
		printf("   Lineno: %i\n",l);*/
		t=(Tree)tnalloc();
		t->less=tnew();
		t->more=tnew();
		t->record=(WRec*)wralloc();
		t->record->word=w;
		t->record->list=ladd(lnew(),l);
		t->record->count=1;
		/*printf("   Pointer %i\n",t);*/
		return t;
		}
	else {
		/*printf("Okay, deciding where to insert\n");*/
		printf("...");
		if (lessthan(w,t->record->word)==TRUE) {
			printf("<<<");
			wprint(w);
			printf(" is less than ");
			wprint(t->record->word);
			printf(">>>");
			t->less=addocct(w,l,t->less);
			return t;
			}
		else {
			if (morethan(w,t->record->word)==TRUE) {
				printf("<<<");
				wprint(w);
				printf(" is less than ");
				wprint(t->record->word);
				printf(">>>");
				t->more=addocct(w,l,t->more);
				return t;
				}
			else {
				/*printf("Node updated for ");
				wprint(w);
				printf("   Lineno: %i\n",l);*/
				t->record->list=ladd(t->record->list,l);
				t->record->count++;
				return t;
				}
			}
		}
	}
	
addocca(Word w, LineNo l, Array a) {
	int i;
	i=(int)(whead(w))-97;
	/*printf("Going to array for %c, number ",whead(w));
	printf("%i\n",i);*/
	a[i]=addocct(w,l,a[i]);
	/*printf("Array now points to %i\n",a[i]);*/
	}

RetRec nextword(FILE* fp) {
	RetRec r;
	Word bef;
	Word w;
	Word aft;
	LineNo ln;
	int result;
	char c;
	result=0; /* End of file not reached */
	bef=wnew();
	while (!isalpha(c) && c!=EOF) {
		c=getc(fp);  ln=l;  listing(c);
		if (c==EOF) {
			result=1; /* End of file reached before word found */
			}
		if (!isalpha(c)) {
			bef=wadd(bef,c);
			}
		}
	w=wadd(wnew(),c);
	while (isalpha(c) && c!=EOF) {
		c=getc(fp);  listing(c);  printf("");  /* I have no idea why, but without this a BUS error appears. */
		if (c==EOF) {
			result=2; /* End of file reached but word found */
			}
		if (isalpha(c)) {
			w=wadd(w,c);
			}
		}
	aft=wadd(wnew(),c);
	r.bef=bef;
	r.w=w;
	r.aft=aft;
	r.ln=ln;
	r.result=result;
	/*printf("Word read: ");
	wprint(w);
	printf("   Result: %i",result);
	printf("   EOF: %i",EOF);
	printf("   Last char no: %i\n",c);*/
	return r;
	}

searchtree(Tree t) {
	int i;
	int j;
	Bool inserted;
	if (t!=NULL) {
		wprint(t->record->word);
		printf("   %i\n",t->record->count);
		/*searchtree(t->less);*/
		if (t->more!=NULL) { printf("%i",t->more); }
		inserted=FALSE;
		for (i=0; (i<20) && (inserted==FALSE); i++) {
			if ((t->record->count) > (results[i].count)) {
				for (j=i; j<19; j++) {
					results[j+1]=results[j];
					inserted=TRUE;
					}
				results[i].word=t->record->word;
				results[i].list=t->record->list;
				results[i].count=t->record->count;
				}
			}
		}
	}

sort(int i) {
	int j;
	WRec tmp;
	for (j=0; j<=i; j++) {
		if (lessthan(results[j+1].word,results[j].word)) {
			tmp=results[j+1];
			results[j+1]=results[j];
			results[j]=tmp;
			}
		}
	}
			

main()
{
	int i;
	Tree a[26];
	RetRec r;
	char file[20];
	
	/* Open input file. */
	/*printf("Enter filename: ");
	scanf("%s",&file);*/
	fp = fopen("problem1.data","r");

	/* Initialise array of empty trees to store part conc. */
	for (i=0; i<26; i++) {
		a[i]=tnew();
		}
	
	/* Initialise line and column number. */
	l=1;
	x=0;
	printf("%i - ",l);
	
	/* Read words, updating trees as neccessary. */
	while (r.result==0) {
		r=nextword(fp);
		if (r.result!=1) {
			printf("!");
			addocca(r.w,r.ln,a);
			}
	}
	
	/* Find 20 most frequently occuring words. */
	for (i=0; i<20; i++) {
		results[i].word=wnew();
		results[i].list=lnew();
		results[i].count=0;
		}
	for (i=0; i<26; i++) {
		searchtree(a[i]);
		}
	
	/* Sort 20 most frequently occuring words into alphabetical order. */
	for (i=19; i>=0; i--) {
		sort(i);
		}
		
	/* Print 20 most frequently occuring words and their line nos. */
	for (i=0; i<20; i++) {
		/*wprint(results[i].word);
		printf("\n");*/
		}
	
	fclose(fp);
	
}
