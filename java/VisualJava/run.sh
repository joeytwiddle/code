# jtop="$HOME/j/code/java"
jtop="`realpath ..`"

export CLASSPATH="$CLASSPATH:$jtop/DLang/bin:$jtop/web/CommonUtils/bin:$jtop/VisualJava/lib/xstream-1.1.2.jar"

cd bin
java visualjava.VisualJava "$@"

