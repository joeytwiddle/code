// To be done:
// Find dates split over adjacent lines
// Find patterned dates
// Ignore dates beginning a reply section and dates in forwarded email headers

#include <joeylib.c>

#include <email.c>

List<String> months;
List<String> days;

class Date {
public:
  // How to decide century for a two-digit year
  #define bottomyear 95
  #define dateseps "/.- "

  int d,m,y;
  Date() {
    d=-1; m=-1; y=-1;
  }
  Date(int dd,int mm,int yy) {
    d=dd;
    m=mm;
    y=yy;
    standardise();
  }
  void standardise() {
    // Give it a century if it has none
    if (y<100) {
      if (m>bottomyear)
        y=1900+y;
      else
        y=2000+y;
    }
  }
  String appendS(int j) {
    String s=Sformat("%i",j);
    int i=intmod(j,10);
    if (i==1)
      return Sconc(s,"st");
    if (i==2)
      return Sconc(s,"nd");
    if (i==3)
      return Sconc(s,"rd");
    return Sconc(s,"th");
  }
  String toString() {
    String s="";
    if (d>-1)
      s=Sconc(appendS(d)," ");
    if (m>-1)
      s=Sconc(s,Sconc(months.num(m)," "));
    if (y>-1)
      s=Sconc(s,Sformat("%i",y));
    return rtsS(s);
  }
};

void setupdates() {
  months.add("January");
  months.add("February");
  months.add("March");
  months.add("April");
  months.add("May");
  months.add("June");
  months.add("July");
  months.add("August");
  months.add("September");
  months.add("October");
  months.add("November");
  months.add("December");
  days.add("Monday");
  days.add("Tuesday");
  days.add("Wednesday");
  days.add("Thursday");
  days.add("Friday");
  days.add("Saturday");
  days.add("Sunday");
}

// Examples dates:
// Wednesday 3rd August
// 2 August 03
// 13/2/99
// 4/Aug/97
// 13.2.99
// 2/13/99 Must be American since month>12
// 1 Jan 2000

// Dates in email header:
// ... Tue Jul 13 12:29:07 1999 (end of first line)
// Date: Tue, 13 Jul 1999 12:59:56 +0100 (BST) (second line)

// Require context: (from email/system clock)
// 3rd August
// next Monday
// last Monday
// on Monday
// next week
// next month

// Not dates:
// 1.2
// 1/2
// 1 2 99 ?
// 16:28:05

int whichmonth(String s) {
  for (int i=1;i<=12;i++) {
    if (Sequiv(s,months.num(i)) || Sequiv(s,Sleft(months.num(i),3))) {
      return i;
    }
  }
  return 0;
}

bool isamonth(String s) {
  return (whichmonth(s)>0);
}

Date *finddate(String s) {
//  printf("%s\n",s);
  // Turn into words, extracting only alphanumerics
  List<String> ws=Ssplit(s,Salphanumeric());
//  for (int i=1;i<=ws.len;i++)
  //  printf("%s,",ws.num(i));
  // First, look for a month
  // Find month
  int f=ws.satisfy(&isamonth);
  if (f>0) {
    // Find adjacent numbers
    int side=-1;
    List<int> nums;
    for (int side=-1;side<=1;side+=2) // Search left side first
    for (int d=1;d<=2;d++) { // Search two words each side
      int c=f+side*d;
      // Collect max 2 numbers
      if (c>0 && c<=ws.len && nums.len<2) {
        int x;
        if (sscanf(ws.num(c),"%i",&x)>0)
          nums.add(x);
      }
    }
    Date *d=new Date();
    d->m=whichmonth(ws.num(f));
    if (nums.len>=1) {
      // Assume first number found is the day
      int p=nums.num(1);
      if (p>=1 && p<=31)
        d->d=p;
      else
        d->y=p;
    }
    if (nums.len>=2) {
      int p=nums.num(2);
      // If year not yet found (second number depends on first)
      if (d->y==-1)
        d->y=p;
      else
        if (p>=1 && p<=31)
          d->d=p;
    }
    d->standardise();
    return d;
  }
  // Now try looking for a pattern ../../..
  return NULL;
}

bool startsdate(String s) {
  return Seq(Sleft(s,6),"Date: ");
}

void main(int argc,String *argv) {
  
  setupdates();

  ArgParser a=ArgParser(argc,argv);
  String fname=a.getarg(1);
  a.done();
  
  List<String> ls=readlinesfromfile(fname);
  StringParser sp=StringParser(ls.num(1));
  sp.expect("From ");
  String from=sp.getbefore(" ");
  int dl=ls.satisfy(&startsdate,2);
  if (dl==0)
    error("No Date: line found");
  Date *date=finddate(ls.num(dl));
  printf("\n%s written %s\n",fname,date->toString());
  // Find end of header
  for (int i=ls.satisfy(&Sempty,3);i<=ls.len;i++) {
    Date *indate=finddate(ls.num(i));
    if (indate!=NULL) {
//      printf("Pertains to  %s\n",indate->toString());
      printf("%s\n",ls.num(i));
    }
  }

}
