#ifdef DOS
#define JFontDefault "c:\joey\code\c\joeylib\cool.jf"
#endif
#ifdef UNIX
#define JFontDefault "~pclark/../code/joeylib/cool.jf"
#endif

#define pi 3.141

class FontBit {
public:
  virtual void writetojbmp(JBmp *j,int x,int y,float scale,int c); // Method

};

class FontChar {
public:
  String name; // Exists
  List<FontBit*> bs; // Exists
   FontChar(); // Method

   FontChar(String n); // Method

  void writetojbmp(JBmp *j,int x,int y,float scale,int c); // Method

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

  void writetojbmp(JBmp *j,int x,int y,float scale,int c); // Method

};

class FArc : public FontBit {
public:
  float x,y,r,aa,ab,w; // Exists
  bool rounded; // Exists
   FArc(float a,float b,float c,float d); // Method

   FArc(float a,float b,float c,float d,bool rn); // Method

  void writetojbmp(JBmp *j,int cx,int cy,float scale,int c); // Method

/*  void writetojbmp(JBmp *j,int cx,int cy,float scale,int c) {
    bool first=true;
    V2d p,l;
    for (float a=aa;a<=ab;a+=pi/32.1) {
      p=V2d(cx,cy)+scale*V2d(x+r*sin(a),-y-r*cos(a));
      if (first)
        first=false;
      else
        j->thickline(l,p,w*scale-2,c);
      l=p;
    }
  }*/
};

List<FontChar> parsefontchars(String fname,float fwgiven); // Method


List<FontChar> parsefontchars(String fname); // Method


class JFont {
  List<FontChar> fcs; // Exists
public:
   JFont(); // Method

   JFont(String fname); // Method

   JFont(String fname,float f); // Method

  static bool fcalled(FontChar *f,String s); // Method

  void writechar(String s,JBmp *j,int x,int y,float scale,int c); // Method

  void writeString(String s,JBmp *j,int x,int y,float scale,int c); // Method

  void centerString(String s,JBmp *j,int y,float scale,int c); // Method

  void centerString(String s,JBmp *j,int cx,int y,float scale,int c); // Method

};
