import java.io.*;
import java.util.Vector;
import Files;

interface SmartObject {


class SmartFile {
  public static Vector readfile(String fname) {
    Vector ls=Files.readlinesfromfile(fname);
    for (int i=0;i<ls.size();i++) {
      String line=(String)ls.get(i);
      if (line.endsWith(" {")) {
        String type=line.substring(0,line.lastIndexOf(" "));
        i++; line=(String)ls.get(i);
        if (!line.equals("}")) {
          line=line.trim();
          int j=line.indexOf(" ");
          String field=line.substring(0,j);
          String data=line.substring(j+1);
          i++; line=(String)ls.get(i);
        }
      }
      i++;
    }


  }
  public static void writefile(String fname,Vector rs) {
    String s="";
    for (int i=0;i<rs.size();i++)
      s=s+((SmartObject)rs.get(i)).smartwrite+"\n";

  }
}