// ***********************************************************************
//              S I G N E D    A P P L E T (I)
//              G E T    U S E R     N A M E
//
// Simple applet to test for acquiring user name
//
// Date    : 8 August 1999
// Author  : Raymond Lee
// Version : 1.0
// email   : csstlee@comp.polyu.edu.hk
//
//
// ***********************************************************************

import java.applet.*;
import java.awt.*;

public class AcquireUname extends Applet {
   private String msg;

   public void init(){
     try{
        msg = "Your own username is "+System.getProperty("user.name");
     }
     catch (SecurityException e){
       msg = "Get username faiure! Security forbidden!!!";
     }
   }

   public void paint(Graphics gp){
     gp.drawString("Acquire your system information ...", 50,50);
     gp.drawString(msg, 50, 80);
   }
}



