cat ../joeylib.c| grep -E "^[^/]*#include" | afterfirst "<" | beforefirst "\."
