/* Partial Concordance Program */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

typedef struct Word {
	char		string[40];
	int		wordcount;
	int		linenum;
} Word;

Word Wordarray[2000];

Word Finalarray[20];

static	int	curline;

void main(void)
{
	int count,counter,next,check,poscheck;
	char filename[40];
	char word[40],tempstring[40];
	int letter,exist,templinenum,tempwordcount;
	FILE *fp;
	
	printf("\n\n\n\n\n   Welcome to the Partial Concordance program.\n");
	printf("\n Please input name of file you wish to read: ");
	scanf("%s",&filename);
	getchar();
	if ((fp=fopen(filename, "r")) == NULL)	/*Open the file named*/
		printf("/n The file was not found!");
	
	next=0;	
	count=0;
	curline=1;
	printf("%d ", curline); /*Print the first line number*/
	letter=fgetc(fp);	/*Get first letter from the file*/
	
	while (letter!=EOF)	/*Loop to print file and take info*/
	{
		printf("%c",letter);	/*Print the charactor to screen*/
		if (letter == '\n')	/*Line number loop*/
		{
			curline++;		/*Adjust line number*/
			printf("%3d ", curline); /*Print the current line*/
		}
		
		if (isalpha(tolower(letter)) || letter=='-')
			word[count++]=letter;	/*Add letter to word*/

		else 
		{
			if (count>0)
				word[count]='\0';  /*Put end charactor on word*/	
			count=0;
			exist = 0;
			for (counter=0; counter < next; counter++)
			{
			if (strcmp(Wordarray[counter].string,word)==0) /*Look for word*/
				{
					exist = 1;	/*Set exist var*/
					Wordarray[counter].linenum=curline;
					Wordarray[counter].wordcount++;    /*Change stats*/
				}
			}
			if (exist == 0) /*If word doesn't exist make a new word*/
			{
				strcpy(Wordarray[next].string,word);
				Wordarray[next].linenum=curline;
				Wordarray[next].wordcount++;
				next++;
			}
		}
		letter = fgetc(fp); /*Get next letter*/
	}	
	fclose(fp);					/*Close the file*/
	printf("\n\n     The file has finished!\n");
	printf("\n     Press a key for Partial Concordance.\n");
	getchar();				/*Wait for user to press a key*/
	
	for (count=0; count <20; count++)	/*Loops for finding most used words*/
	{
		check=0;
		for (counter=0; counter < next; counter++)
		{
			if (Wordarray[counter].wordcount>check) /*If word is used more..*/
			{
				check=Wordarray[counter].wordcount;
				poscheck=counter;		/*save details about the word*/
			}
		}
		strcpy(Finalarray[count].string,Wordarray[poscheck].string);
		Finalarray[count].linenum=Wordarray[poscheck].linenum; /*Copy word to
Finalarray*/
		Finalarray[count].wordcount=Wordarray[poscheck].wordcount;
		
		Wordarray[poscheck].wordcount=0;
		/*Stop word from being found again*/
	}
	
	for (count=0; count <20; count++)  /* --- Sorting loops --- */
	{
		for (counter=0; counter <20; counter++)
		{
			if (count!=counter)	/*If they are not the same word..*/
			{	
				/*check which order they should be in and swap if need be.*/
				if (strcmp(Finalarray[count].string,Finalarray[counter].string)<0)
				{
					strcpy(tempstring,Finalarray[count].string);
					templinenum=Finalarray[count].linenum;
					tempwordcount=Finalarray[count].wordcount;
					
					strcpy(Finalarray[count].string,Finalarray[counter].string);
					Finalarray[count].linenum=Finalarray[counter].linenum;
					Finalarray[count].wordcount=Finalarray[counter].wordcount;
					
					strcpy(Finalarray[counter].string,tempstring);
					Finalarray[counter].linenum=templinenum;
					Finalarray[counter].wordcount=tempwordcount;
				}
			}
		}
	}
	
	/*Print out concordance*/
	for (counter=0; counter <20; counter++)
	{
		printf("WORD:%15s ", Finalarray[counter].string);
		printf(" LAST LINE NO: %i	", Finalarray[counter].linenum);
		printf("AMOUNT USED: %i.\n", Finalarray[counter].wordcount);
	}
	
	
}
