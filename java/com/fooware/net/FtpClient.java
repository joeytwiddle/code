/******************************************************************************
 * FtpClient.java, part of the FtpClient object suite.
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
import java.io.FileReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Enumeration;
import java.util.Vector;


/**
 * Most of the method descriptions have been copied from RFC959, authors 
 * J. Postel and J. Reynolds, 1985. The RFC can be viewed in its entirety
 * at <A HREF="http://sunsite.auc.dk/RFC/rfc/rfc959.html">http://sunsite.auc.dk/RFC/rfc/rfc959.html</A>.
 * <P>
 * Click <A HREF="http://www.fooware.com/docs/API_ftp_client.html">here</A> to 
 * visit the user's guide.
 * @author <A HREF="mailto:cheetham@fooware.com">Chris Cheetham</A>
 * @version $Revision: 1.1 $
 **/
public class FtpClient
{

  //
  // public methods
  //

  /**
   * Return the last response received from the FTP server.  The response
   * are stored in a collection, so repeatedly calling this method will
   * return the same response.  New repsonses are added as requests are
   * issued to the FTP server and when data connections are closed.
   **/
  public FtpResponse getResponse()
    {
      return m_response;
    }
  
  /**
   * Return an enumeration of all the responses received from the FTP
   * server.
   **/
  public Enumeration getAllResponses()
    {
      return m_responseArchive.elements();
    }

  /**
   * Connect to the server at the host on the default FTP port.
   **/
  public void connect(String hostName)
    throws IOException 
    {
      connect(hostName, DEFAULT_PORT);
    }

  /**
   * Connect to the server at the host on the port.
   **/
  public void connect(String hostName, int port)
    throws IOException 
    {
      Socket sock = new Socket(hostName, port);
      m_in = new BufferedReader(new InputStreamReader(sock.getInputStream()));
      m_out = new PrintWriter(sock.getOutputStream(), true);
      setResponse();
    }

  /**
   * USER NAME (USER) <BR>
   * The argument field is a Telnet string identifying the user.
   * The user identification is that which is required by the
   * server for access to its file system.  This command will
   * normally be the first command transmitted by the user after
   * the control connections are made (some servers may require
   * this).  Additional identification information in the form of
   * a password and/or an account command may also be required by
   * some servers.  Servers may allow a new USER command to be
   * entered at any point in order to change the access control
   * and/or accounting information.  This has the effect of
   * flushing any user, password, and account information already
   * supplied and beginning the login sequence again.  All
   * transfer parameters are unchanged and any file transfer in
   * progress is completed under the old access control
   * parameters.
   * <BR><I>Per RFC959</I>
   **/
  public void userName(String userName)
    throws IOException
    {
      sendCommand("USER " + userName);
    }

  /**
   * PASSWORD (PASS) <BR>
   * The argument field is a Telnet string specifying the user's
   * password.  This command must be immediately preceded by the
   * user name command, and, for some sites, completes the user's
   * identification for access control.  Since password
   * information is quite sensitive, it is desirable in general
   * to "mask" it or suppress typeout.  It appears that the
   * server has no foolproof way to achieve this.  It is
   * therefore the responsibility of the user-FTP process to hide
   * the sensitive password information.
   * <BR><I>Per RFC959</I>
   **/
  public void password(String password)
    throws IOException
    {
      sendCommand("PASS " + password);
    }

  /**
   * ACCOUNT (ACCT) <BR>
   * The argument field is a Telnet string identifying the user's
   * account.  The command is not necessarily related to the USER
   * command, as some sites may require an account for login and
   * others only for specific access, such as storing files.  In
   * the latter case the command may arrive at any time. 
   * There are reply codes to differentiate these cases for the
   * automation: when account information is required for login,
   * the response to a successful PASSword command is reply code
   * 332.  On the other hand, if account information is NOT
   * required for login, the reply to a successful PASSword
   * command is 230; and if the account information is needed for
   * a command issued later in the dialogue, the server should
   * return a 332 or 532 reply depending on whether it stores
   * (pending receipt of the ACCounT command) or discards the
   * command, respectively.
   * <BR><I>Per RFC959</I>
   **/
  public void account(String account)
    throws IOException
    {
      sendCommand("ACCT " + account);
    }

