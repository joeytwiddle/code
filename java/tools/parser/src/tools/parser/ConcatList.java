package tools.parser;

public interface ConcatList<T> extends java.util.List<T> {

    public ConcatList<T> with(T item);
    
}