package jlib;

import java.io.*;

import jlib.*;

public class JStream {

	/** Reads the InputStream, writing to the OutputStream, until
	 *  no more can be read.  Closes the InputStream, leaves the OutputStream open.
	 **/
	public static boolean pipe(InputStream in,OutputStream out) {
		try {
			while (true) {
				int a=in.available();
				if (a==0)
					a=1;
				byte[] bs=new byte[a];
				int numread=in.read(bs);
				if (numread==-1)
					break;
				out.write(bs,0,numread);
			}
			in.close();
			// out.close(); No it might still be wanted
			return true;
		} catch (Exception e) {
			JLib.report(e);
			return false;
		}
	}

	public static byte[] streamBytesFrom(InputStream in) {
		return streamBytes(in);
	}

	public static byte[] streamBytes(InputStream in) {
		// TODO: If in not buffered, buffer it!  (Is this worthwhile?)
		try {
			byte[] sofar=new byte[0];
			int tot=0;
			while (true) {
				int a=in.available();
				if (a==0)
					a=1;
				byte[] bs=new byte[a];
				int numread=in.read(bs);
				if (numread==-1)
					break;
				int newsize=tot+numread;
				if (newsize>sofar.length) {
					int size=newsize*2;
					byte[] newsofar=new byte[size];
					for (int i=0;i<tot;i++)
						newsofar[i]=sofar[i];
					sofar=newsofar;
				}
				for (int i=0;i<numread;i++)
					sofar[tot+i]=bs[i];
				tot+=numread;
			}
			in.close();
			byte[] end=new byte[tot];
			for (int i=0;i<tot;i++)
				end[i]=sofar[i];
			return end;
		} catch (Exception e) {
			JLib.report(e);
			return null;
		}
	}

}
