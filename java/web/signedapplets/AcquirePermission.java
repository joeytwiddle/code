// ***********************************************************************
//              S I G N E D    A P P L E T (II)
//      G E T    U S E R     N A M E ( with permission acquired)
//
// Simple applet to test for acquiring user name with permission acquired
// thro' Netscape browser
//
// Date    : 8 August 1999
// Author  : Raymond Lee
// Version : 1.0
// email   : csstlee@comp.polyu.edu.hk
//
// Note : This program is designed for Netscape security control
//        acquisition.
// ***********************************************************************

import java.applet.*;
import java.awt.*;
import netscape.security.PrivilegeManager;

public class AcquirePermission extends Applet {
   private String msg;

   public void init(){
     try{
        PrivilegeManager.enablePrivilege("UniversalPropertyRead");
        msg = "Your own username is "+System.getProperty("user.name");
     }
     catch (netscape.security.ForbiddenTargetException e){
        msg = "Acquire username forbidden, due to netscape security protection!!!";
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
