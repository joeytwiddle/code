#!/usr/bin/env bash
set -e

. get_classpath.shlib

#cd bin
cd classes
java visualjava.VisualJava "$@"
