
import java.lang.String;
import java.lang.Integer;
import java.util.Vector;
import java.io.*;

import jlib.*;

public class dump {

  String fname;
  int numperline = 16;

  public dump(String f) {
    fname=f;
  }

  final static public void main(String[] args) {
    dump d=new dump(args[0]);
    d.showdump();
  }

  public void showdump() {

    try {

      StringBuffer s=new StringBuffer();
      Reader r=new FileReader(new File(fname));

			// String s=Files.readStringfromfile(fname);

    	int i=0;
    	boolean done=false;
    	int size=s.length();
	
    	while (!done) {
	
      	String pos = ""+i;
      	String hex="";
      	String asc="";
	
      	for (int k=0;k<numperline && !done;k++) {
	
        	int j=r.read();
					if (j==-1) {
						done=true;
					} else {
	
        		hex+=JString.padleft(Integer.toHexString(j),2,"0")+" ";
						char c=(char)j;

        		if (c>=' ' && j<127)
          		asc+=c;
        		else
          		asc+=" ";
          		// asc+=".";
	
        		i++;

					}
	
        	// done = ( i >= size );
	
      	}
	
      	String out=JString.padleft(pos,6," ")+"   "+hex;
      	out=JString.padright(out,9+3*numperline+2," ")+asc;
      	System.out.println( out );
	
    	}
      
			r.close();
	
    } catch (Exception e) {
			
    }

  }

}


