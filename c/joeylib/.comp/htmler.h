#ifndef htmler_H
  #define htmler_H

class Pos {
public:
  int r,c; // Exists
   Pos(int a,int b); // Method

  Pos nextrow(); // Method

};

class HTMLer {
public:
  List<String> ls; // Exists
   HTMLer(); // Method

   HTMLer(List<String> l); // Method

   HTMLer(String fname); // Method

  void start(String t); // Method

  void start(); // Method

  void add(String s); // Method

  void write(String f); // Method

  void end(); // Method

  String title(); // Method

  String findpattern(String s); // Method

  int findline(String s); // Method

  void addtitle(); // Method

  Pos find(String s); // Method

  Pos find(String s,Pos p); // Method

  Pos findback(String s,Pos p); // Method

};

#endif
