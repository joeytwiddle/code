package changetheworld; import jlib.JString;

import java.lang.String;
import java.util.Vector;

import changetheworld.XML;
import changetheworld.RenderableXML;

public class VoteRecord extends RenderableXML {
  public String votepath=null;
  public String personpath=null;
  public String votetype=null;
  public String vote=null;
  public VoteRecord() {
  }
  VoteRecord(String p,String i,String t,String v) {
    votepath=p;
    personpath=i;
    votetype=t;
    vote=v;
  }
  public boolean equals(VoteRecord v) {
    return ( votepath.equals(v.votepath) &&
             personpath.equals(v.personpath) &&
             votetype.equals(v.votetype) &&
             vote.equals(v.vote) );
  }
  public String render() {
    XML v=openpath(votepath);
    String vs=( v==null ? "not found!" : ((Voteable)v).renderwithissue(false)+" "+((Voteable)v).renderlist(false) );
    return "<b><i>"+vote+"</i></b> in "+vs;
  }
  public String renderlist() {
    return render();
  }
  public static VoteRecord getrecord(XML x,Vector vrs) {
      // x is either:
      // person ,', looking for person in Voteable.voterecord
      // or voteable ,', looking for vote in Person.votescast
      for (int i=0;i<vrs.size();i++) {
        VoteRecord vr=(VoteRecord)vrs.get(i);
//        if (vr.voteid.equals(((XML)x).id))
        boolean hit=false;
        if (JReflect.canbe(x,"changetheworld.Person"))
          hit=vr.personpath.equals(x.path);
        if (JReflect.canbe(x,"changetheworld.Voteable"))
          hit=vr.votepath.equals(x.path);
        if (hit) {
          vrs.remove(vr);
          return vr;
        }
      }
      return null;
  }
}
