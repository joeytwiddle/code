cd /j
mkdir portmp
mkdir portmp/joeylib
mkdir portmp/phd
mkdir portmp/phd/improc
mkdir portmp/phd/quads
mkdir portmp/phd/regions
mkdir portmp/phd/stats
mkdir portmp/tools

cd /j/c

cp joeylib/*.c /j/portmp/joeylib
cp joeylib/*.h /j/portmp/joeylib
cp joeylib/*.jf /j/portmp/joeylib

portget phd/improc
portget phd/quads
portget phd/stats
portget phd/regions

#cp phd/improc/*.c /j/portmp/phd/improc
#cp phd/quads/*.c /j/portmp/phd/quads
#cp phd/regions/*.c /j/portmp/phd/regions
#cp phd/stats/*.c /j/portmp/phd/stats

cd tools
forall * do mkdir /j/portmp/tools/%f : cp %f/*.c /j/portmp/tools/%f

cd /j/portmp
chmod a-x * -R
tar cf ../portfromhwi.tar *
cd ..
zip portfromhwi.zip portfromhwi.tar
rm -r portmp
