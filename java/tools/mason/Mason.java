package tools.mason;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;

class Tag {
	
	String options;
	public /*ro*/ int start,end;
	
	public Tag(String c,int s,int e) {
		options=c;
		start=s;
		end=e;
	}
	
	public void render(String s,PrintWriter realtime,PrintWriter generator) {
		if (JString.contains(options,'r'))
			realtime.print(s);
		if (JString.contains(options,'g'))
			generator.print(s);
		if (JString.contains(options,'s')) {
			String escaped=JString.replace(JString.replace(JString.replace(s,"\\","\\\\"),"\"","\\\""),"\n","\\n");
			if (escaped.length()>0)
				generator.println("    staticOut.println(\""+escaped+"\");");
		}
		if (JString.contains(options,'e'))
			generator.println("    staticOut.println("+s+");");
			// generator.println("\"+"+s+"\"+");
	}

}

public class Mason {

	static boolean validTag(String s) {
		if ( JString.countOf('r',s)>1
			|| JString.countOf('g',s)>1
			|| JString.countOf('e',s)>1
			|| JString.countOf('s',s)>1
			|| s.length() >  JString.countOf('r',s) +  JString.countOf('g',s) +  JString.countOf('e',s) +  JString.countOf('s',s) )
			return false;
		else
			return true;
	}
	
	static Tag findNextTag(String s,int i) {
		while (true) {
			i=s.indexOf("/*",i);
			if (i<0)
				return new Tag("endtag",s.length(),s.length());
			int j=s.indexOf("*/",i);
			// if (j<0) someone hasn't closed their tags!
			String options=s.substring(i+2,j);
			if (validTag(options))
				return new Tag(options,i,j+2);
			i=j+2;
		}
	}

  public static void main(String[] args) {
		
		try {

		ArgParser a=new ArgParser(args);
		File file=a.getFile("source file");
		a.done();

		String fileName=file.getName();
		File output=new File("mason");
		output.mkdirs();
		File realtime=new File(output,"RealTime"+fileName+".java");
		File generator=new File(output,"Generator"+fileName+".java");
		
		String s=Files.readStringFromFile(file);

		PrintWriter realtimeWriter=new PrintWriter(new FileWriter(realtime));
		PrintWriter generatorWriter=new PrintWriter(new FileWriter(generator));

		Tag lastTag=new Tag("starttag",0,0);
		int i=0;

		while (i<s.length()) {

			Tag nextTag=findNextTag(s,i);
			System.out.println("Got Tag "+nextTag.options);

			String text=s.substring(i,nextTag.start);
			lastTag.render(text,realtimeWriter,generatorWriter);

			i=nextTag.end;
			lastTag=nextTag;

		}

		realtimeWriter.close();
		generatorWriter.close();
		
	} catch (Exception e) {
		System.err.println(""+e);
	}

	}

}
