package jlib.simple;

import jlib.JLib;

public class Code {
	public Object input=null;
	public void execute() {
		JLib.warn("You should overwrite Code.execute() in class "+this.getClass());
	}
}
