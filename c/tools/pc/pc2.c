// Includes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Definitions
#define EmptyString (String)calloc(1,256)

// Types
typedef char* String;

int stringsequal(String a,String b) {
	int i=0;
	while (a[i]==b[i]) {
		if (a[i]=='\0')
			return 1;
		i++;
	}
	return 0;
}

String strconc(String a,String b) {
	String n=EmptyString;
	int i=0;	
	for (int j=1;j<=strlen(a);j++) {
		n[i]=a[j-1];
		i++;
	}
	for (int j=1;j<=strlen(b);j++) {
		n[i]=b[j-1];
		i++;
	}
	n[i]='\0';
	return n;
}

typedef struct {
	int pos,row,col,word;
} Ref;

typedef struct {
	char* word;
	int occs;
	Ref ref[1000];
} Word;

/*
typedef struct {
	List l;
} ListNode;
typedef struct {
	void* o;
	ListNode l;
} List;
*/

// Early declarations
String getword();
Word* getrecordforword(String s);
void newrecord(String s);
void addoccurrencefor(Word* w);
void makepartconc(String fname);
void findclusters(String fname,int numwords,char** words);
int clustscore(Ref r,int numwords,char **words);

FILE* f;
//Word word[1000];
Word *word;
int words,pos,row,col,wor;
int wpos,wrow,wcol;

// pc <filename> <word>s
// makes partial concordance for file
// searches for word clusters, and displays
void main(int argc,char** argv) {
/*	for (int i=1;i<=argc;i++) {
		printf("Argument %d was >%s<\n",i,argv[i-1]);
	}*/
	if (argc<2) {
		printf("pc <filename> <word>s\n");
		exit(0);
	}
	String fname=argv[1];
  word=new Word[1000];
	makepartconc(fname);
	if (argc>2)
		findclusters(fname,argc-2,&argv[2]);
}

void makepartconc(String fname) {

	// Read in data
	f=fopen(fname,"r");
	words=0;pos=0;row=0;col=0,wor=0;
	int mostoccs=0;
	String mostword;
	String s=getword();
	while (s[0]!='\0') {	
		//printf(">%s<\n",s);
		Word* w=getrecordforword(s);
		if (w==NULL)
			newrecord(s);
		else {
			addoccurrencefor(w);
			if ((w->occs)>mostoccs) {
				mostoccs=w->occs;
				mostword=s;
			}
		}
		s=getword();
	}

/*	// Display results
	for (int i=1;i<=words;i++) {
		Word w=word[i-1];
		printf("%s occurred:\n",w.word);
		for (int j=1;j<=w.occs;j++) {
			Ref r=w.ref[j-1];
			printf("  Pos %d Row %d Col %d\n",r.pos,r.row,r.col);
		}
	}*/

	printf("Most frequent word was %s occurring %d times\n",mostword,mostoccs);

	// Output partial concordance
// Version 2
	String oname=strconc(".",strconc(fname,".jpc"));
	FILE* g=fopen(oname,"w");
	fprintf(g,"Joey's Partial concordance of file Ver 2 (Dec 98)\n");
	fprintf(g,"filename %s\n",fname);
	fprintf(g,"size %d\n",pos);
	fprintf(g,"number of words %d\n",words);
	for (int i=1;i<=words;i++) {
		Word w=word[i-1];
		fprintf(g,"%s %d\n",w.word,w.occs);
		for (int j=1;j<=w.occs;j++) {
			Ref r=w.ref[j-1];
			fputc(r.pos,g);
			fputc(r.row,g);
			fputc(r.col,g);
			fputc(r.word,g);
			//fprintf(g,"P %d R %d C %d W  %d\n",r.pos,r.row,r.col,r.word);
		}
		fprintf(g,"\n");
	}
	fprintf(g,"END\n");
/* Version 1
	String oname=strconc(".",strconc(fname,".jpc"));
	FILE* g=fopen(oname,"w");
	fprintf(g,"Joey's Partial concordance of file Ver 1 (Dec 98)\n");
	fprintf(g,"filename %s\n",fname);
	fprintf(g,"size %d\n",pos);
	fprintf(g,"number of words %d\n",words);
	for (int i=1;i<=words;i++) {
		Word w=word[i-1];
		fprintf(g,"word %s %d\n",w.word,w.occs);
		for (int j=1;j<=w.occs;j++) {
			Ref r=w.ref[j-1];
			fprintf(g,"P %d R %d C %d W %d\n",r.pos,r.row,r.col,r.word);
		}
	}
	fprintf(g,"END\n");*/
			
}

