package changetheworld; import jlib.JString;

import changetheworld.RenderableXML;

import java.util.*;

public class Task extends RenderableXML {
  public String urltodome() {
    return CTW.Url+"?command=dotask&taskpath="+path;
  }
  public String render(HTMLRenderContext context) {
    return "";
  }
  public boolean complete(Map args,CTW ctw) { // true means completed - delete me!
    ctw.out.println("Could not complete Task - using base class!");
    return false;
  }
}