import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;

// CyberPet - The applet in which the cyberpet lives
//            an interactive GUI

// Since the Applet is self-contained, and it is a Panel
// we can include it in a Frame.
// Thus it can easily be used on a web page or from the
// command line.

// But you need to follow slight changes below...

public class CyberPet extends Applet implements ActionListener {

    // Owns one pet
    public octopus octo;

    // AWT components
    public Button restartbut,loadbut,savebut;
    public Button feedbut,playbut,cleanbut;
    public Button medicinebut,lightbut;
    public Button leftbut,rightbut;
    public Label coms;

    // Uncomment the following to run in a Frame
    // For some reason crashes applet!
    /*
		public static void main(String argv[]) {
    	// Create new frame, and show it
    	Frame f=new Frame("Octo the cyber pet!");
    	f.setResizable(false);
    	f.setSize(300,250);
      // f.setLayout(new FlowLayout());
      f.addWindowListener(new WindowAdapter() {
  	    public void windowClosing(WindowEvent e) {
  		    System.exit(0);
	      }
    	} );
    	CyberPet octoapplet=new CyberPet("Octo the cyber pet!");
    	f.add(octoapplet);
    	f.show();
    }
    CyberPet(String title) {
    	setSize(300,250);
    	init();
    }
		*/

    public void init() {
	// Set colours of frame
	setBackground(Color.white);
	setForeground(new Color((float)12/255,(float)104/255,(float)100/255));
        // Add buttons and label to applet
	ab(restartbut,"Restart");
	ab(loadbut,"Load");
	ab(savebut,"Save");
	ab(feedbut,"Feed");
	ab(playbut,"Play");
	ab(cleanbut,"Clean");
	ab(lightbut,"Light");
	ab(medicinebut,"Medicine");
	ab(leftbut,"Left");
	ab(rightbut,"Right");
	coms=new Label("",Label.CENTER);
	add(coms);
	// Start up Octo
	octo=new octopus(this,(int)(getSize().width/2),(int)(getSize().height*0.64));
    }
    // My shortcut add button method
    void ab(Button b,String s) {
	b=new Button(s);
	b.setActionCommand(s);
	b.addActionListener(this);
	add(b);
    }
    public void start() { // Applet only
	// Resume animation
	octo.at.resume();
	// Only resume biology if light is on
	if (octo.lighton)
	    octo.bt.resume();
	// Resume communication
	octo.ct.resume();
    }
    public void stop() { // Applet only
	// Suspend threads
	octo.at.suspend();
	octo.bt.suspend();
	octo.ct.suspend();
    }
    public void actionPerformed(ActionEvent e){
	// Call appropriate method for the button pressed
	String command=e.getActionCommand();
	if (command=="Restart")
	    restart();
	if (command=="Load")
	    load();
	if (command=="Save")
	    save();
	if (command=="Feed")
	    octo.feed();
	if (command=="Play")
	    octo.startplay();
	if (command=="Clean")
	    octo.clean();
	if (command=="Medicine")
	    octo.medicine();
	if (command=="Light")
	    octo.light();
	if ((command=="Left" || command=="Right") && octo.playing)
	    octo.userplays(command);
    }
    public void paint(Graphics g) {
	Color fg=getForeground();
	Color bg=getBackground();
	// Pretty border (only shows in applet)
        g.setColor(bg);
        g.draw3DRect(0,0,getSize().width-1,getSize().height-1,true);
        g.draw3DRect(3,3,getSize().width-7,getSize().height-7,false);
	// Display Octo
	if (octo.stillathome)
	    octo.paint(g,fg,bg);
/*	// Debugging data
	g.drawString("Food "+(octo.food)+", Happiness "+(octo.happiness)+", Mess "+(octo.mess)+".",0,180);
	g.drawString("Tiredness "+(octo.tiredness)+", Illness "+(octo.illness)+".",0,200);*/
    }
    void restart() {
	// Stop the current Octo
	octo.stop();
	// Change bg/fg colours if neccessary
	if (!octo.lighton) {
	    Color fg=getForeground();
	    Color bg=getBackground();
	    setForeground(bg);
	    setBackground(fg);
	}
	// Start a new Octo
	octo=new octopus(this,(int)(getSize().width/2),(int)(getSize().height*0.64));
    }
    void load() {
	// Present dialog box and get filename
	FileDialog fd=new FileDialog(new Frame(),"Load pet",FileDialog.LOAD);
	fd.show();
	String s=fd.getFile();
	// If not cancelled then
	if (s!=null) {
	    File f=new File(s);
	    // Stop current Octo (and get some info)
	    octo.stop();
	    boolean currentoctolo=octo.lighton;
	    // Try to load Octo from file
	    try {
	        FileInputStream fis=new FileInputStream(f);
		ObjectInputStream ois=new ObjectInputStream(fis);
		octo=(octopus)ois.readObject();
		fis.close();
	    }
	    catch (Exception e) {
		System.out.println("Oh dear: "+(e));
		System.exit(0);
	    }
	    // Change bg/fg colours if neccessary
	    if (currentoctolo!=octo.lighton) {
		Color fg=getForeground();
		Color bg=getBackground();
		setForeground(bg);
		setBackground(fg);
	    }
	    // Make new Octo look at the correct frame
	    octo.cpf=this;
	    // Start Octo up with some new threads
	    octo.start();
	}
    }
    void save() {
	// Present dialog box and get filename
	FileDialog fd=new FileDialog(new Frame(),"Save pet",FileDialog.SAVE);
	fd.show();
	String s=fd.getFile();
	// If not cancelled then
	if (s!=null) {
	    File f=new File(s);
	    // Try to output Octo to file
	    try {
		FileOutputStream fos=new FileOutputStream(f);
		ObjectOutputStream oos=new ObjectOutputStream(fos);
		oos.writeObject(octo);
		// Make sure all data is written
		oos.flush();
		// before closing file
		fos.close();
	    }
	    catch (Exception e) {
		System.out.println("Oh dear: "+(e));
		System.exit(0);
	    }
	}
    }
}

