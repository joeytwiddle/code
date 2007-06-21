# (
	# ## If you have gzipped your logs:
	# gunzip -c ./ut-server/Logs/ucc.init.log*gz
	# ## If you haven't:
	# cat ./ut-server/Logs/ucc.init.log*
# ) |

find ./ut-server/Logs/ -type f |
sortfilesbydate |

tail -n 200 |

# foreachdo verbosely jzcat |
foreachdo jzcat |
# while read FILE
# do jzcat "$FILE" | prepend_each_line "$FILE: "
# done |

grep -u "^\(ChatLog (\|Level is\)" |

## minus_drivel:
grep -v -u ": nj" | grep -v -u ': !' |
grep -v -u "(EXTERNAL)" |
grep -v -u "Mr\.\(Gentle\|Brutus\)" | ## two annoyingly spammy bots on VietnamWarMonsters and other maps
grep -v -u ": teams$" | ## me testing my teambalance mod

while read WHAT TWO THREE REST
do
	case "$WHAT" in
		Level)
			LEVEL="$REST"
		;;
		ChatLog)
			echo "$LEVEL $TWO $THREE $REST"
		;;
	esac
done