  /**
   * CHANGE WORKING DIRECTORY (CWD) <BR>
   * This command allows the user to work with a different
   * directory or dataset for file storage or retrieval without
   * altering his login or accounting information.  Transfer
   * parameters are similarly unchanged.  The argument is a
   * pathname specifying a directory or other system dependent
   * file group designator.
   * <BR><I>Per RFC959</I>
   **/
  public void changeWorkingDirectory(String path)
    throws IOException
    {
      sendCommand("CWD " + path);
    }

  /**
   * CHANGE TO PARENT DIRECTORY (CDUP) <BR>
   * This command is a special case of CWD, and is included to
   * simplify the implementation of programs for transferring
   * directory trees between operating systems having different    
   * syntaxes for naming the parent directory.  The reply codes
   * shall be identical to the reply codes of CWD.
   * <BR><I>Per RFC959</I>
   **/
  public void changeToParentDirectory()
    throws IOException
    {
      sendCommand("CDUP");
    }

  /**
   * STRUCTURE MOUNT (SMNT) 
   * This command allows the user to mount a different file
   * system data structure without altering his login or
   * accounting information.  Transfer parameters are similarly
   * unchanged.  The argument is a pathname specifying a
   * directory or other system dependent file group designator.
   * <BR><I>Per RFC959</I>
   **/
  public void structureMount(String path)
    throws IOException
    {
      sendCommand("SMNT " + path);
    }

  /**
   * REINITIALIZE (REIN) <BR>
   * This command terminates a USER, flushing all I/O and account
   * information, except to allow any transfer in progress to be
   * completed.  All parameters are reset to the default settings
   * and the control connection is left open.  This is identical
   * to the state in which a user finds himself immediately after
   * the control connection is opened.  A USER command may be
   * expected to follow.
   * <BR><I>Per RFC959</I>
   **/
  public void reinitialize()
    throws IOException
    {
      sendCommand("REIN");
    }

  /**
   * LOGOUT (QUIT) <BR>
   * This command terminates a USER and if file transfer is not
   * in progress, the server closes the control connection.  If
   * file transfer is in progress, the connection will remain
   * open for result response and the server will then close it.
   * If the user-process is transferring files for several USERs
   * but does not wish to close and then reopen connections for
   * each, then the REIN command should be used instead of QUIT. 
   * <BR>
   * An unexpected close on the control connection will cause the
   * server to take the effective action of an abort (ABOR) and a
   * logout (QUIT). 
   * <BR><I>Per RFC959</I>
   **/
  public void logout()
    throws IOException 
    {
      sendCommand("QUIT");
    }

  /**
   * DATA PORT (PORT) <BR>
   * The argument is a HOST-PORT specification for the data port
   * to be used in data connection.  There are defaults for both
   * the user and server data ports, and under normal
   * circumstances this command and its reply are not needed.  If
   * this command is used, the argument is the concatenation of a
   * 32-bit internet host address and a 16-bit TCP port address.
   * This address information is broken into 8-bit fields and the
   * value of each field is transmitted as a decimal number (in
   * character string representation).  The fields are separated
   * by commas.  A port command would be: 
   * <BR>PORT h1,h2,h3,h4,p1,p2 <BR>
   * where h1 is the high order 8 bits of the internet host
   *  address.
   * <BR><I>Per RFC959</I><BR>
   * <B>Note:</B>
   * The preceeding excerpt is provided for informational purposes.
   * For *this* ftp client, you must issue at least one dataPort()
   * command is you plan to do any ftp processing that uses the
   * data connection rather than the command connect, e.g. retrieve.
   * Per the RFC, there is a default user port number, but I have yet
   * to find the port number.  It is not necessary to do more than one.<BR>
   **/
  public void dataPort()
    throws IOException, UnknownHostException
    {
      StringBuffer command = new StringBuffer("PORT ");
      String host = InetAddress.getLocalHost().getHostAddress();
      command.append(host.replace('.', ','));
      if (m_dataSocket != null)
	m_dataSocket.close();
      m_dataSocket = new ServerSocket(0);
      int port = m_dataSocket.getLocalPort();
      command.append(',');
      command.append(port/256);
      command.append(',');
      command.append(port%256);
      sendCommand(command.toString());
    }

