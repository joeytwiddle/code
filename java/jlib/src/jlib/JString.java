package jlib;

import java.lang.String;
import java.lang.StringBuffer;
import java.util.Vector;

public class JString extends org.neuralyte.common.StringUtils {

  public final static String alphaUpper="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  public final static String alphaLower="abcdefghijklmnopqrstuvwxyz";
  public final static String alpha=alphaUpper+alphaLower;
  public final static String num="0123456789";
  public final static String alphanum=alpha+num;

  public static boolean startsWithIgnoreCase(String s,String end) {
    if (s.length()<end.length())
      return false;
    String send=s.substring(0,end.length());
    return send.equalsIgnoreCase(end);
  }

  public static boolean endsWithIgnoreCase(String s,String end) {
    if (s.length()<end.length())
      return false;
    String send=s.substring(s.length()-end.length());
    return send.equalsIgnoreCase(end);
  }

/*  public static String replace(String x,String s,String r) {
    int sl=s.length();
    int rl=r.length();
    boolean goforit=( sl==rl );
    StringBuffer sb=new StringBuffer(x);
    int i=0;
    i=sb.toString().indexOf(s,i);
    while (i>-1) {
//      sb.delete(i,i+s.length());
//      sb.insert(i,r);
      if (goforit)
        for (int j=0;j<sl;j++)
          sb.setCharAt(i+j,r.charAt(j));
      else
        sb.replace(i,i+s.length(),r);
      i=i+r.length();
      i=sb.toString().indexOf(s,i);
    }
    return sb.toString();
  }*/

/*  public static String replace(String x,String s,String r) {
    int i=0;
    i=x.indexOf(s,i);
    while (i>-1) {
      x=x.substring(0,i)+r+x.substring(i+s.length());
      i=i+r.length();
      i=x.indexOf(s,i);
    }
    return x;
  }*/

/*  public static String replace(String x,String[][] srs) {
    String n="";
    try {
    int i=0;
    int l=x.length();
    XML.log+="Got "+srs.length+" srs";
    if (1==1)
    return x;
    while (i<l) {
      int c=x.charAt(i);
      boolean done=false;
      for (int j=0;j<srs.length && !done;j++) {
//        String[] sr=srs[j];
//        XML.log+=sr.length+"<br>\n";
//        String s=sr[0];
//        XML.log+=s+"<br>\n";
//        String s=srs[j][0];
        if (s.charAt(0)==c) {
          int sl=s.length();
          if (i+sl<l)
            if (x.substring(i,i+sl).equals(s)) {
              String r=srs[j][1];
              n+=r;
              i+=sl;
              done=true;
            }
        }
      }
      if (!done) {
        i++;
        n+=c;
      }
    }
    return n;
    } catch (Exception e) {
      XML.log+="JString.replace(list): "+e+"<br>\n";
      return "";
    }
//    return n;
  }*/

  public static String replace(String x,String s,String r) {
		if (x==null || s==null || r==null) {
			Log.error("JString.replace(): x=\""+x+"\" s=\""+s+"\" r=\""+r+"\"");
			return "*ERROR*null";
		}
    String n="";
    int l=0;
    int i=0;
    i=x.indexOf(s,i);
    if (i==-1)
      return x;
    while (i>-1) {
      n+=x.substring(l,i)+r;
//      x=x.substring(0,i)+r+x.substring(i+s.length());
      i+=s.length();
      l=i;
      i=x.indexOf(s,i);
    }
    n+=x.substring(l,x.length());
    return n;
  }

