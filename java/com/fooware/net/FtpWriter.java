/******************************************************************************
 * FtpWriter.java, part of the FtpClient object suite.
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

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.Writer;

/**
 * This is an object that can be used as a java.io.Writer for the
 * writing of data to an FTP server.  You do not construct one
 * explicitly, but via an FtpClient:<BR>
 * <CODE><PRE>
 * FtpClient ftp = new FtpClient();
 * ftp.connect(someServer);
 * ftp.userName(myName);
 * ftp.password(myPassword);
 * ftp.dataPort();
 * <B>FtpWriter out = ftp.store(targetFile);
 * while (someReader.ready())
 *   out.write(someReader.read());
 * in.close();</B>
 * </PRE></CODE>
 * Note: unlike other Writers, it is important to explicitly close this
 * Writer when through.  This signals the FtpClient to get a response
 * from the FTP server.
 * @author <A HREF="mailto:cheetham@fooware.com">Chris Cheetham</A>
 * @version $Revision: 1.1 $
 **/
public class FtpWriter
extends BufferedWriter
{

  //
  // constructors
  //

  /**
   * Contruct an FtpWriter for the specified FtpClient.
   **/
  FtpWriter(Writer out, FtpClient client)
    throws IOException
    {
      super(out, 1024);
      m_client = client;
    }

  /**
   * Close the underlying Writer and signal the FtpClient that
   * Writer processing has completed.
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
 * $Log: FtpWriter.java,v $
 * Revision 1.1  2001/08/27 14:29:59  joey
 * Initial revision
 *
 * Revision 1.3  1998/08/19 05:57:52  cheetham
 * All of RFC959 implemented except for passive.
 *
 * Revision 1.2  1998/08/18 03:49:23  cheetham
 * Added more comments and formatting for HTML.
 *
 * Revision 1.1  1998/08/14 06:01:02  cheetham
 * Genesis.
 *
 **/
