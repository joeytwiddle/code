package jlib;

import java.util.Vector;

public class ScoredList extends Vector {
  Vector scores;
  ScoredList() {
    super();
    scores=new Vector();
  }
  public void add(Object o,double score) {
    // Keep highest near top
    int p=0; // scores.size();
    while (p<scores.size() && score<((Double)scores.get(p)).doubleValue()) {
      p++;
    }
/*    for (int i=0;i<scores.size() && p!=scores.size();i++) {
      if (score<((Double)scores.get(i)).doubleValue()) {
        p=i;
      }
    }*/
    scores.insertElementAt(new Double(score),p);
    insertElementAt(o,p);
  }
  public double getscore(int i) {
    return ((Double)scores.get(i)).doubleValue();
  }
}
