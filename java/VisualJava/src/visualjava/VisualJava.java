package visualjava;

/** joey Jul 11, 2004 3:26:31 AM */
public class VisualJava {

	public static void main(String[] args) {
		visualjava.VisualJavaGui.show(new String("hello test"));
	}

	public static Class viewerForObject(Object toShow) {
		return viewerForType(toShow.getClass());
	}

	public static Class viewerForType(Class type) {
		Class testType = type;
		Class viewerType = null;
		while (viewerType == null && testType != null) {
			String test = "visualjava.viewers." + testType.getName();
			try {
				viewerType = Class.forName(test);
			} catch (Exception e) {
				// e.printStackTrace(System.err);
			}
			if (viewerType == null) {
				Class lastTestType = testType;
				testType = testType.getSuperclass();
				if (lastTestType == testType)
					break;
			}
		}
		return viewerType;
	}

	public static Viewer invokeViewer(Class viewerType,Object argument) {
		Class[] argTypes = { Object.class };
		java.lang.reflect.Constructor constructor = null;
		try {
			constructor = viewerType.getConstructor(argTypes);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		Object[] args = { argument };
		Viewer viewer = null;
		try {
			viewer = (Viewer)constructor.newInstance(args);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return viewer;
	}

}