  /**
   * PASSIVE (PASV) <BR>
   * This command requests the server-DTP to "listen" on a data
   * port (which is not its default data port) and to wait for a
   * connection rather than initiate one upon receipt of a
   * transfer command.  The response to this command includes the
   * host and port address this server is listening on.
   * <BR><I>Per RFC959</I><BR>
   * <B>Note:</B> This one isn't implemented yet and will throw
   *  an exception if used.
   * @exception com.fooware.dev.NotImplementedException always
   **/
  public void passive()
    throws IOException
    {
      throw new IOException("passive()");
    }

  /**
   * REPRESENTATION TYPE (TYPE) <BR>
   * The argument specifies the representation type as described
   * in the Section on Data Representation and Storage.  Several
   * types take a second parameter.  The first parameter is
   * denoted by a single Telnet character, as is the second
   * Format parameter for ASCII and EBCDIC; the second parameter
   * for local byte is a decimal integer to indicate Bytesize.
   * The parameters are separated by a <SP> (Space, ASCII code
   * 32). The following codes are assigned for type: <BR>
   * <CODE><PRE>
   *              \    /               
   *    A - ASCII |    | N - Non-print
   *              |-><-| T - Telnet format effectors
   *    E - EBCDIC|    | C - Carriage Control (ASA)
   *              /    \               
   *    I - Image               
   *
   *    L <byte size> - Local byte Byte size  
   * </PRE></CODE>
   * <BR>
   * The default representation type is ASCII Non-print.  If the
   * Format parameter is changed, and later just the first
   * argument is changed, Format then returns to the Non-print
   * default.
   * <BR><I>Per RFC959</I><BR>
   * For convenience, the following constants can be used:
   * <TABLE>
   *  <TR>
   *    <TD>
   *      <UL>
   *        <LI>ASCII_TYPE</LI>
   *        <LI>EBCDIC_TYPE</LI>
   *        <LI>IMAGE_TYPE</LI>
   *      </UL>
   *    </TD>
   *    <TD>
   *      <UL>
   *        <LI>NON_PRINT_FORMAT</LI>
   *        <LI>TELNET_EFFECTORS_FORMAT</LI>
   *        <LI>CARRIAGE_CONTROL_FORMAT</LI>
   *      </UL>
   *    </TD>
   *  </TR>
   * </TABLE>
   **/
  public void representationType(char type, char format)
    throws IOException
    {
      sendCommand("TYPE " + type + ' ' + format);
    }

  /**
   * @see #representationType(char, char)
   **/
  public void representationType(char type)
    throws IOException
    {
      representationType(type, NON_PRINT_FORMAT);
    }

  /**
   * Special case of specifying the local byte size.
   **/
  public void representationType(int size)
    throws IOException
    {
      sendCommand("TYPE L " + size);
    }

  /**
   * FILE STRUCTURE (STRU) <BR>
   * The argument is a single Telnet character code specifying
   * file structure described in the Section on Data
   * Representation and Storage. 
   * The following codes are assigned for structure: <BR>
   * F - File (no record structure) <BR>
   * R - Record structure <BR>
   * P - Page structure <BR>
   * The default structure is File.
   * <BR><I>Per RFC959</I><BR>
   * For convenience, the following constants can be used:
   * <TABLE>
   *  <TR>
   *    <TD>
   *      <UL>
   *        <LI>FILE_STRUCTURE</LI>
   *        <LI>RECORD_STRUCTURE</LI>
   *        <LI>PAGE_STRUCTURE</LI>
   *      </UL>
   *    </TD>
   *  </TR>
   * </TABLE>
   **/
  public void structure(char structure)
    throws IOException
    {
      sendCommand("STRU " + structure);
    }

