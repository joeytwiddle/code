import java.util.*;
import javax.swing.text.*;
import javax.swing.text.html.*;

public class StyleSheets {

       public static void main(String[] args) {
        HTMLEditorKit kit = new HTMLEditorKit();
        HTMLDocument doc = (HTMLDocument) kit.createDefaultDocument();
        StyleSheet styles = doc.getStyleSheet();

        Enumeration rules = styles.getStyleNames();
        while (rules.hasMoreElements()) {
            String name = (String) rules.nextElement();
            Style rule = styles.getStyle(name);
            System.out.println(rule.toString());
        }
        System.exit(0);
       }
   }