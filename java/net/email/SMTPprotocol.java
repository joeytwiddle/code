//(c) 1999 by Tair Abdurman http://www.tair.freeservers.com/index.html

import java.awt.*;
import java.io.*;
import java.net.*;
import java.util.*;

class SMTPInputStream
{
    private InputStream is=null;
    
    SMTPInputStream(Socket s) throws IOException
    {
        is = s.getInputStream();
    }
    
    String ReadFromStream()  throws IOException
    {
        String s=new String("");
        while ((is.available()>0) && (s.length()<255))
            s=s+String.valueOf((char) is.read());
				System.out.println("Read: "+s);
        return s;
    }
    
    void Close() throws IOException
    {
        is.close();
        is=null;
    }
}


class SMTPOutputStream
{
    private OutputStream os=null;
    
    SMTPOutputStream(Socket s) throws IOException
    {
        os=s.getOutputStream();
    }
    
    void WriteToStream(String s) throws IOException
    {
        if (s.length()>0)
        {
					System.out.println("Write: "+s);
            for (int i=0;(i<s.length());i++)
                os.write((int) s.charAt(i));
        }
    }
    
    void Close() throws IOException
    {
        os.close();
        os=null;
    }
}


class InfoBox extends TextArea
{
    InfoBox(int x,int y)
    {
        super("",x,y);
        setEditable(false);
        setFont(new Font("Courier",Font.PLAIN,12));
    }
    
    void Clear()
    {
        setText("");
    }
    
    void Print(String s)
    {
        insertText(s,getText().length());
    }
    
    void Println(String s)
    {
        insertText(s+"\r\n",getText().length());
    }
}


public class SMTPprotocol extends Frame
{
    
    //******************************************************************
    //RFC 821 Standarts
    //SMTP Commands
    final String[] SMTPCommand =
        {
        "HELO",
            "MAIL FROM:",
            "RCPT TO:",
            "DATA",
            "QUIT",
            "NOOP",
            "TURN",
            "RSET",
            "VRFY",
            "EXPN",
            "SEND FROM:",
            "SOML FROM:",
            "SAML FROM:",
            "\r\n.\r\n",
            "\r\n",
            " "
    };
    
    //Command codes here
    final int SMTP_CMD_HELO=0;
    final int SMTP_CMD_MAIL_FROM=1;
    final int SMTP_CMD_RCPT_TO=2;
    final int SMTP_CMD_DATA=3;
    final int SMTP_CMD_QUIT=4;
    final int SMTP_CMD_NOOP=5;
    final int SMTP_CMD_TURN=6;
    final int SMTP_CMD_RSET=7;
    final int SMTP_CMD_VRFY=8;
    final int SMTP_CMD_EXPN=9;
    final int SMTP_CMD_SEND_FROM=10;
    final int SMTP_CMD_SOML_FROM=11;
    final int SMTP_CMD_SAML_FROM=12;
    final int SMTP_CMD_CRLF_CRLF=13;
    final int SMTP_CMD_CRLF=14;
    final int SMTP_CMD_SPC=15;
    
    
    //REPLY CODES
    //  500 Syntax error, command unrecognized
    //            [This may include errors such as command line too long]
    final int SMTP_RCODE_UNREC_COMMAND=500;
    //      501 Syntax error in parameters or arguments
    final int SMTP_RCODE_PARAM_ERROR=501;
    //      502 Command not implemented
    final int SMTP_RCODE_IMPLEMENT_COMMAND=502;
    //      503 Bad sequence of commands
    final int SMTP_RCODE_SEQUENCE_COMMAND=503;
    //      504 Command parameter not implemented
    final int SMTP_RCODE_PARAM_COMMAND=504;
    
