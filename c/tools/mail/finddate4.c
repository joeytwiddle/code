// To be done:
// Find dates split over adjacent lines
// Find patterned dates
// Ignore dates:
// in forwarded email headers
// commenting forwarded text ("on D X wrote")
// beginning a reply section
// Don't accept duplicate emails in for one day (most of these are above)

#define QUIET

#include <time.h>

#include <joeylib.c>

List<String> months;
List<String> days;

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
  	String sl=Sleft(months.num(i),3);
    if (Sequiv(s,months.num(i)) || Sequiv(s,sl)) {
    	free(sl);
      return i;
    }
    free(sl);
  }
  return 0;
}

bool isamonth(String s) {
  return (whichmonth(s)>0);
}

class Date {
public:
  // How to decide century for a two-digit year
  #define bottomyear 95
  #define dateseps "/.- "

  int d,m,y,wd;
  Date() {
    d=-1; m=-1; y=-1; wd=-1;
  }
  Date(int dd,int mm,int yy) {
    d=dd;
    m=mm;
    y=yy;
    wd=-1;
    standardise();
  }
  Date(int dd,int mm,int yy,int ww) {
    d=dd;
    m=mm;
    y=yy;
    wd=ww;
    standardise();
  }
  Date(String s) {
	  // Note: this function should update to better code in finddate() later
	  d=-1;
	  m=-1;
	  y=-1;
	  wd=-1;
	  List<String> ws=Ssplit(s,Salphanumeric());
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
	    m=whichmonth(ws.num(f));
  	  if (nums.len>=1) {
    	  // Assume first number found is the day
	      int p=nums.num(1);
  	    if (p>=1 && p<=31)
    	    d=p;
	      else
  	      y=p;
    	}
	    if (nums.len>=2) {
  	    int p=nums.num(2);
    	  // If year not yet found (second number depends on first)
	    	if (y==-1)
  	      y=p;
    	  else
	        if (p>=1 && p<=31)
  	        d=p;
   		}
   	}
   	// Otherwise, look for a pattern ../../..
   	standardise();
	}
  void standardise() {
    // Give it a century if it has none
    if (y==-1)
    	y=2000;
    else if (y<100) {
      if (m>bottomyear)
        y=1900+y;
      else
        y=2000+y;
    }
  }
  String appendS(int j) {
    String s=Sformat("%i",j);
    int i=intmod(j,10);
    if (!(j>=10 && j<=19)) { // 11th, 12th, 13th
    if (i==1)
      return Sconc(s,"st");
    if (i==2)
      return Sconc(s,"nd");
    if (i==3)
      return Sconc(s,"rd");
    }
    return Sconc(s,"th");
  }
  String toString() {
    String s="";
    if (wd>-1)
    	s=Sconc(days.num(wd)," ");
    if (d>-1)
      s=Sconc(s,appendS(d)," ");
    if (m>-1)
      s=Sconc(s,Sconc(months.num(m)," "));
    if (y>-1)
      s=Sconc(s,Sformat("%i",y));
    return rtsS(s);
  }
  Date operator-(int i) {
  	Date nd=Date(*this);
  	for (int j=1;j<=i;j++)
  		nd--;
  	return nd;
  }
  Date operator+(int i) {
  	Date nd=Date(*this);
  	for (int j=1;j<=i;j++)
  		nd++;
  	return nd;
  }
  void operator++() {
  	if (wd>-1) {
  		wd++;
	  	if (wd==8)
		 		wd=1;
		}
  	d++;
  	if (d>30) {
  		d=1;
  		m++;
  		if (m>12) {
  			m=1;
  			y++;
  		}
  	}
  }
  void operator--() {
  	if (wd>-1) {
	  	wd--;
  		if (wd==0)
  			wd=7;
  	}
  	d--;
  	if (d<1) {
  		d=30;
  		m--;
  		if (m<1) {
  			m=12;
  			y--;
  		}
  	}
  }
  bool operator==(Date o) {
//  	printf("Comparing %s to %s\n",toString(),o.toString());
		return hash()==o.hash();
	}
  bool operator>=(Date o) {
		return hash()>=o.hash();
	}
  bool operator<=(Date o) {
		return hash()<=o.hash();
	}
  int hash() {
  	return d+m*32+y*367;
  }
};

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
    nums.freedom();
    ws.freedom();
    return d;
  }
  ws.freedom();
  // Now try looking for a pattern ../../..
  return NULL;
}

