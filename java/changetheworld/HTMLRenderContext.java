package changetheworld; import jlib.JString;

public class HTMLRenderContext implements Cloneable {
  public String context="";

  // URL context is rendering for
  public String where="";

  public boolean debug=false;//true;

  // Render look
  public boolean full=true;
  public boolean list=false;
  public boolean link=false;

  // Forms
  public boolean form=false;
  public boolean formnew=false;
  public boolean passid=false;
  public String xmlpath="";
  public String action="*HTMLRenderContext: shouldn't happen*";
  public String formembed="";
  public String button="*HTMLRenderContext: shouldn't happen*";
  // Databases
  public String destpath="*HTMLRenderContext: requested form but no dbase supplied*";

  // Tasks
  public String inside=""; // =path of object if currently rendering its inside

  // Who are you rendering for?
  public Person person=null;

  HTMLRenderContext() {
  }
  HTMLRenderContext(String c) {
    settotype(c);
  }
  HTMLRenderContext(String c,Person p) {
    settotype(c);
    person=p;
  }
  void settotype(String c) {
    context=c;
    if (context.equals("newform")) { // submission form for new object
      form=true;
      formnew=true;
      action=CTW.Url;
      formembed="<input type=\"hidden\" name=\"command\" value=\"submitnew\">";
      button="Create New Data";
    } else if (context.equals("editform")) { // edit form for existing object
      form=true;
      action=CTW.Url;
      formembed="<input type=\"hidden\" name=\"command\" value=\"submitedit\">";
      button="Submit Edition";
      passid=true;
    } else if (context.equals("realtimeeditform")) { // edit form for existing object
      form=true;
      action=CTW.Url;
      formembed="<input type=\"hidden\" name=\"command\" value=\"realtimeedit\">";
      button="Show Changes";
      passid=true;
    } else if (context.equals("list")) { // rendering a list
      list=true;
      full=false;
    } else if (context.equals("link")) { // just render a link to the object
      link=true;
      full=false;
    } else {
      System.out.println("HTMLRenderContext(String): Given context "+c+" does not exist");
    }
  }
  HTMLRenderContext pluspath(String p) {
    try {
    HTMLRenderContext h=(HTMLRenderContext)this.clone();
    h.xmlpath+=p;
    return h;
    } catch (Exception e) {
      xmlpath+="HTMLRenderContext.pluspath: "+e;
      return this;
    }
  }
}

