package tools.tree;

// Bugs: doesn't handly first line well
// doesn't remember current level, can jump down early just because shorter match found, but not necccessarily as short as the previous level

public class Tree {

	static int countMatch(String x,String y) {
		int i=0;
		while (i<x.length() && i<y.length()) {
			if (x.charAt(i) != y.charAt(i))
				return i;
			i++;
		}
		return i;
	}

	public static void main(String[] args) {

		String filename = args[0];
		
		try {

			java.io.BufferedReader read = new java.io.BufferedReader(
				new java.io.FileReader( new java.io.File( filename ) )
			);
		
			String line = read.readLine();
			String lastline = "";
			int lasti = 0;
			
			while ( ( line = read.readLine() ) != null ) {

				int same = countMatch(lastline,line);
				if (same>lasti)
					System.out.println("+ "+line.substring(0,same)+"{");
				if (same<lasti)
					// System.out.println("- "+lastline.substring(0,lasti)+"}");
					System.out.println("- }");
				System.out.println("."+same+" "+line);
				lastline = line;
				lasti = same;

			}

		} catch (Exception e) {
			jlib.JLib.error(e);
		}
		
	}
}


