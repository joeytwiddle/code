/* A simple sort of diff program.
 * I have found it incredibly useful!
 * However, it is not optimised :-P
 * It should really use some efficient structure to perform a quick
 * check if a particular string exists in the other file.
 * At the moment it just searches a big list!
 */

#define QUIET

#include <joeylib.h>

bool simple,yesno,silent,difference=false;
bool common,nolines,dodiff,showpercentage;
String diffstringa;
String diffstringb;
int cntainb,cntbina,cntina,cntinb;

List<String> compare(List<String> als,List<String> bls,String aname,String bname) {
  List<String> diff;
  if (!yesno) {
    if (!simple) {
      if (!silent) {
        if (!common) {
          printf("Lines in %s not present in %s:\n\n",aname,bname);
        } else {
          printf("Lines present in both %s and %s:\n\n",aname,bname);
        }
      }
    }
  }
  for (int i=1;i<=als.len;i++) {
    String al=als.num(i);
    bool found=false;
    for (int j=1;j<=bls.len && !found;j++) {
      String bl=bls.num(j);
      found=Ssimilar(al,bl);
    }
		cntina++;
		if (found) {
			cntainb++;
		}
    if (dodiff) {
      if (found)
        diff.add(al);
      else
        diff.add(Sconc(diffstringa,al,diffstringb));
    }
    if (!found && !common) {
      difference=true;
      if (!yesno && !silent) {
        if (!simple && !nolines)
          printf("%i : ",i);
        printf("%s\n",al);
      }
    } else if (found && common && !silent) {
      printf("%s\n",al);
    }
  }
//  printf("\n");
  return diff;
}

int main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  dodiff=a.argexists("diff","Output .diff files");
  diffstringa=a.argafter("-ds","Diff string","@@>> ");
  diffstringb=a.argafter("-dsf","Diff string on far side","");
  simple=a.argexists("simple","Simple output for pasting into files");
  common=a.argexists("common","Show lines common to both files");
  bool oneway=(common || a.argexists("oneway","Only compare in one direction"));
  bool remred=a.argexists("remred","Remove first file if redundant");
  nolines=a.argexists("nolines","Don't show line numbers");
  String separators=a.argafter("-s","Separators","\\n");
  separators=Sreplaceall(separators,"\\n","\n");
  silent=a.argexists("silent","No output except err code: 0=equivalent 1=different");
  yesno=a.argexists("yesno","Just print whether they are the same or not");
  showpercentage=a.argexists("age","Show what %age of the lines in A appear in B");
  String aname=a.getarg("file");
  String bname=a.getarg("file");
  a.done();

  List<String> als=readlinesfromfile(aname,separators,true);
  List<String> bls=readlinesfromfile(bname,separators,true);

	cntainb=cntbina=cntina=cntinb=0;

  List<String> diff=compare(als,bls,aname,bname);
  if (difference && dodiff)
    writelinestofile(diff,Sformat("%s.diff",aname));
  if (remred && !difference) {
    if (!silent)
      printf("%s redundant and removed.\n",aname);
    removefile(aname);
    exit(0);
    // Don't remove this or you might delete both redundant files!
    // Actually that's probably bollocks
  }

  if (showpercentage) {
	printf("# A's lines in B = %i / %i\n",cntainb,cntina);
	printf("%%age A in B = %.2f\n",100.0*(float)cntainb/(float)cntina);
  }

  if (!oneway) {
    if (!simple && !silent)
      printf("\n");
    diff=compare(bls,als,bname,aname);
    if (difference && dodiff)
      writelinestofile(diff,Sformat("%s.diff",bname));
  }

  if (difference) {
    if (yesno && !silent)
      printf("Different.\n");
    // exit(1);
    // didn't work
    return 1;
  } else {
    if (yesno && !silent)
      printf("The same.\n");
  }

  return 0;

}
