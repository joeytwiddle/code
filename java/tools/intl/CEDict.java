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

class CEDictParser {

	CEDict dict=new DirectCEDict("b");
	// CEDict dict=new DoubleCEDict();
	BufferedReader br;
	StringBuffer sb=new StringBuffer();

	CEDictParser(Reader r) {
		br=new BufferedReader(r);
	}

	void parse() {

		try {

			boolean inBlock=false;

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
						// Process this block:
						// System.out.println(sb.toString());
						while (sb.length()>0) {
							// Try to translate 4 characters
							if ( ! (
										tryTrans(8) ||
										tryTrans(6) ||
										tryTrans(4) ||
										tryTrans(2)
									 ) ) {
								output("<FONT color=\"red\">");
								output(sb.charAt(0));
								output("</FONT>");
								sb.deleteCharAt(0);
							}
						}
					}
					output(c);
				}

				// System.out.println();

			}

		} catch (Exception e) {
			System.err.println(""+e);
		}

	}

	void output(char c) {
		System.out.print(c);
	}

	void output(String s) {
		System.out.print(s);
	}

	boolean tryTrans(int howMany) {
		if (sb.length()<howMany)
			return false;
		else {
			List res=dict.lookup(sb.substring(0,howMany));
			if (res.size()==0)
				return false;
			else {
				// output("<FONT color=\"green\">");
				output("<SPAN title=\"");
				for (int i=0;i<res.size();i++) {
					String s=(String)res.get(i);
					// To appear under Galeon I need to strip Chinese chars
					s=s.substring(s.indexOf(" ")+1);
					output(s);
					if (i<res.size()-1)
						output(" -- ");
				}
				output("\">");
				output(sb.substring(0,howMany));
				output("</SPAN>");
				// output("</FONT>");
				sb.delete(0,howMany);
				return true;
			}
		}
	}

}



public abstract class CEDict {

	public abstract List lookup(String chars);

	public static void main(String[] args) {

		try {

			InputStreamReader r=null;

			if (args.length==0) {

				r=new InputStreamReader(System.in);

			} else {
				
				File f=new File(args[0]);
				r=new FileReader(f);

			}

			// System.out.println("Encoding: "+r.getEncoding());
			CEDictParser p=new CEDictParser(r);
			p.parse();

		} catch (Exception e) {
			System.err.println(""+e);
		}

	}

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

			// System.out.println("Initialising cedict...");
			dict=Runtime.getRuntime().exec("cedictlookup");
			dictin=new BufferedReader(new InputStreamReader(dict.getInputStream()));
			dictout=new PrintStream(dict.getOutputStream());
			// Select dictionary file:
			dictout.println(charStyle);
			dictout.flush();
			String l;
			while ((l=readLine()).length()>0) {
				// System.out.println("Got "+l);
			}
			// System.out.println("Ready to go.");

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
					// System.err.println("Got "+l);
					res.add(l);
				}
			}

			cache.put(chars,res);

		}

		return res;

	}

}

