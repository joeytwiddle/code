cat ../good2.txt |
	sed "s/-/ /" |
	while read X Y; do
		echo "getregres "$X" "$Y""
	done