    //      211 System status, or system help reply
    final int SMTP_RCODE_SYSTEM_REPLY=211;
    //  214 Help message
    //            [Information on how to use the receiver or the meaning of a
    //            particular non-standard command; this reply is useful only
    //            to the human user]
    final int SMTP_RCODE_HELP=214;
    //      220 <domain> Service ready
    final int SMTP_RCODE_READY=220;
    //  221 <domain> Service closing transmission channel
    final int SMTP_RCODE_CLOSING=221;
    //  421 <domain> Service not available, closing transmission channel
    //            [This may be a reply to any command if
    //                 the service knows it must shut down]
    final int SMTP_RCODE_UNAVAILABLE=421;
    //  250 Requested mail action okay, completed
    final int SMTP_RCODE_COMPLETED=250;
    //  251 User not local; will forward to <forward-path>
    final int SMTP_RCODE_FORWARD=251;
    //  450 Requested mail action not taken: mailbox unavailable
    //            [E.g., mailbox busy]
    final int SMTP_RCODE_MAIL_ACTION_NOT_TAKEN=450;
    //  550 Requested action not taken: mailbox unavailable
    //  [E.g., mailbox not found, no access]
    final int SMTP_RCODE_ACTION_NOT_TAKEN=550;
    //  451 Requested action aborted: error in processing
    final int SMTP_RCODE_ACTION_ABORTED=451;
    //  551 User not local; please try <forward-path>
    final int SMTP_RCODE_USER_NOT_LOCAL=551;
    //  452 Requested action not taken: insufficient system storage
    final int SMTP_RCODE_SYSTEM_STORAGE=452;
    //  552 Requested mail action aborted: exceeded storage allocation
    final int SMTP_RCODE_MAIL_STORAGE=552;
    //  553 Requested action not taken: mailbox name not allowed
    //          [E.g., mailbox syntax incorrect]
    final int SMTP_RCODE_NAME_NOT_ALLOWED=553;
    //       354 Start mail input; end with <CRLF>.<CRLF>
    final int SMTP_RCODE_MAIL_START=354;
    //       554 Transaction failed
    final int SMTP_RCODE_TRANS_FAILED=554;
    //******************************************************************
    //timeout 10 seconds
    final int WAIT_TIMEOUT=(10*1000);
    //******************************************************************
    //read data from input stream to globalBuffer String
    //return true if ok
    //false if timeout
    String globalBuffer=new String("");
    
    int waitForTimeout(SMTPInputStream i) throws IOException
    {
        boolean defStatus;
        long startTime;
        int replyCode;
        
        //WAIT FOR RESPONSE (WAIT_TIMEOUT) millis
        startTime=System.currentTimeMillis();
        while ((System.currentTimeMillis()-startTime)<WAIT_TIMEOUT)
        {
            globalBuffer="";
            globalBuffer=i.ReadFromStream();
            if (globalBuffer.length()>0)
            {
                try
                {
                    replyCode=Integer.valueOf(globalBuffer.substring(0,
                                                                     3)).intValue();
                    infoArea.Println("SERVER RESPONSE: "+globalBuffer);
                    return replyCode;
                }
                catch(NumberFormatException e)
                {
                    return -1;
                }
            }
        }
        return 0;
    }
    
