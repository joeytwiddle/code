USCRIPTFILE="$1"

CURRENT="$1.current"

PKGDIR="Decompiled/` echo "$USCRIPTFILE" | afterlast / | beforelast "\.u" `/Classes"
mkdir -p "$PKGDIR"

cat "$USCRIPTFILE" |

## Encode all the real newlines:
sed "s+$+\\\\r\\\\n+" |

## Delete all the existing newlines:
tr -d '\r\n' |

tee /tmp/decompile_unrealscript.debug.1 |

## Delete all the non-printable characters:
sed "s+[^[:print:][:space:]]+\n+g" | ## Works with sed 4.1.2 - takes forever with sed 4.1.5
# sed -u "s+[^[:print:][:space:]][^[:print:][:space:]]*+\n+g" | ## Works with sed 4.1.2 - takes forever with sed 4.1.5
# striptermchars |
tee /tmp/decompile_unrealscript.debug.2 |

# dog "$USCRIPTFILE".decompiled.0
tr -s '\r\n' |

## Select large blocks, which contain "\nclass ____" near their start:
trimempty |
grep "^...................." |
grep "\<[n]*class\>" |

# while read BLOCK
# do : ## TODO: Extract and "perform" classname
# done |

## Reformat for output:
sed 's+$+\n+' |
sed 's+\(\\r\|\)\\n+\n+g' |
# sed 's+^}$+}\n+' |

tee "$USCRIPTFILE".decompiled.1 |

cat > "$CURRENT"

while true
do
	# NEXT_CLASS=` cat "$CURRENT" | fromline "^class " | head -n 1 `
	NEXT_LINE=` grep "^class " "$CURRENT" | head -n 1 `
	NEXT_CLASS=` echo "$NEXT_LINE" | sed 's+^class \([^ ]*\).*+\1+' `
	[ ! "$NEXT_CLASS" ] && break
	echo "Writing: $NEXT_CLASS to $PKGDIR/$NEXT_CLASS.uc" >&2
	(
		echo "$NEXT_LINE"
		cat "$CURRENT" | fromline -x "^class " | toline -x "^class "
	) > "$PKGDIR/$NEXT_CLASS.uc"
	cat "$CURRENT" | fromline -x "^class " | fromline "^class " | dog "$CURRENT"
	echo "Filesize was `filesize "$PKGDIR/$NEXT_CLASS.uc"`; remaining is `filesize "$CURRENT"`]" >&2
done

# echo "Please check that $CURRENT is now empty." >&2
[ 0 = "`filesize "$CURRENT"`" ] || jshwarn "$CURRENT should be empty but it is not!"

