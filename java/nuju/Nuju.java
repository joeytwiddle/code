package nuju;

public class Nuju {
	static boolean comwinstarted=false;
	Nuju() {
		present("Hello!");
		present(new Integer(1));
		present(this);
	}
	public Nuju(Object o) {
		present(o);
	}
	public static void present(Object o) {
		if (!comwinstarted)
			(new NujuComWin()).show();
		comwinstarted=true;
		NujuFrame nf=new NujuFrame(o);
		nf.show();
	}
	public final static void main(String[] args) {
		Nuju n=new Nuju(args);
	}
}
