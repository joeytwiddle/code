package jlib;

import java.lang.*;
import java.util.*;

public class JThread {

	/** You can do some really dirty hacks by attaching Objects to the current thread and retrieving them later! **/
	private static Map threadData=new HashMap();

	public static void put(Object key,Object val) {
		Thread t=Thread.currentThread();
		Object o=threadData.get(t);
		Map map;
		if (o==null) {
			map=new HashMap();
			threadData.put(t,map);
		} else {
			map=(Map)o;
		}
		map.put(key,val);
	}

	public static Object get(Object key) {
		Thread t=Thread.currentThread();
		Object o=threadData.get(t);
		if (o==null) {
			JLib.report("JThread.get(\""+key+"\") on "+t+": no map yet set");
			return null;
		} else {
			Map map=(Map)o;
			return map.get(key);
		}
	}

	public static Map getAll() {
		Thread t=Thread.currentThread();
		Object o=threadData.get(t);
		if (o==null)
			return null;
		else
			return (Map)o;
	}
		
  public static boolean join(Thread t) {
    try {
      t.join();
      return true;
    } catch (Exception e) {
      return false;
    }
  }
  
  public static boolean yield() {
    // try {
      Thread.currentThread().yield();
      return true;
    // } catch (Exception e) {
    //   return false;
    // }
  }
  
  public static boolean sleep(int ms) {
    try {
      Thread.currentThread().sleep(ms);
      return true;
    } catch (Exception e) {
      return false;
    }
  }
  
}
