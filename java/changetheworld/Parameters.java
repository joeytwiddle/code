package changetheworld; import jlib.JString;

import java.util.Vector;

public class Parameters {
  Vector par=new java.util.Vector();
  Vector val=new java.util.Vector();
  Parameters() {
  }
  public void add(String p,String v) {
    remove(p);
    par.add(p);
    val.add(v);
  }
  public int size() { // unused
    return par.size();
  }
  public String get(int i) {
    if (i<0 || i>=par.size())
      return null;
    else
      return (String)par.get(i);
  }
  public String get(String p) {
    for (int i=0;i<par.size();i++) {
      String pc=(String)par.get(i);
      if (pc.compareToIgnoreCase(p)==0)
        return (String)val.get(i);
    }
    return null;
  }
  public void remove(String p) {
    for (int i=0;i<par.size();i++) {
      String pc=(String)par.get(i);
      if (pc.compareToIgnoreCase(p)==0) {
        par.remove(i);
        val.remove(i);
        i--;
      }
    }
  }
  public void set(String p,String v) {
//    remove(p); no need
    add(p,v);
  }
}

