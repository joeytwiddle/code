mkdir -p jars

cd src
jar cf ../jars/VisualJava-src.jar .

cd ../classes
## -m for manifest for -jar to work
## -i for index for javawebstart to work
# jar cfm ../jars/VisualJava.jar ../src/META-INF/MANIFEST.MF .
jar cfm ../jars/VisualJava.jar ../src/META-INF/MANIFEST.MF *

cd ..
## Also needed for javaws (system access):
jarsigner -storepass wibble ./jars/VisualJava.jar mykey
