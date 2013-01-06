package tools.parser;

import jlib.JString;
import jlib.strings.SomeString;


public class StringHelpers {

	public static String strip(SomeString s) {
    	return strip(s.toString());
    }

	public static String strip(String s) {
    	int max = 20;
    	if (s.length() > max)
    	   s = JString.left(s, max) + "..." + (s.length() - max);
    	s = JString.replace(s, "\n", "\\n");
    	return s;
    }

}
