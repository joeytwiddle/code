#ifndef fontforrgb_H
  #define fontforrgb_H

class FontBit {
public:
  virtual void writetojbmp(RGBmp *j,int x,int y,float scale,myRGB c); // Method

};

class FontChar {
public:
  String name; // Exists
  List<FontBit*> bs; // Exists
   FontChar(); // Method

   FontChar(String n); // Method

  void writetojbmp(RGBmp *j,int x,int y,float scale,myRGB c); // Method

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

  void writetojbmp(RGBmp *j,int x,int y,float scale,myRGB c); // Method

};

class FArc : public FontBit {
public:
  float x,y,r,aa,ab,w; // Exists
   FArc(float a,float b,float c,float d); // Method

  void writetojbmp(RGBmp *j,int cx,int cy,float scale,myRGB c); // Method

//
};

List<FontChar> parsefontchars(String fname,float fwgiven); // Method


List<FontChar> parsefontchars(String fname); // Method


class JFont {
  List<FontChar> fcs; // Exists
public:
   JFont(String fname); // Method

   JFont(String fname,float f); // Method

  static bool fcalled(FontChar *f,String s); // Method

  void writechar(String s,RGBmp *j,int x,int y,float scale,myRGB c); // Method

  void writeString(String s,RGBmp *j,int x,int y,float scale,myRGB c); // Method

  void centerString(String s,RGBmp *j,int y,float scale,myRGB c); // Method

  void centerString(String s,RGBmp *j,int cx,int y,float scale,myRGB c); // Method

};

#endif
