/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk */

#include <joeylib.c>

class WordRec {
public:
	String word;
	int cnt;
	WordRec(String w,int c) {
		word=w;
		cnt=c;
	}
};

bool sortbyalpha(WordRec a,WordRec b) {
//	printf("%s <= %s ? %i\n",a.word,b.word,Slesseq(a.word,b.word));
	return Slesseq(a.word,b.word);
}

bool sortbycnt(WordRec a,WordRec b) {
	return a.cnt>b.cnt;
}

class WordDB {
#define DBPath "/j/tools/dictionary.pc"
public:
	List<WordRec> ws;
	bool learntnew;
	WordDB() {
		if (fileexists(DBPath)) {
			List<String> ls=readlinesfromfile(DBPath);
			for (int i=1;i<=ls.len;i++)
				ws.add(WordRec(ls.num(i),0));
		}
		learntnew=false;
	}
	WordDB(String fname) {
		
		learntnew=false;
	}
	int findword(String w) {
		for (int i=1;i<=ws.len;i++) {
			if (Sequiv(w,ws.num(i).word))
				return i;
		}
		return -1;
	}
	void addocc(String w) {
		int f=findword(w);
		if (f>0)
			ws.p2num(f)->cnt++;
		else {
			ws.add(WordRec(w,1));
			learntnew=true;
		}
	}
	void writewords() {
		List<String> ls;
		for (int i=1;i<=ws.len;i++)
			ls.add(ws.num(i).word);
		writelinestofile(ls,DBPath);
		ls.freedom();
	}
	void writepc(String fname) {
		writelinestofile(show(),fname);
	}
	List<String> show() {
		List<String> s;
		for (int i=1;i<=ws.len;i++)
			if (ws.num(i).cnt>0)
				s.add(Sformat("%s: %i",ws.num(i).word,ws.num(i).cnt));
		return s;
	}
	void sortalpha() {
		ws.sort(&sortbyalpha);
	}
	void sortcnt() {
		ws.sort(&sortbycnt);
	}
};

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);

  String action=a.getarg();

	if (Seq(action,"scan")) {
  	String fname=a.getarg();
		WordDB db;
  	List<String> ls=readlinesfromfile(fname);
  	List<String> ws=Ssplittowords(ls);
  	ls.freeall();
  	ls.freedom();
  	for (int i=1;i<=ws.len;i++)
  		db.addocc(ws.num(i));
  	if (db.learntnew)
  		db.writewords();
  	db.sortalpha();
// 		writelinestoscreen(db.show());
  	db.writepc(Sconc(".",fname,".pc"));
	}  	

  a.done();

}
