package functional.simplelib;

public class File {
    
    java.io.File realFile;
    
    public File(String name) {
        realFile = new java.io.File(name);
    }
    
    public String getName() {
        return realFile.getName();
    }
    
    public String getPath() {
        return realFile.getPath();
    }
    
    public String[] readAsLines() {
        return new String[1]; //@todo
    }
    
    // ...
    
}
