package jlib.db;

public abstract class DataChangeAwareDBObj extends NewDBObj {

	public boolean changed=false;

	public void changed() {
		changed=true;
	}
	
}