  /**
   * TRANSFER MODE (MODE) <BR>
   * The argument is a single Telnet character code specifying
   * the data transfer modes described in the Section on
   * Transmission Modes. 
   * The following codes are assigned for transfer modes: <BR>
   * <CODE><PRE>
   *   S - Stream
   *   B - Block
   *   C - Compressed 
   * </PRE></CODE>
   * The default transfer mode is Stream.
   * <BR><I>Per RFC959</I><BR>
   * For convenience, the following constants can be used:
   * <TABLE>
   *  <TR>
   *    <TD>
   *      <UL>
   *        <LI>STREAM_MODE</LI>
   *        <LI>BLOCK_MODE</LI>
   *        <LI>COMPRESSED_MODE</LI>
   *      </UL>
   *    </TD>
   *  </TR>
   * </TABLE>
   **/
  public void transferMode(char mode)
    throws IOException
    {
      sendCommand("MODE " + mode);
    }

  /**
   * RETRIEVE (RETR) <BR>
   * This command causes the server-DTP to transfer a copy of the
   * file, specified in the pathname, to the server- or user-DTP
   * at the other end of the data connection.  The status and
   * contents of the file at the server site shall be unaffected.
   * <BR><I>Per RFC959</I>
   * <BR>
   * <B>Note:</B> It is important that the reader be closed after
   * it is done being processed.  This signals the ftp client to get
   * the next response from the server.
   * @return a reader from which can be read the retrieved file data
   **/
  public FtpReader retrieve(String path)
    throws IOException
    {
      sendCommand("RETR " + path);
      if (!getResponse().isPositivePreliminary())
	return null;
      m_dataXfrSocket = m_dataSocket.accept();
      InputStream istr = m_dataXfrSocket.getInputStream();
      InputStreamReader in = new InputStreamReader(istr);
      return new FtpReader(in, this);
    }

  /**
   * STORE (STOR) <BR>
   * This command causes the server-DTP to accept the data
   * transferred via the data connection and to store the data as
   * a file at the server site.  If the file specified in the
   * pathname exists at the server site, then its contents shall
   * be replaced by the data being transferred.  A new file is
   * created at the server site if the file specified in the
   * pathname does not already exist.
   * <BR><I>Per RFC959</I>
   * <BR>
   * <B>Note:</B> It is important that the writer be closed after
   * it is done being processed.  This signals the ftp client to get
   * the next response from the server.
   * @return a writer into which can be written the data for the host file
   **/
  public FtpWriter store(String path)
    throws IOException
    {
      sendCommand("STOR " + path);
      if (!getResponse().isPositivePreliminary())
	return null;
      m_dataXfrSocket = m_dataSocket.accept();
      OutputStream ostr = m_dataXfrSocket.getOutputStream();
      OutputStreamWriter out = new OutputStreamWriter(ostr);
      return new FtpWriter(out, this);
    }

  /**
   * STORE UNIQUE (STOU) <BR>
   * This command behaves like STOR except that the resultant
   * file is to be created in the current directory under a name
   * unique to that directory.  The 250 Transfer Started response
   * must include the name generated.  
   * <BR><I>Per RFC959</I>
   * <BR>
   * <B>Note:</B> It is important that the writer be closed after
   * it is done being processed.  This signals the ftp client to get
   * the next response from the server.
   * @return a writer into which can be written the data for the host file
   **/
  public FtpWriter storeUnique()
    throws IOException
    {
      sendCommand("STOU");
      if (!getResponse().isPositivePreliminary())
	return null;
      m_dataXfrSocket = m_dataSocket.accept();
      OutputStream ostr = m_dataXfrSocket.getOutputStream();
      OutputStreamWriter out = new OutputStreamWriter(ostr);
      return new FtpWriter(out, this);
    }

