package changetheworld;

import changetheworld.*;
import jlib.*;
import java.lang.*;
import java.lang.reflect.*;

public class FastReflect {

  public static boolean set(String type,Object o,String field,Object value) {

    if (type.equals("changetheworld.Question")) {
      if (field.equals("id")) {
        ((Question)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Question)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((Question)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((Question)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((Question)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("title")) {
        ((Question)o).title=""+value;
        return true;
      }
      if (field.equals("details")) {
        ((Question)o).details=""+value;
        return true;
      }
    }

    if (type.equals("changetheworld.Category")) {
      if (field.equals("id")) {
        ((Category)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Category)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((Category)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((Category)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((Category)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("title")) {
        ((Category)o).title=""+value;
        return true;
      }
      if (field.equals("description")) {
        ((Category)o).description=""+value;
        return true;
      }
      if (field.equals("defaulttype")) {
        ((Category)o).defaulttype=""+value;
        return true;
      }
    }

    if (type.equals("changetheworld.Debate")) {
      if (field.equals("id")) {
        ((Debate)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Debate)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((Debate)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((Debate)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((Debate)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("title")) {
        ((Debate)o).title=""+value;
        return true;
      }
      if (field.equals("content")) {
        ((Debate)o).content=""+value;
        return true;
      }
    }

    if (type.equals("changetheworld.Event")) {
      if (field.equals("id")) {
        ((Event)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Event)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((Event)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((Event)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((Event)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("title")) {
        ((Event)o).title=""+value;
        return true;
      }
      if (field.equals("url")) {
        ((Event)o).url=""+value;
        return true;
      }
      if (field.equals("date")) {
        ((Event)o).date=""+value;
        return true;
      }
      if (field.equals("location")) {
        ((Event)o).location=""+value;
        return true;
      }
      if (field.equals("description")) {
        ((Event)o).description=""+value;
        return true;
      }
    }

    if (type.equals("changetheworld.Decision")) {
      if (field.equals("id")) {
        ((Decision)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Decision)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((Decision)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((Decision)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((Decision)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("title")) {
        ((Decision)o).title=""+value;
        return true;
      }
      if (field.equals("description")) {
        ((Decision)o).description=""+value;
        return true;
      }
      if (field.equals("vote")) {
        ((Decision)o).vote=(changetheworld.Vote)value;
        return true;
      }
    }

    if (type.equals("changetheworld.Issue")) {
      if (field.equals("id")) {
        ((Issue)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Issue)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((Issue)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((Issue)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((Issue)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("title")) {
        ((Issue)o).title=""+value;
        return true;
      }
      if (field.equals("details")) {
        ((Issue)o).details=""+value;
        return true;
      }
    }

    if (type.equals("changetheworld.SolidarityIssue")) {
      if (field.equals("id")) {
        ((SolidarityIssue)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((SolidarityIssue)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((SolidarityIssue)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((SolidarityIssue)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((SolidarityIssue)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("title")) {
        ((SolidarityIssue)o).title=""+value;
        return true;
      }
      if (field.equals("details")) {
        ((SolidarityIssue)o).details=""+value;
        return true;
      }
      if (field.equals("count")) {
        ((SolidarityIssue)o).count=(changetheworld.SolidarityCount)value;
        return true;
      }
    }

    if (type.equals("changetheworld.SolidarityCount")) {
      if (field.equals("id")) {
        ((SolidarityCount)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((SolidarityCount)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((SolidarityCount)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((SolidarityCount)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((SolidarityCount)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("voterecord")) {
        ((SolidarityCount)o).voterecord=(java.util.Vector)value;
        return true;
      }
      if (field.equals("issue")) {
        ((SolidarityCount)o).issue=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("count")) {
        ((SolidarityCount)o).count=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("issue")) {
        ((SolidarityCount)o).issue=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("agreement")) {
        ((SolidarityCount)o).agreement=""+value;
        return true;
      }
    }

    if (type.equals("changetheworld.Link")) {
      if (field.equals("id")) {
        ((Link)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Link)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((Link)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((Link)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((Link)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("title")) {
        ((Link)o).title=""+value;
        return true;
      }
      if (field.equals("seekpath")) {
        ((Link)o).seekpath=""+value;
        return true;
      }
      if (field.equals("transparent")) {
        ((Link)o).transparent=new Boolean(""+value).booleanValue();
        return true;
      }
    }

    if (type.equals("changetheworld.Fact")) {
      if (field.equals("id")) {
        ((Fact)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Fact)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((Fact)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((Fact)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((Fact)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("fact")) {
        ((Fact)o).fact=""+value;
        return true;
      }
      if (field.equals("details")) {
        ((Fact)o).details=""+value;
        return true;
      }
      if (field.equals("source")) {
        ((Fact)o).source=(changetheworld.Link)value;
        return true;
      }
    }

    if (type.equals("changetheworld.Place")) {
      if (field.equals("id")) {
        ((Place)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Place)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((Place)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((Place)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((Place)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("place")) {
        ((Place)o).place=""+value;
        return true;
      }
    }

    if (type.equals("changetheworld.FastLink")) {
      if (field.equals("id")) {
        ((FastLink)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((FastLink)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((FastLink)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((FastLink)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((FastLink)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("title")) {
        ((FastLink)o).title=""+value;
        return true;
      }
      if (field.equals("seekpath")) {
        ((FastLink)o).seekpath=""+value;
        return true;
      }
      if (field.equals("transparent")) {
        ((FastLink)o).transparent=new Boolean(""+value).booleanValue();
        return true;
      }
      if (field.equals("lastrendered")) {
        ((FastLink)o).lastrendered=(java.util.Date)value;
        return true;
      }
      if (field.equals("rendered")) {
        ((FastLink)o).rendered=""+value;
        return true;
      }
      if (field.equals("rendertime")) {
        ((FastLink)o).rendertime=new Integer(""+value).intValue();
        return true;
      }
    }

    if (type.equals("changetheworld.Service")) {
      if (field.equals("id")) {
        ((Service)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Service)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((Service)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((Service)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((Service)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("labour")) {
        ((Service)o).labour=""+value;
        return true;
      }
      if (field.equals("product")) {
        ((Service)o).product=""+value;
        return true;
      }
      if (field.equals("resources")) {
        ((Service)o).resources=""+value;
        return true;
      }
    }

    if (type.equals("changetheworld.Search")) {
      if (field.equals("id")) {
        ((Search)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Search)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("keystring")) {
        ((Search)o).keystring=""+value;
        return true;
      }
      if (field.equals("searchpath")) {
        ((Search)o).searchpath=""+value;
        return true;
      }
      if (field.equals("max")) {
        ((Search)o).max=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("favourchildren")) {
        ((Search)o).favourchildren=new Double(""+value).doubleValue();
        return true;
      }
      if (field.equals("searchfields")) {
        ((Search)o).searchfields=new Boolean(""+value).booleanValue();
        return true;
      }
      if (field.equals("typesallowed")) {
        ((Search)o).typesallowed=""+value;
        return true;
      }
      if (field.equals("byauthor")) {
        ((Search)o).byauthor=""+value;
        return true;
      }
      if (field.equals("showlots")) {
        ((Search)o).showlots=new Boolean(""+value).booleanValue();
        return true;
      }
      if (field.equals("preferrecent")) {
        ((Search)o).preferrecent=new Boolean(""+value).booleanValue();
        return true;
      }
    }

    if (type.equals("changetheworld.Task")) {
      if (field.equals("id")) {
        ((Task)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Task)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
    }

    if (type.equals("changetheworld.VoteRecord")) {
      if (field.equals("id")) {
        ((VoteRecord)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((VoteRecord)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("votepath")) {
        ((VoteRecord)o).votepath=""+value;
        return true;
      }
      if (field.equals("personpath")) {
        ((VoteRecord)o).personpath=""+value;
        return true;
      }
      if (field.equals("votetype")) {
        ((VoteRecord)o).votetype=""+value;
        return true;
      }
      if (field.equals("vote")) {
        ((VoteRecord)o).vote=""+value;
        return true;
      }
    }

    if (type.equals("changetheworld.Database")) {
      if (field.equals("id")) {
        ((Database)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Database)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((Database)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((Database)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((Database)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("title")) {
        ((Database)o).title=""+value;
        return true;
      }
      if (field.equals("description")) {
        ((Database)o).description=""+value;
        return true;
      }
      if (field.equals("defaulttype")) {
        ((Database)o).defaulttype=""+value;
        return true;
      }
    }

    if (type.equals("changetheworld.Person")) {
      if (field.equals("id")) {
        ((Person)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Person)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((Person)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((Person)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((Person)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("name")) {
        ((Person)o).name=""+value;
        return true;
      }
      if (field.equals("pass")) {
        ((Person)o).pass=""+value;
        return true;
      }
      if (field.equals("email")) {
        ((Person)o).email=""+value;
        return true;
      }
      if (field.equals("emailpublic")) {
        ((Person)o).emailpublic=new Boolean(""+value).booleanValue();
        return true;
      }
      if (field.equals("location")) {
        ((Person)o).location=""+value;
        return true;
      }
      if (field.equals("phone")) {
        ((Person)o).phone=""+value;
        return true;
      }
      if (field.equals("locationpublic")) {
        ((Person)o).locationpublic=new Boolean(""+value).booleanValue();
        return true;
      }
      if (field.equals("skills")) {
        ((Person)o).skills=""+value;
        return true;
      }
      if (field.equals("skillspublic")) {
        ((Person)o).skillspublic=new Boolean(""+value).booleanValue();
        return true;
      }
      if (field.equals("page")) {
        ((Person)o).page=""+value;
        return true;
      }
      if (field.equals("votescast")) {
        ((Person)o).votescast=(java.util.Vector)value;
        return true;
      }
      if (field.equals("tasks")) {
        ((Person)o).tasks=(changetheworld.XML)value;
        return true;
      }
    }

    if (type.equals("changetheworld.Vote")) {
      if (field.equals("id")) {
        ((Vote)o).id=""+value;
        return true;
      }
      if (field.equals("insidesize")) {
        ((Vote)o).insidesize=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("creationdate")) {
        ((Vote)o).creationdate=(java.util.Date)value;
        return true;
      }
      if (field.equals("author")) {
        ((Vote)o).author=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("visits")) {
        ((Vote)o).visits=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("voterecord")) {
        ((Vote)o).voterecord=(java.util.Vector)value;
        return true;
      }
      if (field.equals("issue")) {
        ((Vote)o).issue=(changetheworld.Link)value;
        return true;
      }
      if (field.equals("votesFor")) {
        ((Vote)o).votesFor=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("votesAgainst")) {
        ((Vote)o).votesAgainst=new Integer(""+value).intValue();
        return true;
      }
      if (field.equals("votesAbstain")) {
        ((Vote)o).votesAbstain=new Integer(""+value).intValue();
        return true;
      }
    }

    return false;

  }

}