    int doSMTPTransaction(SMTPInputStream in,
                          SMTPOutputStream out) throws IOException
    {
        int replyCode;
        Date today=new Date();
        //GET SERVER RESPONSE on CONNECTION ESTABLISHMENT
        //WAIT FOR RESPONSE (WAIT_TIMEOUT) millis
        //      NOT RESPONSE ON CONNECT ERROR CODE 1
        replyCode=waitForTimeout(in);
        if (replyCode==0)
        {
            infoArea.Println("NOT RESPONSE ON CONNECT ERROR CODE 1");
            return 1;
        }
        if (replyCode==-1)
        {
            infoArea.Println("UNKNOWN REPLY CODE ERROR CODE 4");
            return 4;
        }
        
        if (replyCode!=SMTP_RCODE_READY)
        {
            infoArea.Println("UNSUCCESS CONNECTION ");
            return replyCode;
        }
        
        infoArea.Println("connection response is OK");
        infoArea.Println("request HELO");
        
        //SEND HELO CRLF
        out.WriteToStream(SMTPCommand[SMTP_CMD_HELO]);
        out.WriteToStream(SMTPCommand[SMTP_CMD_SPC]);
        out.WriteToStream("127.0.0.1");
        out.WriteToStream(SMTPCommand[SMTP_CMD_CRLF]);
        
        //WAIT FOR RESPONSE ON HELO
        replyCode=waitForTimeout(in);
        if (replyCode==0)
        {
            infoArea.Println("NOT RESPONSE TO HELO ERROR CODE 2");
            return 2;
        }
        if (replyCode==-1)
        {
            infoArea.Println("UNKNOWN REPLY CODE ERROR CODE 4");
            return 4;
        }
        if (replyCode!=SMTP_RCODE_COMPLETED)
        {
            infoArea.Println("UNABLE TO COMPLETE REPLY");
            return replyCode;
        }
        infoArea.Println("HELO response is OK");
        infoArea.Println("request MAIL FROM");
        
        //SEND MAIL FROM: SPC <sender> CRLF
        out.WriteToStream(SMTPCommand[SMTP_CMD_MAIL_FROM]);
        out.WriteToStream(SMTPCommand[SMTP_CMD_SPC]);
        out.WriteToStream(senderMail.getText());
        out.WriteToStream(SMTPCommand[SMTP_CMD_CRLF]);
        
        //WAIT FOR RESPONSE ON MAIL FROM
        replyCode=waitForTimeout(in);
        if (replyCode==0)
        {
            infoArea.Println("NOT RESPONSE TO HELO ERROR CODE 2");
            return 2;
        }
        if (replyCode==-1)
        {
            infoArea.Println("UNKNOWN REPLY CODE ERROR CODE 4");
            return 4;
        }
        if (replyCode!=SMTP_RCODE_COMPLETED)
        {
            infoArea.Println("UNABLE TO COMPLETE REPLY");
            return replyCode;
        }
        infoArea.Println("MAIL FROM response is OK");
        infoArea.Println("request RCPT TO");
        
        //SEND RCPT TO: SPC <sender> CRLF
        out.WriteToStream(SMTPCommand[SMTP_CMD_RCPT_TO]);
        out.WriteToStream(SMTPCommand[SMTP_CMD_SPC]);
        out.WriteToStream(reciverMail.getText());
        out.WriteToStream(SMTPCommand[SMTP_CMD_CRLF]);
        
        //WAIT FOR RESPONSE ON RCPT TO
        replyCode=waitForTimeout(in);
        if (replyCode==0)
        {
            infoArea.Println("NOT RESPONSE TO HELO ERROR CODE 2");
            return 2;
        }
        if (replyCode==-1)
        {
            infoArea.Println("UNKNOWN REPLY CODE ERROR CODE 4");
            return 4;
        }
        if ((replyCode!=SMTP_RCODE_COMPLETED) && (replyCode!=SMTP_RCODE_FORWARD
                ))
        {
            infoArea.Println("UNABLE TO COMPLETE REPLY");
            return replyCode;
        }
        infoArea.Println("RCPT TO response is OK");
        //TRY TO SEND TO CC
        if ((ccMail.getText().length())>0)
        {
            infoArea.Println("request CC RCPT TO");
            //SEND RCPT TO: SPC <sender> CRLF
            out.WriteToStream(SMTPCommand[SMTP_CMD_RCPT_TO]);
            out.WriteToStream(SMTPCommand[SMTP_CMD_SPC]);
            out.WriteToStream(ccMail.getText());
            out.WriteToStream(SMTPCommand[SMTP_CMD_CRLF]);
            
            //WAIT FOR RESPONSE ON RCPT TO
            replyCode=waitForTimeout(in);
            if (replyCode==0)
            {
                infoArea.Println("NOT RESPONSE TO HELO ERROR CODE 2");
                return 2;
            }
            if (replyCode==-1)
            {
                infoArea.Println("UNKNOWN REPLY CODE ERROR CODE 4");
                return 4;
            }
            if ((replyCode!=SMTP_RCODE_COMPLETED) 
                    && (replyCode!=SMTP_RCODE_FORWARD
                    ))
            {
                infoArea.Println("UNABLE TO COMPLETE CC");
            }
            else
            {
                infoArea.Println("CC.RCPT TO response is OK");
            }
        }
        //TRY TO SEND TO BCC
        if ((bccMail.getText().length())>0)
        {
            //SEND RCPT TO: SPC <sender> CRLF
            out.WriteToStream(SMTPCommand[SMTP_CMD_RCPT_TO]);
            out.WriteToStream(SMTPCommand[SMTP_CMD_SPC]);
            out.WriteToStream(bccMail.getText());
            out.WriteToStream(SMTPCommand[SMTP_CMD_CRLF]);
            
            //WAIT FOR RESPONSE ON RCPT TO
            replyCode=waitForTimeout(in);
            if (replyCode==0)
            {
                infoArea.Println("NOT RESPONSE TO HELO ERROR CODE 2");
                return 2;
            }
            if (replyCode==-1)
            {
                infoArea.Println("UNKNOWN REPLY CODE ERROR CODE 4");
                return 4;
            }
            if ((replyCode!=SMTP_RCODE_COMPLETED) 
                    && (replyCode!=SMTP_RCODE_FORWARD
                    ))
            {
                infoArea.Println("UNABLE TO COMPLETE BCC");
            }
            else
            {
                infoArea.Println("BCC.RCPT TO response is OK");
            }
        }
        infoArea.Println("request DATA");
        
        //SEND DATA CRLF
        out.WriteToStream(SMTPCommand[SMTP_CMD_DATA]);
        out.WriteToStream(SMTPCommand[SMTP_CMD_CRLF]);
        
        //WAIT FOR RESPONSE ON DATA
        replyCode=waitForTimeout(in);
        if (replyCode==0)
        {
            infoArea.Println("NOT RESPONSE TO HELO ERROR CODE 2");
            return 2;
        }
        if (replyCode==-1)
        {
            infoArea.Println("UNKNOWN REPLY CODE ERROR CODE 4");
            return 4;
        }
        if (replyCode!=SMTP_RCODE_MAIL_START)
        {
            infoArea.Println("UNABLE TO COMPLETE REPLY");
            return replyCode;
        }
        infoArea.Println("DATA response is OK");
        infoArea.Println("request DATA");
        
        //SEND mail content CRLF.CRLF
        out.WriteToStream("Subject: "+subjectMail.getText());
        out.WriteToStream(SMTPCommand[SMTP_CMD_CRLF]);
        out.WriteToStream("From: "+senderMail.getText());
        out.WriteToStream(SMTPCommand[SMTP_CMD_CRLF]);
        out.WriteToStream("To: "+reciverMail.getText());
        out.WriteToStream(SMTPCommand[SMTP_CMD_CRLF]);
        out.WriteToStream("Date: "+today.toString());
        out.WriteToStream(SMTPCommand[SMTP_CMD_CRLF]);
        out.WriteToStream(SMTPCommand[SMTP_CMD_CRLF]);
        out.WriteToStream(bodyMail.getText());
        out.WriteToStream(SMTPCommand[SMTP_CMD_CRLF_CRLF]);
        
        //WAIT FOR RESPONSE ON mail content CRLF.CRLF
        replyCode=waitForTimeout(in);
        if (replyCode==0)
        {
            infoArea.Println("NOT RESPONSE TO HELO ERROR CODE 2");
            return 2;
        }
        if (replyCode==-1)
        {
            infoArea.Println("UNKNOWN REPLY CODE ERROR CODE 4");
            return 4;
        }
        if (replyCode!=SMTP_RCODE_COMPLETED)
        {
            infoArea.Println("UNABLE TO COMPLETE REPLY");
            return replyCode;
        }
        infoArea.Println("mail content response is OK");
        infoArea.Println("request QUIT");
        
        //SEND QUIT CRLF
        out.WriteToStream(SMTPCommand[SMTP_CMD_QUIT]);
        out.WriteToStream(SMTPCommand[SMTP_CMD_CRLF]);
        
        //WAIT FOR RESPONSE (WAIT_TIMEOUT) millis
        //      NOT RESPONSE TO QUIT ERROR CODE 3
        replyCode=waitForTimeout(in);
        if (replyCode==0)
        {
            infoArea.Println("NOT RESPONSE TO QUIT ERROR CODE 3");
            return 3;
        }
        if (replyCode==-1)
        {
            infoArea.Println("UNKNOWN REPLY CODE ERROR CODE 4");
            return 4;
        }
        if (replyCode!=SMTP_RCODE_CLOSING)
        {
            infoArea.Println("UNCLOSED CONNECTION REPLY");
            return replyCode;
        }
        infoArea.Println("QUIT response is BYE");
        infoArea.Println("end of SMTP transaction");
        
        //  TRANSACTION OK ERROR CODE 0
        return 0;
    }
    