  /**
   * APPEND (with create) (APPE) <BR>
   * This command causes the server-DTP to accept the data
   * transferred via the data connection and to store the data in
   * a file at the server site.  If the file specified in the
   * pathname exists at the server site, then the data shall be
   * appended to that file; otherwise the file specified in the
   * pathname shall be created at the server site.
   * <BR><I>Per RFC959</I>
   * <BR>
   * <B>Note:</B> It is important that the writer be closed after
   * it is done being processed.  This signals the ftp client to get
   * the next response from the server.
   * @return a writer into which can be written the data for the host file
   **/
  public FtpWriter append(String path)
    throws IOException
    {
      sendCommand("APPE " + path);
      if (!getResponse().isPositivePreliminary())
	return null;
      m_dataXfrSocket = m_dataSocket.accept();
      OutputStream ostr = m_dataXfrSocket.getOutputStream();
      OutputStreamWriter out = new OutputStreamWriter(ostr);
      return new FtpWriter(out, this);
    }

  /**
   * ALLOCATE (ALLO) <BR>
   * This command may be required by some servers to reserve
   * sufficient storage to accommodate the new file to be
   * transferred.  The argument shall be a decimal integer
   * representing the number of bytes (using the logical byte
   * size) of storage to be reserved for the file.  For files
   * sent with record or page structure a maximum record or page
   * size (in logical bytes) might also be necessary; this is
   * indicated by a decimal integer in a second argument field of  
   * the command.  This second argument is optional, but when
   * present should be separated from the first by the three
   * Telnet characters [SP] R [SP].  This command shall be
   * followed by a STORe or APPEnd command.  The ALLO command
   * should be treated as a NOOP (no operation) by those servers
   * which do not require that the maximum size of the file be
   * declared beforehand, and those servers interested in only
   * the maximum record or page size should accept a dummy value
   * in the first argument and ignore it.
   * <BR><I>Per RFC959</I>
   **/
  public void allocate(int size, int recSize)
    throws IOException
    {
      sendCommand("ALLO " + size + " R " + recSize);
    }

  /**
   * @see #allocate(int, int)
   **/
  public void allocate(int size)
    throws IOException
    {
      sendCommand("ALLO " + size);
    }

  /**
   * RESTART (REST) <BR>
   * The argument field represents the server marker at which
   * file transfer is to be restarted.  This command does not
   * cause file transfer but skips over the file to the specified
   * data checkpoint.  This command shall be immediately followed
   * by the appropriate FTP service command which shall cause
   * file transfer to resume.  
   * <BR><I>Per RFC959</I>
   **/
  public void restart(String marker)
    throws IOException
    {
      sendCommand("REST " + marker);
    }

  /**
   * RENAME FROM (RNFR) <BR>
   * This command specifies the old pathname of the file which is
   * to be renamed.  This command must be immediately followed by
   * a "rename to" command specifying the new file pathname.  
   * <BR><I>Per RFC959</I>
   * @see #renameTo(String)
   **/
  public void renameFrom(String path)
    throws IOException
    {
      sendCommand("RNFR " + path);
    }

  /**
   * RENAME TO (RNTO) <BR>
   * This command specifies the new pathname of the file
   * specified in the immediately preceding "rename from"
   * command.  Together the two commands cause a file to be
   * renamed.
   * <BR><I>Per RFC959</I>
   * @see #renameFrom(String)
   **/
  public void renameTo(String path)
    throws IOException
    {
      sendCommand("RNTO " + path);
    }

  /**
   * ABORT (ABOR) <BR>
   * This command tells the server to abort the previous FTP
   * service command and any associated transfer of data.  The
   * abort command may require "special action", as discussed in
   * the Section on FTP Commands, to force recognition by the
   * server.  No action is to be taken if the previous command
   * has been completed (including data transfer).  The control
   * connection is not to be closed by the server, but the data
   * connection must be closed. <BR>
   * There are two cases for the server upon receipt of this
   * command: (1) the FTP service command was already completed,
   * or (2) the FTP service command is still in progress.   
   * In the first case, the server closes the data connection
   * (if it is open) and responds with a 226 reply, indicating
   * that the abort command was successfully processed. <BR>
   * In the second case, the server aborts the FTP service in
   * progress and closes the data connection, returning a 426
   * reply to indicate that the service request terminated
   * abnormally.  The server then sends a 226 reply,
   * indicating that the abort command was successfully
   * processed.  
   * <BR><I>Per RFC959</I>
   **/
  public void abort()
    throws IOException
    {
      sendCommand("ABOR");
    }

