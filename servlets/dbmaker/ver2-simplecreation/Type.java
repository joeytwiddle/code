package dbmaker;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;

public abstract class Type implements Serializable {

	abstract public String getName();

	public String getSQLType() {
		if (this instanceof BaseType)
			return "b...";
		else
			return "o...";
	}

	abstract public String getJavaType();

}
