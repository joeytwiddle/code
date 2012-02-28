package visualjava;

/** joey Nov 5, 2004 4:05:06 PM
 * @deprecated WHY?! */
public class VariableModel {

    Class type;
    String name;
    Object value;

    public VariableModel(Class t,String n,Object v) {
        type = t;
        name = n;
        value = v;
    }
    public VariableModel(String n,Object v) {
        this(v.getClass(),n,v);
    }

    public Class getType() {
        return type;
    }
    public String getName() {
        return name;
    }
    public Object getValue() {
        return value;
    }

}
