package qcode;

// Some languages, C for instance, allow global functions, whereas other, Java, do not (really).
// How do we deal with this?  Is the global a special case.  Or
// can we import globals into our own singular "Global" class
// and forthwith consider them in a module/class.
// It might be nice to mark it as a special case by making it a
// subtype of QClass, rather than a String hack.

// class QGlobal extends QClass {
//     String getNameSpace() {
//         return "org.neuralyte.qlib.Global";
//     }
// }

class QClass implements QIO {

    @Override
    public String toC() {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String toCoffee() {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String toJava() {
        // TODO Auto-generated method stub
        return null;
    }

    /*
    
    String getNameSpace() {
    }

    String getName() {
    }

    */

    // These will be an interface (or set of) that many things implement!
    
    /*
    String toC() {
    }

    String toJava() {
    }

    String toCoffee() {
    }
    */

}
