package visualjava.objecteditingcomponents;


public class SimpleFormFactory implements FormFactory {

    public Object getForm(Object targetObj) {
        return new SimpleFormView(targetObj);
    }
    
}





// MagicWrappers everywhere!
// Allow user/dev to add regexps on packages/classes which apply Wrapper patches:
// Wrapper patches do: if a query is made for object.childThing but childThing is not found, then we attempt to apply a wrapper to get the value of childThing via other means on the object.
// We could even create disgusting Proxy objects: "Class newA is Class A with the getMibbit() and setMibbit() renamed getMeep() and setMeep()".