    //TRANSACTION RULES
    /* S - Send
       F - Fail
       E - Error
    CONNECTION ESTABLISHMENT
                   S: 220
                               F: 421
                            HELO
                               S: 250
                               E: 500, 501, 504, 421
                            MAIL
                               S: 250
                               F: 552, 451, 452
                               E: 500, 501, 421
                            RCPT
                   S: 250, 251
                   F: 550, 551, 552, 553, 450, 451, 452
                   E: 500, 501, 503, 421
                DATA
                   I: 354 -> data -> S: 250
                                     F: 552, 554, 451, 452
                   F: 451, 554
                   E: 500, 501, 503, 421
                RSET
                   S: 250
                   E: 500, 501, 504, 421
                SEND
                   S: 250
                   F: 552, 451, 452
                   E: 500, 501, 502, 421
                SOML
                   S: 250
                   F: 552, 451, 452
                   E: 500, 501, 502, 421
                SAML
                   S: 250
                   F: 552, 451, 452
                   E: 500, 501, 502, 421
                VRFY
                   S: 250, 251
                   F: 550, 551, 553
                   E: 500, 501, 502, 504, 421
                EXPN
                   S: 250
                   F: 550
                   E: 500, 501, 502, 504, 421
                HELP
                   S: 211, 214
                   E: 500, 501, 502, 504, 421
                NOOP
                   S: 250
                   E: 500, 421
                QUIT
                   S: 221
                   E: 500
                TURN
                   S: 250
                   F: 502
                   E: 500, 503       */
    
