package jlib;

import java.util.Vector;

import jlib.Satisfier;

public class SatisfiesAll implements Satisfier {
  Vector satisfiers=new Vector();
  public SatisfiesAll() {
  }
  public void add(Satisfier s) {
    satisfiers.add(s);
  }
  public boolean satisfies(Object o) {
    for (int i=0;i<satisfiers.size();i++)
      if (!((Satisfier)satisfiers.get(i)).satisfies(o))
        return false;
    return true;
  }
}
