import java.lang.String;

public class Test {

  public static void main(String[] args) {

    String s="hello";
    String t="hello";
    System.out.println(""+ (s==t) );
    System.out.println(""+ (s==args[0]) );
    System.out.println(""+ (s.equals(args[0])) );

  }

}