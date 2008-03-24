WORD="$1" ; shift
# find . -name "*.uc" |
find */Classes Epic -name "*.uc" |
# xargs grep -i "\<$1\>" |
# xargs grep -i "$@" |
xargs grep -i "\<$WORD\>" "$@" |
highlight "\<$WORD\>" |
# highlight "\<`caseinsensitiveregex "$WORD"`\>" |
more
