package visualjava;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;

import javax.swing.ImageIcon;

/** joey Nov 2, 2004 3:15:13 PM */
public class VisualJavaGUIStatics {

	public static String getColorForClass(Class type) {
		// TODO: define distinctive colour if it's a primitive, or non-existent (e.g. void)?
		// Logger.info(type+".getPackage().getName() = " + type.getPackage().getName());
		if (type == Field.class)
			return "blue";
		if (type == Constructor.class)
			return "cyan";
		if (type == Method.class)
			return "orange";
		// if (type == Exception.class)
		if (Throwable.class.isAssignableFrom(type))
			return "red";
		// if (type.getSuperclass() == Object.class)
			// return "green";
		try { // We get NPE if type is an array type.
			if (type.getPackage().getName().startsWith("java.")
					|| type.getPackage().getName().startsWith("javax.")
					|| type.getPackage().getName().startsWith("sun.")
					|| type.getPackage().getName().startsWith("sunw.")
			)
				return "yellow";
			if (type.getPackage().getName().startsWith("com."))
				return "red";
			if (type.getPackage().getName().startsWith("org."))
				return "green";
		} catch (Exception e) {
		}
		// Unknown:
		return "magenta";
	}

	public static ImageIcon getImageIconForClass(Class c) {
		String color = VisualJavaGUIStatics.getColorForClass(c);
		ImageIcon imageIcon = new ImageIcon("src/visualjava/"+color+"dot-32x32.png");
		return imageIcon;
	}

	public static ImageIcon getSmallImageIconForClass(Class c) {
		ImageIcon imageIcon = getImageIconForClass(c);
		scaleImage(imageIcon);
		return imageIcon;
	}

    public static ImageIcon scaleImage(ImageIcon imageIcon) {
        imageIcon.setImage(imageIcon.getImage().getScaledInstance(15,15,15));
        return imageIcon;
    }

}
