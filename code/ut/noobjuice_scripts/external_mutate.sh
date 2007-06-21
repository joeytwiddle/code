COMMAND="$*"
[ "$COMMAND" ] || COMMAND=mutate
echo "ChatLog (EXTERNAL): !$COMMAND" >> ./ut-server/Logs/ucc.init.log
## For user only; no good in scripts!:
# tail -0f ./ut-server/Logs/ucc.init.log | head -n 10