Word* getrecordforword(String s) {
	for (int i=1;i<=words;i++) {
		if (stringsequal(word[i-1].word,s))
			return &word[i-1];
	}
	return NULL;
}

void newrecord(String s) {
	words++;
	word[words-1].word=s;
	word[words-1].occs=0;
	addoccurrencefor(&word[words-1]);
}

void addoccurrencefor(Word* w) {
	w->occs++;
	w->ref[w->occs-1].pos=wpos;
	w->ref[w->occs-1].row=wrow;
	w->ref[w->occs-1].col=wcol;
	w->ref[w->occs-1].word=wor;
}

int alphanum(char c) {
	if (c>='1' && c<='9')
		return 1;
	if (c=='0')
		return 1;
	if (c>='a' && c<='z')
		return 1;
	if (c>='A' && c<='Z')
		return 1;
	return 0;
}

String getword() {
	String s=EmptyString;
	int i=0;
	char c;
	wor++;
	while (i==0) {
		wpos=pos;wrow=row;wcol=col;
		c=(char)fgetc(f);
		pos++;
		col++;
		if (c=='\n') {
			row++;
			col=0;
		}
		if (c==EOF)
			break;
		while (alphanum(c)) {
			s[i]=c;
			i++;
			c=(char)fgetc(f);
			pos++; col++;
			if (c=='\n') {
				row++;
				col=0;
			}
			if (c==EOF)
				break;
		}
	}
	s[i]='\0';
	return s;
}

Word getwordrecord(String w) {
	for (int i=1;i<=words;i++) {
		if (!strcmp(w,word[i-1].word))
			return word[i-1];
	}
}

void findclusters(String fname,int numwords,char** words) {
	Ref found[10];
	int score[10];
	int numfound=0;
	Word w=getwordrecord(words[0]);
	for (int i=1;i<=w.occs;i++) {
		//printf("P %d:\n",w.ref[i-1].pos);
		int s=clustscore(w.ref[i-1],numwords-1,&words[1]);
		//printf("end P %d score %d\n",w.ref[i-1].pos,s);
//printf("numfound %d\n",numfound);
		if (numfound==0) {
			found[0]=w.ref[i-1];
			numfound++;
		} else {
			//printf("Putting in...\n");
			for (int j=1;j<=numfound;j++) {
				if (s>score[j-1]) {
					// Shift j-1 to numfound-1 up one
					for (int k=numfound;k>=j;k--) {
						if (k<10) {
							found[k]=found[k-1];
							score[k]=score[k-1];
						}
					}
					found[j-1]=w.ref[i-1];
					score[j-1]=s;
					numfound++;
					break;
				}
			}
			//printf("bananas\n");		
		}
	}
	// Display results
	for (int i=1;i<=10;i++) {
		printf("Try row %d col %d (score %d)\n",found[i-1].row,found[i-1].col,score[i-1]);
	}
}

int clustscore(Ref r,int numwords,char **words) {
	if (numwords==0)
		return 1;
	printf("Checking score for %s against P %d\n",words[0],r.pos);
	Word w=getwordrecord(words[0]);
	int bestscore=0;
	for (int i=1;i<=w.occs;i++) {
		int dist=abs(w.ref[i-1].pos-r.pos);
		int rest=clustscore(w.ref[i-1],numwords-1,&words[1]);
		int s=rest*80/dist;
		if (s>bestscore)
			bestscore=s;
	}
	printf("Best score:%d\n",bestscore);
	return bestscore;
}

/*
typedef struct {
	String wanted[10];
	int score(Ref a) {
		
	WordRef[] best() {
		WordRef[10]  best;
		int[10] score;		
		WordRef[] as=findrefsto(wanted[depth]);
		return best;
	}
	findrefsto(String a) {
		WordRef[] b;
		return b;
	}		
} PartConcCluster;
*/
