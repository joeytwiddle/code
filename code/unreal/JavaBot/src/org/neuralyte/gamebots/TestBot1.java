package org.neuralyte.gamebots;

import edu.isi.gamebots.client.Bot;
import edu.tamu.gamebots.humanbot.HumanBot;

import java.lang.reflect.Method;
import java.util.Arrays;

/** joey Jun 6, 2007 9:32:14 PM */
// public class TestBot1 extends Bot {
public class TestBot1 extends HumanBot {

    public TestBot1() {
        super();
        new Thread() {
            public void run() {
                doStuff();
            }
        }.start();
    }

    protected void connected() {
        super.connected();
    }

    private void doStuff() {

        while (true) {
            try { Thread.sleep(2000); } catch (Exception e) { }
            try {
                connect();
                break;
            } catch (Exception e) {
                System.out.println("Connecting: "+e);
            }
        }

        String[] goodMethodsArray = {
            "changeWeapon",
            // "checkReach",
            // "getPath",
            "jump",
            "rotate",
            "runTo",
            "say",
            "setName",
            "setWalk",
            "shoot",
            // "stop",
            "stopShoot",
            "strafe",
            "turnTo",
            "turnToLoc",
        };
        java.util.List goodMethodsList = Arrays.asList(goodMethodsArray);

        while (true) {

            // Take a random action

            //// Choose a random method from the Bot superclass
            // Method[] methods = this.getClass().getMethods();
            Method[] methods = Bot.class.getDeclaredMethods();
            int i = (int)(Math.random() * methods.length);
            Method method = methods[i];
            // Certain methods we do not wish to call:
            // if (method.getName().indexOf("destroy")>=0
                // || method.getName().indexOf("connect")>=0) {
                // continue;
            // }
            if (!goodMethodsList.contains(method.getName())) {
                continue;
            }

            // Generate random args for the method
            Object[] args = new Object[method.getParameterTypes().length];
            String displayArgs = "";
            for (int j=0;j<method.getParameterTypes().length;j++) {
                Class paramType = method.getParameterTypes()[j];
                Object value = null;
//                try {
//                    value = paramType.newInstance();
//                } catch (Exception e) {
//                    System.out.println(""+e);
//                }
                if (paramType.getName().equals("integer")) {
                    value = new Integer((int)(Math.random()*65536 - 32768)/8);
                    // value = new Integer((int)(Math.random()*1024 - 512));
                } else if (paramType.getName().equals("float")) {
                    value = new Float((Math.random()*65536 - 32768)/8);
                    // value = new Float((int)(Math.random()*1024 - 512));
                } else if (paramType.getName().equals("double")) {
                    value = new Double((Math.random()*65536 - 32768)/8);
                    // value = new Double((int)(Math.random()*1024 - 512));
                } else if (paramType.getName().equals("boolean")) {
                    value = new Boolean(Math.random() >= 0.5);
                } else if (paramType == String.class) {
                    value = "hello";
                } else {
                    System.err.println("TestBot1.doStuff(): do not know how to generate a random " + paramType.getPackage()+"."+paramType.getName());
                    // value = null;
                }
                args[j] = value;
                displayArgs += (j>0?",":"") + value;
            }

            // Call the method
            System.out.println("Calling "+method+"("+displayArgs+")");
            try {
                method.invoke(this,args);
            } catch (Exception e) {
                System.out.println(""+e);
            }

            try { Thread.sleep(500); } catch (Exception e) { }
        }
    }

    /*
    protected void receivedAsyncMessage(Message message) {
        super.receivedAsyncMessage(message);
    }

    protected void receivedSyncMessage(MessageBlock message) {
        super.receivedAsyncMessage(message);
    }
    */

}