// octopus - The cyberpet itself
class octopus implements Serializable {
    // Octo's general state
    public float food=4,happiness=4,mess=0,tiredness=0,illness=-1;
    public int xposition,yposition,direction,frame=0;
    public boolean lighton=true,stillathome=true;
    // Information for playing
    public boolean playing=false; // You have to press Play before Left or Right
    public String octowinks="None";
    public int winkframe=0;
    // Information for displaying mess
    public final int maxmesses=80;
    public mess[] messarray=new mess[maxmesses];
    public int messes=0;
    // Other local variables
    CyberPet cpf;
    animthread at;
    biothread bt;
    comsthread ct;
    Random r=new Random();
    // Constructor of new Octo
    octopus(CyberPet temp1,int temp2,int temp3) {
	// Store information given in local variables
	cpf=temp1;
	xposition=temp2; yposition=temp3;
	// Set up initial variables
	if (rnd(2))
	    direction=+1;
	else
	    direction=-1;
/*	// A new Octo has most buttons enabled, and some disabled
	cpf.restartbut.setEnabled(true);
	cpf.loadbut.setEnabled(true);
	cpf.savebut.setEnabled(true);
	cpf.feedbut.setEnabled(true);
	cpf.playbut.setEnabled(true);
	cpf.cleanbut.setEnabled(true);
	cpf.lightbut.setEnabled(true);
	cpf.medicinebut.setEnabled(true);
	cpf.leftbut.setEnabled(false);
	cpf.rightbut.setEnabled(false); */
	// Create and start animation, biological and coms threads
	start();
	// Greeting message of new Octo
	ct.showmessagefor("Hi I'm Octo, please look after me!",16);
    }
    public void start() {
	// Creates and starts new threads
	at=new animthread(cpf,this,100);
	bt=new biothread(cpf,this,300);
	ct=new comsthread(cpf,this,500);
	at.start();
	bt.start();
	ct.start();
	// If light is off, biology should be suspended
	if (!lighton)
	    bt.suspend();
    }
    public void stop() {
	// Stops threads
	at.stop();
	bt.stop();
	ct.stop();
    }
    public void paint(Graphics g,Color fg,Color bg) {
	int l;
	double ang;
	g.setColor(fg);
	// Mess
        for (l=1; l<=messes; l++) {
	    messarray[l-1].paint(g);
	}
	// Head
	g.setColor(bg);
	g.fillOval(xposition-15,yposition-30,30,30);
	g.setColor(fg);
	g.drawOval(xposition-15,yposition-30,30,30);
	// Eyes (left and right done seperately)
	if (tired() || !lighton || octowinks=="Left") { // tired or asleep or winking eye
	    g.drawLine(xposition-7-2,yposition-15,xposition-7+2,yposition-15);
	} else { // normal eye
	    g.drawOval(xposition-7-2,yposition-15-2,4,4);
	}
	if (tired() || !lighton || octowinks=="Right") { // tired or asleep or winking eye
	    g.drawLine(xposition+7-2,yposition-15,xposition+7+2,yposition-15);
	} else { // normal eye
	    g.drawOval(xposition+7-2,yposition-15-2,4,4);
	}
	// Tentacles (8 of them!)
	for (l=0; l<=7; l++) {
	    ang=Math.PI/3*(l-3.5)/3.5*(0.8+0.3*Math.sin(Math.PI*frame/20));
	    g.drawLine(xposition,yposition,xposition+(int)(30*Math.sin(ang)),yposition+(int)(30*Math.cos(ang)));
	}
	// Mouth (expresses some of current state)
	boolean mouthdrawn=false;
	if (ill()) { // ill mouth
	    g.drawLine(xposition-3,yposition-4,xposition+3,yposition-10);
   	    g.drawLine(xposition-3,yposition-10,xposition+3,yposition-4);
	    mouthdrawn=true;
	}
	if (!mouthdrawn && hungry()) { // hungry mouth
	    g.drawOval(xposition-3,yposition-9,6,6);
	    mouthdrawn=true;
	}
	if (!mouthdrawn && sad()) { // sad mouth
	    g.drawArc(xposition-7,yposition-10,14,7,0,180);
	    mouthdrawn=true;
	}
	if (!mouthdrawn && messy()) { // messy mouth
	    g.drawLine(xposition-7,yposition-6,xposition+7,yposition-10);
	    mouthdrawn=true;
	}
	if (!mouthdrawn && tired()) { // tired mouth
	    g.drawLine(xposition-7,yposition-7,xposition+7,yposition-7);
	    mouthdrawn=true;
	}
	if (!mouthdrawn) { // happy mouth
	    g.drawArc(xposition-7,yposition-12,14,7,180,180);
	}
    }
    // Methods describing Octo's state
    public boolean hungry() {
	return (food==0);
    }
    public boolean sad() {
	return (happiness==0);
    }
    public boolean messy() {
	return (mess==4);
    }
    public boolean tired() {
	return (tiredness==4);
    }
    public boolean ill() {
	return (illness>0);
    }
    // Methods changing Octo's state
    public void feed() {
	food++;
	if (food>4)
	    food=4;
	ct.showmessagefor("Yum!",2);
    }
    // Playing methods
    public void startplay() {
	// User presses play to start a little game
        playing=true;
	ct.showmessagefor("Guess which eye I'm going to wink.",4);
/*	// Enable/disable relevant buttons
	cpf.playbut.setEnabled(false);
	cpf.leftbut.setEnabled(true);
        cpf.rightbut.setEnabled(true); */
    }
    public void userplays(String s) {
	// User presses left/right to conclude game
    	playing=false;
/*	// Enable/disable relevant buttons
	cpf.playbut.setEnabled(true);
	cpf.leftbut.setEnabled(false);
        cpf.rightbut.setEnabled(false); */
    	winkframe=frame;
        if (rnd(2)) // Octo chooses which eye to wink
            octowinks="Left";
        else
            octowinks="Right";
        if (octowinks==s) { // User guessed correctly
	    ct.showmessagefor("Well played!",2);
	    happiness++;
	    if (happiness>4)
	        happiness=4;
	} else
	    ct.showmessagefor("Bad luck!",2);
    }
    public void clean() {
	mess--;
	if (mess<0)
	    mess=0;
	messes=expectedmesses();
	ct.showmessagefor("Nothing like a good spring clean!",2);
    }
    public void medicine() {
	illness=-1;
	ct.showmessagefor("I feel tip-top now!",2);
    }
    public void light() {
	tiredness=0;
	// Switch background and foreground colours
	// ie. turn light on or off
	Color fg=cpf.getForeground();
	Color bg=cpf.getBackground();
	cpf.setForeground(bg);
	cpf.setBackground(fg);
	// If light is being turned off, suspend Octo's biology
	if (lighton) {
	    bt.suspend();
	    ct.showmessagefor("Good night!",2);
	}
	// If it's being turned on, resume his biology
	else {
	    bt.resume();
	    ct.showmessagefor("Good morning!",2);
	}
	lighton=!lighton;
    }
    public void leavehome() {
	stillathome=false;
/*	// Disable all buttons except restart and load
	cpf.savebut.setEnabled(false);
	cpf.feedbut.setEnabled(false);
	cpf.playbut.setEnabled(false);
	cpf.cleanbut.setEnabled(false);
	cpf.lightbut.setEnabled(false);
	cpf.medicinebut.setEnabled(false);
	cpf.leftbut.setEnabled(false);
	cpf.rightbut.setEnabled(false); */
	// Stop Octo's biology
	bt.stop();
    }
    // Handy methods
    // Expected number of mess globules for Octo's mess value
    int expectedmesses() {
	return (int)(maxmesses*mess/4);
    }
    // Returns true with probability 1/p
    boolean rnd(int p) {
    	int q=r.nextInt() % p;
    	if (q<0)
    	    q=-q;
    	if (q==0)
    	    return true;
    	else
    	    return false;
    }
}

