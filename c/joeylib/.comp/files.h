#ifndef files_H
  #define files_H

  #include <globals.h>
  #include <list.h>

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

extern int numfileswritten; // Variable initialised in .c file

int getnextfilenum(); // Method


String getnextfilename(); // Method


String getnextfilename(String ext); // Method


String getnextfilename(String pre,String ext); // Method


void unfuckfile(String fname,String oname); // Method
    

int close(FILE *fp); // Method


List<String> readlinesfromfile(String fname,String seperators,boolean trim); // Method


List<String> readlinesfromfile(String fname,String seperators); // Method



List<String> mail2list(String fname); // Method


List<String> readlinesfromfile(String fname); // Method


/* Inefficient due to Sconc
List<String> readlinesfromfile(String fname) {
  // printf("Opening %s\n",fname);
  #ifdef DEBUG
    printf("Reading lines from file...\n");
  #endif
  FILE *fp=fopen(fname,"r");
  if (fp==NULL)
    error("Problem reading %s\n",fname);
  List<String> ls;
  char c;
  String s="";
  int i;
  while ((i=getc(fp))!=EOF) {
    c=(char)i;
    #ifdef DEBUG
      printf("*");
    #endif
    if (c=='\n') {
      ls.add(s);
      // printf("* %s\n",s);
      s="";
    } else
    s=Sconc(s,chtoString(c));
  }
  ls.add(s);
  fclose(fp);
  #ifdef DEBUG
    printf("\n\n");
  #endif
  return ls;
}*/

void writelinestofile(List<String> ls,String fname,String sep); // Method


void writelinestofile(List<String> ls,String fname); // Method


void writelinestofile(String fname,List<String> ls); // Method


void writelinestofile(List<String> *ls,String fname); // Method


void writelinestofile(String fname,List<String> *ls); // Method


String getlinefromfile(FILE *fp); // Method


extern bool JFilesRecursive; // Variable initialised in .c file

bool islink(String fname); // Method


bool isdir(String fname); // Method


#ifdef DJGPP

List<String> getfilesrecursive(String dn); // Method


List<String> getfilesindir(String dn); // Method


#ifndef DOS
List<String> getfiles(String dn,String format); // Method


List<String> getfiles(String p); // Method

#endif

int getfilesize(String fname); // Method


class FileRec {
public:
  String fname; // Exists
  int size; // Exists
   FileRec(); // Method

   FileRec(String f,int s); // Method

  virtual void display(); // Method

  virtual String type(); // Method

};

float getsize(FileRec *r); // Method


class DirRec : public FileRec {
public:
  List<FileRec *> fs; // Exists
   DirRec(); // Method

   DirRec(String dir); // Method

  void display(); // Method

  void displayratio(); // Method

  virtual String type(); // Method

};

#endif

void removefile(String f); // Method


String wholepath(); // Method


List<char> *readbinaryfile(String fname); // Method


void writebinaryfile(String fname,List<char> *c); // Method


void deletefile(String fname); // Method


void copyfile(String fname,String tname); // Method


/* Usage:
bool zipped=unzipfile(fname);
...
if (zipped)
  zipfile(fname);
*/

bool unzipfile(String s); // Method


void zipfile(String s); // Method


bool fileexists(String fname); // Method


/*
List<String> filesmatching(String s) {
  String dir,pat;
  if (Sinstrlast(s,"/")>0) {
    dir=Sbeforelast(s,"/");
    pat=Safterlast(s,"/");
  } else {
    dir=".";
    pat=s;
  }
  DirRec dr=DirRec(dir);

*/

void writeinttofile(String fname, int i); // Method


int readintfromfile(String fname); // Method


void writefloattofile(String fname, float i); // Method


float readfloatfromfile(String fname); // Method


#endif
