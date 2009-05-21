. addtoclasspath $HOME/j/code/javawebtools/CommonUtils/bin
. addtoclasspath $HOME/j/code/javawebtools/SimpleProxy/classes
for JAR in $PWD/lib/*.jar
do . addtoclasspath "$JAR"
done
. addtoclasspath $PWD/bin

# VM_OPTIONS="-Xrs" ## With this I can't access 4 signals, without it I can access 2.  Go figure.

# java org.neuralyte.superproxy.SuperProxy "$@"
java $VM_OPTIONS org.neuralyte.superproxy.grimeape.GrimeApe "$@"

