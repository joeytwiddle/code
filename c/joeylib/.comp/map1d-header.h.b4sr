template <class Object>
 class Map2d;

// Template class appears in original format:
template <class Object>
class Map1d {
public:
  int width;
  Object *pos;

  Map1d() {
    // printf("Empty map created.\n");
  }
  Map1d(int x) {
    width=x;
    pos=new (Object)[width];
    for (int j=0;j<width;j++)
    pos[j]=0;
    // printf("done %i\n",x);
  }
  Map1d(int x,Object o) {
    width=x;
    pos=new (Object)[width];
    for (int j=0;j<width;j++)
    pos[j]=o;
    // printf("done %i\n",x);
  }
  void freedom() {
    free(pos);
  }
  bool inmap(int i) {
    return (i>=0 && i<width);
  }
  Object getpos(int i) {
    if (inmap(i))
      return pos[i];
    else
      error("Tried to get pos outside Map1d.");
  }
  void setpos(int i,Object o) {
    if (inmap(i))
      pos[i]=o;
    else
      error("Tried to set pos outside Map1d.");
  }

  // Increment position i by 1
  void incpos(int i) {
    if (inmap(i))
      pos[i]++;
    else
      error("Tried to increment pos outside Map1d.");
  }

  // Increment position i by amount
  void incpos(int i,Object amount) {
    if (inmap(i))
      pos[i]=pos[i]+amount;
    else
      error("Tried to increment pos outside Map1d.");
  }

  // Increment pos i proportionally (properly)
  // ie. the increment is correctly distributed
  // over the window it spans (assumed width and weight 1)
  void incposprop(float ri) { // r means real not right!
    int li=(int)ri;
    float amountonright=ri-(float)li;
    incpos(li,1.0-amountonright);
    incpos(li+1,amountonright);
  }

  Map2d<bool> draw();
  Object largest() {
    Object largest=0;
    for (int i=0;i<width;i++)
      if (getpos(i)>largest)
        largest=getpos(i);
    return largest;
  }
  Object total() {
    Object t=0;
    for (int i=0;i<width;i++)
      t+=getpos(i);
    return t;
  }
  float entropy() {
    float f=0.0;
    float t=total();
    float d=t*log(t);
    for (int i=0;i<width;i++) {
      float x=getpos(i);
      f+=x*log(1.0+x)/d;
    }
    return -f;
  }
  Object totalsquare() {
    Object f=0.0;
    for (int i=0;i<width;i++)
      f+=mysquare(getpos(i));
    return f;
  }
  Object totalderiv() {
    Object f=0.0;
    for (int i=0;i<width-1;i++)
      f+=mysquare(getpos(i+1)-getpos(i));
    return f;
  }
  Object variance() {
    Object ave=total()/(float)width;
    Object v=0.0;
    for (int i=0;i<width-1;i++)
      v+=mysquare(getpos(i)-ave);
    return v;
  }
  int firstnonzero() {
    for (int i=0;i<width;i++)
      if (getpos(i)>0)
        return i;
    return -1;
  }
  int lastnonzero() {
    for (int i=width-1;i>=0;i--)
      if (getpos(i)>0)
        return i;
    return -1;
  }

  Map1d<Object> smoothed(int rad) {
    Map1d<Object> n=Map1d<Object>(width-rad+1);
    for (int i=0;i<width-rad+1;i++) {
      Object o=0;
      for (int j=0;j<rad;j++)
        o+=getpos(i+j);
      n.setpos(i,o/(float)rad);
    }
    return n;
  }

  Map1d<Object> cropped() {
    int l=firstnonzero();
    int r=lastnonzero();
    Map1d<Object> n=Map1d<Object>(r-l+1);
    for (int i=l;i<=r;i++) {
      n.setpos(i,getpos(i-l));
    }
    return n;
  }

  float average() {
    Object ave=0;
    for (int i=0;i<width;i++) {
      ave=ave+getpos(i);
    }
    return ave/(float)width;
  }

  float averageCropped() {
    int l=firstnonzero();
    int r=lastnonzero();
    Object ave=0;
    for (int i=l;i<=r;i++) {
      ave=ave+getpos(i);
    }
    return ave/(float)(r-l+1);
  }

};// End template class


