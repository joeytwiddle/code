package changetheworld; import jlib.JString;

public class Fact extends CTWInput {
  public String fact="What is it that we can be sure of?";
  public String details="Explain more...\nDish the dirty on careless companies, tell us how to manage the ecosystem...";
  public Link source=null;
  public Fact() {
  }
  public String gettitle() {
    return fact;
  }
  public String render() {
    return "<font size=\"+2\">"+fact+"</font><br><br>"+details;
  }
}