class mess implements Serializable {
    int x,y;
    Random r=new Random();
    // A mess is a random point on the screen
    mess(CyberPet cpf) {
	x=(int)(cpf.getSize().width*r.nextFloat());
	y=(int)(cpf.getSize().height*r.nextFloat());
    }
    // And also a display method
    void paint(Graphics g) {
	g.fillOval(x-1,y-1,3,3);
    }
}

// General virtual pet thread, extended for use later
class mythread extends Thread implements Serializable {
    // Thread has access to frame and to Octo
    CyberPet cpf;
    octopus octo;
    int sleeptime;
    // Constructor stores info
    mythread(CyberPet temp1,octopus temp2,int temp3) {
	cpf=temp1;
	octo=temp2;
	sleeptime=temp3;
    }
    public void run() {
	// Keep looping
	while (true) {
	    // Call the moment method at each loop
	    this.moment();
	    // Then sleep for sleeptime before going around again
	    try {
		this.sleep(sleeptime);
	    }
	    catch (Exception e) {
		System.out.println("Oh dear: "+(e));
		System.exit(0);
	    }
	}
    }
    // Overwritten when extended
    void moment() {
    }
}

// Octo's animation
class animthread extends mythread {
    animthread(CyberPet temp1,octopus temp2,int temp3) {
	super(temp1,temp2,temp3);
    }
    void moment() {
	// If light is on, Octo moves
	if (octo.lighton) {
	    octo.xposition=octo.xposition+octo.direction;
	    if (octo.rnd(50) || octo.xposition<50 || octo.xposition>cpf.getSize().width-50)
	        octo.direction=-octo.direction;
	    // When awake, tentacle animation is faster
	    octo.frame=octo.frame+2;
	} else {
	    // than when asleep
	    octo.frame++;
	}
	// Display Octo
	cpf.repaint();
    }
}