  /**
   * DELETE (DELE) 
   * This command causes the file specified in the pathname to be
   * deleted at the server site.  If an extra level of protection
   * is desired (such as the query, "Do you really wish to
   * delete?"), it should be provided by the user-FTP process.
   * <BR><I>Per RFC959</I>
   **/
  public void delete(String path)
    throws IOException
    {
      sendCommand("DELE " + path);
    }

  /**
   * REMOVE DIRECTORY (RMD) <BR>
   * This command causes the directory specified in the pathname
   * to be removed as a directory (if the pathname is absolute)
   * or as a subdirectory of the current working directory (if
   * the pathname is relative).    
   * <BR><I>Per RFC959</I>
   **/
  public void removeDirectory(String path)
    throws IOException
    {
      sendCommand("RMD " + path);
    }

  /**
   * MAKE DIRECTORY (MKD) <BR>
   * This command causes the directory specified in the pathname
   * to be created as a directory (if the pathname is absolute)
   * or as a subdirectory of the current working directory (if
   * the pathname is relative).    
   * <BR><I>Per RFC959</I>
   **/
  public void makeDirectory(String path)
    throws IOException
    {
      sendCommand("MKD " + path);
    }

  /**
   * PRINT WORKING DIRECTORY (PWD) <BR>
   * This command causes the name of the current working
   * directory to be returned in the reply.  
   * <BR><I>Per RFC959</I>
   **/
  public void printWorkingDirectory()
    throws IOException
    {
      sendCommand("PWD");
    }

  /**
   * LIST (LIST) <BR>
   * This command causes a list to be sent from the server to the
   * passive DTP.  If the pathname specifies a directory or other
   * group of files, the server should transfer a list of files
   * in the specified directory.  If the pathname specifies a
   * file then the server should send current information on the
   * file.  A null argument implies the user's current working or
   * default directory.  The data transfer is over the data
   * connection in type ASCII or type EBCDIC.  (The user must  
   * ensure that the TYPE is appropriately ASCII or EBCDIC).
   * Since the information on a file may vary widely from system
   * to system, this information may be hard to use automatically
   * in a program, but may be quite useful to a human user.
   * <BR><I>Per RFC959</I>
   * <BR>
   * <B>Note:</B> It is important that the reader be closed after
   * it is done being processed.  This signals the ftp client to get
   * the next response from the server.
   * @return a reader from which can be read the retrieved file data
   **/ 
  public FtpReader list(String path)
    throws IOException
    {
      if (path == null)
	sendCommand("LIST");
      else
	sendCommand("LIST " + path);
      if (!getResponse().isPositivePreliminary())
	return null;
      m_dataXfrSocket = m_dataSocket.accept();
      InputStream istr = m_dataXfrSocket.getInputStream();
      InputStreamReader in = new InputStreamReader(istr);
      return new FtpReader(in, this);
    }

  /**
   * @see #list(String)
   **/
  public FtpReader list()
    throws IOException
    {
      return list(null);
    }


  /**
   * NAME LIST (NLST) 
   * This command causes a directory listing to be sent from
   * server to user site.  The pathname should specify a
   * directory or other system-specific file group descriptor; a
   * null argument implies the current directory.  The server
   * will return a stream of names of files and no other
   * information.  The data will be transferred in ASCII or
   * EBCDIC type over the data connection as valid pathname
   * strings separated by <CRLF> or <NL>.  (Again the user must
   * ensure that the TYPE is correct.)  This command is intended
   * to return information that can be used by a program to
   * further process the files automatically.  For example, in
   * the implementation of a "multiple get" function.  
   * <BR><I>Per RFC959</I>
   * <BR>
   * <B>Note:</B> It is important that the reader be closed after
   * it is done being processed.  This signals the ftp client to get
   * the next response from the server.
   * @return a reader from which can be read the retrieved file data
   **/
  public FtpReader nameList(String path)
    throws IOException
    {
      if (path == null)
	sendCommand("NLIST");
      else
	sendCommand("NLST " + path);
      if (!getResponse().isPositivePreliminary())
	return null;
      m_dataXfrSocket = m_dataSocket.accept();
      InputStream istr = m_dataXfrSocket.getInputStream();
      InputStreamReader in = new InputStreamReader(istr);
      return new FtpReader(in, this);
    }

