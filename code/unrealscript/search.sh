WORD="$1" ; shift
# find . -name "*.uc" |
find */Classes */*/Classes -name "*.uc" |
# xargs grep -i "\<$1\>" |
# xargs grep -i "$@" |
memo -t "1 hour" xargs grep -i "\<$WORD\>" "$@" |
highlight "\<$WORD\>" |
# highlight "\<`caseinsensitiveregex "$WORD"`\>" |
more
