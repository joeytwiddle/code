import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;

import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;

public class TemplateCompile {

  public static void main(String[] args) {
		ArgParser a=new ArgParser(args);
		String fileName=a.get("template.tpl");
		String outName=a.get("output file");
		a.done();

		String s=Files.readStringfromfile(fileName);
		StringBuffer sb=new StringBuffer();
		int i=0;
		while (i<s.length()) {
			char c=s.charAt(i); i++;
			if (c=='%') {
				if (s.charAt(i)=='%') {
					i++;
					int j=s.indexOf("%%",i);
					String code=s.substring(i,j);
					i=j+2;
					echo("\");\n"+code+"\n  echo(\"");
					try {
					if (s.charAt(i)=='\n')
						i++;
					} catch (Exception o) {}
				} else {
					int j=s.indexOf("%",i);
					String var=s.substring(i,j);
					i=j+1;
					echo("\" + "+var+" + \"");
				}
			} else {
				if (c=='\n')
					echo("\\n");
				else
					echo(""+c);
			}
		}

	}

	static void echo(String s) {
		System.out.print(s);
	}

}
