#ifndef args_H
  #define args_H

String safegetenv(String s); // Method


String JPATH(); // Method


// Deprecated: non-class Argument Parser
extern int Jargc; // Exists
extern char **Jargv; // Exists
void Jargsetup(int argc,char **argv); // Method

String Jgetargafter(String pre,String fail); // Method


class ArgParser {
public:
  int argc; // Exists
  String *argv; // Exists
  bool helpon; // Exists
  String com; // Exists
  List<String> opts; // Exists
  
   ArgParser(); // Method

  
   ArgParser(int c,char **v); // Method

  
  String strafter(String pre,String why,String fail); // Method

  
  String strafter(String pre,String fail); // Method

  
  //
  
  String getargafter(String pre,String fail); // Method

  
  String getargafter(String pre,String why,String fail); // Method

  
  String argafter(String pre,String fail); // Method

  
  String argafter(String pre,String why,String fail); // Method

  
  float floatafter(String pre,String why,float fail); // Method

  
  float floatafter(String pre,float fail); // Method

  
  int intafter(String pre,String why,int fail); // Method

  
  int intafter(String pre,int fail); // Method

  
  bool argexists(String x,String d, bool remove); // Method

  
  bool argexists(String x,String d); // Method


  bool argexists(String x); // Method



  
  String argnum(int i); // Method

  
  String arg(int i); // Method

  
  String getarg(int i); // Method


  String getarg(); // Method


  String arg(String w); // Method


  String getarg(String w); // Method


  String argor(String w,String o); // Method

  
  String argor(String o); // Method

  
  void remarg(int i); // Method

  
  void remargs(int i,int j); // Method

  
  int argsleft(); // Method


  void error(String s); // Method

  
  void done(); // Method

  
  void showargs(); // Method

  
  void comment(String s); // Method

  
};

#endif
