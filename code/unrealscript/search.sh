find . -name "*.uc" | grep -v "/CVS/" | xargs higrep "$@"
