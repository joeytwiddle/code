package changetheworld; import jlib.JString;

import java.util.Vector;
import changetheworld.XML;
import changetheworld.Link;

// Vote on a real-world decision
// Vote to remove an object

public class Rating extends XML {
  public float rating=(float)0;
  public Link src=null;
  public Rating() {
  }
  public Rating(XML src) {
    src=new Link(src.id);
  }
}