bool startsdate(String s) {
  return Seq(Sleft(s,6),"Date: ");
}

#include "email.c"

class DateRec {
public:
	Date d;
	Email *e;
	DateRec(Date dd,Email *ee) {
		d=dd;
		e=ee;
	}
};

List<DateRec> drs;

void main(int argc,String *argv) {
  
  setupdates();

  ArgParser a=ArgParser(argc,argv);
  String fname=a.getarg(1);
  a.done();

//  List<String> ls=mail2list(fname);
  List<String> ls=readlinesfromfile(fname);
	printf("Done\n");
	
  String destdir=Sformat("O%s/",fname);
  system(Sformat("mkdir %s",destdir));
  system(Sformat("rm %s*",destdir));

  time_t t;
  t=time(&t);
  struct tm *tm=localtime(&t);
  Date todaysdate=Date(tm->tm_mday,tm->tm_mon+1,1900+tm->tm_year,tm->tm_wday+1);
  Date stdate=todaysdate-14;
  Date enddate=todaysdate+60;

  int i=1;
  int ecnt=0;
  printf("Parsing emails\n");
  while (i>0 && i<=ls.len) {
  	fprintf(stdout,".");
//  	printf("%i/%i\n",i,ls.len);
    Email *e=new Email();
    int j=e->parsefrom(ls,i);
    ecnt++;
//		printf("Email %i\n",i);
//		writelinestofile(e.whole(),getnextfilename("em","txt"));
		bool used=false;
		for (int k=1;k<=e->content.len;k++) {
		  String cur=e->content.num(k);
		  Date *d=finddate(cur);
			if (d!=NULL) {
				// Check it's not a forwarded message quote
				if (Sinstr(cur,"On")>0 && Sinstr(cur,"wrote")>0) { }
				else
				// Check it's not a forwarded message header
		    if (*d>=stdate && *d<=enddate) {
		      // Check its not already in there
		      bool found=false;
		      for (int i=1;i<=drs.len && !found;i++)
		      	if (drs.num(i).d==*d && drs.num(i).e==e)
		      		found=true;
  		  	if (!found) {
	  		  	drs.add(DateRec(*d,e));
	  		  	used=true;
  		  	}
  		  }
  		  free(d);
//		    printf("Line %s\ncontains date %s\n",e.content.num(k),d->toString());
		  }
		}
		if (!used) {
			free(e);
			for (int k=i;k<j;k++)
				ls.freenum(i);
		} else
	    i=j;
  }
  printf("\n");
  printf("Found %i emails\n",ecnt);

  printf("Generating diary for dates %s to %s\n",stdate.toString(),enddate.toString());
  HTMLer h;
  h.start(Sconc("Mails regarding ",stdate.toString()," to ",enddate.toString()));
	h.addtitle();
	for (Date d=stdate;d<=enddate;d++) {
		printf(".");
		if (d.wd==1)
			h.add("<hr>");
		if (d.d==1)
		  h.add(Sformat("<h2><cen>%s</cen></h2>",Stoupper(months.num(d.m))));
		if (d==todaysdate)
		  h.add("<h2><cen>* TODAY *</cen></h2>");
		h.add(Sformat("<h3>%s</h3>",d.toString()));
		for (int i=1;i<=drs.len;i++) {
			if (drs.num(i).d==d) {
				Email *e=drs.num(i).e;
				int n=getnextfilenum();
				String emn=Sformat("Email%i.txt",n);
				writelinestofile(e->whole(),Sconc(destdir,emn));
				h.add(Sformat("<a href=\"%s\">%s: <b>\"%s\"</b> (%s)</a><br>",emn,e->from(),e->subject(),Date(e->date()).toString()));
			}
		}
	}
	printf("\n");
	h.end();
	h.write(Sconc(destdir,"index.html"));

/*  List<String> ls=readlinesfromfile(fname);
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
	*/
	
}
