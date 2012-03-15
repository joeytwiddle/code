package visualjava;

import java.io.FileReader;
import java.io.FileWriter;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Member;
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.List;
import java.util.Vector;

import org.common.nap.Nap;
import org.neuralyte.Logger;

import visualjava.guicomponents.Desktop;
import visualjava.guicomponents.MethodFrame;
import visualjava.guicomponents.VariableHolder;
import visualjava.guicomponents.VariableView;

import com.thoughtworks.xstream.XStream;

import dlang.java.DJavaMethod;
import dlang.lang.expressions.DExpression;
import dlang.lang.expressions.DMethodCall;
import dlang.lang.expressions.DNamedExpression;
import dlang.lang.expressions.DVarReference;
import dlang.lang.tokens.DMethod;

/** joey Jul 11, 2004 3:26:31 AM */
public class VisualJava {

	public static Desktop desktop;
	public static VisualJavaOptions options; // All its methods are static, but the user can get it easily here, and the dev if he wants to. :o Does he? ^^

	public static void main(String[] args) {

		Logger.log("Welcome to VisualJava.");

		VisualJavaGUI visualJavaGUI = new VisualJavaGUI();
		desktop = visualJavaGUI.desktop;

		// Display a few objects by default:

		desktop.showObject("Hello");
		// desktop.showObject(frame); // lost access to this
		desktop.showObject(desktop);

		try { desktop.displayMethod(Arrays.class.getMethod("asList",new Class[]{ new Object[0].getClass() }),null); }
		catch (Exception e) { }

		desktop.showObject(new VisualJava());
		desktop.showObject("VisualJavaOptions");
		/*
		desktop.showObject(new Dyno());
		desktop.showObject(new DynoWorkspace());
		desktop.showObject(new DynoInterpreter());
		*/
		// TODO: take out these dependencies unless we really want to ship them too
		desktop.showObject(new XStream(new com.thoughtworks.xstream.io.xml.DomDriver()));
		// desktop.showObject(new com.thoughtworks.xstream.io.xml.DomDriver());
		try {
            desktop.showObject(org.common.nap.Napper.class);
			Class[] argTypes;
			argTypes = new Class[0];
            desktop.showObject(com.thoughtworks.xstream.io.xml.DomDriver.class.getConstructor());
			argTypes = new Class[1];
			argTypes[0] = String.class;
			desktop.showObject(XStream.class.getMethod("fromXML", argTypes));
			argTypes[0] = Object.class;
			desktop.showObject(XStream.class.getMethod("toXML", argTypes));
			argTypes[0] = String.class;
			desktop.showObject(FileReader.class.getConstructor(argTypes));
			desktop.showObject(FileWriter.class.getConstructor(argTypes));
			argTypes[0] = Constructor.class;
			desktop.showObject(new MethodFrame(MethodFrame.class.getConstructor(argTypes)));
		} catch (SecurityException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (NoSuchMethodException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		/* Let the user set it.
		try {
            javax.swing.UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel");
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        */
        
	}

	public static void showObject(Object obj) {
		if (desktop == null) {
			// main(new String[0]);
			desktop = new VisualJavaGUI().desktop;
		}
		desktop.showObject(obj);
	}

	public static void executeMethodOrConstructor(
			final VariableHolder objTarget, final Member member,
			final VariableHolder[] parameterTargets) {
		
		new Thread() {
			public void run() {
				Logger.info("STARTING NEW THREAD: "+this+" ( "+objTarget+"."+member+"("+parameterTargets+")");

				try {
					StringBuffer code = new StringBuffer();
					final Object[] arguments = getObjectsArrayFromParameterTargets(parameterTargets);
					Object result;
					
					if (member instanceof Method) {
						Method method = (Method)member;

						String on = getVariableName(objTarget);
						if (on == null)
							on = method.getDeclaringClass().getName(); // static method i hope!
						code.append(on);
						code.append("." + method.getName()+ "(");

						Object reallyOn = ( objTarget == null ? null : objTarget.getObject() );
						result = method.invoke(reallyOn,arguments);
					} else {
						Constructor constructor = (Constructor)member;
						code.append("new " + constructor.getDeclaringClass().getName() /*+ "." + constructor.getName()*/ + "(");
						result = constructor.newInstance(arguments);
					}

					for (int i=0;i<parameterTargets.length;i++) {
						VariableHolder parameterTarget = parameterTargets[i];
						String toShow = VisualJava.getVariableName(parameterTarget);
						code.append(toShow);
						if (i < parameterTargets.length - 1) {
							code.append(", ");
						}
					}
					code.append(");");

					if (result == null) {
						org.neuralyte.Logger.log("Result was null.");
					} else {
						String nameOfNewDesktopVariable = VisualJava.desktop.showObject(result);
						Class returnType = ( member instanceof Method ? ((Method)member).getReturnType() : ((Constructor)member).getDeclaringClass() );
						code.insert(0,returnType.getName() + " " + nameOfNewDesktopVariable + " = ");
					}

					org.neuralyte.Logger.log("[CODE] " + code);

					VariableModel var = ( objTarget == null ? null :
						objTarget.variable == null ? null :
							objTarget.variable.variable
					);
					passToDyno(var, member, parameterTargets);

				} catch (InvocationTargetException e) {
					VisualJava.desktop.showObject(e.getCause());
				} catch (Exception e) {
					// e.printStackTrace();
					// org.neuralyte.Logger.error(e);
					Logger.logStackTrace(e);
					VisualJava.desktop.showObject(e);
				}
				
				Logger.info("END THREAD: "+this);
			}
		}.start();
	}

	// Might be called on the target object, or on one of the parameter objects.
	public static String getVariableName(VariableHolder objTarget) {
		Object on = (
		    objTarget == null
		    ? null
		    : objTarget.variable == null
		        ? objTarget.getObject()
		        : objTarget.variable
		);
		if (on == null)
			return null;
		else 
			return getTextForObject(on);
	}
	
	public static String getVariableName(VariableModel variable) {
		if (variable == null)
			return null;
		else
			if (variable.name == null)
				return getTextForObject(variable.value);
			else	
				return variable.name;
	}

	public static Object[] getObjectsArrayFromParameterTargets(VariableHolder[] parameterTargets) {
		Object[] arguments = new Object[parameterTargets.length];
        for (int i=0;i<parameterTargets.length;i++) {
            VariableHolder parameterTarget = parameterTargets[i];
            arguments[i] = parameterTarget.getObject();
        }
		return arguments;
	}

    public static String getTextForObject(Object o) {
        if (o instanceof VariableView) {
            return ((VariableView)o).variable.name;
        } else {
            return "<unknown " + o.getClass().getName()+">";
        }
    }

    public static List<DNamedExpression> namedExpressions = new Vector<DNamedExpression>();
    
	// public static DynoMethodImpl globalDynoMethod = new DynoMethodImpl();
	public static void passToDyno(VariableModel variable,
			Member member, VariableHolder[] parameterTargets) {
		String newVariableName = "newVar"+(int)(4096*Math.random());
		String calledObject = getVariableName(variable);
		String[] parameterNames = null;
		if (parameterTargets != null) { // Because we are accessing a field, not calling a method.
			parameterNames = new String[parameterTargets.length];
			for (int i=0;i<parameterNames.length;i++) {
				// parameterNames[i] = parameterTargets[i].getName();
				parameterNames[i] = getVariableName(parameterTargets[i]);
			}
		}
		if (member instanceof Method) {
		    
			/*DynoCommand dynoCommand = new DynoCommand(newVariableName,
					calledObject, new DynoJavaMethod((Method)member), parameterNames);
			*/
			// globalDynoMethod.commands.add(dynoCommand);
		    
		    DVarReference dObj = new DVarReference(variable);
		    DVarReference[] dArgs = new DVarReference[parameterNames.length];
		    
		    for (int i=0;i<parameterNames.length;i++) {
		        dArgs[i] = new DVarReference(parameterTargets[i].variable.variable);
		    }
		    
		    DMethod dMethod = new DJavaMethod((Method)member);
		    
		    DExpression expression = new DMethodCall(dObj, dMethod, dArgs);
		    
		    DNamedExpression dne = new DNamedExpression(newVariableName, expression);
		    
		    namedExpressions.add(dne);
		    
		    // @todo
		    // Test calling the dne through DInterpreter.
		    // If it returns something significantly from the Java, that is interesting.
		    // If it returns the same, that would be great!
		    
		    Logger.log("[CODE] Created " + Nap.toString(dne) );
		    
		} else {
			Logger.warn("I do not yet know how to handle: "+VisualJavaStatics.getShortClassName(member)+"s");
		}
	}

}