  public static String remove(String x,String s) {
    return replace(x,s,"");
  }
  public static Vector splitv(String s,String sep) {
    Vector v=new Vector();
    int i=s.indexOf(sep);
    while (i>-1) {
      String t=s.substring(0,i);
      if (t.length()>0)
        v.add(t);
      s=s.substring(i+sep.length());
      i=s.indexOf(sep);
    }
    if (s.length()>0)
      v.add(s);
    return v;
  }
  public static String[] split(String s,String sep) {
    Vector v=new Vector();
    int i=s.indexOf(sep);
    while (i>-1) {
      String t=s.substring(0,i);
      if (t.length()>0)
        v.add(t);
      s=s.substring(i+sep.length());
      i=s.indexOf(sep);
    }
    if (s.length()>0)
      v.add(s);
    return JString.Stringarray(v.toArray());
  }
  public static String random(int l) {
    String s="";
    for (int i=0;i<l;i++) {
      int j=(int)(Math.random()*(float)(26+26+10));
      char c=( j<26 ? (char)((int)'a'+j) :
               ( j-26 < 26 ? (char)((int)'A'+(j-26)) : (char)((int)'0'+(j-26-26)) ) );
      s=s+c;
    }
    return s;
  }
  public static boolean isvowel(char c) {
    c=tolower(c);
    return (c=='a' || c=='e' || c=='i' || c=='o' || c=='u');
  }
  public static char tolower(char c) {
    return (char)(c>='A' && c<='Z' ? 'a'+c-'A' : c);
  }
  public static char toupper(char c) {
    return (char)(c>='a' && c<='z' ? 'A'+c-'a' : c);
  }
  public static String tolower(String s) {
    StringBuffer n=new StringBuffer("");
    for (int i=0;i<s.length();i++)
      n.append(tolower(s.charAt(i)));
    return n.toString();
  }

  public static String stripendsof(String x,String s) {
    boolean go=true;
    while (go) {
      go=false;
      char a=x.charAt(0);
      if (s.indexOf(a)>=0) {
        x=x.substring(1);
        go=true;
      }
      char z=x.charAt(x.length()-1);
      if (s.indexOf(z)>=0) {
        x=x.substring(0,x.length()-1);
        go=true;
      }
    }
    return x;
  }

  public static String removeHTMLtags(String x) {
    String n="";
    int i=0;
    int j=x.indexOf("<");
    while (j>-1) {
      int k=x.indexOf(">",j);
      if (k==-1)
        k=x.length();
      n+=x.substring(i,j)+"&nbsp;";
      i=k+1;
      j=x.indexOf("<",i);
    }
    n+=x.substring(i);
    return n;
  }

  public static String clip(String x) {
    return clip(x,200);
  }

  public static String clip(String x,int i) {
    x=removeHTMLtags(x);
    if (x.length()>i)
      return x.substring(0,i)+"&nbsp;<font color=\"008000\"><b>...</b></font>";
    else
      return x;
  }

  public static String[] Stringarray(Object[] os) {
    String[] ss=new String[os.length];
    for (int i=0;i<os.length;i++)
      ss[i]=(String)os[i];
    return ss;
  }

  public static double similarity(String a,String b) {
    if (a.compareToIgnoreCase(b)==0)
      return 1.0;
    if (a.length()==0 && b.length()==0)
      return 1.0;
    return Math.abs(a.length()-b.length())/(a.length()+b.length());
  }

  /** For back-compatibility with changetheworld.FastLink **/
  public static String time(int time) {
    return time((long)time);
  }
  public static String time(long time) {
    long years=  time/1000/60/60/24/30/12;
    time=time-  years*1000*60*60*24*30*12;
    long months= time/1000/60/60/24/30;
    time=time- months*1000*60*60*24*30;
    long days=   time/1000/60/60/24;
    time=time-   days*1000*60*60*24;
    long hours=  time/1000/60/60;
    time=time-  hours*1000*60*60;
    long minutes=time/1000/60;
    time=time-minutes*1000*60;
    long seconds=time/1000;
    time=time-seconds*1000;
    String s="";
    if (years>0)
      s+=years+" years, ";
    if (months>0 || s.length()>0)
      s+=months+" months, ";
    if (days>0 || s.length()>0)
      s+=days+" days, ";
    if (hours>0 || s.length()>0)
      s+=hours+"h:";
    if (minutes>0 || s.length()>0)
      s+=minutes+"m:";
    if (seconds>0 || s.length()>0)
      s+=seconds+"s";
    return s;
  }

  public static String repeat(String s,int n) {
    String x="";
    for (int i=0;i<n;i++)
      x+=s;
    return x;
  }

  public static String padleft(String x,int n,String s) {
    if (x.length()>=n)
      return x;
    int diff=n-x.length();
    String pad=left(repeat(s,diff),diff);
    return pad+x;
  }

  public static String left(String x,int i) {
    if (x.length()>i)
      return x.substring(0,i);
    else
      return x;
  }

  public static String formencode(String s) {
    s=replace(s,"\"","&quot;");
    return s;
  }

