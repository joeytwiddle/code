class QFunction implements QIO {

    // Namespace?  Backlink to class/lib/module?  No.

    String name;  // May be null for anon

    List arglist;  // List of pairs of type and name

    Code code;  // "content"

    // The code may reference vars in the arglist
    // properties of this (in the case of a method)
    // vars in the scope (closures in JS/Ruby/...)

}

