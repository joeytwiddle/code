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

public class CEDict {

	public static void main(String[] args) {

		ArgParser a=new ArgParser(args);
		File f=a.getFile("input page");
		a.done();

		try {

			FileReader fr=new FileReader(f);
			System.out.println("Encoding: "+fr.getEncoding());
			BufferedReader br=new BufferedReader(fr);

			Process dict=Runtime.getRuntime().exec("cedictlookup");
			BufferedReader dictin=new BufferedReader(new InputStreamReader(dict.getInputStream()));
			PrintStream dictout=new PrintStream(dict.getOutputStream());
			dictout.println("g");

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
						for (int j=0;j<sb.length()-3;j+=4) {
							dictout.println(s.substring(j,j+4));
							dictout.flush();
							String l;
							while ((l=dictin.readLine()).length()>-1) {
								System.out.println("Got "+l);
							}
						}
						for (int j=0;j<sb.length()-1;j+=2) {
							dictout.println(s.substring(j,j+2));
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