    TextField smtpServer;
    TextField smtpPort;
    
    TextField senderMail;
    TextField reciverMail;
    TextField ccMail;
    TextField bccMail;
    TextField subjectMail;
    
    InfoBox bodyMail;
    InfoBox infoArea;
    
    Button sendMail;
    Button clearMail;
    Button quitProgram;
    //******************************************************************
    
    SMTPprotocol()
    {
        super("SMTP Protocol Test v.1.0 by Tair Abdurman");
        setBackground(Color.lightGray);
        setLayout(new FlowLayout());
        //******************************************************************
        smtpServer=new TextField("smtp.blueyonder.co.uk",50);
        smtpPort=new TextField("25",10);
        senderMail=new TextField("you@youraddress.com",80);
        reciverMail=new TextField("java@tair.freeservers.com",0);
        ccMail=new TextField("",80);
        bccMail=new TextField("",80);
        subjectMail=new TextField("SMTP deneme",80);
        
        add(new Label("SMTP Server:"));
        add(smtpServer);
        add(new Label("SMTP Port:"));
        add(smtpPort);
        
        add(new Label("From:    "));
        add(senderMail);
        add(new Label("To  :    "));
        add(reciverMail);
        add(new Label("Cc  :    "));
        add(ccMail);
        add(new Label("Bcc :    "));
        add(bccMail);
        add(new Label("Subject :"));
        add(subjectMail);
        
        add(new Label("Content :               "));
        bodyMail=new InfoBox(5,90);
        bodyMail.setEditable(true);
        
        add(bodyMail);
        
        sendMail=new Button("send");
        clearMail=new Button("clear");
        quitProgram=new Button("quit");
        
        add(sendMail);
        add(clearMail);
        add(quitProgram);
        
        infoArea=new InfoBox(4,90);
        add(infoArea);
        
        //******************************************************************
        resize(700,450);
        show();
        infoArea.Println("ready ...");
        
    }
    