// Octo's biology
class biothread extends mythread {
    biothread(CyberPet temp1,octopus temp2,int temp3) {
	super(temp1,temp2,temp3);
    }
    void moment() {
	// Octo digests some food
	octo.food=(float)(octo.food-0.02);
	if (octo.food<0)
	    octo.food=0;
	// Octo gets a little bit sadder
	octo.happiness=(float)(octo.happiness-0.02);
	if (octo.happiness<0)
	    octo.happiness=0;
	// Octo makes some mess
	octo.mess=(float)(octo.mess+0.01);
	if (octo.mess>4)
	    octo.mess=4;
	// If Octo has made enough mess, add another mess globule to the screen
	if (octo.expectedmesses()>octo.messes) {
	    octo.messes=octo.expectedmesses();
	    octo.messarray[octo.messes-1]=new mess(cpf);
	}
	// Octo gets more tired
	octo.tiredness=(float)(octo.tiredness+0.01);
	if (octo.tiredness>4)
	    octo.tiredness=4;
	// If Octo is not being looked after, he gets iller
	if (octo.hungry() || octo.sad() || octo.messy() || octo.tired())
	    octo.illness=(float)(octo.illness+0.01);
	// If Octo is completely tip-top, and has not fallen ill already, reset health
	if (!octo.hungry() && !octo.sad() && !octo.messy() && !octo.tired() && !octo.ill())
	    octo.illness=-1;
	// Octo stops winking after a few frames
	if (octo.frame>octo.winkframe+6)
	    octo.octowinks="None";
	// If Octo gets too ill (ie. has been neglected), he leaves home
	if (octo.illness>1)
	    octo.leavehome();
    }
}

// Octo's communication with user
class comsthread extends mythread {
    // Current messsage
    String s;
    // Time to show for
    int showfor=0;
    comsthread(CyberPet temp1,octopus temp2,int temp3) {
	super(temp1,temp2,temp3);
    }
    public void showmessagefor(String temp1,int temp2) {
	s=temp1;
	showfor=temp2;
    }
    void moment() {
	boolean done=false;
	// If Octo has a message to show, show it
	if (showfor>0) {
	    cpf.coms.setText(s);
	    // and reduce time to show it for
	    showfor--;
	    done=true;
	}
	// Otherwise, if Octo has any ailments, he lets the user know!
	if (!done && !octo.stillathome) {
	    cpf.coms.setText("I've left home.  Byebye!");
	    done=true;
	}
	if (!done && !octo.lighton) {
	    cpf.coms.setText("Zzzzzzzzzz...");
	    done=true;
	}
	if (!done && octo.ill()) {
	    cpf.coms.setText("I'm ill.");
	    done=true;
	}
	if (!done && octo.hungry()) {
	    cpf.coms.setText("I'm hungry.");
	    done=true;
	}
	if (!done && octo.sad()) {
	    cpf.coms.setText("I'm sad.");
	    done=true;
	}
	if (!done && octo.messy()) {
	    cpf.coms.setText("Yuck - too much mess!");
	    done=true;
	}
	if (!done && octo.tired()) {
	    cpf.coms.setText("Yawn - I'm tired.");
	    done=true;
	}
	// If Octo doesn't want to say anything, make sure he's not
	if (!done)
	    cpf.coms.setText("");
    }
}
