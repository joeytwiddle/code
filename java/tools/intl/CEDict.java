package tools.intl;

// import java.applet.*;
// import java.awt.*;
// import java.awt.event.*;
import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
// import java.net.*;
import java.util.*;
// import javax.swing.*;
// import javax.swing.text.*;
// import javax.swing.text.html.*;
import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;

public abstract class CEDict {

	public abstract List lookup(String chars);

	public static void main(String[] args) {

		ArgParser a=new ArgParser(args);
		File f=a.getFile("input page");
		a.done();

		try {

			// CEDict dict=new DirectCEDict("b");
			CEDict dict=new DoubleCEDict();

			FileReader fr=new FileReader(f);
			System.out.println("Encoding: "+fr.getEncoding());
			BufferedReader br=new BufferedReader(fr);

			boolean inBlock=false;
			StringBuffer sb=new StringBuffer();

			int i;

			while ((i=br.read())>=0) {

				// System.out.println("int: "+i);

				char c=(char)i;
				Character.UnicodeBlock ub=Character.UnicodeBlock.of(c);
				// System.out.println("Unicode: "+ub);

				if (ub!=Character.UnicodeBlock.BASIC_LATIN) {
					sb.append(c);
					inBlock=true;
				} else {
					if (inBlock) {
						inBlock=false;
						// processBlock(sb);
						// System.out.println(sb.toString());
						String s=sb.toString();
						// Pairs
						for (int j=0;j<sb.length()-3;j+=2) {
							List res=dict.lookup(s.substring(j,j+4));
						}
						for (int j=0;j<sb.length()-1;j+=2) {
							List res=dict.lookup(s.substring(j,j+2));
						}
						sb.delete(0,sb.length());
					}
				}

				// System.out.println();

			}

		} catch (Exception e) {
			System.err.println(""+e);
		}
	}

	// static void processBlock(StringBuffer sb) {
	// }


}

class DoubleCEDict extends CEDict {

	CEDict gdict=new DirectCEDict("g");
	CEDict bdict=new DirectCEDict("b");

	public DoubleCEDict() {
	}

	public List lookup(String chars) {
		List l=gdict.lookup(chars);
		List l2=bdict.lookup(chars);
		l.addAll(l2);
		return l;
	}

}

class DirectCEDict extends CEDict {

	Process dict;
	BufferedReader dictin;
	PrintStream dictout;

	Map cache=new HashMap();

	String readLine() {
		try {
			return dictin.readLine();
		} catch (Exception e) {
			System.err.println(""+e);
			return null;
		}
	}

	public DirectCEDict(String charStyle) {

		try {

			System.out.println("Initialising cedict...");
			dict=Runtime.getRuntime().exec("cedictlookup");
			dictin=new BufferedReader(new InputStreamReader(dict.getInputStream()));
			dictout=new PrintStream(dict.getOutputStream());
			// Select dictionary file:
			dictout.println(charStyle);
			dictout.flush();
			String l;
			while ((l=readLine()).length()>0) {
				System.out.println("Got "+l);
			}
			System.out.println("Ready to go.");

		} catch (Exception e) {
			System.err.println(""+e);
		}

	}

	public List lookup(String chars) {

		List res=(List)cache.get(chars);

		if (res==null) {

			res=new Vector();

			dictout.println(chars);
			dictout.flush();

			for (int i=0;i<"Enter word (-h for help): ".length();i++)
				try { dictin.read(); } catch (Exception e) { }

			String l;
			while ((l=readLine()).length()>0) {
				if (!l.equals("No match found.")) {
					System.out.println("Got "+l);
					res.add(l);
				}
			}

			cache.put(chars,res);

		}

		return res;

	}

}

