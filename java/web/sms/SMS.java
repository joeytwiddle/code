import java.lang.String;
import java.net.*;
import jlib.*;

public class SMS {

  public static void main(String[] args) {

    ArgParser a=new ArgParser(args);
    String telno="029832";//a.get("tel no");
    String message="slkdj%20sldkjf%20lksjdf";//a.get("message");
    a.done();

    try {

//    URL u=new URL("http://www.lycos.co.uk/service/sms/eingabe.html");
    // URL u=JNet.url("http://www.lycos.co.uk/service/sms/agb.html");
    // URL u=new URL("http://hwi/servlet/changetheworld.CTW?command=display&page=/theindex.html");
    URL u=new URL("http://smscgi02-1.lycos.de/uk/cgi-bin/sms/send_uk_sms5.cgi");
    // URL u=new URL("http://smscgi02-1.lycos.de/uk/cgi-bin/sms/send_uk_sms5.cgi?msg=test&ren=07833596498&SUBMIT=Abschicken&msgnr=37499&Len=110&prg=1");

    URLConnection c=u.openConnection();
    c.setDoInput(true);
    c.setDoOutput(true);
    c.setAllowUserInteraction(true);
  c.setRequestProperty("accept-charset","iso-8859-1,*,utf-8");
  c.setRequestProperty("accept-language","en");
//  c.setRequestProperty("host","hwi");
  c.setRequestProperty("accept","image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, image/png, */*");
  c.setRequestProperty("user-agent","Mozilla/4.74 [en] (X11; U; Linux 2.2.5-15 i686)");
  c.setRequestProperty("referer","http://www.lycos.co.uk/service/sms/eingabe.html");
  c.setRequestProperty("connection","Keep-Alive");
  c.setRequestProperty("accept-encoding","gzip");
  c.setRequestProperty("pragma","no-cache");

  c.setRequestProperty("msgnr","37499");
  c.setRequestProperty("msg","test");
  c.setRequestProperty("ren","07833596498");
  c.setRequestProperty("Len","110");
  c.setRequestProperty("SUBMIT","Abschicken");
  c.setRequestProperty("prg","1");

  ((HttpURLConnection)c).setRequestMethod("GET");
    c.connect();
    System.out.println(""+c);
    Files.writeStringtofile(JNet.streamFrom(c.getContent()),"content.html");
    boolean done=false;
    for (int i=0;i<50 && !done;i++) {
      String s=c.getHeaderFieldKey(i);
      if (s==null && i>0)
        done=true;
      else
        System.out.println(s+": "+c.getHeaderField(s));
    }

    } catch (Exception e) {
      System.out.println(""+e);
    }

  }

}