	public static String squeeze(String x,String s) {
		StringBuffer n=new StringBuffer();
		for (int i=0;i<x.length();i++) {
			int j=i+1;
			boolean keep=true;
			if (j<x.length()) {
				if (s.indexOf(x.charAt(i))>=0 && s.indexOf(x.charAt(j))>=0)
					keep=false;
			}
			if (keep)
				n.append(x.charAt(i));
		}
		return n.toString();
	}

	public static String trim(String s,String unwanted) {
		boolean done=false;
		while (!done) {
			done=true;
			if (s.length()>0) {
				if (unwanted.indexOf(s.charAt(0))>=0) {
					s=s.substring(1);
					done=false;
				} else if (unwanted.indexOf(s.charAt(s.length()-1))>=0) {
					s=s.substring(0,s.length()-1);
					done=false;
				}
			}
		}
		return s;
	}

  public static String trimwhitespace(String s) {
    int n=s.length();
    int l=n+1;
    while (n<l) {
      l=n;
      s=replace(s,"\n ","\n");
      s=replace(s," \n","\n");
      s=replace(s,"  "," ");
      n=s.length();
    }
    return s;
  }

  public static String indent(int n,String s) {
    return repeat(" ",n)+replace(s,"\n","\n"+repeat(" ",n));
  }

  public static String indent(int n) {
    return repeat(" ",n);
  }


/*  public static String padleft(String s,int n,String c) {
    if (s.length()>=n)
      return s;
    else
      return repeat(c,(n-s.length())/c.length())+s;
  } */

  public static String padright(String s,int n,String c) {
    if (s.length()>=n)
      return s;
    else
      return s+repeat(c,(n-s.length())/c.length());
  }

  public static String expect(String s,String e) throws JException {
    if (!s.startsWith(e)) {
      throw new JException();
    }
    return after(s,e);
  }

	public static boolean contains(String s,char c) {
		return (s.indexOf(c)>=0);
	}

  public static boolean containsAnyOf(String s,String cs) {
    for (int i=0;i<cs.length();i++)
      if (s.indexOf(cs.charAt(i))!=-1)
        return true;
    return false;
  }

  public static boolean containedBy(String s,String cs) {
    for (int i=0;i<s.length();i++)
      if (cs.indexOf(s.charAt(i))==-1)
        return false;
    return true;
  }

  public static int countOf(char c,String s) {
    int cnt=0;
    for (int i=0;i<s.length();i++)
      if (s.charAt(i)==c)
        cnt++;
    return cnt;
  }

  public static String[] splitSharedAt(String a,String b,char at) {
    String[] ss=splitShared(a,b);
    int i=ss[0].lastIndexOf(at);
    if (i==-1)
      i=-1;
    String[] sss=new String[3];
    sss[0]=a.substring(0,i+1);
    sss[1]=a.substring(i+1);
    sss[2]=b.substring(i+1);
    return sss;
  }

  public static String[] splitShared(String a,String b) {
    int i=0;
    String common="";
    boolean done=false;
    while (!done) {
      if (i>=a.length() || i>=b.length())
        done=true;
      else
        if (a.charAt(i)==b.charAt(i)) {
          common+=a.charAt(i);
          i++;
        } else
          done=true;
    }
    String[] ss=new String[3];
    ss[0]=common;
    ss[1]=a.substring(i);
    ss[2]=b.substring(i);
    return ss;
  }

  public static String lineChar(String big,String in) {
    int i=big.indexOf(in);
    String sub=big.substring(0,i);
    return "line "+countOf('\n',sub)+", char "+(i-sub.lastIndexOf('\n')-1);
  }

  public static void clearBuffer(StringBuffer sb) {
    sb.delete(0,sb.length());
  }

	// Here follow encoding operations:

	public static String toHTML(String s) {
		s=replace(s,"<","&lt;");
		s=replace(s,">","&gt;");
		s=replace(s,"\n","<br>\n");
		return s;
	}

	public static String initCap(String s) {
		return Character.toUpperCase(s.charAt(0))+s.substring(1);
	}
	
    public static String before(String str, String search) {
        return str.substring(0, str.indexOf(search));
    }

    public static String after(String str, String search) {
        return str.substring(str.indexOf(search) + search.length());
    }

    public static String beforelast(String str, String search) {
        return str.substring(0, str.lastIndexOf(search));
    }

    public static String afterlast(String str, String search) {
        return str.substring(str.lastIndexOf(search) + search.length());
    }

}

