#ifndef jfontformaps_H
  #define jfontformaps_H

class FontBit {
public:
  template <class Object>
  void writetomap(Map2d<Object> *j,int x,int y,float scale,Object c); // Method

};

class FontChar {
public:
  String name; // Exists
  List<FontBit*> bs; // Exists
   FontChar(); // Method

   FontChar(String n); // Method

  template<class Object>
  void writetomap(Map2d<Object> *j,int x,int y,float scale,Object c); // Method

};

class FLine : public FontBit {
public:
  float w; // Exists
  V2d a,b; // Exists
  bool hor; // Variable initialised in .c file
  bool ver; // Variable initialised in .c file
  bool round; // Variable initialised in .c file
  bool lng; // Variable initialised in .c file
   FLine(float aa,float bb,float c,float d,float e,String t); // Method

  template<class Object>
  void writetomap(Map2d<Object> *j,int x,int y,float scale,Object c); // Method

};

class FArc : public FontBit {
public:
  float x,y,r,aa,ab,w; // Exists
   FArc(float a,float b,float c,float d); // Method

  template<class Object>
  void writetomap(Map2d<Object> *j,int cx,int cy,float scale,Object c); // Method

//

};

List<FontChar> parsefontchars(String fname); // Method


class JFont {
  List<FontChar> fcs; // Exists
public:
   JFont(String fname); // Method

  static bool fcalled(FontChar *f,String s); // Method

  template<class Object>
  void writechar(String s,Map2d<Object> *j,int x,int y,float scale,Object c); // Method

  template<class Object>
  void writeString(String s,Map2d<Object> *j,int x,int y,float scale,Object c); // Method

  template<class Object>
  void centerString(String s,Map2d<Object> *j,int y,float scale,Object c); // Method

  template<class Object>
  void centerString(String s,Map2d<Object> *j,int cx,int y,float scale,Object c); // Method

};

#endif
