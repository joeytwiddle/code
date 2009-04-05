package org.neuralyte.gamebots;

import edu.isi.gamebots.client.Bot;
import edu.isi.gamebots.client.MessageBlock;
import edu.isi.gamebots.client.Message;
import edu.tamu.gamebots.humanbot.HumanBot;

import java.lang.reflect.Method;
import java.util.Arrays;

/** joey Jun 6, 2007 9:32:14 PM */
// public class TestBot1 extends Bot {
public class TestBot1 extends HumanBot {

    java.util.List inventory = new java.util.Vector();
    // Since we haven't yet detected respawn, our inventory gets filled with
    // old weapons (some of which we can switch to, but then we get no mesh in game!)
    // Maybe we could detect respawn by checking when we are given a Hammer or Translocator (won't work in IG games though)

    double[] location = new double[3];
    double[] rotation = new double[3]; // 360 degrees = 65536

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
        // inventory.clear(); // Happens too late?  I think it was clearing initial spawn weapons
    }

    private void doStuff() {

        // Auto-connect at start
        // @todo auto-reconnect when disconnected, and stop spamming instructions when disconnected, until reconnected
        while (true) {
            threadSleep(2000);
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
            // "say",
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

            if (Math.random() <= 0.01) {
                // Try to use translocator
                String tlFound = null;
                for (int i=0;i<inventory.size();i++) {
                    String invItem = (String)inventory.get(i);
                    if (invItem.indexOf("Translocator")>=0) {
                        tlFound = invItem; // break;
                    }
                }
                if (tlFound != null) {
                    System.out.println("Trying to use "+tlFound);
                    say("Trying to use "+tlFound,true);
                    threadSleep(1000);
                    changeWeapon(tlFound);
                    threadSleep(1000);
                    // shoot();
                    // shoot(0,0,0,"CTF-BleakCE-100.FlagBase1",false);
                    shoot(0,0,0,"None",false);
                    // shoot("CTF-BleakCE-100.FlagBase1",false);
                    // shoot("",false);
                    threadSleep(1000);
                    // shoot(1,1,1,"CTF-BleakCE-100.FlagBase1",true);
                    // shoot(0,0,0,"None",true);
                    shoot("CTF-BleakCE-100.FlagBase1",true);
                    // shoot("",true);
                    threadSleep(1000);
                    continue;
                }
            }

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
                    // Choose a random String/item from our inventory
                    if (inventory.size()>0) {
                        int k = (int)(Math.random() * inventory.size());
                        value = inventory.get(k);
                    }
                    // value = "CTF-BleakCE-100.translocator"+(int)(Math.random()*256);
                } else {
                    System.err.println("TestBot1.doStuff(): do not know how to generate a random " + paramType.getPackage()+"."+paramType.getName());
                    // value = null;
                }
                args[j] = value;
                displayArgs += (j>0?",":"") + value;
            }

            // Call the method
            // System.out.println("Calling "+method+"("+displayArgs+")");
            try {
                method.invoke(this,args);
            } catch (Exception e) {
                System.out.println(""+e);
            }

            threadSleep(200); // keeping it at 200 until i work out the secret of how the bot can throw its TL
        }
    }

    protected void receivedAsyncMessage(Message message) {
        super.receivedAsyncMessage(message);

        if (!message.getType().equals(BEG)) {
            System.out.println("AsyncMessage: "+message.getType()+" "+message.getPropertySet());
        }

        if (message.getType().equals(ITEM)) {
            inventory.add(message.getProperty("Id"));
            System.out.println("My inventory is now: "+inventory);
        }

        String newLocation = message.getProperty(LOCATION);
        if (newLocation != null) {
            System.out.println("Location: "+newLocation);
            location = parseVector( newLocation );
        }

        String newRotation = message.getProperty(ROTATION);
        if (newRotation != null) {
            System.out.println("Rotation: "+newRotation);
            rotation = parseVector( newRotation );
        }

        if (message.getType().equals(DAMAGE)) {
            System.out.println("I lost "+message.getProperty("Damage")+"hp from "+message.getProperty("DamageType"));
        }

        if (message.getType().equals("VMS")) {
            String msg = message.getProperty("String");
            if (msg.endsWith(" entered the game.")) {
                final String newPlayer = msg.substring(0,msg.indexOf(" entered the game."));
                // say("Hello "+newPlayer,true); // needs to be delayed
                new Thread() {
                    public void run() {
                        threadSleep(randint(2000,8000));
                        if (newPlayer.equals("[eC]`Ormolu^"))
                            say("zomg it's the uberest "+newPlayer+"!! \\o/ Hi and welcome imo! <3 <3 xxx",true);
                        else
                            say("Hello "+newPlayer,true);
                    }
                }.start();
            }
        }
    }

    protected void receivedSyncMessage(MessageBlock message) {
        super.receivedSyncMessage(message);
        if (!message.getType().equals(BEG)) {
            System.out.println("SyncMessage: "+message.getType()+" "+message.getPropertySet());
        }
    }

    public static int randint(int lowest, int highest) {
        return lowest + (int)( (highest-lowest+1)*Math.random() );
    }

    public static void threadSleep(int millis) {
        try { Thread.sleep(millis); } catch (Exception e) { }
    }

}
