/******************************************************************************
 * FtpResponse.java, part of the FtpClient object suite.
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


/**
 * This class represents an FTP reply as outlined in section 4.2 of RFC959,
 * authors J. Postel and J. Reynolds, 1985.  The RFC can be viewed in its 
 * entirety at <A HREF="http://sunsite.auc.dk/RFC/rfc/rfc959.html">http://sunsite.auc.dk/RFC/rfc/rfc959.html</A>.
 * <P>
 * @author <A HREF="mailto:cheetham@fooware.com">Chris Cheetham</A>
 * @version $Revision: 1.1 $
 **/
public class FtpResponse
{

  /**
   * Construct a new FtpResponse, whose contents will be
   * derived from the BufferedReader.
   **/
  FtpResponse(BufferedReader in)
    throws IOException
    {
      setMessage(in);
    }

  /**
   * Return the reply from the FTP server in its entirety.
   **/
  public String getMessage()
    {
      return m_message;
    }

  /**
   * Return the 3-digit return code that is also the first 3 characters
   * of the FTP reply.
   **/
  public String getReturnCode()
    {
      return m_returnCode;
    }

  /**
   * Return a String representation of this object.  Return the same
   * as getMessage().
   **/
  public String toString()
    {
      return m_message;
    }

  /**
   * Returns <CODE>true</CODE> if the first character of the return
   * code indicates a positive prelimary reply, as outlined in RFC959.
   **/
  public boolean isPositivePreliminary()
    {
      return m_returnCode.charAt(0) == REPLY_POSITIVE_PRELIMINARY;
    }

  /**
   * Returns <CODE>true</CODE> if the first character of the return
   * code indicates a positive completion reply, as outlined in RFC959.
   **/
  public boolean isPositiveCompletion()
    {
      return m_returnCode.charAt(0) == REPLY_POSITIVE_COMPLETION;
    }

  /**
   * Returns <CODE>true</CODE> if the first character of the return
   * code indicates a positive intermediary reply, as outlined in RFC959.
   **/
  public boolean isPositiveIntermediary()
    {
      return m_returnCode.charAt(0) == REPLY_POSITIVE_INTERMEDIARY;
    }

  /**
   * Returns <CODE>true</CODE> if the first character of the return
   * code indicates a transient negative reply, as outlined in RFC959.
   **/
  public boolean isTransientNegativeCompletion()
    {
      return m_returnCode.charAt(0) == REPLY_TRANSIENT_NEGATIVE_COMPLETION;
    }

  /**
   * Returns <CODE>true</CODE> if the first character of the return
   * code indicates a permanent negative reply, as outlined in RFC959.
   **/
  public boolean isPermanentNegativeCompletion()
    {
      return m_returnCode.charAt(0) == REPLY_PERMANENT_NEGATIVE_COMPLETION;
    }

  /**
   * Returns <CODE>true</CODE> if the second character of the return
   * code indicates a reply pertaining to syntax (or maybe just superfluous), 
   * as outlined in RFC959.
   **/
  public boolean isRegardingSyntax()
    {
      return m_returnCode.charAt(1) == REGARDING_SYNTAX;
    }

  /**
   * Returns <CODE>true</CODE> if the second character of the return
   * code indicates a reply pertaining to information, as outlined in RFC959.
   **/
  public boolean isRegardingInformation()
    {
      return m_returnCode.charAt(1) == REGARDING_INFORMATION;
    }

  /**
   * Returns <CODE>true</CODE> if the second character of the return
   * code indicates a reply pertaining to connection, as outlined in RFC959.
   **/
  public boolean isRegardingConnection()
    {
      return m_returnCode.charAt(1) == REGARDING_CONNECTION;
    }

  /**
   * Returns <CODE>true</CODE> if the second character of the return
   * code indicates a reply pertaining to authentication, as outlined in 
   * RFC959.
   **/
  public boolean isRegardingAuthentication()
    {
      return m_returnCode.charAt(1) == REGARDING_AUTHENTICATION;
    }

  /**
   * Returns <CODE>true</CODE> if the second character of the return
   * code indicates a reply pertaining to file system, as outlined in 
   * RFC959.
   **/
  public boolean isRegardingFileSystem()
    {
      return m_returnCode.charAt(1) == REGARDING_FILE_SYSTEM;
    }

  private void setMessage(BufferedReader in)
    throws IOException
    {
      StringBuffer buffer = new StringBuffer();
      while (true) {
	while (!in.ready()) {
	  try { Thread.sleep(10L); }
	  catch (InterruptedException exc) {}
	}
	String line = in.readLine();
	if (m_returnCode == null)
	  m_returnCode = line.substring(0, 3);
	buffer.append(line);
	buffer.append('\n');
	if (line.charAt(3) == ' ')
	  if (m_returnCode.equals( line.substring(0, 3) ))
	    break;
      }
      m_message = buffer.toString();
    }

  private String m_message;
  private String m_returnCode;

  static final public char REPLY_POSITIVE_PRELIMINARY = '1';
  static final public char REPLY_POSITIVE_COMPLETION = '2';
  static final public char REPLY_POSITIVE_INTERMEDIARY = '3';
  static final public char REPLY_TRANSIENT_NEGATIVE_COMPLETION = '4';
  static final public char REPLY_PERMANENT_NEGATIVE_COMPLETION = '5';

  static final public char REGARDING_SYNTAX = '0';
  static final public char REGARDING_INFORMATION = '1';
  static final public char REGARDING_CONNECTION = '2';
  static final public char REGARDING_AUTHENTICATION = '3';
  static final public char REGARDING_UNSPECIFIED = '4';
  static final public char REGARDING_FILE_SYSTEM = '5';

}

/**
 * $Log: FtpResponse.java,v $
 * Revision 1.1  2001/08/27 14:29:59  joey
 * Initial revision
 *
 * Revision 1.5  1998/08/19 05:57:52  cheetham
 * All of RFC959 implemented except for passive.
 *
 * Revision 1.4  1998/08/18 03:49:23  cheetham
 * Added more comments and formatting for HTML.
 *
 * Revision 1.3  1998/08/14 05:54:52  cheetham
 * 90% of base implementation.
 *
 * Revision 1.2  1998/08/09 01:41:10  cheetham
 * Re-worked.  Now stores the entire message as it was received, as well as
 * the 3 character ftp return code embedded at the front of the message.
 *
 * Revision 1.1  1998/08/06 04:58:43  cheetham
 * Genesis.
 *
 **/
