package jlib.db;

public abstract class NewDBObj extends ADBObj {

	// I'd rather not have this next lines, but Java complained, but Java complained!
	public NewDBObj() { }

	public NewDBObj(jlib.db.statement.ReadStatement wS) {
		readFromStatement(wS);
	}

	public abstract void readFromStatement(jlib.db.statement.ReadStatement wS);
	
}
