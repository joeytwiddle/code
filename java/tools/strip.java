import jlib.Files;
import java.lang.String;
import java.util.Vector;

public class strip {
  public final static void main(String[] argv) {
    String fname=argv[0];
    Vector lines=Files.readlinesfromfile(fname);
    for (int i=0;i<lines.size();i++) {
      String s=(String)lines.get(i);
      lines.set(i,s.trim());
    }
    Files.writelinestofile(lines,fname);
  }
}