  /**
   * @see #nameList(String)
   **/
  public FtpReader nameList()
    throws IOException
    {
      return nameList(null);
    }

  /**
   * SITE PARAMETERS (SITE) <BR>
   * This command is used by the server to provide services
   * specific to his system that are essential to file transfer
   * but not sufficiently universal to be included as commands in
   * the protocol.  The nature of these services and the
   * specification of their syntax can be stated in a reply to
   * the HELP SITE command.  
   * <BR><I>Per RFC959</I>
   **/
  public void siteParameters(String param)
    throws IOException
    {
      sendCommand("SITE " + param);
    }

  /**
   * SYSTEM (SYST) <BR>
   * This command is used to find out the type of operating
   * system at the server.  The reply shall have as its first
   * word one of the system names listed in the current version
   * of the Assigned Numbers document [4].
   * <BR><I>Per RFC959</I>
   **/
  public void system()
    throws IOException
    {
      sendCommand("SYST");
    }

  /**
   * STATUS (STAT) <BR>
   * This command shall cause a status response to be sent over
   * the control connection in the form of a reply.  The command
   * may be sent during a file transfer (along with the Telnet IP
   * and Synch signals--see the Section on FTP Commands) in which
   * case the server will respond with the status of the
   * operation in progress, or it may be sent between file
   * transfers.  In the latter case, the command may have an
   * argument field.  If the argument is a pathname, the command
   * is analogous to the "list" command except that data shall be  
   * transferred over the control connection.  If a partial
   * pathname is given, the server may respond with a list of
   * file names or attributes associated with that specification.
   * If no argument is given, the server should return general
   * status information about the server FTP process.  This
   * should include current values of all transfer parameters and
   * the status of connections.  
   * <BR><I>Per RFC959</I>
   **/
  public void status(String path)
    throws IOException
    {
      if (path == null)
	sendCommand("STAT");
      else
	sendCommand("STAT " + path);
    }

  /**
   * @see #status(String)
   **/
  public void status()
    throws IOException
    {
      status(null);
    }

  /**
   * HELP (HELP) <BR>
   * This command shall cause the server to send helpful
   * information regarding its implementation status over the
   * control connection to the user.  The command may take an
   * argument (e.g., any command name) and return more specific
   * information as a response.  The reply is type 211 or 214.
   * It is suggested that HELP be allowed before entering a USER
   * command. The server may use this reply to specify
   * site-dependent parameters, e.g., in response to HELP SITE.  
   * <BR><I>Per RFC959</I>
   **/
  public void help(String arg)
    throws IOException
    {
      if (arg == null)
	sendCommand("HELP");
      else
	sendCommand("HELP " + arg);
    }

  /**
   * @see #help(String)
   **/
  public void help()
    throws IOException
    {
      help(null);
    }
  
  /**
   * NOOP (NOOP) <BR>
   * This command does not affect any parameters or previously
   * entered commands. It specifies no action other than that the
   * server send an OK reply.
   * <BR><I>Per RFC959</I>
   **/
  public void noOp()
    throws IOException
    {
      sendCommand("NOOP");
    }

  //
  // package methods
  //

  /**
   Set the current response.  Also, add the response to the
   * archive.
   **/
  void setResponse()
    throws IOException 
    {
      m_response = new FtpResponse(m_in);
      m_responseArchive.addElement(m_response);
    }

  /**
   * Called by the FtpReader and FtpWriter classes when they are
   * explicitly closed.
   **/
  void closeTransferSocket()
    throws IOException 
    {
      m_dataXfrSocket.close();
      setResponse();
    }

  //
  // private methods
  //

  /**
   * Send the command string to the FTP server.  Then get the response.
   **/
  private void sendCommand(String command)
    throws IOException
    {
      m_out.println(command);
      setResponse();
    }

