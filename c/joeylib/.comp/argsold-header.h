extern int Jargc; // Exists
extern char **Jargv; // Exists

void Jargsetup(int argc,char **argv); // Method


String Jgetargafter(String pre,String fail); // Method


class ArgParser {
public:
  int argc; // Exists
  String *argv; // Exists
  bool helpon; // Exists
  
   ArgParser(int c,char **v); // Method

  
  String strafter(String pre,String why,String fail); // Method

  
  String strafter(String pre,String fail); // Method

  
  /* String strafter(String pre,String fail) {
       return strafter(pre,"",fail);
     }*/
  
  String getargafter(String pre,String fail); // Method

  
  String getargafter(String pre,String why,String fail); // Method

  
  String argafter(String pre,String fail); // Method

  
  String argafter(String pre,String why,String fail); // Method

  
  float floatafter(String pre,String why,float fail); // Method

  
  float floatafter(String pre,float fail); // Method

  
  int intafter(String pre,String why,int fail); // Method

  
  int intafter(String pre,int fail); // Method

  
  bool argexists(String x,String d); // Method

  
  bool argexists(String x); // Method

  
  String argnum(int i); // Method

  
  String arg(int i); // Method

  
  String getarg(int i); // Method

  
  void remarg(int i); // Method

  
  void remargs(int i,int j); // Method

  
  int argsleft(); // Method

  
  void done(); // Method

  
  void showargs(); // Method

  
};
