echolines ut-server/Logs/*2005* |
while read FILE
do
	# jzcat "$FILE" | grep -C6 "^\(Exiting\|Aborting\).$" && echo " \_ $FILE"
	jzcat "$FILE" | HIGREP_NO_CONTEXT=1 higrep "^\(Exiting\|Aborting\).$" -C6 | indenteachline "$FILE: "
done
