cat good2.txt |
	sed "s/-/ /" |
	while read X Y; do
		0open "$X"
		cd projprof
		./getregres "$X" "$Y"
		./colres "$X" "$Y"
		cd ..
		0close "$X"
	done
