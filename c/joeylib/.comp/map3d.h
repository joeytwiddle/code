#ifndef map3d_H
  #define map3d_H

// Template class appears in original format:
template<class Object>
class Map3d {
public:
  int width,height,depth;
  Object ***pos;
  Map3d(int x,int y,int z,Object o) {
    width=x;
    height=y;
    depth=z;
    pos=new (Object **)[width];
    for (int i=0;i<width;i++) {
      pos[i]=new (Object *)[height];
      for (int j=0;j<height;j++) {
        pos[i][j]=new Object[depth];
        for (int k=0;k<depth;k++) {
          pos[i][j][k]=o;
        }
      }
    }
    #ifdef DEBUG
    printf("Map3d created, size %i %i %i.\n",width,height,depth);
    #endif
  }
  Map3d(int x,int y,int z,Object (*fn)()) {
    width=x;
    height=y;
    depth=z;
    pos=new (Object **)[width];
    for (int i=0;i<width;i++) {
      pos[i]=new (Object *)[height];
      for (int j=0;j<height;j++) {
        pos[i][j]=new Object[depth];
        for (int k=0;k<depth;k++) {
          pos[i][j][k]=fn();
        }
      }
    }
    #ifdef DEBUG
    printf("Map3d created, size %i %i %i.\n",width,height,depth);
    #endif
  }
  Map3d(int x,int y,int z) {
    width=x;
    height=y;
    depth=z;
    pos=new (Object **)[width];
    for (int i=0;i<width;i++) {
      pos[i]=new (Object *)[height];
      for (int j=0;j<height;j++) {
        pos[i][j]=new Object[depth];
      }
    }
    #ifdef DEBUG
    printf("Map3d created, size %i %i %i.\n",width,height,depth);
    #endif
  }
};// End template class



//

//

#endif
