/******************************************************************************
 * FtpReader.java, part of the FtpClient object suite.
 * Copyright (C) 1998 Chris Cheetham
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * To contact the author, email cheetham@fooware.com.
 */

package com.fooware.net;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;


/**
 * This is an object that can be used as a java.io.Reader for the
 * reading of data from an FTP server.  You do not construct one
 * explicitly, but via an FtpClient:<BR>
 * <CODE><PRE>
 * FtpClient ftp = new FtpClient();
 * ftp.connect(someServer);
 * ftp.userName(myName);
 * ftp.password(myPassword);
 * ftp.dataPort();
 * <B>FtpReader in = ftp.list();
 * while (in.ready())
 *   System.out.println(in.readLine());
 * in.close();</B>
 * </PRE></CODE>
 * Note: unlike other Readers, it is important to explicitly close this
 * Reader when through.  This signals the FtpClient to get a response
 * from the FTP server.
 * @author <A HREF="mailto:cheetham@fooware.com">Chris Cheetham</A>
 * @version $Revision: 1.1 $
 **/
public class FtpReader
extends BufferedReader
{

  //
  // constructors
  //

  /**
   * Contruct an FtpReader for the specified FtpClient.
   **/
  FtpReader(Reader in, FtpClient client)
    throws IOException
    {
      super(in);
      m_client = client;
      while (!in.ready()) {
	try { Thread.sleep(50L); }
	catch (InterruptedException exc) {}
      }
    }

  /**
   * Close the underlying Reader and signal the FtpClient that
   * Reader processing has completed.
   **/
  public void close() 
    throws IOException
    {
      super.close();
      m_client.closeTransferSocket();
    }

  //
  // member variables
  //

  private FtpClient m_client;
  
}

/**
 * $Log: FtpReader.java,v $
 * Revision 1.1  2001/08/27 14:29:59  joey
 * Initial revision
 *
 * Revision 1.4  1998/08/19 05:57:52  cheetham
 * All of RFC959 implemented except for passive.
 *
 * Revision 1.3  1998/08/18 03:49:22  cheetham
 * Added more comments and formatting for HTML.
 *
 * Revision 1.2  1998/08/14 05:54:52  cheetham
 * 90% of base implementation.
 *
 * Revision 1.1  1998/08/09 01:38:53  cheetham
 * Genesis.
 *
 **/
