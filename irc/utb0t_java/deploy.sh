LIVE_HOST=joey@neuralyte.org
# LIVE_HOST=joey@77.37.18.99 ## mox's machine

## Make a backup of the existing deploy:
BACKUP_NUM=` echolines u v w x y z | chooserandomline `
verbosely ssh "$LIVE_HOST" eval "tar cfz ircbot-backup.$BACKUP_NUM.tgz ircbot"

## Send the locally built copy to the live server:
# tar cz . | ssh "$LIVE_HOST" eval "cd ircbot && tar xz"
# tar cz bin lib *.sh | ssh "$LIVE_HOST" eval "cd ircbot && tar xz"
## Sending src why not it's all small
verbosely tar cz src bin *.sh DO_NOT_* | verbosely ssh "$LIVE_HOST" eval "cd ircbot && tar xzv"

