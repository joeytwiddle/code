class Field {
  public String type="";
  public String name="";
  public String initial="";
  boolean pub,stat,fin;
  Field(String t,String n) {
    type=t;
    name=n;
  }
  Field(String t,String n,String i) {
    type=t;
    name=n;
    initial=i;
  }
  Field(boolean p,boolean s,boolean f,String t,String n,String i) {
    pub=p;
    stat=s;
    fin=f;
    type=t;
    name=n;
    initial=i;
  }
}