  //
  // member variables
  //

  /**
   * The input stream from the FTP server.
   **/
  private BufferedReader m_in;

  /**
   * The output stream to the FTP server.
   **/
  private PrintWriter m_out;

  /**
   * Accepts data connections from the FTP server.
   **/
  private ServerSocket m_dataSocket;

  /**
   * The accepted connection from m_dataSocket.
   **/
  private Socket m_dataXfrSocket;

  /**
   * The last response from the FTP server.
   **/
  private FtpResponse m_response;

  /**
   * All responses from the FTP server.
   **/
  private Vector m_responseArchive = new Vector();

  //
  // class methods
  //

  /**
   * Run from the command line.
   **/
  public static void main(String[] args)
    {
      java.io.PrintStream out = System.out;
      if (args.length != 1) {
	out.println("\nusage:\n\tjava com.fooware.net.FtpClient hostname\n");
	return;
      }
      try {
	FtpClient ftp = new FtpClient();
	ftp.connect(args[0]);
	if (!ftp.getResponse().isPositiveCompletion()) {
	  out.println("Failed to connect to " + args[0] + ".");
	  return;
	}
	ftp.userName("anonymous");
	if (!ftp.getResponse().isPositiveIntermediary()) {
	  out.println("Anonymous ftp not supported at " + args[0] + ".");
	  return;
	}
	ftp.password("nobody@nowhere.com");
	if (!ftp.getResponse().isPositiveCompletion()) {
	  out.println("Password not accepted.");
	  return;
	}
	ftp.representationType(ASCII_TYPE);
	if (!ftp.getResponse().isPositiveCompletion()) {
	  out.println("TYPE ASCII failed.");
	  return;
	}
	ftp.dataPort();
	if (!ftp.getResponse().isPositiveCompletion()) {
	  out.println("PORT failed.");
	  return;
	}
	FtpReader ftpin = ftp.list();
	if (!ftp.getResponse().isPositivePreliminary()) {
	  out.println("LIST failed.");
	  return;
	}
	while (ftpin.ready())
	  System.out.println(ftpin.readLine());
	ftpin.close();
	if (!ftp.getResponse().isPositiveCompletion()) {
	  out.println("List processing failed.");
	  return;
	}
	ftp.logout();
	if (!ftp.getResponse().isPositiveCompletion()) {
	  out.println("Logout failed.");
	  return;
	}
      }
      catch (Exception exc) {
	exc.printStackTrace();
      }
    }
	
  //
  // class variables
  //

  static final public int DEFAULT_PORT = 21;

  static final public char ASCII_TYPE = 'A';
  static final public char IMAGE_TYPE = 'I';
  static final public char EBCDIC_TYPE = 'E';

  static final public char LOCAL_FORMAT = 'L';
  static final public char NON_PRINT_FORMAT = 'N';
  static final public char TELNET_EFFECTORS_FORMAT = 'T';
  static final public char CARRIAGE_CONTROL_FORMAT = 'C';

  static final public char FILE_STRUCTURE = 'F';
  static final public char RECORD_STRUCTURE = 'R';
  static final public char PAGE_STRUCTURE = 'P';

  static final public char STREAM_MODE = 'S';
  static final public char BLOCK_MODE = 'B';
  static final public char COMPRESSED_MODE = 'C';

}

/**
 * $Log: FtpClient.java,v $
 * Revision 1.1  2001/08/27 14:29:59  joey
 * Initial revision
 *
 * Revision 1.6  1998/08/19 05:57:52  cheetham
 * All of RFC959 implemented except for passive.
 *
 * Revision 1.5  1998/08/18 03:49:22  cheetham
 * Added more comments and formatting for HTML.
 *
 * Revision 1.4  1998/08/15 03:57:19  cheetham
 * Added comments.
 *
 * Revision 1.3  1998/08/14 05:54:52  cheetham
 * 90% of base implementation.
 *
 * Revision 1.2  1998/08/09 01:41:36  cheetham
 * Still implementing.
 *
 * Revision 1.1  1998/08/06 04:58:42  cheetham
 * Genesis.
 *
 **/
