package changetheworld; import jlib.JString;

import java.lang.reflect.*;
import changetheworld.Satisfier;

public class ObjectContainsField implements Satisfier {
  String field;
  Object data;
  public ObjectContainsField(String f,Object v) {
    field=f;
    data=v;
  }
  public boolean satisfies(Object o) {
    Class c=o.getClass();
    Field f;
    try {
      f=c.getField(field);
    } catch (Exception e) {
      System.out.println("ObjectContainsField: couln't find field "+field+" in class "+c.getName());
      return false;
    }
    Object d;
    try {
      d=f.get(o);
    } catch (Exception e) {
      System.out.println("ObjectContainsField: couln't get field "+field+" from object "+o);
      return false;
    }
    return (data.equals(d));
  }
}

