/*
 *      @(#)ShowPP.java 1.16 00/03/31 14:29:17
 *
 * Copyright (c) 1996-2000 Sun Microsystems, Inc. All Rights Reserved.
 *
 * Sun grants you ("Licensee") a non-exclusive, royalty free, license to use,
 * modify and redistribute this software in source and binary code form,
 * provided that i) this copyright notice and license appear on all copies of
 * the software; and ii) Licensee does not utilize the software in a manner
 * which is disparaging to Sun.
 *
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY
 * IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN AND ITS LICENSORS SHALL NOT BE
 * LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL SUN OR ITS
 * LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT,
 * INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER
 * CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF
 * OR INABILITY TO USE SOFTWARE, EVEN IF SUN HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 * This software is not designed or intended for use in on-line control of
 * aircraft, air traffic, aircraft navigation or aircraft communications; or in
 * the design, construction, operation or maintenance of any nuclear
 * facility. Licensee represents and warrants that it will not use or
 * redistribute the Software for such purposes.
 */

import com.sun.j3d.utils.image.TextureLoader;
import com.sun.j3d.utils.geometry.Box;
import com.sun.j3d.utils.behaviors.mouse.*;
import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;
import com.sun.j3d.utils.applet.MainFrame;
import com.sun.j3d.utils.universe.*;
import javax.media.j3d.*;
import javax.vecmath.*;
import java.awt.image.BufferedImage;
import java.awt.Image;
import java.awt.Toolkit;



public class ShowPP extends Applet {
    
    private java.net.URL texImage = null;
    
    public BranchGroup createSceneGraph(Canvas3D c) {
        // Create the root of the branch graph
        BranchGroup objRoot = new BranchGroup();
        
        // Create a Transformgroup to scale all objects so they
        // appear in the scene.
        TransformGroup objScale = new TransformGroup();
        Transform3D t3d = new Transform3D();
        t3d.setScale(0.4);
        objScale.setTransform(t3d);
        objRoot.addChild(objScale);
        
        TransformGroup objTrans = new TransformGroup();
        //write-enable for behaviors
        objTrans.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
        objTrans.setCapability( TransformGroup.ALLOW_TRANSFORM_READ );
        objTrans.setCapability(TransformGroup.ENABLE_PICK_REPORTING);
        objScale.addChild(objTrans);
        
        Appearance ap = new Appearance();
        Color3f aColor  = new Color3f(0.1f, 0.1f, 0.1f);
        Color3f eColor  = new Color3f(0.0f, 0.0f, 0.0f);
        Color3f dColor  = new Color3f(0.8f, 0.8f, 0.8f);
        Color3f sColor  = new Color3f(1.0f, 1.0f, 1.0f);
        
        Material m = new Material(aColor, eColor, dColor, sColor, 80.0f);
        m.setLightingEnable(true);
        ap.setMaterial(m);

        Image img=Toolkit.getDefaultToolkit().getImage(texImage);

        //Create a Box
        Box BoxObj = new Box(1.5f, 1.5f*img.getHeight(c)/img.getWidth(c), 0.0f, Box.GENERATE_NORMALS |
                             Box.GENERATE_TEXTURE_COORDS, ap);
        // add it to the scene graph.
        objTrans.addChild(BoxObj);
        
        TextureLoader tex = new TextureLoader(img, "RGB", this);
        ap.setTexture(tex.getTexture());
        
        TextureAttributes texAttr = new TextureAttributes();
        texAttr.setTextureMode(TextureAttributes.MODULATE);
        ap.setTextureAttributes(texAttr);
        
        
        BoundingSphere bounds =
            new BoundingSphere(new Point3d(0.0,0.0,0.0), 100.0);
        
        // Create the rotate behavior node
        MouseRotate behavior = new MouseRotate(objTrans);
        objTrans.addChild(behavior);
        behavior.setSchedulingBounds(bounds);
        
        // Create the zoom behavior node
        MouseZoom behavior2 = new MouseZoom(objTrans);
        objTrans.addChild(behavior2);
        behavior2.setSchedulingBounds(bounds);
        
        //Shine it with two lights.
        Color3f lColor1 = new Color3f(0.7f, 0.7f, 0.7f);
        Color3f lColor2 = new Color3f(0.2f, 0.2f, 0.1f);
        Vector3f lDir1  = new Vector3f(-1.0f, -1.0f, -1.0f);
        Vector3f lDir2  = new Vector3f(0.0f, 0.0f, -1.0f);
        DirectionalLight lgt1 = new DirectionalLight(lColor1, lDir1);
        DirectionalLight lgt2 = new DirectionalLight(lColor2, lDir2);
        lgt1.setInfluencingBounds(bounds);
        lgt2.setInfluencingBounds(bounds);
        objScale.addChild(lgt1);
        objScale.addChild(lgt2);
        
        // Let Java 3D perform optimizations on this scene graph.
        objRoot.compile();
        
        return objRoot;
    }
    
    public ShowPP (){
    }
    
    public ShowPP(java.net.URL url) {
        texImage = url;
    }
    
    public void init() {
        if (texImage == null) {
            // the path to the image for an applet
            try {
                texImage = new java.net.URL(getCodeBase().toString() +
                                            "../images/earth.jpg");
            }
            catch (java.net.MalformedURLException ex) {
                System.out.println(ex.getMessage());
                System.exit(1);
            }
        }
        
        setLayout(new BorderLayout());
        GraphicsConfiguration config =
            SimpleUniverse.getPreferredConfiguration();
        
        Canvas3D c = new Canvas3D(config);
        add("Center", c);
        
        BranchGroup scene = createSceneGraph(c);
        SimpleUniverse u = new SimpleUniverse(c);
        
        // This will move the ViewPlatform back a bit so the
        // objects in the scene can be viewed.
        u.getViewingPlatform().setNominalViewingTransform();
        
        u.addBranchGraph(scene);
    }
    
    
    public static void main(String args[])
    {
        java.net.URL url = null;
        if (args.length > 0) {
            try {
                url = new java.net.URL("file:" + args[0]);
            }
            catch (java.net.MalformedURLException ex) {
                System.out.println(ex.getMessage());
                System.exit(1);
            }
        }
        else {
            // the path to the image file for an application
            try {
                url = new java.net.URL("file:origimage.bmp");
            }
            catch (java.net.MalformedURLException ex) {
                System.out.println(ex.getMessage());
                System.exit(1);
            }
        }
        
        new MainFrame(new ShowPP(url), 200, 200);
    }
}

