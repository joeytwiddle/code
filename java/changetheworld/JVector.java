package changetheworld; import jlib.JString;

import java.util.Vector;
import changetheworld.Satisfier;
import changetheworld.XML;

public class JVector {

	public static Object getfirst(Satisfier s,List v) {
    try {
    for (int i=0;i<v.size();i++) {
      boolean sat=false;
      try {
        sat=s.satisfies(v.get(i));
      } catch (Exception e) {
        XML.log+="JVector.getfirst: satisfier failed "+e+"<br>\n";
      }
      if (sat)
        return v.get(i);
    }
    } catch (Exception e) {
      XML.log+="JList.getfirst: "+e+"<br>\n";
    }
    return null;
  }

	public static boolean contains(Object[] os,Object o) {
		for (int i=0;i<os.length;i++)
      if (os[i].equals(o))
        return true;
    return false;
  }

}
