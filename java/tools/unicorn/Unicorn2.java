/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

// Fix: It appears writeunique and keepunique are redundant.
// Fix: and ignoreComments because we use File.readlines() to parse
// 13/Apr/01: Unicorn now supports matching of non-unique lines
//            Comparisons made by character histogram or transition histogram of string.
// Now uses a tree search structure.

import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;
import java.util.*;

//import NujuFrame;
//import NujuComWin;

import jlib.*;
import jlib.Files;
import jlib.Satisfier;
import jlib.JString;
import jlib.JCurses;

class FileHash {
	long []hash;
	int length;
	FileHash(File f) {
		hash=new long[(int)f.length()];
		length=f.length();
		int i=0;
		int h=0;
    try {
      BufferedReader br=new BufferedReader(new FileReader(f));
      while (br.ready()) {
				int c=br.read();
				h=h+c;
				hash[i]=h;
      }
      br.close();
    } catch (Exception e) {
      System.out.println("What should I do with exception "+e+" ?");
    }
		System.out.println(""+h);
  }
	long hashfrom(int i,int sz) {
		return hash[i+sz]-hash[i];
	}
	long hashtween(int i,int j) {
		return hash[j]-hash[i];
	}
}

class Matcher {
	FileHash a,b;
	Matcher(FileHash aa,FileHash bb) {
		a=aa; b=bb;
	}

	void findBiggestMatchFor(int i,int j) {
		biggest=0;
		where=i;
		search(i,j);
	}

	void search(int i,int j) {
		int ssz=biggest;
		int sz=refineUp(i,j,ssz);

	int refineUp(int i,int j,int sz) {
		if (j+sz>b.length)
			return -1;
		if (a.hashfrom(i,sz)==b.hashfrom(j,sz))
			




class Searcher {
	FileHash a;
	FileHash b;
	Searcher(FileHash aa,FileHash bb) {
		a=aa; b=bb;
	}
	void search() {
		int i=0;
		Matcher matcher=new Matcher(a,b);
		while (i<a.length-1) {
			int j=i;
			matcher.findBiggestMatchForFrom(i,j);
			
			findbiggestmatchfrom(i,j,0);

		}
	}
	findbiggestmatchfrom(int i,int j,int biggestSoFar) {
		if (biggestSoFar>b.length-
		// Try biggest possible
		

class Unicorn2 {

  public static void main(String args[]) {
    Unicorn2 u=new Unicorn2();
    if (args.length==0) {
      //System.out.println("Unicorn: Unique or not?  By Joey (pclark@cs.bris.ac.uk) =)");
      System.out.println("Usage: unicorn <files> [-label] [-quiet] [-show-lines] [-no-curses] [-match-similar] [-separator <delimiter>]");
    }
    u.comparefiles(args);
  }

  Unicorn2() {
  }

  public void comparefiles(String args[]) {
  
		FileHash[] files=new FileHash[args.length];
		
		for (int i=0;i<args.length;i++) {
			File f=new File(args[i]);
			files[i]=new FileHash(f);
		}

		for (int i=0;i<files.length-1;i++) {
			Searcher s=new Searcher(files[i],files[i+1]);
		}
	
	}

}
