package changetheworld; import jlib.JString;

public class Service extends CTWInput {
  public String labour="What is the labour involved in this service?";
  public String product="What is the product of this labour?  Please provide smallest unit of time the labour may be performed for, and units of product generated over such time";
  public String resources="What other resources are required?";
  public Service() {
  }
  public String gettitle() {
    return JString.clip(labour,50)+":&nbsp;"+JString.clip(product,50);
  }
  public String render() {
    String s="";
    s+="<b>Labour</b>:&nbsp;"+labour+"<br>\n";
    s+="<b>Product</b>:&nbsp;"+product+"<br>\n";
    s+="<b>Resources required</b>:&nbsp;"+resources+"<br>\n";
    return s;
  }
}