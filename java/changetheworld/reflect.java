import java.lang.reflect.*;
import changetheworld.Files;

public class reflect {
  public static void main(String[] argv) {
    try {
      Class c=Class.forName(argv[0]);
      Field[] fs=c.getFields();
      Method[] ms=c.getMethods();
      System.out.println(c+"\n"+fs+"\n"+ms);
      for (int i=0;i<fs.length;i++)
        System.out.println(""+fs[i]);
      for (int i=0;i<ms.length;i++)
        System.out.println(""+ms[i]);
    } catch (Exception e) {
      System.out.println(""+e);
    }
  }
}