    //ON QUIT PROGRAM
    void onQuit()
    {
        infoArea.Println("quit programm ...");
        System.exit(0);
    }
    //ON CLEAR MAIL
    void onClearMail()
    {
        infoArea.Println("clearing mail ...");
        bodyMail.Clear();
    }
    
    //ON SEND MAIL
    int onSendMail()
    {
        String inBuffer;
        String outBuffer;
        Socket sock;
        SMTPInputStream fromServer;
        SMTPOutputStream toServer;
        int intPort;
        //TEST RECIPIENT ADDRESS it must be at once
        if (reciverMail.getText().length()<1)
        {
            infoArea.Println("enter recipient please ...");
            return -1;
        }
        //TEST PORT NUMBER FORMAT and ALERT WHEN NOT 25
        intPort=-1;
        infoArea.Clear();
        infoArea.Println("check port ...");
        try
        {
            intPort=Integer.valueOf(smtpPort.getText()).intValue();
            if ((intPort<0) || (intPort>65356))
            {
                infoArea.Println("invalid port ...");
                return -1;
            }
            if (intPort!=25)
                infoArea.Println("attention port is not 25 ...");
            infoArea.Println("port number is "+smtpPort.getText());
        }
        catch(NumberFormatException e)
        {
            infoArea.Println("invalid port ...");
            return -1;
        }
        //CREATE SMTP CONNECTION
        infoArea.Println("connect to server ...");
        try
        {
            sock=new Socket(smtpServer.getText(),intPort);
        }
        catch(IOException e)
        {
            infoArea.Println("unable to connect(SK) ... "+e);
            return -1;
        }
        //CREATE SERVER STREAMS
        //INPUT STREAM
        try
        {
            fromServer=new SMTPInputStream(sock);
        }
        catch(IOException e)
        {
            infoArea.Println("unable to connect(IS) ...");
            return -1;
        }
        //OUTPUT STREAM
        try
        {
            toServer=new SMTPOutputStream(sock);
        }
        catch(IOException e)
        {
            infoArea.Println("unable to connect(OS) ...");
            return -1;
        }
        infoArea.Println("connected ...");
        //Mail Sending
        infoArea.Println("sending mail ...");
        //*******************************************************************
        try
        {
            int i=doSMTPTransaction(fromServer,toServer);
            if (i==0)
                infoArea.Println("complete ...");
            else
                infoArea.Println("error ...");
        }
        catch(IOException e)
        {
            infoArea.Println("error ...");
        }
        //*******************************************************************
        //CLOSE SERVER STREAMS
        //OUTPUT STREAM
        infoArea.Println("disconnect from server ...");
        try
        {
            toServer.Close();
        }
        catch(IOException e)
        {
            infoArea.Println("alert OUTPUT STREAM ...");
        }
        //INPUT STREAM
        try
        {
            fromServer.Close();
        }
        catch(IOException e)
        {
            infoArea.Println("alert INPUT STREAM ...");
        }
        //CLOSE CONNECTION
        try
        {
            sock.close();
            infoArea.Println("disconnected ...");
        }
        catch(IOException e)
        {
            infoArea.Println("unable to disconnect ...");
            return -1;
        }
        return 0;
    }
    
    //EVENT HANDLER
    public synchronized boolean handleEvent(Event e)
    {
        if (e.id==Event.WINDOW_DESTROY)
        {
            System.exit(0);
            return true;
        }
        
        else
        {
            return super.handleEvent(e);
        }
    }
    
    
    //ACTION HANDLER
    public boolean action(Event evt,Object obj)
    {
        if (evt.target instanceof Button)
        {
            String label=(String) obj;
            if (label.equals("quit"))
            {
                onQuit();
            }
            else if (label.equals("send"))
            {
                int i=onSendMail();
            }
            else if (label.equals("clear"))
            {
                onClearMail();
            }
        }
        return true;
    }
    
    
    //ENTRY POINT FOR CORE CLASS
    public static void main(String args[])
    {
        new SMTPprotocol();
    }
}
