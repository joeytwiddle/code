/* Partial Concordance
   Paul Clark PC5065   */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define TRUE 1
#define FALSE 0
#define CR 13

typedef struct WordNode {
	char		ch;
	struct WordNode	*next;
	} WordNode;
typedef struct WordNode *Word;
typedef int Bool;
typedef struct RetRec {
	Word		bef;
	Word		w;
	Word		aft;
	int		result;
	} RetRec;
typedef int LineNo;
typedef int Count;
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
typedef WRec Result[];

LineNo l;
int x;
FILE* fp;

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
	if (a==NULL) {
		wprint(a);  printf(" less than ");  wprint(b);  printf("\n");
		return TRUE;
		}
	else {
		if (a->ch <= b->ch) {
			wprint(a);  printf(" less than ");  wprint(b);  printf("\n");
			return TRUE;
			}
		else {
			wprint(a);  printf(" not less than ");  wprint(b);  printf("\n");
			return FALSE;
			}
		}
	}

Bool morethan(Word a, Word b) {
	/* Returns true if a is less than b. */
	if (a==NULL) {
		return FALSE;
		}
	else {
		if (a->ch >= b->ch) {
			return TRUE;
			}
		else {
			return FALSE;
			}
		}
	}

Tree addocct(Word w, LineNo l, Tree t) {
	Tree new;
	if (t==NULL) {
		printf("New tree/node for ");
		wprint(w);
		printf("   Lineno: %i\n",l);
		t=(Tree)tnalloc();
		t->record=(WRec*)wralloc();
		t->record->word=w;
		t->record->list=ladd(lnew(),l);
		t->record->count=1;
		return t;
		}
	else {
		if (lessthan(w,t->record->word)==1) {
			t->less=addocct(w,l,t->less);
			return t;
			}
		else {
			if (morethan(w,t->record->word)) {
				t->more=addocct(w,l,t->more);
				return t;
				}
			else {
				printf("Node updated for ");
				wprint(w);
				printf("   Lineno: %i\n",l);
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
	printf("Going to array for %c, number ",whead(w));
	printf("%i\n",i);
	a[i]=addocct(w,l,a[i]);
	}

RetRec nextword(FILE* fp) {
	Word bef;
	Word w;
	Word aft;
	RetRec r;
	int result;
	char c;
	result=0; /* End of file not reached */
	bef=wnew();
	while (!isalpha(c) && c!=EOF) {
		c=getc(fp); /*printf("%c",c);*/
		if (c==EOF) {
			printf("##EOF##");
			result=1; /* End of file reached before word found */
			}
		if (!isalpha(c)) {
			bef=wadd(bef,c);
			}
		}
	w=wadd(wnew(),c);
	while (isalpha(c) && c!=EOF) {
		c=getc(fp); /*printf("%c",c);*/
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
	r.result=result;
	printf("Word read: ");
	wprint(w);
	printf("   Result: %i",result);
	printf("   EOF: %i",EOF);
	printf("   Last char no: %i\n",c);
	return r;
	}

main()
{
	int i;
	Tree a[26];
	RetRec r;
	
	FILE * fp = fopen("problem1.data","r");

	l=1;
	x=0;
	
	for (i=0; i<26; i++) {
		printf("New tree no %i\n",i);
		a[i]=tnew();
		}
	
	while (r.result==0) {
		printf("\n");
		r=nextword(fp);
		/*printf("%i",r.w->ch);*/
	
		/*sprint(r.bef);*/
	
		if (r.result!=1) {
			addocca(r.w,l,a);
			/*sprint(r.w);
			sprint(r.aft);*/
			}
	
	}
	
	/*w=wnew();
	
	printf("Got this far 1%c\n",c);
	
	while (c!=EOF) {
		c=getc(fp);
		if (c!=EOF) {
			printf("%c",c);
			}
		if isalpha(c) {
			w=wadd(w,c);
			}
		}
	
	printf("\n");
	
	wprint(w);
	printf("\n");
	
	addocca(w,5,a);
	addocca(w,6,a);*/
	
	fclose(fp);
	
}
